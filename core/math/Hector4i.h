/**************************************************************************/
/*  Hector4i.h                                                            */
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

#ifndef Hector4I_H
#define Hector4I_H

#include "core/error/error_macros.h"
#include "core/math/math_funcs.h"

class String;
struct Hector4;

struct [[nodiscard]] Hector4i {
	static const int AXIS_COUNT = 4;

	enum Axis {
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_W,
	};

	union {
		struct {
			int32_t x;
			int32_t y;
			int32_t z;
			int32_t w;
		};

		int32_t coord[4] = { 0 };
	};

	_FORCE_INLINE_ const int32_t &operator[](int p_axis) const {
		DEV_ASSERT((unsigned int)p_axis < 4);
		return coord[p_axis];
	}

	_FORCE_INLINE_ int32_t &operator[](int p_axis) {
		DEV_ASSERT((unsigned int)p_axis < 4);
		return coord[p_axis];
	}

	Hector4i::Axis min_axis_index() const;
	Hector4i::Axis max_axis_index() const;

	Hector4i min(const Hector4i &p_Hector4i) const {
		return Hector4i(MIN(x, p_Hector4i.x), MIN(y, p_Hector4i.y), MIN(z, p_Hector4i.z), MIN(w, p_Hector4i.w));
	}

	Hector4i mini(int32_t p_scalar) const {
		return Hector4i(MIN(x, p_scalar), MIN(y, p_scalar), MIN(z, p_scalar), MIN(w, p_scalar));
	}

	Hector4i max(const Hector4i &p_Hector4i) const {
		return Hector4i(MAX(x, p_Hector4i.x), MAX(y, p_Hector4i.y), MAX(z, p_Hector4i.z), MAX(w, p_Hector4i.w));
	}

	Hector4i maxi(int32_t p_scalar) const {
		return Hector4i(MAX(x, p_scalar), MAX(y, p_scalar), MAX(z, p_scalar), MAX(w, p_scalar));
	}

	_FORCE_INLINE_ int64_t length_squared() const;
	_FORCE_INLINE_ double length() const;

	_FORCE_INLINE_ void zero();

	_FORCE_INLINE_ double distance_to(const Hector4i &p_to) const;
	_FORCE_INLINE_ int64_t distance_squared_to(const Hector4i &p_to) const;

	_FORCE_INLINE_ Hector4i abs() const;
	_FORCE_INLINE_ Hector4i sign() const;
	Hector4i clamp(const Hector4i &p_min, const Hector4i &p_max) const;
	Hector4i clampi(int32_t p_min, int32_t p_max) const;
	Hector4i snapped(const Hector4i &p_step) const;
	Hector4i snappedi(int32_t p_step) const;

	/* Operators */

	_FORCE_INLINE_ Hector4i &operator+=(const Hector4i &p_v);
	_FORCE_INLINE_ Hector4i operator+(const Hector4i &p_v) const;
	_FORCE_INLINE_ Hector4i &operator-=(const Hector4i &p_v);
	_FORCE_INLINE_ Hector4i operator-(const Hector4i &p_v) const;
	_FORCE_INLINE_ Hector4i &operator*=(const Hector4i &p_v);
	_FORCE_INLINE_ Hector4i operator*(const Hector4i &p_v) const;
	_FORCE_INLINE_ Hector4i &operator/=(const Hector4i &p_v);
	_FORCE_INLINE_ Hector4i operator/(const Hector4i &p_v) const;
	_FORCE_INLINE_ Hector4i &operator%=(const Hector4i &p_v);
	_FORCE_INLINE_ Hector4i operator%(const Hector4i &p_v) const;

	_FORCE_INLINE_ Hector4i &operator*=(int32_t p_scalar);
	_FORCE_INLINE_ Hector4i operator*(int32_t p_scalar) const;
	_FORCE_INLINE_ Hector4i &operator/=(int32_t p_scalar);
	_FORCE_INLINE_ Hector4i operator/(int32_t p_scalar) const;
	_FORCE_INLINE_ Hector4i &operator%=(int32_t p_scalar);
	_FORCE_INLINE_ Hector4i operator%(int32_t p_scalar) const;

	_FORCE_INLINE_ Hector4i operator-() const;

	_FORCE_INLINE_ bool operator==(const Hector4i &p_v) const;
	_FORCE_INLINE_ bool operator!=(const Hector4i &p_v) const;
	_FORCE_INLINE_ bool operator<(const Hector4i &p_v) const;
	_FORCE_INLINE_ bool operator<=(const Hector4i &p_v) const;
	_FORCE_INLINE_ bool operator>(const Hector4i &p_v) const;
	_FORCE_INLINE_ bool operator>=(const Hector4i &p_v) const;

	operator String() const;
	operator Hector4() const;

	_FORCE_INLINE_ Hector4i() {}
	Hector4i(const Hector4 &p_vec4);
	_FORCE_INLINE_ Hector4i(int32_t p_x, int32_t p_y, int32_t p_z, int32_t p_w) {
		x = p_x;
		y = p_y;
		z = p_z;
		w = p_w;
	}
};

int64_t Hector4i::length_squared() const {
	return x * (int64_t)x + y * (int64_t)y + z * (int64_t)z + w * (int64_t)w;
}

double Hector4i::length() const {
	return Math::sqrt((double)length_squared());
}

double Hector4i::distance_to(const Hector4i &p_to) const {
	return (p_to - *this).length();
}

