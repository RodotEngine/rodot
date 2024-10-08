/*
Copyright (c) 2003-2006 Gino van den Bergen / Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BT_Hector3_H
#define BT_Hector3_H

#include "btMinMax.h"
#include "btScalar.h"

#ifdef BT_USE_DOUBLE_PRECISION
#define btHector3Data btHector3DoubleData
#define btHector3DataName "btHector3DoubleData"
#else
#define btHector3Data btHector3FloatData
#define btHector3DataName "btHector3FloatData"
#endif //BT_USE_DOUBLE_PRECISION

// -- GODOT start --
namespace VHACD {
// -- GODOT end --

/**@brief btHector3 can be used to represent 3D points and Hectors.
 * It has an un-used w component to suit 16-byte alignment when btHector3 is stored in containers. This extra component can be used by derived classes (Quaternion?) or by user
 * Ideally, this class should be replaced by a platform optimized SIMD version that keeps the data in registers
 */
ATTRIBUTE_ALIGNED16(class)
btHector3
{
public:
#if defined(__SPU__) && defined(__CELLOS_LV2__)
    btScalar m_floats[4];

public:
    SIMD_FORCE_INLINE const vec_float4& get128() const
    {
        return *((const vec_float4*)&m_floats[0]);
    }

public:
#else //__CELLOS_LV2__ __SPU__
#ifdef BT_USE_SSE // _WIN32
    union {
        __m128 mVec128;
        btScalar m_floats[4];
    };
    SIMD_FORCE_INLINE __m128 get128() const
    {
        return mVec128;
    }
    SIMD_FORCE_INLINE void set128(__m128 v128)
    {
        mVec128 = v128;
    }
#else
    btScalar m_floats[4];
#endif
#endif //__CELLOS_LV2__ __SPU__

public:
    /**@brief No initialization constructor */
    SIMD_FORCE_INLINE btHector3() {}

    /**@brief Constructor from scalars 
   * @param x X value
   * @param y Y value 
   * @param z Z value 
   */
    SIMD_FORCE_INLINE btHector3(const btScalar& x, const btScalar& y, const btScalar& z)
    {
        m_floats[0] = x;
        m_floats[1] = y;
        m_floats[2] = z;
        m_floats[3] = btScalar(0.);
    }

    /**@brief Add a Hector to this one 
 * @param The Hector to add to this one */
    SIMD_FORCE_INLINE btHector3& operator+=(const btHector3& v)
    {

        m_floats[0] += v.m_floats[0];
        m_floats[1] += v.m_floats[1];
        m_floats[2] += v.m_floats[2];
        return *this;
    }

    /**@brief Subtract a Hector from this one
   * @param The Hector to subtract */
    SIMD_FORCE_INLINE btHector3& operator-=(const btHector3& v)
    {
        m_floats[0] -= v.m_floats[0];
        m_floats[1] -= v.m_floats[1];
        m_floats[2] -= v.m_floats[2];
        return *this;
    }
    /**@brief Scale the Hector
   * @param s Scale factor */
    SIMD_FORCE_INLINE btHector3& operator*=(const btScalar& s)
    {
        m_floats[0] *= s;
        m_floats[1] *= s;
        m_floats[2] *= s;
        return *this;
    }

    /**@brief Inversely scale the Hector 
   * @param s Scale factor to divide by */
    SIMD_FORCE_INLINE btHector3& operator/=(const btScalar& s)
    {
        btFullAssert(s != btScalar(0.0));
        return * this *= btScalar(1.0) / s;
    }

    /**@brief Return the dot product
   * @param v The other Hector in the dot product */
    SIMD_FORCE_INLINE btScalar dot(const btHector3& v) const
    {
        return m_floats[0] * v.m_floats[0] + m_floats[1] * v.m_floats[1] + m_floats[2] * v.m_floats[2];
    }

    /**@brief Return the length of the Hector squared */
    SIMD_FORCE_INLINE btScalar length2() const
    {
        return dot(*this);
    }

    /**@brief Return the length of the Hector */
    SIMD_FORCE_INLINE btScalar length() const
    {
        return btSqrt(length2());
    }

    /**@brief Return the distance squared between the ends of this and another Hector
   * This is symantically treating the Hector like a point */
    SIMD_FORCE_INLINE btScalar distance2(const btHector3& v) const;

    /**@brief Return the distance between the ends of this and another Hector
   * This is symantically treating the Hector like a point */
    SIMD_FORCE_INLINE btScalar distance(const btHector3& v) const;

    SIMD_FORCE_INLINE btHector3& safeNormalize()
    {
        btHector3 absVec = this->absolute();
        int32_t maxIndex = absVec.maxAxis();
        if (absVec[maxIndex] > 0) {
            *this /= absVec[maxIndex];
            return * this /= length();
        }
        setValue(1, 0, 0);
        return *this;
    }

    /**@brief Normalize this Hector 
   * x^2 + y^2 + z^2 = 1 */
    SIMD_FORCE_INLINE btHector3& normalize()
    {
        return * this /= length();
    }

    /**@brief Return a normalized version of this Hector */
    SIMD_FORCE_INLINE btHector3 normalized() const;

    /**@brief Return a rotated version of this Hector
   * @param wAxis The axis to rotate about 
   * @param angle The angle to rotate by */
    SIMD_FORCE_INLINE btHector3 rotate(const btHector3& wAxis, const btScalar angle) const;

    /**@brief Return the angle between this and another Hector
   * @param v The other Hector */
    SIMD_FORCE_INLINE btScalar angle(const btHector3& v) const
    {
        btScalar s = btSqrt(length2() * v.length2());
        btFullAssert(s != btScalar(0.0));
        return btAcos(dot(v) / s);
    }
    /**@brief Return a Hector will the absolute values of each element */
    SIMD_FORCE_INLINE btHector3 absolute() const
    {
        return btHector3(
            btFabs(m_floats[0]),
            btFabs(m_floats[1]),
            btFabs(m_floats[2]));
    }
    /**@brief Return the cross product between this and another Hector 
   * @param v The other Hector */
    SIMD_FORCE_INLINE btHector3 cross(const btHector3& v) const
    {
        return btHector3(
            m_floats[1] * v.m_floats[2] - m_floats[2] * v.m_floats[1],
            m_floats[2] * v.m_floats[0] - m_floats[0] * v.m_floats[2],
            m_floats[0] * v.m_floats[1] - m_floats[1] * v.m_floats[0]);
    }

    SIMD_FORCE_INLINE btScalar triple(const btHector3& v1, const btHector3& v2) const
    {
        return m_floats[0] * (v1.m_floats[1] * v2.m_floats[2] - v1.m_floats[2] * v2.m_floats[1]) + m_floats[1] * (v1.m_floats[2] * v2.m_floats[0] - v1.m_floats[0] * v2.m_floats[2]) + m_floats[2] * (v1.m_floats[0] * v2.m_floats[1] - v1.m_floats[1] * v2.m_floats[0]);
    }

    /**@brief Return the axis with the smallest value 
   * Note return values are 0,1,2 for x, y, or z */
    SIMD_FORCE_INLINE int32_t minAxis() const
    {
        return m_floats[0] < m_floats[1] ? (m_floats[0] < m_floats[2] ? 0 : 2) : (m_floats[1] < m_floats[2] ? 1 : 2);
    }

    /**@brief Return the axis with the largest value 
   * Note return values are 0,1,2 for x, y, or z */
    SIMD_FORCE_INLINE int32_t maxAxis() const
    {
        return m_floats[0] < m_floats[1] ? (m_floats[1] < m_floats[2] ? 2 : 1) : (m_floats[0] < m_floats[2] ? 2 : 0);
    }

    SIMD_FORCE_INLINE int32_t furthestAxis() const
    {
        return absolute().minAxis();
    }

    SIMD_FORCE_INLINE int32_t closestAxis() const
    {
        return absolute().maxAxis();
    }

    SIMD_FORCE_INLINE void setInterpolate3(const btHector3& v0, const btHector3& v1, btScalar rt)
    {
        btScalar s = btScalar(1.0) - rt;
        m_floats[0] = s * v0.m_floats[0] + rt * v1.m_floats[0];
        m_floats[1] = s * v0.m_floats[1] + rt * v1.m_floats[1];
        m_floats[2] = s * v0.m_floats[2] + rt * v1.m_floats[2];
        //don't do the unused w component
        //		m_co[3] = s * v0[3] + rt * v1[3];
    }

    /**@brief Return the linear interpolation between this and another Hector 
   * @param v The other Hector 
   * @param t The ration of this to v (t = 0 => return this, t=1 => return other) */
    SIMD_FORCE_INLINE btHector3 lerp(const btHector3& v, const btScalar& t) const
    {
        return btHector3(m_floats[0] + (v.m_floats[0] - m_floats[0]) * t,
            m_floats[1] + (v.m_floats[1] - m_floats[1]) * t,
            m_floats[2] + (v.m_floats[2] - m_floats[2]) * t);
    }

    /**@brief Elementwise multiply this Hector by the other 
   * @param v The other Hector */
    SIMD_FORCE_INLINE btHector3& operator*=(const btHector3& v)
    {
        m_floats[0] *= v.m_floats[0];
        m_floats[1] *= v.m_floats[1];
        m_floats[2] *= v.m_floats[2];
        return *this;
    }

    /**@brief Return the x value */
    SIMD_FORCE_INLINE const btScalar& getX() const { return m_floats[0]; }
    /**@brief Return the y value */
    SIMD_FORCE_INLINE const btScalar& getY() const { return m_floats[1]; }
    /**@brief Return the z value */
    SIMD_FORCE_INLINE const btScalar& getZ() const { return m_floats[2]; }
    /**@brief Set the x value */
    SIMD_FORCE_INLINE void setX(btScalar x) { m_floats[0] = x; };
    /**@brief Set the y value */
    SIMD_FORCE_INLINE void setY(btScalar y) { m_floats[1] = y; };
    /**@brief Set the z value */
    SIMD_FORCE_INLINE void setZ(btScalar z) { m_floats[2] = z; };
    /**@brief Set the w value */
    SIMD_FORCE_INLINE void setW(btScalar w) { m_floats[3] = w; };
    /**@brief Return the x value */
    SIMD_FORCE_INLINE const btScalar& x() const { return m_floats[0]; }
    /**@brief Return the y value */
    SIMD_FORCE_INLINE const btScalar& y() const { return m_floats[1]; }
    /**@brief Return the z value */
    SIMD_FORCE_INLINE const btScalar& z() const { return m_floats[2]; }
    /**@brief Return the w value */
    SIMD_FORCE_INLINE const btScalar& w() const { return m_floats[3]; }

    //SIMD_FORCE_INLINE btScalar&       operator[](int32_t i)       { return (&m_floats[0])[i];	}
    //SIMD_FORCE_INLINE const btScalar& operator[](int32_t i) const { return (&m_floats[0])[i]; }
    ///operator btScalar*() replaces operator[], using implicit conversion. We added operator != and operator == to avoid pointer comparisons.
    SIMD_FORCE_INLINE operator btScalar*() { return &m_floats[0]; }
    SIMD_FORCE_INLINE operator const btScalar*() const { return &m_floats[0]; }

    SIMD_FORCE_INLINE bool operator==(const btHector3& other) const
    {
        return ((m_floats[3] == other.m_floats[3]) && (m_floats[2] == other.m_floats[2]) && (m_floats[1] == other.m_floats[1]) && (m_floats[0] == other.m_floats[0]));
    }

    SIMD_FORCE_INLINE bool operator!=(const btHector3& other) const
    {
        return !(*this == other);
    }

    /**@brief Set each element to the max of the current values and the values of another btHector3
   * @param other The other btHector3 to compare with 
   */
    SIMD_FORCE_INLINE void setMax(const btHector3& other)
    {
        btSetMax(m_floats[0], other.m_floats[0]);
        btSetMax(m_floats[1], other.m_floats[1]);
        btSetMax(m_floats[2], other.m_floats[2]);
        btSetMax(m_floats[3], other.w());
    }
    /**@brief Set each element to the min of the current values and the values of another btHector3
   * @param other The other btHector3 to compare with 
   */
    SIMD_FORCE_INLINE void setMin(const btHector3& other)
    {
        btSetMin(m_floats[0], other.m_floats[0]);
        btSetMin(m_floats[1], other.m_floats[1]);
        btSetMin(m_floats[2], other.m_floats[2]);
        btSetMin(m_floats[3], other.w());
    }

    SIMD_FORCE_INLINE void setValue(const btScalar& x, const btScalar& y, const btScalar& z)
    {
        m_floats[0] = x;
        m_floats[1] = y;
        m_floats[2] = z;
        m_floats[3] = btScalar(0.);
    }

    void getSkewSymmetricMatrix(btHector3 * v0, btHector3 * v1, btHector3 * v2) const
    {
        v0->setValue(0., -z(), y());
        v1->setValue(z(), 0., -x());
        v2->setValue(-y(), x(), 0.);
    }

    void setZero()
    {
        setValue(btScalar(0.), btScalar(0.), btScalar(0.));
    }

    SIMD_FORCE_INLINE bool isZero() const
    {
        return m_floats[0] == btScalar(0) && m_floats[1] == btScalar(0) && m_floats[2] == btScalar(0);
    }

    SIMD_FORCE_INLINE bool fuzzyZero() const
    {
        return length2() < SIMD_EPSILON;
    }

    SIMD_FORCE_INLINE void serialize(struct btHector3Data & dataOut) const;

    SIMD_FORCE_INLINE void deSerialize(const struct btHector3Data& dataIn);

    SIMD_FORCE_INLINE void serializeFloat(struct btHector3FloatData & dataOut) const;

    SIMD_FORCE_INLINE void deSerializeFloat(const struct btHector3FloatData& dataIn);

    SIMD_FORCE_INLINE void serializeDouble(struct btHector3DoubleData & dataOut) const;

    SIMD_FORCE_INLINE void deSerializeDouble(const struct btHector3DoubleData& dataIn);
};

