/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// FILE: BaseType.h ///////////////////////////////////////////////////////////
//
// Project:  RTS3
//
// Basic types and constants
// Author: Michael S. Booth, January 1995, September 2000
//
///////////////////////////////////////////////////////////////////////////////

// tell the compiler to only load this file once
#pragma once


#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#include <math.h>
#include <string.h>
#include <cstdint>
#include <chrono>

#ifndef PI
#define PI     3.14159265359f
#define TWO_PI 6.28318530718f
#endif

#ifndef NULL
//#define NULL ((void *)0)
#define NULL 0						// C++ doesn't like casting void *'s into other pointers
#endif

// MSVC math.h defines overloaded functions with this name...
//#ifndef abs
//#define abs(x) (((x) < 0) ? -(x) : (x))
//#endif

// #ifndef min
// #define min(x,y) (((x)<(y)) ? (x) : (y))
// #endif

// #ifndef max
// #define max(x,y) (((x)>(y)) ? (x) : (y))
// #endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

// Elements in an array
#ifndef ELEMENTS_OF
#define ELEMENTS_OF( x ) ( sizeof( x ) / sizeof( x[0] ) )
#endif

//--------------------------------------------------------------------
// Fundamental type definitions
//--------------------------------------------------------------------
typedef float    Real;          // 4 bytes 
typedef int32_t  Int;           // 4 bytes 
typedef uint32_t UnsignedInt;   // 4 bytes 
typedef uint16_t UnsignedShort; // 2 bytes 
typedef int16_t  Short;         // 2 bytes 
typedef uint8_t  UnsignedByte;  // 1 byte USED TO BE "Byte"
typedef int8_t   Byte;          // 1 byte USED TO BE "SignedByte"
typedef char     Char;          // 1 byte of text
typedef bool     Bool;          // 
// note, the types below should use "long long", but MSVC doesn't support it yet
typedef int64_t  Int64;         // 8 bytes 
typedef uint64_t UnsignedInt64; // 8 bytes 

/** @todo we need real color representation, this is just place holder so we
can more easily identify sections of the code that need it */
typedef UnsignedInt Color;

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using Millis = std::chrono::milliseconds;
using Duration = std::chrono::duration<double, std::milli>;

#include "Lib/Trig.h"

//-----------------------------------------------------------------------------
typedef wchar_t WideChar;  ///< multi-byte character representations

//-----------------------------------------------------------------------------
template <typename NUM>
inline NUM sqr(NUM x)
{
   return x*x;
}

template <typename NUM>
inline NUM clamp(NUM lo, NUM val, NUM hi)
{
   if (val < lo) return lo;
   else if (val > hi) return hi;
   else return val;
}

template <typename NUM>
inline int sign(NUM x)
{
   if (x > 0) return 1;
   else if (x < 0) return -1;
   else return 0;
}

//-----------------------------------------------------------------------------
inline Real rad2deg(Real rad) { return rad * (180/PI); }
inline Real deg2rad(Real rad) { return rad * (PI/180); }

//-----------------------------------------------------------------------------
// For twiddling bits
//-----------------------------------------------------------------------------
#define BitTest( x, i ) ( ( (x) & (i) ) != 0 )
#define BitSet( x, i ) ( (x) |= (i) )
#define BitClear( x, i ) ( (x ) &= ~(i) )
#define BitToggle( x, i ) ( (x) ^= (i) )

//-------------------------------------------------------------------------------------------------

// note, this function depends on the cpu rounding mode, which we set to CHOP every frame, 
// but apparently tends to be left in unpredictable modes by various system bits of
// code, so use this function with caution -- it might not round in the way you want.
inline long fast_float2long_round(float f)
{
   // FIXME: asm is a problem.
   return std::round(f);
//    long i;

//    asm (
//       "fld %[f];"
//       "fistp %[i];"
//    );

//    return i;
}

// super fast float trunc routine, works always (independent of any FPU modes)
// code courtesy of Martin Hoffesommer (grin)
inline float fast_float_trunc(float f)
{
   // FIXME: asm is a problem.
   return std::trunc(f);
//    asm (
//       "mov ecx,%[f];"
//       "shr ecx,23;"
//       "mov eax,0xff800000;"
//       "xor ebx,ebx;"
//       "sub cl,127;"
//       "cmovc eax,ebx;"
//       "sar eax,cl;"
//       "and %[f],eax;"
//   );
//   return f;
}

