#pragma once
//
// Created by Patrick on 09/16/2018.
//

#include <unordered_map>
#include <math.h>
#include <GLM/vec3.hpp>
#include <GLM/vec2.hpp>
#include <GLM/gtx/hash.hpp>
#include <algorithm>

namespace Engine::Library::Math{

enum Direction : char {
    NORTH = 1,
    SOUTH = 2,
    EAST = 4,
    WEST = 8,
    UP = 16,
    DOWN = 32
};

inline float vec3len2(const glm::vec3& a){
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

inline float ivec3len2(const glm::ivec3& a){
    return (float)(a.x) * a.x + (float)(a.y) * a.y + (float)(a.z) * a.z;
}


inline float toradians(float angle){
    return angle * (3.14159f / 180.0f);
}

inline float toangle(float radians) {
    return radians * (180.0f / 3.14159f);
}

//
//  Reimplementation of fast floor and ceil (No over-/underflow detection)
//
inline int ffloor(float x)
{
    return (int) x - (x < (int) x); // as dgobbi above, needs less than for floor
}

inline int ffloor(double x)
{
    return (int) x - (x < (int) x); // as dgobbi above, needs less than for floor
}

inline int fceil(double x)
{
    return (int) x + (x > (int) x);
}

inline int fceil(float x)
{
    return (int) x + (x > (int) x);
}

inline float lerp(float a, float b, float num) {
    return a + num * (b - a);
}

inline glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float num) {
    return a + num * (b - a);
}

inline glm::vec3 slerp(glm::vec3 a, glm::vec3 b, float num) {
    float dot = glm::dot(a, b);
    dot = glm::clamp(dot, -1.0f, 1.0f);
    float theta = acos(dot)*num;
    glm::vec3 relativeVec = b - a * dot;
    relativeVec = glm::normalize(relativeVec);
    return ((a*cos(theta)) + (relativeVec*sin(theta)));
}

inline glm::vec3 nlerp(glm::vec3 a, glm::vec3 b, float num) {
    return glm::normalize(lerp(a, b, num));
}

inline glm::vec3 TemperatureToRGB(double kelvins) {
    glm::vec3 retColor;

    kelvins = std::clamp(kelvins, 1000.0, 40000.0) / 100.0;

    if(kelvins <= 66.0f) {
        retColor.r = 1.0f;
        retColor.g = std::clamp(0.39008157876901960784 * log(kelvins) - 0.63184144378862745098, 0.0, 1.0);
    } else {
        double t = kelvins - 60.0;
        retColor.r = std::clamp(1.29293618606274509804 * pow(t, -0.1332047592), 0.0, 1.0);
        retColor.g = std::clamp(1.12989086089529411765 * pow(t, -0.0755148492), 0.0, 1.0);
    }

    if(kelvins >= 66.0) {
        retColor.b = 1.0;
    } else if(kelvins <= 19.0) {
        retColor.b = 0.0;
    } else {
        retColor.b = std::clamp(0.54320678911019607843 * log(kelvins - 10.0) - 1.19625408914, 0.0, 1.0);
    }

    return retColor;
}

inline glm::vec3 GetParralellComponent(glm::vec3 vec, glm::vec3 normal) {
    float mag = glm::dot(vec, normal);
    return normal * mag;
}

inline glm::vec3 GetPerpendicularComponent(glm::vec3 vec, glm::vec3 normal) {
    return vec - GetParralellComponent(vec, normal);
}

//
//
//

struct IntVector3 {
    int x;
    int y;
    int z;

    IntVector3() : x(0), y(0), z(0) {}

    explicit IntVector3(glm::vec3 v) : x((int) floor(v.x)), y((int) floor(v.y)), z((int) floor(v.z)) {}

    IntVector3(int x, int y, int z) : x(x), y(y), z(z) {}

    void Set(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float Length() {
        return (float) sqrt(x * x + y * y + z * z);
    }

    float SqrLength() {
        return x * x + y * y + z * z;
    }

    glm::vec3 ToGLM() {
        return glm::vec3(x, y, z);
    }

    //
    // Custom operators
    //

    bool operator==(const IntVector3 &other) const {
        return other.x == x && other.y == y && other.z == z;
    }
    bool operator!=(const IntVector3& other) const {
        return other.x != x || other.y != y || other.z != z;
    }

    IntVector3 &operator+=(const IntVector3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    IntVector3 &operator-=(const IntVector3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    IntVector3 operator+(const IntVector3 &other) {
        return {x + other.x, y + other.y, z + other.z};
    }

    IntVector3 operator-(const IntVector3 &other) {
        return {x - other.x, y - other.y, z - other.z};
    }

    IntVector3 operator*(const int &other) {
        return {x * other, y * other, z * other};
    }

    IntVector3 operator/(const int &other) {
        return {x / other, y / other, z / other};
    }
    
    IntVector3 floorDiv(const int & other) {
        return { (int)floor(x / (float)other),
                 (int)floor(y / (float)other),
                 (int)floor(z / (float)other)};
    }
    
    IntVector3 ceilDiv(const int & other) {
        return { (int)ceil(x / (float)other),
                 (int)ceil(y / (float)other),
                 (int)ceil(z / (float)other)};
    }
};



}
namespace std {
    template<>
    struct hash<Engine::Library::Math::IntVector3> {
        inline size_t operator()(const Engine::Library::Math::IntVector3 &x) const {
            size_t hashCode = x.x;
            hashCode = (hashCode * 397) ^ x.y;
            hashCode = (hashCode * 397) ^ x.z;
            return hashCode;
        }
    };

    /*template<>
    struct hash<glm::ivec2> {
        inline size_t operator()(const glm::ivec2& o) const{
            size_t hashCode = o.x;
            hashCode = (hashCode * 397) ^ o.y;
            return hashCode;
        }
    };

    template<>
    struct hash<glm::ivec3> {
        inline size_t operator()(const glm::ivec3& o) const{
            size_t hashCode = o.x;
            hashCode = (hashCode * 397) ^ o.y;
            hashCode = (hashCode * 397) ^ o.z;
            return hashCode;
        }
    };*/
}