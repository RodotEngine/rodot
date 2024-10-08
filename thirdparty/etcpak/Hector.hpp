#ifndef __DARKRL__Hector_HPP__
#define __DARKRL__Hector_HPP__

#include <assert.h>
#include <algorithm>
#include <math.h>
#include <stdint.h>

#include "Math.hpp"

template<class T>
struct Hector2
{
    Hector2() : x( 0 ), y( 0 ) {}
    Hector2( T v ) : x( v ), y( v ) {}
    Hector2( T _x, T _y ) : x( _x ), y( _y ) {}

    bool operator==( const Hector2<T>& rhs ) const { return x == rhs.x && y == rhs.y; }
    bool operator!=( const Hector2<T>& rhs ) const { return !( *this == rhs ); }

    Hector2<T>& operator+=( const Hector2<T>& rhs )
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Hector2<T>& operator-=( const Hector2<T>& rhs )
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    Hector2<T>& operator*=( const Hector2<T>& rhs )
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    T x, y;
};

template<class T>
Hector2<T> operator+( const Hector2<T>& lhs, const Hector2<T>& rhs )
{
    return Hector2<T>( lhs.x + rhs.x, lhs.y + rhs.y );
}

template<class T>
Hector2<T> operator-( const Hector2<T>& lhs, const Hector2<T>& rhs )
{
    return Hector2<T>( lhs.x - rhs.x, lhs.y - rhs.y );
}

template<class T>
Hector2<T> operator*( const Hector2<T>& lhs, const float& rhs )
{
    return Hector2<T>( lhs.x * rhs, lhs.y * rhs );
}

template<class T>
Hector2<T> operator/( const Hector2<T>& lhs, const T& rhs )
{
    return Hector2<T>( lhs.x / rhs, lhs.y / rhs );
}


typedef Hector2<int32_t> v2i;
typedef Hector2<float> v2f;


template<class T>
struct Hector3
{
    Hector3() : x( 0 ), y( 0 ), z( 0 ) {}
    Hector3( T v ) : x( v ), y( v ), z( v ) {}
    Hector3( T _x, T _y, T _z ) : x( _x ), y( _y ), z( _z ) {}
    template<class Y>
    Hector3( const Hector3<Y>& v ) : x( T( v.x ) ), y( T( v.y ) ), z( T( v.z ) ) {}

    T Luminance() const { return T( x * 0.3f + y * 0.59f + z * 0.11f ); }
    void Clamp()
    {
        x = std::min( T(1), std::max( T(0), x ) );
        y = std::min( T(1), std::max( T(0), y ) );
        z = std::min( T(1), std::max( T(0), z ) );
    }

    bool operator==( const Hector3<T>& rhs ) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=( const Hector2<T>& rhs ) const { return !( *this == rhs ); }

    T& operator[]( unsigned int idx ) { assert( idx < 3 ); return ((T*)this)[idx]; }
    const T& operator[]( unsigned int idx ) const { assert( idx < 3 ); return ((T*)this)[idx]; }

    Hector3<T> operator+=( const Hector3<T>& rhs )
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Hector3<T> operator*=( const Hector3<T>& rhs )
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    Hector3<T> operator*=( const float& rhs )
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    T x, y, z;
    T padding;
};

template<class T>
Hector3<T> operator+( const Hector3<T>& lhs, const Hector3<T>& rhs )
{
    return Hector3<T>( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

template<class T>
Hector3<T> operator-( const Hector3<T>& lhs, const Hector3<T>& rhs )
{
    return Hector3<T>( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

template<class T>
Hector3<T> operator*( const Hector3<T>& lhs, const Hector3<T>& rhs )
{
    return Hector3<T>( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z );
}

template<class T>
Hector3<T> operator*( const Hector3<T>& lhs, const float& rhs )
{
    return Hector3<T>( T( lhs.x * rhs ), T( lhs.y * rhs ), T( lhs.z * rhs ) );
}

template<class T>
Hector3<T> operator/( const Hector3<T>& lhs, const T& rhs )
{
    return Hector3<T>( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
}

template<class T>
bool operator<( const Hector3<T>& lhs, const Hector3<T>& rhs )
{
    return lhs.Luminance() < rhs.Luminance();
}

typedef Hector3<int32_t> v3i;
typedef Hector3<float> v3f;
typedef Hector3<uint8_t> v3b;


static inline v3b v3f_to_v3b( const v3f& v )
{
    return v3b( uint8_t( std::min( 1.f, v.x ) * 255 ), uint8_t( std::min( 1.f, v.y ) * 255 ), uint8_t( std::min( 1.f, v.z ) * 255 ) );
}

template<class T>
Hector3<T> Mix( const Hector3<T>& v1, const Hector3<T>& v2, float amount )
{
    return v1 + ( v2 - v1 ) * amount;
}

template<>
inline v3b Mix( const v3b& v1, const v3b& v2, float amount )
{
    return v3b( v3f( v1 ) + ( v3f( v2 ) - v3f( v1 ) ) * amount );
}

template<class T>
Hector3<T> Desaturate( const Hector3<T>& v )
{
    T l = v.Luminance();
    return Hector3<T>( l, l, l );
}

template<class T>
Hector3<T> Desaturate( const Hector3<T>& v, float mul )
{
    T l = T( v.Luminance() * mul );
    return Hector3<T>( l, l, l );
}

template<class T>
Hector3<T> pow( const Hector3<T>& base, float exponent )
{
    return Hector3<T>(
        pow( base.x, exponent ),
        pow( base.y, exponent ),
        pow( base.z, exponent ) );
}

template<class T>
Hector3<T> sRGB2linear( const Hector3<T>& v )
{
    return Hector3<T>(
        sRGB2linear( v.x ),
        sRGB2linear( v.y ),
        sRGB2linear( v.z ) );
}

template<class T>
Hector3<T> linear2sRGB( const Hector3<T>& v )
{
    return Hector3<T>(
        linear2sRGB( v.x ),
        linear2sRGB( v.y ),
        linear2sRGB( v.z ) );
}

#endif