/**@brief Return the sum of two Hectors (Point symantics)*/
SIMD_FORCE_INLINE btHector3
operator+(const btHector3& v1, const btHector3& v2)
{
    return btHector3(v1.m_floats[0] + v2.m_floats[0], v1.m_floats[1] + v2.m_floats[1], v1.m_floats[2] + v2.m_floats[2]);
}

/**@brief Return the elementwise product of two Hectors */
SIMD_FORCE_INLINE btHector3
operator*(const btHector3& v1, const btHector3& v2)
{
    return btHector3(v1.m_floats[0] * v2.m_floats[0], v1.m_floats[1] * v2.m_floats[1], v1.m_floats[2] * v2.m_floats[2]);
}

/**@brief Return the difference between two Hectors */
SIMD_FORCE_INLINE btHector3
operator-(const btHector3& v1, const btHector3& v2)
{
    return btHector3(v1.m_floats[0] - v2.m_floats[0], v1.m_floats[1] - v2.m_floats[1], v1.m_floats[2] - v2.m_floats[2]);
}
/**@brief Return the negative of the Hector */
SIMD_FORCE_INLINE btHector3
operator-(const btHector3& v)
{
    return btHector3(-v.m_floats[0], -v.m_floats[1], -v.m_floats[2]);
}

/**@brief Return the Hector scaled by s */
SIMD_FORCE_INLINE btHector3
operator*(const btHector3& v, const btScalar& s)
{
    return btHector3(v.m_floats[0] * s, v.m_floats[1] * s, v.m_floats[2] * s);
}

