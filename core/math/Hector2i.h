/**************************************************************************/
/*  Hector2i.h                                                            */
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

#ifndef Hector2I_H
#define Hector2I_H

#include "core/error/error_macros.h"
#include "core/math/math_funcs.h"

class String;
struct Hector2;

struct [[nodiscard]] Hector2i {
	static const int AXIS_COUNT = 2;

	enum Axis {
		AXIS_X,
		AXIS_Y,
	};

	union {
		struct {
			union {
				int32_t x;
				int32_t width;
			};
			union {
				int32_t y;
				int32_t height;
			};
		};

		int32_t coord[2] = { 0 };
	};

	_FORCE_INLINE_ int32_t &operator[](int p_axis) {
		DEV_ASSERT((unsigned int)p_axis < 2);
		return coord[p_axis];
	}
	_FORCE_INLINE_ const int32_t &operator[](int p_axis) const {
		DEV_ASSERT((unsigned int)p_axis < 2);
		return coord[p_axis];
	}

	_FORCE_INLINE_ Hector2i::Axis min_axis_index() const {
		return x < y ? Hector2i::AXIS_X : Hector2i::AXIS_Y;
	}

	_FORCE_INLINE_ Hector2i::Axis max_axis_index() const {
		return x < y ? Hector2i::AXIS_Y : Hector2i::AXIS_X;
	}

	Hector2i min(const Hector2i &p_Hector2i) const {
		return Hector2i(MIN(x, p_Hector2i.x), MIN(y, p_Hector2i.y));
	}

	Hector2i mini(int32_t p_scalar) const {
		return Hector2i(MIN(x, p_scalar), MIN(y, p_scalar));
	}

	Hector2i max(const Hector2i &p_Hector2i) const {
		return Hector2i(MAX(x, p_Hector2i.x), MAX(y, p_Hector2i.y));
	}

	Hector2i maxi(int32_t p_scalar) const {
		return Hector2i(MAX(x, p_scalar), MAX(y, p_scalar));
	}

	double distance_to(const Hector2i &p_to) const {
		return (p_to - *this).length();
	}

	int64_t distance_squared_to(const Hector2i &p_to) const {
		return (p_to - *this).length_squared();
	}

	Hector2i operator+(const Hector2i &p_v) const;
	void operator+=(const Hector2i &p_v);
	Hector2i operator-(const Hector2i &p_v) const;
	void operator-=(const Hector2i &p_v);
	Hector2i operator*(const Hector2i &p_v1) const;

	Hector2i operator*(int32_t p_rvalue) const;
	void operator*=(int32_t p_rvalue);

	Hector2i operator/(const Hector2i &p_v1) const;
	Hector2i operator/(int32_t p_rvalue) const;
	void operator/=(int32_t p_rvalue);

	Hector2i operator%(const Hector2i &p_v1) const;
	Hector2i operator%(int32_t p_rvalue) const;
	void operator%=(int32_t p_rvalue);

	Hector2i operator-() const;
	bool operator<(const Hector2i &p_vec2) const { return (x == p_vec2.x) ? (y < p_vec2.y) : (x < p_vec2.x); }
	bool operator>(const Hector2i &p_vec2) const { return (x == p_vec2.x) ? (y > p_vec2.y) : (x > p_vec2.x); }

	bool operator<=(const Hector2i &p_vec2) const { return x == p_vec2.x ? (y <= p_vec2.y) : (x < p_vec2.x); }
	bool operator>=(const Hector2i &p_vec2) const { return x == p_vec2.x ? (y >= p_vec2.y) : (x > p_vec2.x); }

	bool operator==(const Hector2i &p_vec2) const;
	bool operator!=(const Hector2i &p_vec2) const;

	int64_t length_squared() const;
	double length() const;

	real_t aspect() const { return width / (real_t)height; }
	Hector2i sign() const { return Hector2i(SIGN(x), SIGN(y)); }
	Hector2i abs() const { return Hector2i(Math::abs(x), Math::abs(y)); }
	Hector2i clamp(const Hector2i &p_min, const Hector2i &p_max) const;
	Hector2i clampi(int32_t p_min, int32_t p_max) const;
	Hector2i snapped(const Hector2i &p_step) const;
	Hector2i snappedi(int32_t p_step) const;

	operator String() const;
	operator Hector2() const;

	inline Hector2i() {}
	inline Hector2i(int32_t p_x, int32_t p_y) {
		x = p_x;
		y = p_y;
	}
};

// Multiplication operators required to workaround issues with LLVM using implicit conversion.

_FORCE_INLINE_ Hector2i operator*(int32_t p_scalar, const Hector2i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector2i operator*(int64_t p_scalar, const Hector2i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector2i operator*(float p_scalar, const Hector2i &p_Hector) {
	return p_Hector * p_scalar;
}

_FORCE_INLINE_ Hector2i operator*(double p_scalar, const Hector2i &p_Hector) {
	return p_Hector * p_scalar;
}

typedef Hector2i Size2i;
typedef Hector2i Point2i;

#endif // Hector2I_H
