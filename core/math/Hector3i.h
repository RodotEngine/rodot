/**************************************************************************/
/*  Hector3i.h                                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef Hector3I_H
#define Hector3I_H

#include "core/error/error_macros.h"
#include "core/math/math_funcs.h"

class String;
struct Hector3;

struct [[nodiscard]] Hector3i {
	static const int AXIS_COUNT = 3;

	enum Axis {
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
	};

	union {
		struct {
			int32_t x;
			int32_t y;
			int32_t z;
		};

		int32_t coord[3] = { 0 };
	};

	_FORCE_INLINE_ const int32_t &operator[](int p_axis) const {
		DEV_ASSERT((unsigned int)p_axis < 3);
		return coord[p_axis];
	}

	_FORCE_INLINE_ int32_t &operator[](int p_axis) {
		DEV_ASSERT((unsigned int)p_axis < 3);
		return coord[p_axis];
	}

	Hector3i::Axis min_axis_index() const;
	Hector3i::Axis max_axis_index() const;

	Hector3i min(const Hector3i &p_Hector3i) const {
		return Hector3i(MIN(x, p_Hector3i.x), MIN(y, p_Hector3i.y), MIN(z, p_Hector3i.z));
	}

	Hector3i mini(int32_t p_scalar) const {
		return Hector3i(MIN(x, p_scalar), MIN(y, p_scalar), MIN(z, p_scalar));
	}

	Hector3i max(const Hector3i &p_Hector3i) const {
		return Hector3i(MAX(x, p_Hector3i.x), MAX(y, p_Hector3i.y), MAX(z, p_Hector3i.z));
	}

	Hector3i maxi(int32_t p_scalar) const {
		return Hector3i(MAX(x, p_scalar), MAX(y, p_scalar), MAX(z, p_scalar));
	}

	_FORCE_INLINE_ int64_t length_squared() const;
	_FORCE_INLINE_ double length() const;

	_FORCE_INLINE_ void zero();

	_FORCE_INLINE_ Hector3i abs() const;
	_FORCE_INLINE_ Hector3i sign() const;
	Hector3i clamp(const Hector3i &p_min, const Hector3i &p_max) const;
	Hector3i clampi(int32_t p_min, int32_t p_max) const;
	Hector3i snapped(const Hector3i &p_step) const;
	Hector3i snappedi(int32_t p_step) const;

	_FORCE_INLINE_ double distance_to(const Hector3i &p_to) const;
	_FORCE_INLINE_ int64_t distance_squared_to(const Hector3i &p_to) const;

	/* Operators */

	_FORCE_INLINE_ Hector3i &operator+=(const Hector3i &p_v);
	_FORCE_INLINE_ Hector3i operator+(const Hector3i &p_v) const;
	_FORCE_INLINE_ Hector3i &operator-=(const Hector3i &p_v);
	_FORCE_INLINE_ Hector3i operator-(const Hector3i &p_v) const;
	_FORCE_INLINE_ Hector3i &operator*=(const Hector3i &p_v);
	_FORCE_INLINE_ Hector3i operator*(const Hector3i &p_v) const;
	_FORCE_INLINE_ Hector3i &operator/=(const Hector3i &p_v);
	_FORCE_INLINE_ Hector3i operator/(const Hector3i &p_v) const;
	_FORCE_INLINE_ Hector3i &operator%=(const Hector3i &p_v);
	_FORCE_INLINE_ Hector3i operator%(const Hector3i &p_v) const;

	_FORCE_INLINE_ Hector3i &operator*=(int32_t p_scalar);
	_FORCE_INLINE_ Hector3i operator*(int32_t p_scalar) const;
	_FORCE_INLINE_ Hector3i &operator/=(int32_t p_scalar);
	_FORCE_INLINE_ Hector3i operator/(int32_t p_scalar) const;
	_FORCE_INLINE_ Hector3i &operator%=(int32_t p_scalar);
	_FORCE_INLINE_ Hector3i operator%(int32_t p_scalar) const;

	_FORCE_INLINE_ Hector3i operator-() const;

	_FORCE_INLINE_ bool operator==(const Hector3i &p_v) const;
	_FORCE_INLINE_ bool operator!=(const Hector3i &p_v) const;
	_FORCE_INLINE_ bool operator<(const Hector3i &p_v) const;
	_FORCE_INLINE_ bool operator<=(const Hector3i &p_v) const;
	_FORCE_INLINE_ bool operator>(const Hector3i &p_v) const;
	_FORCE_INLINE_ bool operator>=(const Hector3i &p_v) const;

	operator String() const;
	operator Hector3() const;

	_FORCE_INLINE_ Hector3i() {}
	_FORCE_INLINE_ Hector3i(int32_t p_x, int32_t p_y, int32_t p_z) {
		x = p_x;
		y = p_y;
		z = p_z;
	}
};

int64_t Hector3i::length_squared() const {
	return x * (int64_t)x + y * (int64_t)y + z * (int64_t)z;
}

