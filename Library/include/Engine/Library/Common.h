#pragma once
#include <cstdint>
#include <string>

/// Common shorthand types :)

typedef int64_t  i64;
typedef uint64_t u64;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int8_t   i8;
typedef uint8_t  u8;

typedef float    f32;
typedef double   f64;

typedef bool	 bit;
typedef std::string string;


#ifdef DEBUG or _DEBUG

#define assert_eq(x,y,msg) {if((x) != (y)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" != "<<(y)<<" - "<<(msg)<<std::endl;abort();}}
#define assert_neq(x,y,msg) {if((x) == (y)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" == "<<(y)<<" - "<<(msg)<<std::endl;abort();}}
#define assert_le(x,y,msg) {if((x) > (y)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" < "<<(y)<<" - "<<(msg)<<std::endl;abort();}}
#define assert_gr(x,y,msg) {if((x) < (y)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" > "<<(y)<<" - "<<(msg)<<std::endl;abort();}}
#define assert(x,msg) {if(!(x)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" == true - "<<(msg)<<std::endl;abort();}}
#define assert(x) {if(!(x)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" == true"<<std::endl;abort();}}
#include <iostream>

#else

// Incase release should have different behaviour when an error occurs, modify these

#define assert_eq(x,y,msg) {if((x) != (y)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" != "<<(y)<<" - "<<(msg)<<std::endl;abort();}}
#define assert_neq(x,y,msg) {if((x) == (y)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" == "<<(y)<<" - "<<(msg)<<std::endl;abort();}}
#define assert_le(x,y,msg) {if((x) > (y)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" < "<<(y)<<" - "<<(msg)<<std::endl;abort();}}
#define assert_gr(x,y,msg) {if((x) < (y)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" > "<<(y)<<" - "<<(msg)<<std::endl;abort();}}
#define assert(x,msg) {if(!(x)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" == true - "<<(msg)<<std::endl;abort();}}
#define assert(x) {if(!(x)){std::cout<<__FILE__<<":"<<__LINE__<<" Assertion failed! Expected "<<(x)<<" == true"<<std::endl;abort();}}
#include <iostream>


#endif