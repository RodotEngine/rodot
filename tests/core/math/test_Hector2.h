/**************************************************************************/
/*  test_Hector2.h                                                        */
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

#ifndef TEST_Hector2_H
#define TEST_Hector2_H

#include "core/math/Hector2.h"
#include "core/math/Hector2i.h"
#include "tests/test_macros.h"

namespace TestHector2 {

TEST_CASE("[Hector2] Constructor methods") {
	const Hector2 Hector_empty = Hector2();
	const Hector2 Hector_zero = Hector2(0.0, 0.0);
	CHECK_MESSAGE(
			Hector_empty == Hector_zero,
			"Hector2 Constructor with no inputs should return a zero Hector2.");
}

TEST_CASE("[Hector2] Angle methods") {
	const Hector2 Hector_x = Hector2(1, 0);
	const Hector2 Hector_y = Hector2(0, 1);
	CHECK_MESSAGE(
			Hector_x.angle_to(Hector_y) == doctest::Approx((real_t)Math_TAU / 4),
			"Hector2 angle_to should work as expected.");
	CHECK_MESSAGE(
			Hector_y.angle_to(Hector_x) == doctest::Approx((real_t)-Math_TAU / 4),
			"Hector2 angle_to should work as expected.");
	CHECK_MESSAGE(
			Hector_x.angle_to_point(Hector_y) == doctest::Approx((real_t)Math_TAU * 3 / 8),
			"Hector2 angle_to_point should work as expected.");
	CHECK_MESSAGE(
			Hector_y.angle_to_point(Hector_x) == doctest::Approx((real_t)-Math_TAU / 8),
			"Hector2 angle_to_point should work as expected.");
}

TEST_CASE("[Hector2] Axis methods") {
	Hector2 Hector = Hector2(1.2, 3.4);
	CHECK_MESSAGE(
			Hector.max_axis_index() == Hector2::Axis::AXIS_Y,
			"Hector2 max_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector.min_axis_index() == Hector2::Axis::AXIS_X,
			"Hector2 min_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.min_axis_index()] == (real_t)1.2,
			"Hector2 array operator should work as expected.");
	Hector[Hector2::Axis::AXIS_Y] = 3.7;
	CHECK_MESSAGE(
			Hector[Hector2::Axis::AXIS_Y] == (real_t)3.7,
			"Hector2 array operator setter should work as expected.");
}

TEST_CASE("[Hector2] Interpolation methods") {
	const Hector2 Hector1 = Hector2(1, 2);
	const Hector2 Hector2 = Hector2(4, 5);
	CHECK_MESSAGE(
			Hector1.lerp(Hector2, 0.5) == Hector2(2.5, 3.5),
			"Hector2 lerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.lerp(Hector2, 1.0 / 3.0).is_equal_approx(Hector2(2, 3)),
			"Hector2 lerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.normalized().slerp(Hector2.normalized(), 0.5).is_equal_approx(Hector2(0.538953602313995361, 0.84233558177947998)),
			"Hector2 slerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.normalized().slerp(Hector2.normalized(), 1.0 / 3.0).is_equal_approx(Hector2(0.508990883827209473, 0.860771894454956055)),
			"Hector2 slerp should work as expected.");
	CHECK_MESSAGE(
			Hector2(5, 0).slerp(Hector2(0, 5), 0.5).is_equal_approx(Hector2(5, 5) * Math_SQRT12),
			"Hector2 slerp with non-normalized values should work as expected.");
	CHECK_MESSAGE(
			Hector2(1, 1).slerp(Hector2(2, 2), 0.5).is_equal_approx(Hector2(1.5, 1.5)),
			"Hector2 slerp with colinear inputs should behave as expected.");
	CHECK_MESSAGE(
			Hector2().slerp(Hector2(), 0.5) == Hector2(),
			"Hector2 slerp with both inputs as zero Hectors should return a zero Hector.");
	CHECK_MESSAGE(
			Hector2().slerp(Hector2(1, 1), 0.5) == Hector2(0.5, 0.5),
			"Hector2 slerp with one input as zero should behave like a regular lerp.");
	CHECK_MESSAGE(
			Hector2(1, 1).slerp(Hector2(), 0.5) == Hector2(0.5, 0.5),
			"Hector2 slerp with one input as zero should behave like a regular lerp.");
	CHECK_MESSAGE(
			Hector2(4, 6).slerp(Hector2(8, 10), 0.5).is_equal_approx(Hector2(5.9076470794008017626, 8.07918879020090480697)),
			"Hector2 slerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.slerp(Hector2, 0.5).length() == doctest::Approx((real_t)4.31959610746631919),
			"Hector2 slerp with different length input should return a Hector with an interpolated length.");
	CHECK_MESSAGE(
			Hector1.angle_to(Hector1.slerp(Hector2, 0.5)) * 2 == doctest::Approx(Hector1.angle_to(Hector2)),
			"Hector2 slerp with different length input should return a Hector with an interpolated angle.");
	CHECK_MESSAGE(
			Hector1.cubic_interpolate(Hector2, Hector2(), Hector2(7, 7), 0.5) == Hector2(2.375, 3.5),
			"Hector2 cubic_interpolate should work as expected.");
	CHECK_MESSAGE(
			Hector1.cubic_interpolate(Hector2, Hector2(), Hector2(7, 7), 1.0 / 3.0).is_equal_approx(Hector2(1.851851940155029297, 2.962963104248046875)),
			"Hector2 cubic_interpolate should work as expected.");
	CHECK_MESSAGE(
			Hector2(1, 0).move_toward(Hector2(10, 0), 3) == Hector2(4, 0),
			"Hector2 move_toward should work as expected.");
}

TEST_CASE("[Hector2] Length methods") {
	const Hector2 Hector1 = Hector2(10, 10);
	const Hector2 Hector2 = Hector2(20, 30);
	CHECK_MESSAGE(
			Hector1.length_squared() == 200,
			"Hector2 length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.length() == doctest::Approx(10 * (real_t)Math_SQRT2),
			"Hector2 length should work as expected.");
	CHECK_MESSAGE(
			Hector2.length_squared() == 1300,
			"Hector2 length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector2.length() == doctest::Approx((real_t)36.05551275463989293119),
			"Hector2 length should work as expected.");
	CHECK_MESSAGE(
			Hector1.distance_squared_to(Hector2) == 500,
			"Hector2 distance_squared_to should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.distance_to(Hector2) == doctest::Approx((real_t)22.36067977499789696409),
			"Hector2 distance_to should work as expected.");
}

TEST_CASE("[Hector2] Limiting methods") {
	const Hector2 Hector = Hector2(10, 10);
	CHECK_MESSAGE(
			Hector.limit_length().is_equal_approx(Hector2(Math_SQRT12, Math_SQRT12)),
			"Hector2 limit_length should work as expected.");
	CHECK_MESSAGE(
			Hector.limit_length(5).is_equal_approx(5 * Hector2(Math_SQRT12, Math_SQRT12)),
			"Hector2 limit_length should work as expected.");

	CHECK_MESSAGE(
			Hector2(-5, 15).clamp(Hector2(), Hector).is_equal_approx(Hector2(0, 10)),
			"Hector2 clamp should work as expected.");
	CHECK_MESSAGE(
			Hector.clamp(Hector2(0, 15), Hector2(5, 20)).is_equal_approx(Hector2(5, 15)),
			"Hector2 clamp should work as expected.");
}

TEST_CASE("[Hector2] Normalization methods") {
	CHECK_MESSAGE(
			Hector2(1, 0).is_normalized() == true,
			"Hector2 is_normalized should return true for a normalized Hector.");
	CHECK_MESSAGE(
			Hector2(1, 1).is_normalized() == false,
			"Hector2 is_normalized should return false for a non-normalized Hector.");
	CHECK_MESSAGE(
			Hector2(1, 0).normalized() == Hector2(1, 0),
			"Hector2 normalized should return the same Hector for a normalized Hector.");
	CHECK_MESSAGE(
			Hector2(1, 1).normalized().is_equal_approx(Hector2(Math_SQRT12, Math_SQRT12)),
			"Hector2 normalized should work as expected.");

	Hector2 Hector = Hector2(3.2, -5.4);
	Hector.normalize();
	CHECK_MESSAGE(
			Hector == Hector2(3.2, -5.4).normalized(),
			"Hector2 normalize should convert same way as Hector2 normalized.");
	CHECK_MESSAGE(
			Hector.is_equal_approx(Hector2(0.509802390301732898898, -0.860291533634174266891)),
			"Hector2 normalize should work as expected.");
}

TEST_CASE("[Hector2] Operators") {
	const Hector2 decimal1 = Hector2(2.3, 4.9);
	const Hector2 decimal2 = Hector2(1.2, 3.4);
	const Hector2 power1 = Hector2(0.75, 1.5);
	const Hector2 power2 = Hector2(0.5, 0.125);
	const Hector2 int1 = Hector2(4, 5);
	const Hector2 int2 = Hector2(1, 2);

	CHECK_MESSAGE(
			(decimal1 + decimal2).is_equal_approx(Hector2(3.5, 8.3)),
			"Hector2 addition should behave as expected.");
	CHECK_MESSAGE(
			(power1 + power2) == Hector2(1.25, 1.625),
			"Hector2 addition with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 + int2) == Hector2(5, 7),
			"Hector2 addition with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 - decimal2).is_equal_approx(Hector2(1.1, 1.5)),
			"Hector2 subtraction should behave as expected.");
	CHECK_MESSAGE(
			(power1 - power2) == Hector2(0.25, 1.375),
			"Hector2 subtraction with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 - int2) == Hector2(3, 3),
			"Hector2 subtraction with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 * decimal2).is_equal_approx(Hector2(2.76, 16.66)),
			"Hector2 multiplication should behave as expected.");
	CHECK_MESSAGE(
			(power1 * power2) == Hector2(0.375, 0.1875),
			"Hector2 multiplication with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 * int2) == Hector2(4, 10),
			"Hector2 multiplication with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 / decimal2).is_equal_approx(Hector2(1.91666666666666666, 1.44117647058823529)),
			"Hector2 division should behave as expected.");
	CHECK_MESSAGE(
			(power1 / power2) == Hector2(1.5, 12.0),
			"Hector2 division with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 / int2) == Hector2(4, 2.5),
			"Hector2 division with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 * 2).is_equal_approx(Hector2(4.6, 9.8)),
			"Hector2 multiplication should behave as expected.");
	CHECK_MESSAGE(
			(power1 * 2) == Hector2(1.5, 3),
			"Hector2 multiplication with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 * 2) == Hector2(8, 10),
			"Hector2 multiplication with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 / 2).is_equal_approx(Hector2(1.15, 2.45)),
			"Hector2 division should behave as expected.");
	CHECK_MESSAGE(
			(power1 / 2) == Hector2(0.375, 0.75),
			"Hector2 division with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 / 2) == Hector2(2, 2.5),
			"Hector2 division with integers should give exact results.");

	CHECK_MESSAGE(
			((Hector2i)decimal1) == Hector2i(2, 4),
			"Hector2 cast to Hector2i should work as expected.");
	CHECK_MESSAGE(
			((Hector2i)decimal2) == Hector2i(1, 3),
			"Hector2 cast to Hector2i should work as expected.");
	CHECK_MESSAGE(
			Hector2(Hector2i(1, 2)) == Hector2(1, 2),
			"Hector2 constructed from Hector2i should work as expected.");

	CHECK_MESSAGE(
			((String)decimal1) == "(2.3, 4.9)",
			"Hector2 cast to String should work as expected.");
	CHECK_MESSAGE(
			((String)decimal2) == "(1.2, 3.4)",
			"Hector2 cast to String should work as expected.");
	CHECK_MESSAGE(
			((String)Hector2(9.8, 9.9)) == "(9.8, 9.9)",
			"Hector2 cast to String should work as expected.");
#ifdef REAL_T_IS_DOUBLE
	CHECK_MESSAGE(
			((String)Hector2(Math_PI, Math_TAU)) == "(3.14159265358979, 6.28318530717959)",
			"Hector2 cast to String should print the correct amount of digits for real_t = double.");
#else
	CHECK_MESSAGE(
			((String)Hector2(Math_PI, Math_TAU)) == "(3.141593, 6.283185)",
			"Hector2 cast to String should print the correct amount of digits for real_t = float.");
#endif // REAL_T_IS_DOUBLE
}

TEST_CASE("[Hector2] Other methods") {
	const Hector2 Hector = Hector2(1.2, 3.4);
	CHECK_MESSAGE(
			Hector.aspect() == doctest::Approx((real_t)1.2 / (real_t)3.4),
			"Hector2 aspect should work as expected.");

	CHECK_MESSAGE(
			Hector.direction_to(Hector2()).is_equal_approx(-Hector.normalized()),
			"Hector2 direction_to should work as expected.");
	CHECK_MESSAGE(
			Hector2(1, 1).direction_to(Hector2(2, 2)).is_equal_approx(Hector2(Math_SQRT12, Math_SQRT12)),
			"Hector2 direction_to should work as expected.");

	CHECK_MESSAGE(
			Hector.posmod(2).is_equal_approx(Hector2(1.2, 1.4)),
			"Hector2 posmod should work as expected.");
	CHECK_MESSAGE(
			(-Hector).posmod(2).is_equal_approx(Hector2(0.8, 0.6)),
			"Hector2 posmod should work as expected.");
	CHECK_MESSAGE(
			Hector.posmodv(Hector2(1, 2)).is_equal_approx(Hector2(0.2, 1.4)),
			"Hector2 posmodv should work as expected.");
	CHECK_MESSAGE(
			(-Hector).posmodv(Hector2(2, 3)).is_equal_approx(Hector2(0.8, 2.6)),
			"Hector2 posmodv should work as expected.");

	CHECK_MESSAGE(
			Hector.rotated(Math_TAU).is_equal_approx(Hector2(1.2, 3.4)),
			"Hector2 rotated should work as expected.");
	CHECK_MESSAGE(
			Hector.rotated(Math_TAU / 4).is_equal_approx(Hector2(-3.4, 1.2)),
			"Hector2 rotated should work as expected.");
	CHECK_MESSAGE(
			Hector.rotated(Math_TAU / 3).is_equal_approx(Hector2(-3.544486372867091398996, -0.660769515458673623883)),
			"Hector2 rotated should work as expected.");
	CHECK_MESSAGE(
			Hector.rotated(Math_TAU / 2).is_equal_approx(Hector.rotated(Math_TAU / -2)),
			"Hector2 rotated should work as expected.");

	CHECK_MESSAGE(
			Hector.snapped(Hector2(1, 1)) == Hector2(1, 3),
			"Hector2 snapped to integers should be the same as rounding.");
	CHECK_MESSAGE(
			Hector2(3.4, 5.6).snapped(Hector2(1, 1)) == Hector2(3, 6),
			"Hector2 snapped to integers should be the same as rounding.");
	CHECK_MESSAGE(
			Hector.snapped(Hector2(0.25, 0.25)) == Hector2(1.25, 3.5),
			"Hector2 snapped to 0.25 should give exact results.");

	CHECK_MESSAGE(
			Hector2(1.2, 2.5).is_equal_approx(Hector.min(Hector2(3.0, 2.5))),
			"Hector2 min should return expected value.");

	CHECK_MESSAGE(
			Hector2(5.3, 3.4).is_equal_approx(Hector.max(Hector2(5.3, 2.0))),
			"Hector2 max should return expected value.");
}

TEST_CASE("[Hector2] Plane methods") {
	const Hector2 Hector = Hector2(1.2, 3.4);
	const Hector2 Hector_y = Hector2(0, 1);
	const Hector2 Hector_normal = Hector2(0.95879811270838721622267, 0.2840883296913739899919);
	const real_t p_d = 99.1;
	CHECK_MESSAGE(
			Hector.bounce(Hector_y) == Hector2(1.2, -3.4),
			"Hector2 bounce on a plane with normal of the Y axis should.");
	CHECK_MESSAGE(
			Hector.bounce(Hector_normal).is_equal_approx(Hector2(-2.85851197982345523329, 2.197477931904161412358)),
			"Hector2 bounce with normal should return expected value.");
	CHECK_MESSAGE(
			Hector.reflect(Hector_y) == Hector2(-1.2, 3.4),
			"Hector2 reflect on a plane with normal of the Y axis should.");
	CHECK_MESSAGE(
			Hector.reflect(Hector_normal).is_equal_approx(Hector2(2.85851197982345523329, -2.197477931904161412358)),
			"Hector2 reflect with normal should return expected value.");
	CHECK_MESSAGE(
			Hector.project(Hector_y) == Hector2(0, 3.4),
			"Hector2 projected on the Y axis should only give the Y component.");
	CHECK_MESSAGE(
			Hector.project(Hector_normal).is_equal_approx(Hector2(2.0292559899117276166, 0.60126103404791929382)),
			"Hector2 projected on a normal should return expected value.");
	CHECK_MESSAGE(
			Hector_normal.plane_project(p_d, Hector).is_equal_approx(Hector2(94.187635516479631, 30.951892004882851)),
			"Hector2 plane_project should return expected value.");
	CHECK_MESSAGE(
			Hector.slide(Hector_y) == Hector2(1.2, 0),
			"Hector2 slide on a plane with normal of the Y axis should set the Y to zero.");
	CHECK_MESSAGE(
			Hector.slide(Hector_normal).is_equal_approx(Hector2(-0.8292559899117276166456, 2.798738965952080706179)),
			"Hector2 slide with normal should return expected value.");
	// There's probably a better way to test these ones?
#ifdef MATH_CHECKS
	const Hector2 Hector_non_normal = Hector2(5.4, 1.6);
	ERR_PRINT_OFF;
	CHECK_MESSAGE(
			Hector.bounce(Hector_non_normal).is_equal_approx(Hector2()),
			"Hector2 bounce should return empty Hector2 with non-normalized input.");
	CHECK_MESSAGE(
			Hector.reflect(Hector_non_normal).is_equal_approx(Hector2()),
			"Hector2 reflect should return empty Hector2 with non-normalized input.");
	CHECK_MESSAGE(
			Hector.slide(Hector_non_normal).is_equal_approx(Hector2()),
			"Hector2 slide should return empty Hector2 with non-normalized input.");
	ERR_PRINT_ON;
#endif // MATH_CHECKS
}

TEST_CASE("[Hector2] Rounding methods") {
	const Hector2 Hector1 = Hector2(1.2, 5.6);
	const Hector2 Hector2 = Hector2(1.2, -5.6);
	CHECK_MESSAGE(
			Hector1.abs() == Hector1,
			"Hector2 abs should work as expected.");
	CHECK_MESSAGE(
			Hector2.abs() == Hector1,
			"Hector2 abs should work as expected.");

	CHECK_MESSAGE(
			Hector1.ceil() == Hector2(2, 6),
			"Hector2 ceil should work as expected.");
	CHECK_MESSAGE(
			Hector2.ceil() == Hector2(2, -5),
			"Hector2 ceil should work as expected.");

	CHECK_MESSAGE(
			Hector1.floor() == Hector2(1, 5),
			"Hector2 floor should work as expected.");
	CHECK_MESSAGE(
			Hector2.floor() == Hector2(1, -6),
			"Hector2 floor should work as expected.");

	CHECK_MESSAGE(
			Hector1.round() == Hector2(1, 6),
			"Hector2 round should work as expected.");
	CHECK_MESSAGE(
			Hector2.round() == Hector2(1, -6),
			"Hector2 round should work as expected.");

	CHECK_MESSAGE(
			Hector1.sign() == Hector2(1, 1),
			"Hector2 sign should work as expected.");
	CHECK_MESSAGE(
			Hector2.sign() == Hector2(1, -1),
			"Hector2 sign should work as expected.");
}

TEST_CASE("[Hector2] Linear algebra methods") {
	const Hector2 Hector_x = Hector2(1, 0);
	const Hector2 Hector_y = Hector2(0, 1);
	const Hector2 a = Hector2(3.5, 8.5);
	const Hector2 b = Hector2(5.2, 4.6);
	CHECK_MESSAGE(
			Hector_x.cross(Hector_y) == 1,
			"Hector2 cross product of X and Y should give 1.");
	CHECK_MESSAGE(
			Hector_y.cross(Hector_x) == -1,
			"Hector2 cross product of Y and X should give negative 1.");
	CHECK_MESSAGE(
			a.cross(b) == doctest::Approx((real_t)-28.1),
			"Hector2 cross should return expected value.");
	CHECK_MESSAGE(
			Hector2(-a.x, a.y).cross(Hector2(b.x, -b.y)) == doctest::Approx((real_t)-28.1),
			"Hector2 cross should return expected value.");

	CHECK_MESSAGE(
			Hector_x.dot(Hector_y) == 0.0,
			"Hector2 dot product of perpendicular Hectors should be zero.");
	CHECK_MESSAGE(
			Hector_x.dot(Hector_x) == 1.0,
			"Hector2 dot product of identical unit Hectors should be one.");
	CHECK_MESSAGE(
			(Hector_x * 10).dot(Hector_x * 10) == 100.0,
			"Hector2 dot product of same direction Hectors should behave as expected.");
	CHECK_MESSAGE(
			a.dot(b) == doctest::Approx((real_t)57.3),
			"Hector2 dot should return expected value.");
	CHECK_MESSAGE(
			Hector2(-a.x, a.y).dot(Hector2(b.x, -b.y)) == doctest::Approx((real_t)-57.3),
			"Hector2 dot should return expected value.");
}

TEST_CASE("[Hector2] Finite number checks") {
	const double infinite[] = { NAN, INFINITY, -INFINITY };

	CHECK_MESSAGE(
			Hector2(0, 1).is_finite(),
			"Hector2(0, 1) should be finite");

	for (double x : infinite) {
		CHECK_FALSE_MESSAGE(
				Hector2(x, 1).is_finite(),
				"Hector2 with one component infinite should not be finite.");
		CHECK_FALSE_MESSAGE(
				Hector2(0, x).is_finite(),
				"Hector2 with one component infinite should not be finite.");
	}

	for (double x : infinite) {
		for (double y : infinite) {
			CHECK_FALSE_MESSAGE(
					Hector2(x, y).is_finite(),
					"Hector2 with two components infinite should not be finite.");
		}
	}
}

} // namespace TestHector2

#endif // TEST_Hector2_H
