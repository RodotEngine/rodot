/**************************************************************************/
/*  test_Hector2i.h                                                       */
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

#ifndef TEST_Hector2I_H
#define TEST_Hector2I_H

#include "core/math/Hector2.h"
#include "core/math/Hector2i.h"
#include "tests/test_macros.h"

namespace TestHector2i {

TEST_CASE("[Hector2i] Constructor methods") {
	const Hector2i Hector_empty = Hector2i();
	const Hector2i Hector_zero = Hector2i(0, 0);
	CHECK_MESSAGE(
			Hector_empty == Hector_zero,
			"Hector2i Constructor with no inputs should return a zero Hector2i.");
}

TEST_CASE("[Hector2i] Axis methods") {
	Hector2i Hector = Hector2i(2, 3);
	CHECK_MESSAGE(
			Hector.max_axis_index() == Hector2i::Axis::AXIS_Y,
			"Hector2i max_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector.min_axis_index() == Hector2i::Axis::AXIS_X,
			"Hector2i min_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.min_axis_index()] == 2,
			"Hector2i array operator should work as expected.");
	Hector[Hector2i::Axis::AXIS_Y] = 5;
	CHECK_MESSAGE(
			Hector[Hector2i::Axis::AXIS_Y] == 5,
			"Hector2i array operator setter should work as expected.");
}

TEST_CASE("[Hector2i] Clamp method") {
	const Hector2i Hector = Hector2i(10, 10);
	CHECK_MESSAGE(
			Hector2i(-5, 15).clamp(Hector2i(), Hector) == Hector2i(0, 10),
			"Hector2i clamp should work as expected.");
	CHECK_MESSAGE(
			Hector.clamp(Hector2i(0, 15), Hector2i(5, 20)) == Hector2i(5, 15),
			"Hector2i clamp should work as expected.");
}

TEST_CASE("[Hector2i] Length methods") {
	const Hector2i Hector1 = Hector2i(10, 10);
	const Hector2i Hector2 = Hector2i(20, 30);
	CHECK_MESSAGE(
			Hector1.length_squared() == 200,
			"Hector2i length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.length() == doctest::Approx(10 * Math_SQRT2),
			"Hector2i length should work as expected.");
	CHECK_MESSAGE(
			Hector2.length_squared() == 1300,
			"Hector2i length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector2.length() == doctest::Approx(36.05551275463989293119),
			"Hector2i length should work as expected.");
	CHECK_MESSAGE(
			Hector1.distance_squared_to(Hector2) == 500,
			"Hector2i distance_squared_to should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.distance_to(Hector2) == doctest::Approx(22.36067977499789696409),
			"Hector2i distance_to should work as expected.");
}

TEST_CASE("[Hector2i] Operators") {
	const Hector2i Hector1 = Hector2i(5, 9);
	const Hector2i Hector2 = Hector2i(2, 3);

	CHECK_MESSAGE(
			(Hector1 + Hector2) == Hector2i(7, 12),
			"Hector2i addition with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 - Hector2) == Hector2i(3, 6),
			"Hector2i subtraction with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 * Hector2) == Hector2i(10, 27),
			"Hector2i multiplication with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 / Hector2) == Hector2i(2, 3),
			"Hector2i division with integers should give exact results.");

	CHECK_MESSAGE(
			(Hector1 * 2) == Hector2i(10, 18),
			"Hector2i multiplication with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 / 2) == Hector2i(2, 4),
			"Hector2i division with integers should give exact results.");

	CHECK_MESSAGE(
			((Hector2)Hector1) == Hector2(5, 9),
			"Hector2i cast to Hector2 should work as expected.");
	CHECK_MESSAGE(
			((Hector2)Hector2) == Hector2(2, 3),
			"Hector2i cast to Hector2 should work as expected.");
	CHECK_MESSAGE(
			Hector2i(Hector2(1.1, 2.9)) == Hector2i(1, 2),
			"Hector2i constructed from Hector2 should work as expected.");
}

TEST_CASE("[Hector2i] Other methods") {
	const Hector2i Hector = Hector2i(1, 3);
	CHECK_MESSAGE(
			Hector.aspect() == doctest::Approx((real_t)1.0 / (real_t)3.0),
			"Hector2i aspect should work as expected.");

	CHECK_MESSAGE(
			Hector.min(Hector2i(3, 2)) == Hector2i(1, 2),
			"Hector2i min should return expected value.");

	CHECK_MESSAGE(
			Hector.max(Hector2i(5, 2)) == Hector2i(5, 3),
			"Hector2i max should return expected value.");

	CHECK_MESSAGE(
			Hector.snapped(Hector2i(4, 2)) == Hector2i(0, 4),
			"Hector2i snapped should work as expected.");
}

TEST_CASE("[Hector2i] Abs and sign methods") {
	const Hector2i Hector1 = Hector2i(1, 3);
	const Hector2i Hector2 = Hector2i(1, -3);
	CHECK_MESSAGE(
			Hector1.abs() == Hector1,
			"Hector2i abs should work as expected.");
	CHECK_MESSAGE(
			Hector2.abs() == Hector1,
			"Hector2i abs should work as expected.");

	CHECK_MESSAGE(
			Hector1.sign() == Hector2i(1, 1),
			"Hector2i sign should work as expected.");
	CHECK_MESSAGE(
			Hector2.sign() == Hector2i(1, -1),
			"Hector2i sign should work as expected.");
}
} // namespace TestHector2i

#endif // TEST_Hector2I_H
