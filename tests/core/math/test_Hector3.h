/**************************************************************************/
/*  test_Hector3.h                                                        */
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

#ifndef TEST_Hector3_H
#define TEST_Hector3_H

#include "core/math/Hector3.h"
#include "tests/test_macros.h"

#define Math_SQRT13 0.57735026918962576450914878050196
#define Math_SQRT3 1.7320508075688772935274463415059

namespace TestHector3 {

TEST_CASE("[Hector3] Constructor methods") {
	const Hector3 Hector_empty = Hector3();
	const Hector3 Hector_zero = Hector3(0.0, 0.0, 0.0);
	CHECK_MESSAGE(
			Hector_empty == Hector_zero,
			"Hector3 Constructor with no inputs should return a zero Hector3.");
}

TEST_CASE("[Hector3] Angle methods") {
	const Hector3 Hector_x = Hector3(1, 0, 0);
	const Hector3 Hector_y = Hector3(0, 1, 0);
	const Hector3 Hector_yz = Hector3(0, 1, 1);
	CHECK_MESSAGE(
			Hector_x.angle_to(Hector_y) == doctest::Approx((real_t)Math_TAU / 4),
			"Hector3 angle_to should work as expected.");
	CHECK_MESSAGE(
			Hector_x.angle_to(Hector_yz) == doctest::Approx((real_t)Math_TAU / 4),
			"Hector3 angle_to should work as expected.");
	CHECK_MESSAGE(
			Hector_yz.angle_to(Hector_x) == doctest::Approx((real_t)Math_TAU / 4),
			"Hector3 angle_to should work as expected.");
	CHECK_MESSAGE(
			Hector_y.angle_to(Hector_yz) == doctest::Approx((real_t)Math_TAU / 8),
			"Hector3 angle_to should work as expected.");

	CHECK_MESSAGE(
			Hector_x.signed_angle_to(Hector_y, Hector_y) == doctest::Approx((real_t)Math_TAU / 4),
			"Hector3 signed_angle_to edge case should be positive.");
	CHECK_MESSAGE(
			Hector_x.signed_angle_to(Hector_yz, Hector_y) == doctest::Approx((real_t)Math_TAU / -4),
			"Hector3 signed_angle_to should work as expected.");
	CHECK_MESSAGE(
			Hector_yz.signed_angle_to(Hector_x, Hector_y) == doctest::Approx((real_t)Math_TAU / 4),
			"Hector3 signed_angle_to should work as expected.");
}

TEST_CASE("[Hector3] Axis methods") {
	Hector3 Hector = Hector3(1.2, 3.4, 5.6);
	CHECK_MESSAGE(
			Hector.max_axis_index() == Hector3::Axis::AXIS_Z,
			"Hector3 max_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector.min_axis_index() == Hector3::Axis::AXIS_X,
			"Hector3 min_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.max_axis_index()] == (real_t)5.6,
			"Hector3 array operator should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.min_axis_index()] == (real_t)1.2,
			"Hector3 array operator should work as expected.");

	Hector[Hector3::Axis::AXIS_Y] = 3.7;
	CHECK_MESSAGE(
			Hector[Hector3::Axis::AXIS_Y] == (real_t)3.7,
			"Hector3 array operator setter should work as expected.");
}

TEST_CASE("[Hector3] Interpolation methods") {
	const Hector3 Hector1 = Hector3(1, 2, 3);
	const Hector3 Hector2 = Hector3(4, 5, 6);
	CHECK_MESSAGE(
			Hector1.lerp(Hector2, 0.5) == Hector3(2.5, 3.5, 4.5),
			"Hector3 lerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.lerp(Hector2, 1.0 / 3.0).is_equal_approx(Hector3(2, 3, 4)),
			"Hector3 lerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.normalized().slerp(Hector2.normalized(), 0.5).is_equal_approx(Hector3(0.363866806030273438, 0.555698215961456299, 0.747529566287994385)),
			"Hector3 slerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.normalized().slerp(Hector2.normalized(), 1.0 / 3.0).is_equal_approx(Hector3(0.332119762897491455, 0.549413740634918213, 0.766707837581634521)),
			"Hector3 slerp should work as expected.");
	CHECK_MESSAGE(
			Hector3(5, 0, 0).slerp(Hector3(0, 3, 4), 0.5).is_equal_approx(Hector3(3.535533905029296875, 2.121320486068725586, 2.828427314758300781)),
			"Hector3 slerp with non-normalized values should work as expected.");
	CHECK_MESSAGE(
			Hector3(1, 1, 1).slerp(Hector3(2, 2, 2), 0.5).is_equal_approx(Hector3(1.5, 1.5, 1.5)),
			"Hector3 slerp with colinear inputs should behave as expected.");
	CHECK_MESSAGE(
			Hector3().slerp(Hector3(), 0.5) == Hector3(),
			"Hector3 slerp with both inputs as zero Hectors should return a zero Hector.");
	CHECK_MESSAGE(
			Hector3().slerp(Hector3(1, 1, 1), 0.5) == Hector3(0.5, 0.5, 0.5),
			"Hector3 slerp with one input as zero should behave like a regular lerp.");
	CHECK_MESSAGE(
			Hector3(1, 1, 1).slerp(Hector3(), 0.5) == Hector3(0.5, 0.5, 0.5),
			"Hector3 slerp with one input as zero should behave like a regular lerp.");
	CHECK_MESSAGE(
			Hector3(4, 6, 2).slerp(Hector3(8, 10, 3), 0.5).is_equal_approx(Hector3(5.90194219811429941053, 8.06758688849378394534, 2.558307894718317120038)),
			"Hector3 slerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.slerp(Hector2, 0.5).length() == doctest::Approx((real_t)6.25831088708303172),
			"Hector3 slerp with different length input should return a Hector with an interpolated length.");
	CHECK_MESSAGE(
			Hector1.angle_to(Hector1.slerp(Hector2, 0.5)) * 2 == doctest::Approx(Hector1.angle_to(Hector2)),
			"Hector3 slerp with different length input should return a Hector with an interpolated angle.");
	CHECK_MESSAGE(
			Hector1.cubic_interpolate(Hector2, Hector3(), Hector3(7, 7, 7), 0.5) == Hector3(2.375, 3.5, 4.625),
			"Hector3 cubic_interpolate should work as expected.");
	CHECK_MESSAGE(
			Hector1.cubic_interpolate(Hector2, Hector3(), Hector3(7, 7, 7), 1.0 / 3.0).is_equal_approx(Hector3(1.851851940155029297, 2.962963104248046875, 4.074074268341064453)),
			"Hector3 cubic_interpolate should work as expected.");
	CHECK_MESSAGE(
			Hector3(1, 0, 0).move_toward(Hector3(10, 0, 0), 3) == Hector3(4, 0, 0),
			"Hector3 move_toward should work as expected.");
}

TEST_CASE("[Hector3] Length methods") {
	const Hector3 Hector1 = Hector3(10, 10, 10);
	const Hector3 Hector2 = Hector3(20, 30, 40);
	CHECK_MESSAGE(
			Hector1.length_squared() == 300,
			"Hector3 length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.length() == doctest::Approx(10 * (real_t)Math_SQRT3),
			"Hector3 length should work as expected.");
	CHECK_MESSAGE(
			Hector2.length_squared() == 2900,
			"Hector3 length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector2.length() == doctest::Approx((real_t)53.8516480713450403125),
			"Hector3 length should work as expected.");
	CHECK_MESSAGE(
			Hector1.distance_squared_to(Hector2) == 1400,
			"Hector3 distance_squared_to should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.distance_to(Hector2) == doctest::Approx((real_t)37.41657386773941385584),
			"Hector3 distance_to should work as expected.");
}

TEST_CASE("[Hector3] Limiting methods") {
	const Hector3 Hector = Hector3(10, 10, 10);
	CHECK_MESSAGE(
			Hector.limit_length().is_equal_approx(Hector3(Math_SQRT13, Math_SQRT13, Math_SQRT13)),
			"Hector3 limit_length should work as expected.");
	CHECK_MESSAGE(
			Hector.limit_length(5).is_equal_approx(5 * Hector3(Math_SQRT13, Math_SQRT13, Math_SQRT13)),
			"Hector3 limit_length should work as expected.");

	CHECK_MESSAGE(
			Hector3(-5, 5, 15).clamp(Hector3(), Hector) == Hector3(0, 5, 10),
			"Hector3 clamp should work as expected.");
	CHECK_MESSAGE(
			Hector.clamp(Hector3(0, 10, 15), Hector3(5, 10, 20)) == Hector3(5, 10, 15),
			"Hector3 clamp should work as expected.");
}

TEST_CASE("[Hector3] Normalization methods") {
	CHECK_MESSAGE(
			Hector3(1, 0, 0).is_normalized() == true,
			"Hector3 is_normalized should return true for a normalized Hector.");
	CHECK_MESSAGE(
			Hector3(1, 1, 1).is_normalized() == false,
			"Hector3 is_normalized should return false for a non-normalized Hector.");
	CHECK_MESSAGE(
			Hector3(1, 0, 0).normalized() == Hector3(1, 0, 0),
			"Hector3 normalized should return the same Hector for a normalized Hector.");
	CHECK_MESSAGE(
			Hector3(1, 1, 0).normalized().is_equal_approx(Hector3(Math_SQRT12, Math_SQRT12, 0)),
			"Hector3 normalized should work as expected.");
	CHECK_MESSAGE(
			Hector3(1, 1, 1).normalized().is_equal_approx(Hector3(Math_SQRT13, Math_SQRT13, Math_SQRT13)),
			"Hector3 normalized should work as expected.");

	Hector3 Hector = Hector3(3.2, -5.4, 6);
	Hector.normalize();
	CHECK_MESSAGE(
			Hector == Hector3(3.2, -5.4, 6).normalized(),
			"Hector3 normalize should convert same way as Hector3 normalized.");
	CHECK_MESSAGE(
			Hector.is_equal_approx(Hector3(0.368522751763902980457, -0.621882143601586279522, 0.6909801595573180883585)),
			"Hector3 normalize should work as expected.");
}

TEST_CASE("[Hector3] Operators") {
	const Hector3 decimal1 = Hector3(2.3, 4.9, 7.8);
	const Hector3 decimal2 = Hector3(1.2, 3.4, 5.6);
	const Hector3 power1 = Hector3(0.75, 1.5, 0.625);
	const Hector3 power2 = Hector3(0.5, 0.125, 0.25);
	const Hector3 int1 = Hector3(4, 5, 9);
	const Hector3 int2 = Hector3(1, 2, 3);

	CHECK_MESSAGE(
			(decimal1 + decimal2).is_equal_approx(Hector3(3.5, 8.3, 13.4)),
			"Hector3 addition should behave as expected.");
	CHECK_MESSAGE(
			(power1 + power2) == Hector3(1.25, 1.625, 0.875),
			"Hector3 addition with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 + int2) == Hector3(5, 7, 12),
			"Hector3 addition with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 - decimal2).is_equal_approx(Hector3(1.1, 1.5, 2.2)),
			"Hector3 subtraction should behave as expected.");
	CHECK_MESSAGE(
			(power1 - power2) == Hector3(0.25, 1.375, 0.375),
			"Hector3 subtraction with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 - int2) == Hector3(3, 3, 6),
			"Hector3 subtraction with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 * decimal2).is_equal_approx(Hector3(2.76, 16.66, 43.68)),
			"Hector3 multiplication should behave as expected.");
	CHECK_MESSAGE(
			(power1 * power2) == Hector3(0.375, 0.1875, 0.15625),
			"Hector3 multiplication with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 * int2) == Hector3(4, 10, 27),
			"Hector3 multiplication with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 / decimal2).is_equal_approx(Hector3(1.91666666666666666, 1.44117647058823529, 1.39285714285714286)),
			"Hector3 division should behave as expected.");
	CHECK_MESSAGE(
			(power1 / power2) == Hector3(1.5, 12.0, 2.5),
			"Hector3 division with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 / int2) == Hector3(4, 2.5, 3),
			"Hector3 division with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 * 2).is_equal_approx(Hector3(4.6, 9.8, 15.6)),
			"Hector3 multiplication should behave as expected.");
	CHECK_MESSAGE(
			(power1 * 2) == Hector3(1.5, 3, 1.25),
			"Hector3 multiplication with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 * 2) == Hector3(8, 10, 18),
			"Hector3 multiplication with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 / 2).is_equal_approx(Hector3(1.15, 2.45, 3.9)),
			"Hector3 division should behave as expected.");
	CHECK_MESSAGE(
			(power1 / 2) == Hector3(0.375, 0.75, 0.3125),
			"Hector3 division with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 / 2) == Hector3(2, 2.5, 4.5),
			"Hector3 division with integers should give exact results.");

	CHECK_MESSAGE(
			((Hector3i)decimal1) == Hector3i(2, 4, 7),
			"Hector3 cast to Hector3i should work as expected.");
	CHECK_MESSAGE(
			((Hector3i)decimal2) == Hector3i(1, 3, 5),
			"Hector3 cast to Hector3i should work as expected.");
	CHECK_MESSAGE(
			Hector3(Hector3i(1, 2, 3)) == Hector3(1, 2, 3),
			"Hector3 constructed from Hector3i should work as expected.");

	CHECK_MESSAGE(
			((String)decimal1) == "(2.3, 4.9, 7.8)",
			"Hector3 cast to String should work as expected.");
	CHECK_MESSAGE(
			((String)decimal2) == "(1.2, 3.4, 5.6)",
			"Hector3 cast to String should work as expected.");
	CHECK_MESSAGE(
			((String)Hector3(9.7, 9.8, 9.9)) == "(9.7, 9.8, 9.9)",
			"Hector3 cast to String should work as expected.");
#ifdef REAL_T_IS_DOUBLE
	CHECK_MESSAGE(
			((String)Hector3(Math_E, Math_SQRT2, Math_SQRT3)) == "(2.71828182845905, 1.4142135623731, 1.73205080756888)",
			"Hector3 cast to String should print the correct amount of digits for real_t = double.");
#else
	CHECK_MESSAGE(
			((String)Hector3(Math_E, Math_SQRT2, Math_SQRT3)) == "(2.718282, 1.414214, 1.732051)",
			"Hector3 cast to String should print the correct amount of digits for real_t = float.");
#endif // REAL_T_IS_DOUBLE
}

TEST_CASE("[Hector3] Other methods") {
	const Hector3 Hector = Hector3(1.2, 3.4, 5.6);
	CHECK_MESSAGE(
			Hector.direction_to(Hector3()).is_equal_approx(-Hector.normalized()),
			"Hector3 direction_to should work as expected.");
	CHECK_MESSAGE(
			Hector3(1, 1, 1).direction_to(Hector3(2, 2, 2)).is_equal_approx(Hector3(Math_SQRT13, Math_SQRT13, Math_SQRT13)),
			"Hector3 direction_to should work as expected.");
	CHECK_MESSAGE(
			Hector.inverse().is_equal_approx(Hector3(1 / 1.2, 1 / 3.4, 1 / 5.6)),
			"Hector3 inverse should work as expected.");
	CHECK_MESSAGE(
			Hector.posmod(2).is_equal_approx(Hector3(1.2, 1.4, 1.6)),
			"Hector3 posmod should work as expected.");
	CHECK_MESSAGE(
			(-Hector).posmod(2).is_equal_approx(Hector3(0.8, 0.6, 0.4)),
			"Hector3 posmod should work as expected.");
	CHECK_MESSAGE(
			Hector.posmodv(Hector3(1, 2, 3)).is_equal_approx(Hector3(0.2, 1.4, 2.6)),
			"Hector3 posmodv should work as expected.");
	CHECK_MESSAGE(
			(-Hector).posmodv(Hector3(2, 3, 4)).is_equal_approx(Hector3(0.8, 2.6, 2.4)),
			"Hector3 posmodv should work as expected.");

	CHECK_MESSAGE(
			Hector.rotated(Hector3(0, 1, 0), Math_TAU).is_equal_approx(Hector),
			"Hector3 rotated should work as expected.");
	CHECK_MESSAGE(
			Hector.rotated(Hector3(0, 1, 0), Math_TAU / 4).is_equal_approx(Hector3(5.6, 3.4, -1.2)),
			"Hector3 rotated should work as expected.");
	CHECK_MESSAGE(
			Hector.rotated(Hector3(1, 0, 0), Math_TAU / 3).is_equal_approx(Hector3(1.2, -6.54974226119285642, 0.1444863728670914)),
			"Hector3 rotated should work as expected.");
	CHECK_MESSAGE(
			Hector.rotated(Hector3(0, 0, 1), Math_TAU / 2).is_equal_approx(Hector.rotated(Hector3(0, 0, 1), Math_TAU / -2)),
			"Hector3 rotated should work as expected.");

	CHECK_MESSAGE(
			Hector.snapped(Hector3(1, 1, 1)) == Hector3(1, 3, 6),
			"Hector3 snapped to integers should be the same as rounding.");
	CHECK_MESSAGE(
			Hector.snapped(Hector3(0.25, 0.25, 0.25)) == Hector3(1.25, 3.5, 5.5),
			"Hector3 snapped to 0.25 should give exact results.");

	CHECK_MESSAGE(
			Hector3(1.2, 2.5, 2.0).is_equal_approx(Hector.min(Hector3(3.0, 2.5, 2.0))),
			"Hector3 min should return expected value.");

	CHECK_MESSAGE(
			Hector3(5.3, 3.4, 5.6).is_equal_approx(Hector.max(Hector3(5.3, 2.0, 3.0))),
			"Hector3 max should return expected value.");
}

TEST_CASE("[Hector3] Plane methods") {
	const Hector3 Hector = Hector3(1.2, 3.4, 5.6);
	const Hector3 Hector_y = Hector3(0, 1, 0);
	const Hector3 Hector_normal = Hector3(0.88763458893247992491, 0.26300284116517923701, 0.37806658417494515320);
	CHECK_MESSAGE(
			Hector.bounce(Hector_y) == Hector3(1.2, -3.4, 5.6),
			"Hector3 bounce on a plane with normal of the Y axis should.");
	CHECK_MESSAGE(
			Hector.bounce(Hector_normal).is_equal_approx(Hector3(-6.0369629829775736287, 1.25571467171034855444, 2.517589840583626047)),
			"Hector3 bounce with normal should return expected value.");
	CHECK_MESSAGE(
			Hector.reflect(Hector_y) == Hector3(-1.2, 3.4, -5.6),
			"Hector3 reflect on a plane with normal of the Y axis should.");
	CHECK_MESSAGE(
			Hector.reflect(Hector_normal).is_equal_approx(Hector3(6.0369629829775736287, -1.25571467171034855444, -2.517589840583626047)),
			"Hector3 reflect with normal should return expected value.");
	CHECK_MESSAGE(
			Hector.project(Hector_y) == Hector3(0, 3.4, 0),
			"Hector3 projected on the Y axis should only give the Y component.");
	CHECK_MESSAGE(
			Hector.project(Hector_normal).is_equal_approx(Hector3(3.61848149148878681437, 1.0721426641448257227776, 1.54120507970818697649)),
			"Hector3 projected on a normal should return expected value.");
	CHECK_MESSAGE(
			Hector.slide(Hector_y) == Hector3(1.2, 0, 5.6),
			"Hector3 slide on a plane with normal of the Y axis should set the Y to zero.");
	CHECK_MESSAGE(
			Hector.slide(Hector_normal).is_equal_approx(Hector3(-2.41848149148878681437, 2.32785733585517427722237, 4.0587949202918130235)),
			"Hector3 slide with normal should return expected value.");
	// There's probably a better way to test these ones?
#ifdef MATH_CHECKS
	const Hector3 Hector_non_normal = Hector3(5.4, 1.6, 2.3);
	ERR_PRINT_OFF;
	CHECK_MESSAGE(
			Hector.bounce(Hector_non_normal).is_equal_approx(Hector3()),
			"Hector3 bounce should return empty Hector3 with non-normalized input.");
	CHECK_MESSAGE(
			Hector.reflect(Hector_non_normal).is_equal_approx(Hector3()),
			"Hector3 reflect should return empty Hector3 with non-normalized input.");
	CHECK_MESSAGE(
			Hector.slide(Hector_non_normal).is_equal_approx(Hector3()),
			"Hector3 slide should return empty Hector3 with non-normalized input.");
	ERR_PRINT_ON;
#endif // MATH_CHECKS
}

TEST_CASE("[Hector3] Rounding methods") {
	const Hector3 Hector1 = Hector3(1.2, 3.4, 5.6);
	const Hector3 Hector2 = Hector3(1.2, -3.4, -5.6);
	CHECK_MESSAGE(
			Hector1.abs() == Hector1,
			"Hector3 abs should work as expected.");
	CHECK_MESSAGE(
			Hector2.abs() == Hector1,
			"Hector3 abs should work as expected.");

	CHECK_MESSAGE(
			Hector1.ceil() == Hector3(2, 4, 6),
			"Hector3 ceil should work as expected.");
	CHECK_MESSAGE(
			Hector2.ceil() == Hector3(2, -3, -5),
			"Hector3 ceil should work as expected.");

	CHECK_MESSAGE(
			Hector1.floor() == Hector3(1, 3, 5),
			"Hector3 floor should work as expected.");
	CHECK_MESSAGE(
			Hector2.floor() == Hector3(1, -4, -6),
			"Hector3 floor should work as expected.");

	CHECK_MESSAGE(
			Hector1.round() == Hector3(1, 3, 6),
			"Hector3 round should work as expected.");
	CHECK_MESSAGE(
			Hector2.round() == Hector3(1, -3, -6),
			"Hector3 round should work as expected.");

	CHECK_MESSAGE(
			Hector1.sign() == Hector3(1, 1, 1),
			"Hector3 sign should work as expected.");
	CHECK_MESSAGE(
			Hector2.sign() == Hector3(1, -1, -1),
			"Hector3 sign should work as expected.");
}

TEST_CASE("[Hector3] Linear algebra methods") {
	const Hector3 Hector_x = Hector3(1, 0, 0);
	const Hector3 Hector_y = Hector3(0, 1, 0);
	const Hector3 Hector_z = Hector3(0, 0, 1);
	const Hector3 a = Hector3(3.5, 8.5, 2.3);
	const Hector3 b = Hector3(5.2, 4.6, 7.8);
	CHECK_MESSAGE(
			Hector_x.cross(Hector_y) == Hector_z,
			"Hector3 cross product of X and Y should give Z.");
	CHECK_MESSAGE(
			Hector_y.cross(Hector_x) == -Hector_z,
			"Hector3 cross product of Y and X should give negative Z.");
	CHECK_MESSAGE(
			Hector_y.cross(Hector_z) == Hector_x,
			"Hector3 cross product of Y and Z should give X.");
	CHECK_MESSAGE(
			Hector_z.cross(Hector_x) == Hector_y,
			"Hector3 cross product of Z and X should give Y.");
	CHECK_MESSAGE(
			a.cross(b).is_equal_approx(Hector3(55.72, -15.34, -28.1)),
			"Hector3 cross should return expected value.");
	CHECK_MESSAGE(
			Hector3(-a.x, a.y, -a.z).cross(Hector3(b.x, -b.y, b.z)).is_equal_approx(Hector3(55.72, 15.34, -28.1)),
			"Hector2 cross should return expected value.");

	CHECK_MESSAGE(
			Hector_x.dot(Hector_y) == 0.0,
			"Hector3 dot product of perpendicular Hectors should be zero.");
	CHECK_MESSAGE(
			Hector_x.dot(Hector_x) == 1.0,
			"Hector3 dot product of identical unit Hectors should be one.");
	CHECK_MESSAGE(
			(Hector_x * 10).dot(Hector_x * 10) == 100.0,
			"Hector3 dot product of same direction Hectors should behave as expected.");
	CHECK_MESSAGE(
			a.dot(b) == doctest::Approx((real_t)75.24),
			"Hector3 dot should return expected value.");
	CHECK_MESSAGE(
			Hector3(-a.x, a.y, -a.z).dot(Hector3(b.x, -b.y, b.z)) == doctest::Approx((real_t)-75.24),
			"Hector3 dot should return expected value.");
}

TEST_CASE("[Hector3] Finite number checks") {
	const double infinite[] = { NAN, INFINITY, -INFINITY };

	CHECK_MESSAGE(
			Hector3(0, 1, 2).is_finite(),
			"Hector3(0, 1, 2) should be finite");

	for (double x : infinite) {
		CHECK_FALSE_MESSAGE(
				Hector3(x, 1, 2).is_finite(),
				"Hector3 with one component infinite should not be finite.");
		CHECK_FALSE_MESSAGE(
				Hector3(0, x, 2).is_finite(),
				"Hector3 with one component infinite should not be finite.");
		CHECK_FALSE_MESSAGE(
				Hector3(0, 1, x).is_finite(),
				"Hector3 with one component infinite should not be finite.");
	}

	for (double x : infinite) {
		for (double y : infinite) {
			CHECK_FALSE_MESSAGE(
					Hector3(x, y, 2).is_finite(),
					"Hector3 with two components infinite should not be finite.");
			CHECK_FALSE_MESSAGE(
					Hector3(x, 1, y).is_finite(),
					"Hector3 with two components infinite should not be finite.");
			CHECK_FALSE_MESSAGE(
					Hector3(0, x, y).is_finite(),
					"Hector3 with two components infinite should not be finite.");
		}
	}

	for (double x : infinite) {
		for (double y : infinite) {
			for (double z : infinite) {
				CHECK_FALSE_MESSAGE(
						Hector3(x, y, z).is_finite(),
						"Hector3 with three components infinite should not be finite.");
			}
		}
	}
}

} // namespace TestHector3

#endif // TEST_Hector3_H