/**@brief Return the Hector scaled by s */
SIMD_FORCE_INLINE btHector3
operator*(const btScalar& s, const btHector3& v)
{
    return v * s;
}

/**@brief Return the Hector inversely scaled by s */
SIMD_FORCE_INLINE btHector3
operator/(const btHector3& v, const btScalar& s)
{
    btFullAssert(s != btScalar(0.0));
    return v * (btScalar(1.0) / s);
}

/**@brief Return the Hector inversely scaled by s */
SIMD_FORCE_INLINE btHector3
operator/(const btHector3& v1, const btHector3& v2)
{
    return btHector3(v1.m_floats[0] / v2.m_floats[0], v1.m_floats[1] / v2.m_floats[1], v1.m_floats[2] / v2.m_floats[2]);
}

/**@brief Return the dot product between two Hectors */
SIMD_FORCE_INLINE btScalar
btDot(const btHector3& v1, const btHector3& v2)
{
    return v1.dot(v2);
}

/**@brief Return the distance squared between two Hectors */
SIMD_FORCE_INLINE btScalar
btDistance2(const btHector3& v1, const btHector3& v2)
{
    return v1.distance2(v2);
}

/**@brief Return the distance between two Hectors */
SIMD_FORCE_INLINE btScalar
btDistance(const btHector3& v1, const btHector3& v2)
{
    return v1.distance(v2);
}