// same here, fast floor function
inline float fast_float_floor(float f)
{
   return std::floor(f);
//    static unsigned almost1=(126<<23)|0x7fffff;
//    if (*(unsigned *)&f &0x80000000)
//       f-=*(float *)&almost1;
//    return fast_float_trunc(f);
}

// same here, fast ceil function
inline float fast_float_ceil(float f)
{
   return std::ceil(f);
//    static unsigned almost1=(126<<23)|0x7fffff;
//    if ( (*(unsigned *)&f &0x80000000)==0)
//       f+=*(float *)&almost1;
//    return fast_float_trunc(f);
}

//-------------------------------------------------------------------------------------------------
#define REAL_TO_INT(x)						((Int)(fast_float2long_round(fast_float_trunc(x))))
#define REAL_TO_UNSIGNEDINT(x)		((UnsignedInt)(fast_float2long_round(fast_float_trunc(x))))
#define REAL_TO_SHORT(x)					((Short)(fast_float2long_round(fast_float_trunc(x))))
#define REAL_TO_UNSIGNEDSHORT(x)	((UnsignedShort)(fast_float2long_round(fast_float_trunc(x))))
#define REAL_TO_BYTE(x)						((Byte)(fast_float2long_round(fast_float_trunc(x))))
#define REAL_TO_UNSIGNEDBYTE(x)		((UnsignedByte)(fast_float2long_round(fast_float_trunc(x))))
#define REAL_TO_CHAR(x)						((Char)(fast_float2long_round(fast_float_trunc(x))))
#define DOUBLE_TO_REAL(x)					((Real) (x))
#define DOUBLE_TO_INT(x)					((Int) (fast_float2long_round(fast_float_trunc(x))))
#define INT_TO_REAL(x)						((Real) (x))

// once we've ceiled/floored, trunc and round are identical, and currently, round is faster... (srj)
#define REAL_TO_INT_CEIL(x)				(fast_float2long_round(fast_float_ceil(x)))
#define REAL_TO_INT_FLOOR(x)			(fast_float2long_round(fast_float_floor(x)))
#define REAL_TO_UNSIGNEDINT_CEIL(x)		((UnsignedInt)(fast_float2long_round(fast_float_ceil(x))))
#define REAL_TO_UNSIGNEDINT_FLOOR(x)	((UnsignedInt)(fast_float2long_round(fast_float_floor(x))))

#define FAST_REAL_TRUNC(x)        fast_float_trunc(x)
#define FAST_REAL_CEIL(x)         fast_float_ceil(x)
#define FAST_REAL_FLOOR(x)        fast_float_floor(x)

//--------------------------------------------------------------------
// Derived type definitions
//--------------------------------------------------------------------

// NOTE: Keep these derived types simple, and avoid constructors and destructors
// so they can be used within unions.

// real-valued range defined by low and high values
struct RealRange 
{
    Real lo, hi;							// low and high values of the range

    // combine the given range with us such that we now encompass
    // both ranges
    void combine( RealRange &other )
    {
        lo = std::min( lo, other.lo );
        hi = std::max( hi, other.hi );
    }
};

struct Coord2D 
{
    Real x, y;

    Real length( void ) const { return (Real)sqrt( x*x + y*y ); }

    void normalize( void )
    {
        Real len = length();
        if( len != 0 )
        {
            x /= len;
            y /= len;
        }
    }
    
    Real toAngle( void ) const;  ///< turn 2D vector into angle (where angle 0 is down the +x axis)

};

inline Real Coord2D::toAngle( void ) const
{
    const Real len = length();
    if (len == 0.0f)
        return 0.0f;

    Real c = x/len;
    // bound it in case of numerical error
    if (c < -1.0f)
        c = -1.0f;
    else if (c > 1.0f)
        c = 1.0f;

    return y < 0.0f ? -ACos(c) : ACos(c);
}  // end toAngle

struct ICoord2D 
{
    Int x, y;

    Int length( void ) const { return (Int)sqrt( (double)(x*x + y*y) ); }
};

struct Region2D
{
    Coord2D lo, hi;						// bounds of 2D rectangular region

    Real width( void ) const { return hi.x - lo.x; }
    Real height( void ) const { return hi.y - lo.y; }
};

struct IRegion2D
{
    ICoord2D lo, hi;					// bounds of 2D rectangular region

    Int width( void ) const { return hi.x - lo.x; }
    Int height( void ) const { return hi.y - lo.y; }
};


