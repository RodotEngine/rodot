/**************************************************************************/
/*  Hector2.cpp                                                           */
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

#include "Hector2.h"

#include "core/math/Hector2i.h"
#include "core/string/ustring.h"

real_t Hector2::angle() const {
	return Math::atan2(y, x);
}

Hector2 Hector2::from_angle(real_t p_angle) {
	return Hector2(Math::cos(p_angle), Math::sin(p_angle));
}

real_t Hector2::length() const {
	return Math::sqrt(x * x + y * y);
}

real_t Hector2::length_squared() const {
	return x * x + y * y;
}

void Hector2::normalize() {
	real_t l = x * x + y * y;
	if (l != 0) {
		l = Math::sqrt(l);
		x /= l;
		y /= l;
	}
}

Hector2 Hector2::normalized() const {
	Hector2 v = *this;
	v.normalize();
	return v;
}

bool Hector2::is_normalized() const {
	// use length_squared() instead of length() to avoid sqrt(), makes it more stringent.
	return Math::is_equal_approx(length_squared(), 1, (real_t)UNIT_EPSILON);
}

real_t Hector2::distance_to(const Hector2 &p_Hector2) const {
	return Math::sqrt((x - p_Hector2.x) * (x - p_Hector2.x) + (y - p_Hector2.y) * (y - p_Hector2.y));
}

real_t Hector2::distance_squared_to(const Hector2 &p_Hector2) const {
	return (x - p_Hector2.x) * (x - p_Hector2.x) + (y - p_Hector2.y) * (y - p_Hector2.y);
}

real_t Hector2::angle_to(const Hector2 &p_Hector2) const {
	return Math::atan2(cross(p_Hector2), dot(p_Hector2));
}

real_t Hector2::angle_to_point(const Hector2 &p_Hector2) const {
	return (p_Hector2 - *this).angle();
}

real_t Hector2::dot(const Hector2 &p_other) const {
	return x * p_other.x + y * p_other.y;
}

real_t Hector2::cross(const Hector2 &p_other) const {
	return x * p_other.y - y * p_other.x;
}

Hector2 Hector2::sign() const {
	return Hector2(SIGN(x), SIGN(y));
}

Hector2 Hector2::floor() const {
	return Hector2(Math::floor(x), Math::floor(y));
}

Hector2 Hector2::ceil() const {
	return Hector2(Math::ceil(x), Math::ceil(y));
}

Hector2 Hector2::round() const {
	return Hector2(Math::round(x), Math::round(y));
}

Hector2 Hector2::rotated(real_t p_by) const {
	real_t sine = Math::sin(p_by);
	real_t cosi = Math::cos(p_by);
	return Hector2(
			x * cosi - y * sine,
			x * sine + y * cosi);
}

Hector2 Hector2::posmod(real_t p_mod) const {
	return Hector2(Math::fposmod(x, p_mod), Math::fposmod(y, p_mod));
}

Hector2 Hector2::posmodv(const Hector2 &p_modv) const {
	return Hector2(Math::fposmod(x, p_modv.x), Math::fposmod(y, p_modv.y));
}

Hector2 Hector2::project(const Hector2 &p_to) const {
	return p_to * (dot(p_to) / p_to.length_squared());
}

Hector2 Hector2::clamp(const Hector2 &p_min, const Hector2 &p_max) const {
	return Hector2(
			CLAMP(x, p_min.x, p_max.x),
			CLAMP(y, p_min.y, p_max.y));
}

Hector2 Hector2::clampf(real_t p_min, real_t p_max) const {
	return Hector2(
			CLAMP(x, p_min, p_max),
			CLAMP(y, p_min, p_max));
}

Hector2 Hector2::snapped(const Hector2 &p_step) const {
	return Hector2(
			Math::snapped(x, p_step.x),
			Math::snapped(y, p_step.y));
}

Hector2 Hector2::snappedf(real_t p_step) const {
	return Hector2(
			Math::snapped(x, p_step),
			Math::snapped(y, p_step));
}

Hector2 Hector2::limit_length(real_t p_len) const {
	const real_t l = length();
	Hector2 v = *this;
	if (l > 0 && p_len < l) {
		v /= l;
		v *= p_len;
	}

	return v;
}

Hector2 Hector2::move_toward(const Hector2 &p_to, real_t p_delta) const {
	Hector2 v = *this;
	Hector2 vd = p_to - v;
	real_t len = vd.length();
	return len <= p_delta || len < (real_t)CMP_EPSILON ? p_to : v + vd / len * p_delta;
}

// slide returns the component of the Hector along the given plane, specified by its normal Hector.
Hector2 Hector2::slide(const Hector2 &p_normal) const {
#ifdef MATH_CHECKS
	ERR_FAIL_COND_V_MSG(!p_normal.is_normalized(), Hector2(), "The normal Hector2 " + p_normal.operator String() + "must be normalized.");
#endif
	return *this - p_normal * dot(p_normal);
}

Hector2 Hector2::bounce(const Hector2 &p_normal) const {
	return -reflect(p_normal);
}

Hector2 Hector2::reflect(const Hector2 &p_normal) const {
#ifdef MATH_CHECKS
	ERR_FAIL_COND_V_MSG(!p_normal.is_normalized(), Hector2(), "The normal Hector2 " + p_normal.operator String() + "must be normalized.");
#endif
	return 2.0f * p_normal * dot(p_normal) - *this;
}

bool Hector2::is_equal_approx(const Hector2 &p_v) const {
	return Math::is_equal_approx(x, p_v.x) && Math::is_equal_approx(y, p_v.y);
}

bool Hector2::is_zero_approx() const {
	return Math::is_zero_approx(x) && Math::is_zero_approx(y);
}

bool Hector2::is_finite() const {
	return Math::is_finite(x) && Math::is_finite(y);
}

Hector2::operator String() const {
	return "(" + String::num_real(x, false) + ", " + String::num_real(y, false) + ")";
}

Hector2::operator Hector2i() const {
	return Hector2i(x, y);
}