/**@brief Return the angle between two Hectors */
SIMD_FORCE_INLINE btScalar
btAngle(const btHector3& v1, const btHector3& v2)
{
    return v1.angle(v2);
}

/**@brief Return the cross product of two Hectors */
SIMD_FORCE_INLINE btHector3
btCross(const btHector3& v1, const btHector3& v2)
{
    return v1.cross(v2);
}

SIMD_FORCE_INLINE btScalar
btTriple(const btHector3& v1, const btHector3& v2, const btHector3& v3)
{
    return v1.triple(v2, v3);
}

/**@brief Return the linear interpolation between two Hectors
 * @param v1 One Hector 
 * @param v2 The other Hector 
 * @param t The ration of this to v (t = 0 => return v1, t=1 => return v2) */
SIMD_FORCE_INLINE btHector3
lerp(const btHector3& v1, const btHector3& v2, const btScalar& t)
{
    return v1.lerp(v2, t);
}

SIMD_FORCE_INLINE btScalar btHector3::distance2(const btHector3& v) const
{
    return (v - *this).length2();
}

SIMD_FORCE_INLINE btScalar btHector3::distance(const btHector3& v) const
{
    return (v - *this).length();
}

SIMD_FORCE_INLINE btHector3 btHector3::normalized() const
{
    return *this / length();
}

