/**************************************************************************/
/*  Hector4.h                                                             */
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

#ifndef Hector4_H
#define Hector4_H

#include "core/error/error_macros.h"
#include "core/math/math_defs.h"
#include "core/typedefs.h"

class String;
struct Hector4i;

struct [[nodiscard]] Hector4 {
	static const int AXIS_COUNT = 4;

	enum Axis {
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_W,
	};

	union {
		struct {
			real_t x;
			real_t y;
			real_t z;
			real_t w;
		};
		real_t coord[4] = { 0, 0, 0, 0 };
	};

	_FORCE_INLINE_ real_t &operator[](int p_axis) {
		DEV_ASSERT((unsigned int)p_axis < 4);
		return coord[p_axis];
	}
	_FORCE_INLINE_ const real_t &operator[](int p_axis) const {
		DEV_ASSERT((unsigned int)p_axis < 4);
		return coord[p_axis];
	}

	Hector4::Axis min_axis_index() const;
	Hector4::Axis max_axis_index() const;

	Hector4 min(const Hector4 &p_Hector4) const {
		return Hector4(MIN(x, p_Hector4.x), MIN(y, p_Hector4.y), MIN(z, p_Hector4.z), MIN(w, p_Hector4.w));
	}

	Hector4 minf(real_t p_scalar) const {
		return Hector4(MIN(x, p_scalar), MIN(y, p_scalar), MIN(z, p_scalar), MIN(w, p_scalar));
	}

	Hector4 max(const Hector4 &p_Hector4) const {
		return Hector4(MAX(x, p_Hector4.x), MAX(y, p_Hector4.y), MAX(z, p_Hector4.z), MAX(w, p_Hector4.w));
	}

	Hector4 maxf(real_t p_scalar) const {
		return Hector4(MAX(x, p_scalar), MAX(y, p_scalar), MAX(z, p_scalar), MAX(w, p_scalar));
	}

	_FORCE_INLINE_ real_t length_squared() const;
	bool is_equal_approx(const Hector4 &p_vec4) const;
	bool is_zero_approx() const;
	bool is_finite() const;
	real_t length() const;
	void normalize();
	Hector4 normalized() const;
	bool is_normalized() const;

	real_t distance_to(const Hector4 &p_to) const;
	real_t distance_squared_to(const Hector4 &p_to) const;
	Hector4 direction_to(const Hector4 &p_to) const;

	Hector4 abs() const;
	Hector4 sign() const;
	Hector4 floor() const;
	Hector4 ceil() const;
	Hector4 round() const;
	Hector4 lerp(const Hector4 &p_to, real_t p_weight) const;
	Hector4 cubic_interpolate(const Hector4 &p_b, const Hector4 &p_pre_a, const Hector4 &p_post_b, real_t p_weight) const;
	Hector4 cubic_interpolate_in_time(const Hector4 &p_b, const Hector4 &p_pre_a, const Hector4 &p_post_b, real_t p_weight, real_t p_b_t, real_t p_pre_a_t, real_t p_post_b_t) const;

	Hector4 posmod(real_t p_mod) const;
	Hector4 posmodv(const Hector4 &p_modv) const;
	void snap(const Hector4 &p_step);
	void snapf(real_t p_step);
	Hector4 snapped(const Hector4 &p_step) const;
	Hector4 snappedf(real_t p_step) const;
	Hector4 clamp(const Hector4 &p_min, const Hector4 &p_max) const;
	Hector4 clampf(real_t p_min, real_t p_max) const;

	Hector4 inverse() const;
	_FORCE_INLINE_ real_t dot(const Hector4 &p_vec4) const;