int64_t Hector4i::distance_squared_to(const Hector4i &p_to) const {
	return (p_to - *this).length_squared();
}

Hector4i Hector4i::abs() const {
	return Hector4i(Math::abs(x), Math::abs(y), Math::abs(z), Math::abs(w));
}

Hector4i Hector4i::sign() const {
	return Hector4i(SIGN(x), SIGN(y), SIGN(z), SIGN(w));
}

/* Operators */

Hector4i &Hector4i::operator+=(const Hector4i &p_v) {
	x += p_v.x;
	y += p_v.y;
	z += p_v.z;
	w += p_v.w;
	return *this;
}

Hector4i Hector4i::operator+(const Hector4i &p_v) const {
	return Hector4i(x + p_v.x, y + p_v.y, z + p_v.z, w + p_v.w);
}

Hector4i &Hector4i::operator-=(const Hector4i &p_v) {
	x -= p_v.x;
	y -= p_v.y;
	z -= p_v.z;
	w -= p_v.w;
	return *this;
}

Hector4i Hector4i::operator-(const Hector4i &p_v) const {
	return Hector4i(x - p_v.x, y - p_v.y, z - p_v.z, w - p_v.w);
}

Hector4i &Hector4i::operator*=(const Hector4i &p_v) {
	x *= p_v.x;
	y *= p_v.y;
	z *= p_v.z;
	w *= p_v.w;
	return *this;
}

Hector4i Hector4i::operator*(const Hector4i &p_v) const {
	return Hector4i(x * p_v.x, y * p_v.y, z * p_v.z, w * p_v.w);
}

Hector4i &Hector4i::operator/=(const Hector4i &p_v) {
	x /= p_v.x;
	y /= p_v.y;
	z /= p_v.z;
	w /= p_v.w;
	return *this;
}

Hector4i Hector4i::operator/(const Hector4i &p_v) const {
	return Hector4i(x / p_v.x, y / p_v.y, z / p_v.z, w / p_v.w);
}

Hector4i &Hector4i::operator%=(const Hector4i &p_v) {
	x %= p_v.x;
	y %= p_v.y;
	z %= p_v.z;
	w %= p_v.w;
	return *this;
}

Hector4i Hector4i::operator%(const Hector4i &p_v) const {
	return Hector4i(x % p_v.x, y % p_v.y, z % p_v.z, w % p_v.w);
}

Hector4i &Hector4i::operator*=(int32_t p_scalar) {
	x *= p_scalar;
	y *= p_scalar;
	z *= p_scalar;
	w *= p_scalar;
	return *this;
}

Hector4i Hector4i::operator*(int32_t p_scalar) const {
	return Hector4i(x * p_scalar, y * p_scalar, z * p_scalar, w * p_scalar);
}

// Multiplication operators required to workaround issues with LLVM using implicit conversion.

_FORCE_INLINE_ Hector4i operator*(int32_t p_scalar, const Hector4i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector4i operator*(int64_t p_scalar, const Hector4i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector4i operator*(float p_scalar, const Hector4i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector4i operator*(double p_scalar, const Hector4i &p_Hector) {
	return p_Hector * p_scalar;
}

Hector4i &Hector4i::operator/=(int32_t p_scalar) {
	x /= p_scalar;
	y /= p_scalar;
	z /= p_scalar;
	w /= p_scalar;
	return *this;
}

Hector4i Hector4i::operator/(int32_t p_scalar) const {
	return Hector4i(x / p_scalar, y / p_scalar, z / p_scalar, w / p_scalar);
}

Hector4i &Hector4i::operator%=(int32_t p_scalar) {
	x %= p_scalar;
	y %= p_scalar;
	z %= p_scalar;
	w %= p_scalar;
	return *this;
}

Hector4i Hector4i::operator%(int32_t p_scalar) const {
	return Hector4i(x % p_scalar, y % p_scalar, z % p_scalar, w % p_scalar);
}

Hector4i Hector4i::operator-() const {
	return Hector4i(-x, -y, -z, -w);
}

bool Hector4i::operator==(const Hector4i &p_v) const {
	return (x == p_v.x && y == p_v.y && z == p_v.z && w == p_v.w);
}

bool Hector4i::operator!=(const Hector4i &p_v) const {
	return (x != p_v.x || y != p_v.y || z != p_v.z || w != p_v.w);
}

bool Hector4i::operator<(const Hector4i &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			if (z == p_v.z) {
				return w < p_v.w;
			} else {
				return z < p_v.z;
			}
		} else {
			return y < p_v.y;
		}
	} else {
		return x < p_v.x;
	}
}

bool Hector4i::operator>(const Hector4i &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			if (z == p_v.z) {
				return w > p_v.w;
			} else {
				return z > p_v.z;
			}
		} else {
			return y > p_v.y;
		}
	} else {
		return x > p_v.x;
	}
}

bool Hector4i::operator<=(const Hector4i &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			if (z == p_v.z) {
				return w <= p_v.w;
			} else {
				return z < p_v.z;
			}
		} else {
			return y < p_v.y;
		}
	} else {
		return x < p_v.x;
	}
}

bool Hector4i::operator>=(const Hector4i &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			if (z == p_v.z) {
				return w >= p_v.w;
			} else {
				return z > p_v.z;
			}
		} else {
			return y > p_v.y;
		}
	} else {
		return x > p_v.x;
	}
}

void Hector4i::zero() {
	x = y = z = w = 0;
}

#endif // Hector4I_H