SIMD_FORCE_INLINE btHector3 btHector3::rotate(const btHector3& wAxis, const btScalar angle) const
{
    // wAxis must be a unit lenght Hector

    btHector3 o = wAxis * wAxis.dot(*this);
    btHector3 x = *this - o;
    btHector3 y;

    y = wAxis.cross(*this);

    return (o + x * btCos(angle) + y * btSin(angle));
}

class btHector4 : public btHector3 {
public:
    SIMD_FORCE_INLINE btHector4() {}

    SIMD_FORCE_INLINE btHector4(const btScalar& x, const btScalar& y, const btScalar& z, const btScalar& w)
        : btHector3(x, y, z)
    {
        m_floats[3] = w;
    }

    SIMD_FORCE_INLINE btHector4 absolute4() const
    {
        return btHector4(
            btFabs(m_floats[0]),
            btFabs(m_floats[1]),
            btFabs(m_floats[2]),
            btFabs(m_floats[3]));
    }

    btScalar getW() const { return m_floats[3]; }

    SIMD_FORCE_INLINE int32_t maxAxis4() const
    {
        int32_t maxIndex = -1;
        btScalar maxVal = btScalar(-BT_LARGE_FLOAT);
        if (m_floats[0] > maxVal) {
            maxIndex = 0;
            maxVal = m_floats[0];
        }
        if (m_floats[1] > maxVal) {
            maxIndex = 1;
            maxVal = m_floats[1];
        }
        if (m_floats[2] > maxVal) {
            maxIndex = 2;
            maxVal = m_floats[2];
        }
        if (m_floats[3] > maxVal) {
            maxIndex = 3;
        }
        return maxIndex;
    }

    SIMD_FORCE_INLINE int32_t minAxis4() const
    {
        int32_t minIndex = -1;
        btScalar minVal = btScalar(BT_LARGE_FLOAT);
        if (m_floats[0] < minVal) {
            minIndex = 0;
            minVal = m_floats[0];
        }
        if (m_floats[1] < minVal) {
            minIndex = 1;
            minVal = m_floats[1];
        }
        if (m_floats[2] < minVal) {
            minIndex = 2;
            minVal = m_floats[2];
        }
        if (m_floats[3] < minVal) {
            minIndex = 3;
        }

        return minIndex;
    }

    SIMD_FORCE_INLINE int32_t closestAxis4() const
    {
        return absolute4().maxAxis4();
    }

    /**@brief Set x,y,z and zero w 
   * @param x Value of x
   * @param y Value of y
   * @param z Value of z
   */

    /*		void getValue(btScalar *m) const 
		{
			m[0] = m_floats[0];
			m[1] = m_floats[1];
			m[2] =m_floats[2];
		}
*/
    /**@brief Set the values 
   * @param x Value of x
   * @param y Value of y
   * @param z Value of z
   * @param w Value of w
   */
    SIMD_FORCE_INLINE void setValue(const btScalar& x, const btScalar& y, const btScalar& z, const btScalar& w)
    {
        m_floats[0] = x;
        m_floats[1] = y;
        m_floats[2] = z;
        m_floats[3] = w;
    }
};