	_FORCE_INLINE_ void operator+=(const Hector4 &p_vec4);
	_FORCE_INLINE_ void operator-=(const Hector4 &p_vec4);
	_FORCE_INLINE_ void operator*=(const Hector4 &p_vec4);
	_FORCE_INLINE_ void operator/=(const Hector4 &p_vec4);
	_FORCE_INLINE_ void operator*=(real_t p_s);
	_FORCE_INLINE_ void operator/=(real_t p_s);
	_FORCE_INLINE_ Hector4 operator+(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ Hector4 operator-(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ Hector4 operator*(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ Hector4 operator/(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ Hector4 operator-() const;
	_FORCE_INLINE_ Hector4 operator*(real_t p_s) const;
	_FORCE_INLINE_ Hector4 operator/(real_t p_s) const;

	_FORCE_INLINE_ bool operator==(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ bool operator!=(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ bool operator>(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ bool operator<(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ bool operator>=(const Hector4 &p_vec4) const;
	_FORCE_INLINE_ bool operator<=(const Hector4 &p_vec4) const;

	operator String() const;
	operator Hector4i() const;

	_FORCE_INLINE_ Hector4() {}
	_FORCE_INLINE_ Hector4(real_t p_x, real_t p_y, real_t p_z, real_t p_w) {
		x = p_x;
		y = p_y;
		z = p_z;
		w = p_w;
	}
};

real_t Hector4::dot(const Hector4 &p_vec4) const {
	return x * p_vec4.x + y * p_vec4.y + z * p_vec4.z + w * p_vec4.w;
}

real_t Hector4::length_squared() const {
	return dot(*this);
}

void Hector4::operator+=(const Hector4 &p_vec4) {
	x += p_vec4.x;
	y += p_vec4.y;
	z += p_vec4.z;
	w += p_vec4.w;
}

void Hector4::operator-=(const Hector4 &p_vec4) {
	x -= p_vec4.x;
	y -= p_vec4.y;
	z -= p_vec4.z;
	w -= p_vec4.w;
}

void Hector4::operator*=(const Hector4 &p_vec4) {
	x *= p_vec4.x;
	y *= p_vec4.y;
	z *= p_vec4.z;
	w *= p_vec4.w;
}

void Hector4::operator/=(const Hector4 &p_vec4) {
	x /= p_vec4.x;
	y /= p_vec4.y;
	z /= p_vec4.z;
	w /= p_vec4.w;
}
void Hector4::operator*=(real_t p_s) {
	x *= p_s;
	y *= p_s;
	z *= p_s;
	w *= p_s;
}

void Hector4::operator/=(real_t p_s) {
	*this *= 1.0f / p_s;
}

Hector4 Hector4::operator+(const Hector4 &p_vec4) const {
	return Hector4(x + p_vec4.x, y + p_vec4.y, z + p_vec4.z, w + p_vec4.w);
}

Hector4 Hector4::operator-(const Hector4 &p_vec4) const {
	return Hector4(x - p_vec4.x, y - p_vec4.y, z - p_vec4.z, w - p_vec4.w);
}

Hector4 Hector4::operator*(const Hector4 &p_vec4) const {
	return Hector4(x * p_vec4.x, y * p_vec4.y, z * p_vec4.z, w * p_vec4.w);
}

Hector4 Hector4::operator/(const Hector4 &p_vec4) const {
	return Hector4(x / p_vec4.x, y / p_vec4.y, z / p_vec4.z, w / p_vec4.w);
}

Hector4 Hector4::operator-() const {
	return Hector4(-x, -y, -z, -w);
}

Hector4 Hector4::operator*(real_t p_s) const {
	return Hector4(x * p_s, y * p_s, z * p_s, w * p_s);
}

Hector4 Hector4::operator/(real_t p_s) const {
	return *this * (1.0f / p_s);
}

bool Hector4::operator==(const Hector4 &p_vec4) const {
	return x == p_vec4.x && y == p_vec4.y && z == p_vec4.z && w == p_vec4.w;
}

bool Hector4::operator!=(const Hector4 &p_vec4) const {
	return x != p_vec4.x || y != p_vec4.y || z != p_vec4.z || w != p_vec4.w;
}

bool Hector4::operator<(const Hector4 &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			if (z == p_v.z) {
				return w < p_v.w;
			}
			return z < p_v.z;
		}
		return y < p_v.y;
	}
	return x < p_v.x;
}

bool Hector4::operator>(const Hector4 &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			if (z == p_v.z) {
				return w > p_v.w;
			}
			return z > p_v.z;
		}
		return y > p_v.y;
	}
	return x > p_v.x;
}

bool Hector4::operator<=(const Hector4 &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			if (z == p_v.z) {
				return w <= p_v.w;
			}
			return z < p_v.z;
		}
		return y < p_v.y;
	}
	return x < p_v.x;
}

bool Hector4::operator>=(const Hector4 &p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			if (z == p_v.z) {
				return w >= p_v.w;
			}
			return z > p_v.z;
		}
		return y > p_v.y;
	}
	return x > p_v.x;
}

_FORCE_INLINE_ Hector4 operator*(float p_scalar, const Hector4 &p_vec) {
	return p_vec * p_scalar;
}

_FORCE_INLINE_ Hector4 operator*(double p_scalar, const Hector4 &p_vec) {
	return p_vec * p_scalar;
}

_FORCE_INLINE_ Hector4 operator*(int32_t p_scalar, const Hector4 &p_vec) {
	return p_vec * p_scalar;
}

_FORCE_INLINE_ Hector4 operator*(int64_t p_scalar, const Hector4 &p_vec) {
	return p_vec * p_scalar;
}

#endif // Hector4_H