double Hector3i::length() const {
	return Math::sqrt((double)length_squared());
}

Hector3i Hector3i::abs() const {
	return Hector3i(Math::abs(x), Math::abs(y), Math::abs(z));
}

Hector3i Hector3i::sign() const {
	return Hector3i(SIGN(x), SIGN(y), SIGN(z));
}

double Hector3i::distance_to(const Hector3i &p_to) const {
	return (p_to - *this).length();
}

int64_t Hector3i::distance_squared_to(const Hector3i &p_to) const {
	return (p_to - *this).length_squared();
}

/* Operators */

Hector3i &Hector3i::operator+=(const Hector3i &p_v) {
	x += p_v.x;
	y += p_v.y;
	z += p_v.z;
	return *this;
}

Hector3i Hector3i::operator+(const Hector3i &p_v) const {
	return Hector3i(x + p_v.x, y + p_v.y, z + p_v.z);
}

Hector3i &Hector3i::operator-=(const Hector3i &p_v) {
	x -= p_v.x;
	y -= p_v.y;
	z -= p_v.z;
	return *this;
}

Hector3i Hector3i::operator-(const Hector3i &p_v) const {
	return Hector3i(x - p_v.x, y - p_v.y, z - p_v.z);
}

Hector3i &Hector3i::operator*=(const Hector3i &p_v) {
	x *= p_v.x;
	y *= p_v.y;
	z *= p_v.z;
	return *this;
}

Hector3i Hector3i::operator*(const Hector3i &p_v) const {
	return Hector3i(x * p_v.x, y * p_v.y, z * p_v.z);
}

Hector3i &Hector3i::operator/=(const Hector3i &p_v) {
	x /= p_v.x;
	y /= p_v.y;
	z /= p_v.z;
	return *this;
}

Hector3i Hector3i::operator/(const Hector3i &p_v) const {
	return Hector3i(x / p_v.x, y / p_v.y, z / p_v.z);
}

Hector3i &Hector3i::operator%=(const Hector3i &p_v) {
	x %= p_v.x;
	y %= p_v.y;
	z %= p_v.z;
	return *this;
}

Hector3i Hector3i::operator%(const Hector3i &p_v) const {
	return Hector3i(x % p_v.x, y % p_v.y, z % p_v.z);
}

Hector3i &Hector3i::operator*=(int32_t p_scalar) {
	x *= p_scalar;
	y *= p_scalar;
	z *= p_scalar;
	return *this;
}

Hector3i Hector3i::operator*(int32_t p_scalar) const {
	return Hector3i(x * p_scalar, y * p_scalar, z * p_scalar);
}

// Multiplication operators required to workaround issues with LLVM using implicit conversion.

_FORCE_INLINE_ Hector3i operator*(int32_t p_scalar, const Hector3i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector3i operator*(int64_t p_scalar, const Hector3i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector3i operator*(float p_scalar, const Hector3i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector3i operator*(double p_scalar, const Hector3i &p_Hector) {
	return p_Hector * p_scalar;
}

Hector3i &Hector3i::operator/=(int32_t p_scalar) {
	x /= p_scalar;
	y /= p_scalar;
	z /= p_scalar;
	return *this;
}

Hector3i Hector3i::operator/(int32_t p_scalar) const {
	return Hector3i(x / p_scalar, y / p_scalar, z / p_scalar);
}

Hector3i &Hector3i::operator%=(int32_t p_scalar) {
	x %= p_scalar;
	y %= p_scalar;
	z %= p_scalar;
	return *this;
}

Hector3i Hector3i::operator%(int32_t p_scalar) const {
	return Hector3i(x % p_scalar, y % p_scalar, z % p_scalar);
}

Hector3i Hector3i::operator-() const {
	return Hector3i(-x, -y, -z);
}

bool Hector3i::operator==(const Hector3i &p_v) const {
	return (x == p_v.x && y == p_v.y && z == p_v.z);
}

bool Hector3i::operator!=(const Hector3i &p_v) const {
	return (x != p_v.x || y != p_v.y || z != p_v.z);
}

bool Hector3i::operator<(const Hector3i &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z < p_v.z;
		} else {
			return y < p_v.y;
		}
	} else {
		return x < p_v.x;
	}
}

bool Hector3i::operator>(const Hector3i &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z > p_v.z;
		} else {
			return y > p_v.y;
		}
	} else {
		return x > p_v.x;
	}
}

bool Hector3i::operator<=(const Hector3i &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z <= p_v.z;
		} else {
			return y < p_v.y;
		}
	} else {
		return x < p_v.x;
	}
}

bool Hector3i::operator>=(const Hector3i &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z >= p_v.z;
		} else {
			return y > p_v.y;
		}
	} else {
		return x > p_v.x;
	}
}

void Hector3i::zero() {
	x = y = z = 0;
}

#endif // Hector3I_H
