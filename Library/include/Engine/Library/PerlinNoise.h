//----------------------------------------------------------------------------------------
//
//	siv::PerlinNoise
//	Perlin noise library for modern C++
//
//	Copyright (C) 2013-2018 Ryo Suzuki <reputeless@gmail.com>
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files(the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions :
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
//
//----------------------------------------------------------------------------------------

# pragma once
# include <cstdint>
# include <numeric>
# include <algorithm>
# include <random>
#include <Engine/Library/Math.h>

namespace Engine::Library::siv
{
    class PerlinNoise
    {
    private:

        int perm[256] = {
                151,160,137,91,90,15,
                131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
                190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
                88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
                77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
                102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
                135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
                5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
                223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
                129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
                251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
                49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
        };
        int p[512];

        template <class URNG>
        explicit PerlinNoise(URNG& urng)
        {
            //reseed(urng);
        }


        static inline double Fade(double t) noexcept
        {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }

        static inline double Lerp(double t, double a, double b) noexcept
        {
            return a + t * (b - a);
        }

        static inline double Grad(std::int32_t hash, double x, double y, double z) noexcept
        {
            const int h = hash & 15;
            const double u = h < 8 ? x : y;
            const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }

    public:

        explicit PerlinNoise(std::uint32_t seed = std::default_random_engine::default_seed)
        {
            reseed(seed);
        }

        void reseed(std::uint32_t seed)
        {
            // Copy over permutation
            memcpy(&p[0], &perm[0], sizeof(int) * 256);
            
            // Add duplicate to the end
            memcpy(&p[256], &perm[0], sizeof(int) * 256);
        }
        
        double noise(double x) const
        {
            return noise(x, 0.0, 0.0);
        }
    
        double noise(double x, double y) const
        {
            return noise(x, y, 0.0);
        }
    
        double noise(double x, double y, double z) const
        {
            //const std::int32_t X = static_cast<std::int32_t>(ffloor(x)) & 255;
            //const std::int32_t Y = static_cast<std::int32_t>(ffloor(y)) & 255;
            //const std::int32_t Z = static_cast<std::int32_t>(ffloor(z)) & 255;

            const int X = Math::ffloor(x) & 255;
            const int Y = Math::ffloor(y) & 255;
            const int Z = Math::ffloor(z) & 255;
            
            x -= Math::ffloor(x);
            y -= Math::ffloor(y);
            z -= Math::ffloor(z);

            const double u = Fade(x);
            const double v = Fade(y);
            const double w = Fade(z);

            const int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
            const int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

            return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
                                        Grad(p[BA], x - 1, y, z)),
                                Lerp(u, Grad(p[AB], x, y - 1, z),
                                     Grad(p[BB], x - 1, y - 1, z))),
                        Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
                                     Grad(p[BA + 1], x - 1, y, z - 1)),
                             Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
                                  Grad(p[BB + 1], x - 1, y - 1, z - 1))));
        }
    
        float octaveNoise(float x, int octaves) const
        {
            float result = 0.0;
            float amp = 1.0;
            float div = 0.0;

            for (int i = 0; i < octaves; ++i)
            {
                result += noise(x) * amp;
                x *= 2.0;
                div += amp;
                amp *= 0.5;
            }

            return result / div;
        }
    
        float octaveNoise(float x, float y, std::int32_t octaves, float baseZoom = 1000.0f, float baseAmplitude = 400.0f) const
        {
            float result = 0.0;
            float amp = baseAmplitude;
            float zoom = baseZoom;

            for (std::int32_t i = 0; i < octaves; ++i)
            {
                result += noise(x/zoom, y/zoom) * amp;
                amp *= 0.5;
                zoom *= 0.5;
            }

            return result;
        }
    
        double octaveNoise(double x, double y, double z, std::int32_t octaves) const
        {
            double result = 0.0;
            double amp = 1.0;
            double div = 0.0;

            for (std::int32_t i = 0; i < octaves; ++i)
            {
                result += noise(x, y, z) * amp;
                x *= 2.0;
                y *= 2.0;
                z *= 2.0;
                div += amp;
                amp *= 0.5;
            }

            return result/div;
        }
    
        
        double static inline fade(double t){
            return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
        }
    
        static double lerp(double t, double a, double b) { return a + t * (b - a); }
    
        static double grad(int hash, double x, double y, double z) {
            int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
            double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
                    v = h<4 ? y : h==12||h==14 ? x : z;
            return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
        }
        
        double noise2(double x, double y, double z) {
            int X = Math::ffloor(x) & 255;
            int Y = Math::ffloor(y) & 255;
            int Z = Math::ffloor(z) & 255;
            
            x -= Math::ffloor(x);
            y -= Math::ffloor(y);
            z -= Math::ffloor(z);
            
            double u = fade(x);
            double v = fade(y);
            double w = fade(z);
    
            int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,      // HASH COORDINATES OF
                B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,
    
            return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
                                        grad(p[BA  ], x-1, y  , z   )), // BLENDED
                                lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
                                     grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
                        lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
                                     grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                  grad(p[BB+1], x-1, y-1, z-1 ))));
        }
        
    };
}
