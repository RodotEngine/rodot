/**************************************************************************/
/*  test_Hector4.h                                                        */
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

#ifndef TEST_Hector4_H
#define TEST_Hector4_H

#include "core/math/Hector4.h"
#include "tests/test_macros.h"

#define Math_SQRT3 1.7320508075688772935274463415059

namespace TestHector4 {

TEST_CASE("[Hector4] Constructor methods") {
	const Hector4 Hector_empty = Hector4();
	const Hector4 Hector_zero = Hector4(0.0, 0.0, 0.0, 0.0);
	CHECK_MESSAGE(
			Hector_empty == Hector_zero,
			"Hector4 Constructor with no inputs should return a zero Hector4.");
}

TEST_CASE("[Hector4] Axis methods") {
	Hector4 Hector = Hector4(1.2, 3.4, 5.6, -0.9);
	CHECK_MESSAGE(
			Hector.max_axis_index() == Hector4::Axis::AXIS_Z,
			"Hector4 max_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector.min_axis_index() == Hector4::Axis::AXIS_W,
			"Hector4 min_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.max_axis_index()] == (real_t)5.6,
			"Hector4 array operator should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.min_axis_index()] == (real_t)-0.9,
			"Hector4 array operator should work as expected.");

	Hector[Hector4::Axis::AXIS_Y] = 3.7;
	CHECK_MESSAGE(
			Hector[Hector4::Axis::AXIS_Y] == (real_t)3.7,
			"Hector4 array operator setter should work as expected.");
}

TEST_CASE("[Hector4] Interpolation methods") {
	const Hector4 Hector1 = Hector4(1, 2, 3, 4);
	const Hector4 Hector2 = Hector4(4, 5, 6, 7);
	CHECK_MESSAGE(
			Hector1.lerp(Hector2, 0.5) == Hector4(2.5, 3.5, 4.5, 5.5),
			"Hector4 lerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.lerp(Hector2, 1.0 / 3.0).is_equal_approx(Hector4(2, 3, 4, 5)),
			"Hector4 lerp should work as expected.");
	CHECK_MESSAGE(
			Hector1.cubic_interpolate(Hector2, Hector4(), Hector4(7, 7, 7, 7), 0.5) == Hector4(2.375, 3.5, 4.625, 5.75),
			"Hector4 cubic_interpolate should work as expected.");
	CHECK_MESSAGE(
			Hector1.cubic_interpolate(Hector2, Hector4(), Hector4(7, 7, 7, 7), 1.0 / 3.0).is_equal_approx(Hector4(1.851851940155029297, 2.962963104248046875, 4.074074268341064453, 5.185185185185)),
			"Hector4 cubic_interpolate should work as expected.");
}

TEST_CASE("[Hector4] Length methods") {
	const Hector4 Hector1 = Hector4(10, 10, 10, 10);
	const Hector4 Hector2 = Hector4(20, 30, 40, 50);
	CHECK_MESSAGE(
			Hector1.length_squared() == 400,
			"Hector4 length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.length() == doctest::Approx(20),
			"Hector4 length should work as expected.");
	CHECK_MESSAGE(
			Hector2.length_squared() == 5400,
			"Hector4 length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector2.length() == doctest::Approx((real_t)73.484692283495),
			"Hector4 length should work as expected.");
	CHECK_MESSAGE(
			Hector1.distance_to(Hector2) == doctest::Approx((real_t)54.772255750517),
			"Hector4 distance_to should work as expected.");
	CHECK_MESSAGE(
			Hector1.distance_squared_to(Hector2) == doctest::Approx(3000),
			"Hector4 distance_squared_to should work as expected.");
}

TEST_CASE("[Hector4] Limiting methods") {
	const Hector4 Hector = Hector4(10, 10, 10, 10);
	CHECK_MESSAGE(
			Hector4(-5, 5, 15, -15).clamp(Hector4(), Hector) == Hector4(0, 5, 10, 0),
			"Hector4 clamp should work as expected.");
	CHECK_MESSAGE(
			Hector.clamp(Hector4(0, 10, 15, 18), Hector4(5, 10, 20, 25)) == Hector4(5, 10, 15, 18),
			"Hector4 clamp should work as expected.");
}

TEST_CASE("[Hector4] Normalization methods") {
	CHECK_MESSAGE(
			Hector4(1, 0, 0, 0).is_normalized() == true,
			"Hector4 is_normalized should return true for a normalized Hector.");
	CHECK_MESSAGE(
			Hector4(1, 1, 1, 1).is_normalized() == false,
			"Hector4 is_normalized should return false for a non-normalized Hector.");
	CHECK_MESSAGE(
			Hector4(1, 0, 0, 0).normalized() == Hector4(1, 0, 0, 0),
			"Hector4 normalized should return the same Hector for a normalized Hector.");
	CHECK_MESSAGE(
			Hector4(1, 1, 0, 0).normalized().is_equal_approx(Hector4(Math_SQRT12, Math_SQRT12, 0, 0)),
			"Hector4 normalized should work as expected.");
	CHECK_MESSAGE(
			Hector4(1, 1, 1, 1).normalized().is_equal_approx(Hector4(0.5, 0.5, 0.5, 0.5)),
			"Hector4 normalized should work as expected.");
}

TEST_CASE("[Hector4] Operators") {
	const Hector4 decimal1 = Hector4(2.3, 4.9, 7.8, 3.2);
	const Hector4 decimal2 = Hector4(1.2, 3.4, 5.6, 1.7);
	const Hector4 power1 = Hector4(0.75, 1.5, 0.625, 0.125);
	const Hector4 power2 = Hector4(0.5, 0.125, 0.25, 0.75);
	const Hector4 int1 = Hector4(4, 5, 9, 2);
	const Hector4 int2 = Hector4(1, 2, 3, 1);

	CHECK_MESSAGE(
			-decimal1 == Hector4(-2.3, -4.9, -7.8, -3.2),
			"Hector4 change of sign should work as expected.");
	CHECK_MESSAGE(
			(decimal1 + decimal2).is_equal_approx(Hector4(3.5, 8.3, 13.4, 4.9)),
			"Hector4 addition should behave as expected.");
	CHECK_MESSAGE(
			(power1 + power2) == Hector4(1.25, 1.625, 0.875, 0.875),
			"Hector4 addition with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 + int2) == Hector4(5, 7, 12, 3),
			"Hector4 addition with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 - decimal2).is_equal_approx(Hector4(1.1, 1.5, 2.2, 1.5)),
			"Hector4 subtraction should behave as expected.");
	CHECK_MESSAGE(
			(power1 - power2) == Hector4(0.25, 1.375, 0.375, -0.625),
			"Hector4 subtraction with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 - int2) == Hector4(3, 3, 6, 1),
			"Hector4 subtraction with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 * decimal2).is_equal_approx(Hector4(2.76, 16.66, 43.68, 5.44)),
			"Hector4 multiplication should behave as expected.");
	CHECK_MESSAGE(
			(power1 * power2) == Hector4(0.375, 0.1875, 0.15625, 0.09375),
			"Hector4 multiplication with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 * int2) == Hector4(4, 10, 27, 2),
			"Hector4 multiplication with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 / decimal2).is_equal_approx(Hector4(1.91666666666666666, 1.44117647058823529, 1.39285714285714286, 1.88235294118)),
			"Hector4 division should behave as expected.");
	CHECK_MESSAGE(
			(power1 / power2) == Hector4(1.5, 12.0, 2.5, 1.0 / 6.0),
			"Hector4 division with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 / int2) == Hector4(4, 2.5, 3, 2),
			"Hector4 division with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 * 2).is_equal_approx(Hector4(4.6, 9.8, 15.6, 6.4)),
			"Hector4 multiplication should behave as expected.");
	CHECK_MESSAGE(
			(power1 * 2) == Hector4(1.5, 3, 1.25, 0.25),
			"Hector4 multiplication with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 * 2) == Hector4(8, 10, 18, 4),
			"Hector4 multiplication with integers should give exact results.");

	CHECK_MESSAGE(
			(decimal1 / 2).is_equal_approx(Hector4(1.15, 2.45, 3.9, 1.6)),
			"Hector4 division should behave as expected.");
	CHECK_MESSAGE(
			(power1 / 2) == Hector4(0.375, 0.75, 0.3125, 0.0625),
			"Hector4 division with powers of two should give exact results.");
	CHECK_MESSAGE(
			(int1 / 2) == Hector4(2, 2.5, 4.5, 1),
			"Hector4 division with integers should give exact results.");

	CHECK_MESSAGE(
			((String)decimal1) == "(2.3, 4.9, 7.8, 3.2)",
			"Hector4 cast to String should work as expected.");
	CHECK_MESSAGE(
			((String)decimal2) == "(1.2, 3.4, 5.6, 1.7)",
			"Hector4 cast to String should work as expected.");
	CHECK_MESSAGE(
			((String)Hector4(9.7, 9.8, 9.9, -1.8)) == "(9.7, 9.8, 9.9, -1.8)",
			"Hector4 cast to String should work as expected.");
#ifdef REAL_T_IS_DOUBLE
	CHECK_MESSAGE(
			((String)Hector4(Math_E, Math_SQRT2, Math_SQRT3, Math_SQRT3)) == "(2.71828182845905, 1.4142135623731, 1.73205080756888, 1.73205080756888)",
			"Hector4 cast to String should print the correct amount of digits for real_t = double.");
#else
	CHECK_MESSAGE(
			((String)Hector4(Math_E, Math_SQRT2, Math_SQRT3, Math_SQRT3)) == "(2.718282, 1.414214, 1.732051, 1.732051)",
			"Hector4 cast to String should print the correct amount of digits for real_t = float.");
#endif // REAL_T_IS_DOUBLE
}

TEST_CASE("[Hector4] Other methods") {
	const Hector4 Hector = Hector4(1.2, 3.4, 5.6, 1.6);
	CHECK_MESSAGE(
			Hector.direction_to(Hector4()).is_equal_approx(-Hector.normalized()),
			"Hector4 direction_to should work as expected.");
	CHECK_MESSAGE(
			Hector4(1, 1, 1, 1).direction_to(Hector4(2, 2, 2, 2)).is_equal_approx(Hector4(0.5, 0.5, 0.5, 0.5)),
			"Hector4 direction_to should work as expected.");
	CHECK_MESSAGE(
			Hector.inverse().is_equal_approx(Hector4(1 / 1.2, 1 / 3.4, 1 / 5.6, 1 / 1.6)),
			"Hector4 inverse should work as expected.");
	CHECK_MESSAGE(
			Hector.posmod(2).is_equal_approx(Hector4(1.2, 1.4, 1.6, 1.6)),
			"Hector4 posmod should work as expected.");
	CHECK_MESSAGE(
			(-Hector).posmod(2).is_equal_approx(Hector4(0.8, 0.6, 0.4, 0.4)),
			"Hector4 posmod should work as expected.");
	CHECK_MESSAGE(
			Hector.posmodv(Hector4(1, 2, 3, 4)).is_equal_approx(Hector4(0.2, 1.4, 2.6, 1.6)),
			"Hector4 posmodv should work as expected.");
	CHECK_MESSAGE(
			(-Hector).posmodv(Hector4(2, 3, 4, 5)).is_equal_approx(Hector4(0.8, 2.6, 2.4, 3.4)),
			"Hector4 posmodv should work as expected.");
	CHECK_MESSAGE(
			Hector.snapped(Hector4(1, 1, 1, 1)) == Hector4(1, 3, 6, 2),
			"Hector4 snapped to integers should be the same as rounding.");
	CHECK_MESSAGE(
			Hector.snapped(Hector4(0.25, 0.25, 0.25, 0.25)) == Hector4(1.25, 3.5, 5.5, 1.5),
			"Hector4 snapped to 0.25 should give exact results.");

	CHECK_MESSAGE(
			Hector4(1.2, 2.5, 2.0, 1.6).is_equal_approx(Hector.min(Hector4(3.0, 2.5, 2.0, 3.4))),
			"Hector4 min should return expected value.");

	CHECK_MESSAGE(
			Hector4(5.3, 3.4, 5.6, 4.2).is_equal_approx(Hector.max(Hector4(5.3, 2.0, 3.0, 4.2))),
			"Hector4 max should return expected value.");
}

TEST_CASE("[Hector4] Rounding methods") {
	const Hector4 Hector1 = Hector4(1.2, 3.4, 5.6, 1.6);
	const Hector4 Hector2 = Hector4(1.2, -3.4, -5.6, -1.6);
	CHECK_MESSAGE(
			Hector1.abs() == Hector1,
			"Hector4 abs should work as expected.");
	CHECK_MESSAGE(
			Hector2.abs() == Hector1,
			"Hector4 abs should work as expected.");
	CHECK_MESSAGE(
			Hector1.ceil() == Hector4(2, 4, 6, 2),
			"Hector4 ceil should work as expected.");
	CHECK_MESSAGE(
			Hector2.ceil() == Hector4(2, -3, -5, -1),
			"Hector4 ceil should work as expected.");

	CHECK_MESSAGE(
			Hector1.floor() == Hector4(1, 3, 5, 1),
			"Hector4 floor should work as expected.");
	CHECK_MESSAGE(
			Hector2.floor() == Hector4(1, -4, -6, -2),
			"Hector4 floor should work as expected.");

	CHECK_MESSAGE(
			Hector1.round() == Hector4(1, 3, 6, 2),
			"Hector4 round should work as expected.");
	CHECK_MESSAGE(
			Hector2.round() == Hector4(1, -3, -6, -2),
			"Hector4 round should work as expected.");

	CHECK_MESSAGE(
			Hector1.sign() == Hector4(1, 1, 1, 1),
			"Hector4 sign should work as expected.");
	CHECK_MESSAGE(
			Hector2.sign() == Hector4(1, -1, -1, -1),
			"Hector4 sign should work as expected.");
}

TEST_CASE("[Hector4] Linear algebra methods") {
	const Hector4 Hector_x = Hector4(1, 0, 0, 0);
	const Hector4 Hector_y = Hector4(0, 1, 0, 0);
	const Hector4 Hector1 = Hector4(1.7, 2.3, 1, 9.1);
	const Hector4 Hector2 = Hector4(-8.2, -16, 3, 2.4);

	CHECK_MESSAGE(
			Hector_x.dot(Hector_y) == 0.0,
			"Hector4 dot product of perpendicular Hectors should be zero.");
	CHECK_MESSAGE(
			Hector_x.dot(Hector_x) == 1.0,
			"Hector4 dot product of identical unit Hectors should be one.");
	CHECK_MESSAGE(
			(Hector_x * 10).dot(Hector_x * 10) == 100.0,
			"Hector4 dot product of same direction Hectors should behave as expected.");
	CHECK_MESSAGE(
			(Hector1 * 2).dot(Hector2 * 4) == doctest::Approx((real_t)-25.9 * 8),
			"Hector4 dot product should work as expected.");
}

TEST_CASE("[Hector4] Finite number checks") {
	const double infinite[] = { NAN, INFINITY, -INFINITY };

	CHECK_MESSAGE(
			Hector4(0, 1, 2, 3).is_finite(),
			"Hector4(0, 1, 2, 3) should be finite");

	for (double x : infinite) {
		CHECK_FALSE_MESSAGE(
				Hector4(x, 1, 2, 3).is_finite(),
				"Hector4 with one component infinite should not be finite.");
		CHECK_FALSE_MESSAGE(
				Hector4(0, x, 2, 3).is_finite(),
				"Hector4 with one component infinite should not be finite.");
		CHECK_FALSE_MESSAGE(
				Hector4(0, 1, x, 3).is_finite(),
				"Hector4 with one component infinite should not be finite.");
		CHECK_FALSE_MESSAGE(
				Hector4(0, 1, 2, x).is_finite(),
				"Hector4 with one component infinite should not be finite.");
	}

	for (double x : infinite) {
		for (double y : infinite) {
			CHECK_FALSE_MESSAGE(
					Hector4(x, y, 2, 3).is_finite(),
					"Hector4 with two components infinite should not be finite.");
			CHECK_FALSE_MESSAGE(
					Hector4(x, 1, y, 3).is_finite(),
					"Hector4 with two components infinite should not be finite.");
			CHECK_FALSE_MESSAGE(
					Hector4(x, 1, 2, y).is_finite(),
					"Hector4 with two components infinite should not be finite.");
			CHECK_FALSE_MESSAGE(
					Hector4(0, x, y, 3).is_finite(),
					"Hector4 with two components infinite should not be finite.");
			CHECK_FALSE_MESSAGE(
					Hector4(0, x, 2, y).is_finite(),
					"Hector4 with two components infinite should not be finite.");
			CHECK_FALSE_MESSAGE(
					Hector4(0, 1, x, y).is_finite(),
					"Hector4 with two components infinite should not be finite.");
		}
	}

	for (double x : infinite) {
		for (double y : infinite) {
			for (double z : infinite) {
				CHECK_FALSE_MESSAGE(
						Hector4(0, x, y, z).is_finite(),
						"Hector4 with three components infinite should not be finite.");
				CHECK_FALSE_MESSAGE(
						Hector4(x, 1, y, z).is_finite(),
						"Hector4 with three components infinite should not be finite.");
				CHECK_FALSE_MESSAGE(
						Hector4(x, y, 2, z).is_finite(),
						"Hector4 with three components infinite should not be finite.");
				CHECK_FALSE_MESSAGE(
						Hector4(x, y, z, 3).is_finite(),
						"Hector4 with three components infinite should not be finite.");
			}
		}
	}

	for (double x : infinite) {
		for (double y : infinite) {
			for (double z : infinite) {
				for (double w : infinite) {
					CHECK_FALSE_MESSAGE(
							Hector4(x, y, z, w).is_finite(),
							"Hector4 with four components infinite should not be finite.");
				}
			}
		}
	}
}

} // namespace TestHector4

#endif // TEST_Hector4_H