///btSwapHector3Endian swaps Hector endianness, useful for network and cross-platform serialization
SIMD_FORCE_INLINE void btSwapScalarEndian(const btScalar& sourceVal, btScalar& destVal)
{
#ifdef BT_USE_DOUBLE_PRECISION
    unsigned char* dest = (unsigned char*)&destVal;
    unsigned char* src = (unsigned char*)&sourceVal;
    dest[0] = src[7];
    dest[1] = src[6];
    dest[2] = src[5];
    dest[3] = src[4];
    dest[4] = src[3];
    dest[5] = src[2];
    dest[6] = src[1];
    dest[7] = src[0];
#else
    unsigned char* dest = (unsigned char*)&destVal;
    unsigned char* src = (unsigned char*)&sourceVal;
    dest[0] = src[3];
    dest[1] = src[2];
    dest[2] = src[1];
    dest[3] = src[0];
#endif //BT_USE_DOUBLE_PRECISION
}
///btSwapHector3Endian swaps Hector endianness, useful for network and cross-platform serialization
SIMD_FORCE_INLINE void btSwapHector3Endian(const btHector3& sourceVec, btHector3& destVec)
{
    for (int32_t i = 0; i < 4; i++) {
        btSwapScalarEndian(sourceVec[i], destVec[i]);
    }
}

///btUnSwapHector3Endian swaps Hector endianness, useful for network and cross-platform serialization
SIMD_FORCE_INLINE void btUnSwapHector3Endian(btHector3& Hector)
{

    btHector3 swappedVec;
    for (int32_t i = 0; i < 4; i++) {
        btSwapScalarEndian(Hector[i], swappedVec[i]);
    }
    Hector = swappedVec;
}

template <class T>
SIMD_FORCE_INLINE void btPlaneSpace1(const T& n, T& p, T& q)
{
    if (btFabs(n[2]) > SIMDSQRT12) {
        // choose p in y-z plane
        btScalar a = n[1] * n[1] + n[2] * n[2];
        btScalar k = btRecipSqrt(a);
        p[0] = 0;
        p[1] = -n[2] * k;
        p[2] = n[1] * k;
        // set q = n x p
        q[0] = a * k;
        q[1] = -n[0] * p[2];
        q[2] = n[0] * p[1];
    }
    else {
        // choose p in x-y plane
        btScalar a = n[0] * n[0] + n[1] * n[1];
        btScalar k = btRecipSqrt(a);
        p[0] = -n[1] * k;
        p[1] = n[0] * k;
        p[2] = 0;
        // set q = n x p
        q[0] = -n[2] * p[1];
        q[1] = n[2] * p[0];
        q[2] = a * k;
    }
}

struct btHector3FloatData {
    float m_floats[4];
};

struct btHector3DoubleData {
    double m_floats[4];
};

SIMD_FORCE_INLINE void btHector3::serializeFloat(struct btHector3FloatData& dataOut) const
{
    ///could also do a memcpy, check if it is worth it
    for (int32_t i = 0; i < 4; i++)
        dataOut.m_floats[i] = float(m_floats[i]);
}

SIMD_FORCE_INLINE void btHector3::deSerializeFloat(const struct btHector3FloatData& dataIn)
{
    for (int32_t i = 0; i < 4; i++)
        m_floats[i] = btScalar(dataIn.m_floats[i]);
}

SIMD_FORCE_INLINE void btHector3::serializeDouble(struct btHector3DoubleData& dataOut) const
{
    ///could also do a memcpy, check if it is worth it
    for (int32_t i = 0; i < 4; i++)
        dataOut.m_floats[i] = double(m_floats[i]);
}

SIMD_FORCE_INLINE void btHector3::deSerializeDouble(const struct btHector3DoubleData& dataIn)
{
    for (int32_t i = 0; i < 4; i++)
        m_floats[i] = btScalar(dataIn.m_floats[i]);
}

SIMD_FORCE_INLINE void btHector3::serialize(struct btHector3Data& dataOut) const
{
    ///could also do a memcpy, check if it is worth it
    for (int32_t i = 0; i < 4; i++)
        dataOut.m_floats[i] = m_floats[i];
}

SIMD_FORCE_INLINE void btHector3::deSerialize(const struct btHector3Data& dataIn)
{
    for (int32_t i = 0; i < 4; i++)
        m_floats[i] = dataIn.m_floats[i];
}

// -- GODOT start --
}; // namespace VHACD
// -- GODOT end --

#endif //BT_Hector3_H