struct Coord3D 
{
    Real x, y, z;

    Real length( void ) const { return (Real)sqrt( x*x + y*y + z*z ); }
    Real lengthSqr( void ) const { return ( x*x + y*y + z*z ); }

    void normalize( void )
    {
        Real len = length();

        if( len != 0 )
        {
            x /= len;
            y /= len;
            z /= len;
        }
    }
    
    static void crossProduct( const Coord3D *a, const Coord3D *b, Coord3D *r )
    {
        r->x = (a->y * b->z - a->z * b->y);
        r->y = (a->z * b->x - a->x * b->z);
        r->z = (a->x * b->y - a->y * b->x);
    }
    
    void zero( void )
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    void add( const Coord3D *a )
    {
        x += a->x;
        y += a->y;
        z += a->z;
    }
    
    void sub( const Coord3D *a )
    {
        x -= a->x;
        y -= a->y;
        z -= a->z;
    }
    
    void set( const Coord3D *a )
    {
        x = a->x;
        y = a->y;
        z = a->z;
    }
    
    void set( Real ax, Real ay, Real az )
    {
        x = ax;
        y = ay;
        z = az;
    }

    void scale( Real scale )
    {
        x *= scale;
        y *= scale;
        z *= scale;
    }

    Bool equals( const Coord3D &r )
    {
        return (x == r.x && 
                        y == r.y &&
                        z == r.z);
    }

    Bool operator==( const Coord3D &r )
    {
        return (x == r.x &&
                        y == r.y &&
                        z == r.z);
    }
};

struct ICoord3D 
{
    Int x, y, z;

    Int length( void ) const { return (Int)sqrt( (double)(x*x + y*y + z*z) ); }
    void zero( void )
    {

        x = 0;
        y = 0;
        z = 0;
    }
};

struct Region3D
{
    Coord3D lo, hi;						// axis-aligned bounding box

    Real width( void ) const { return hi.x - lo.x; }
    Real height( void ) const { return hi.y - lo.y; }
    Real depth( void ) const { return hi.z - lo.z; }

    void zero() { lo.zero(); hi.zero(); }
    Bool isInRegionNoZ( const Coord3D *query ) const
    {
        return (lo.x < query->x) && (query->x < hi.x) 
                        && (lo.y < query->y) && (query->y < hi.y);
    }
    Bool isInRegionWithZ( const Coord3D *query ) const
    {
        return (lo.x < query->x) && (query->x < hi.x) 
                        && (lo.y < query->y) && (query->y < hi.y)
                        && (lo.z < query->z) && (query->z < hi.z);
    }
};

struct IRegion3D
{
    ICoord3D lo, hi;					// axis-aligned bounding box

    Int width( void ) const { return hi.x - lo.x; }
    Int height( void ) const { return hi.y - lo.y; }
    Int depth( void ) const { return hi.z - lo.z; }
};


struct RGBColor
{
    Real red, green, blue;		// range between 0 and 1

    inline Int getAsInt() const
    {
        return
            ((Int)(red * 255.0) << 16) |
            ((Int)(green * 255.0) << 8) |
            ((Int)(blue * 255.0) << 0);
    }

    inline UnsignedInt getAsUnsignedInt() const
    {
        return
            ((UnsignedInt)(red * 255.0) << 16) |
            ((UnsignedInt)(green * 255.0) << 8) |
            ((UnsignedInt)(blue * 255.0) << 0);
    }

    inline void setFromInt(Int c)
    {
        red = ((c >> 16) & 0xff) / 255.0f;
        green = ((c >>  8) & 0xff) / 255.0f;
        blue = ((c >>  0) & 0xff) / 255.0f;
    }

    inline void setFromUnsignedInt(UnsignedInt c)
    {
        red = ((c >> 16) & 0xff) / 255.0f;
        green = ((c >>  8) & 0xff) / 255.0f;
        blue = ((c >>  0) & 0xff) / 255.0f;
    }

    inline void setFromColor(Color c)
    {
        red = ((c >> 16) & 0xff) / 255.0f;
        green = ((c >>  8) & 0xff) / 255.0f;
        blue = ((c >>  0) & 0xff) / 255.0f;
    }

};

struct RGBAColorReal
{

    Real red, green, blue, alpha;  // range between 0.0 and 1.0

};

struct RGBAColorInt
{

    UnsignedInt red, green, blue, alpha;  // range between 0 and 255

};

#endif // _BASE_TYPE_H_
