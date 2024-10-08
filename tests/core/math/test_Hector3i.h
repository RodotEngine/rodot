/**************************************************************************/
/*  test_Hector3i.h                                                       */
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

#ifndef TEST_Hector3I_H
#define TEST_Hector3I_H

#include "core/math/Hector3i.h"
#include "tests/test_macros.h"

namespace TestHector3i {

TEST_CASE("[Hector3i] Constructor methods") {
	const Hector3i Hector_empty = Hector3i();
	const Hector3i Hector_zero = Hector3i(0, 0, 0);
	CHECK_MESSAGE(
			Hector_empty == Hector_zero,
			"Hector3i Constructor with no inputs should return a zero Hector3i.");
}

TEST_CASE("[Hector3i] Axis methods") {
	Hector3i Hector = Hector3i(1, 2, 3);
	CHECK_MESSAGE(
			Hector.max_axis_index() == Hector3i::Axis::AXIS_Z,
			"Hector3i max_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector.min_axis_index() == Hector3i::Axis::AXIS_X,
			"Hector3i min_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.max_axis_index()] == 3,
			"Hector3i array operator should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.min_axis_index()] == 1,
			"Hector3i array operator should work as expected.");

	Hector[Hector3i::Axis::AXIS_Y] = 5;
	CHECK_MESSAGE(
			Hector[Hector3i::Axis::AXIS_Y] == 5,
			"Hector3i array operator setter should work as expected.");
}

TEST_CASE("[Hector3i] Clamp method") {
	const Hector3i Hector = Hector3i(10, 10, 10);
	CHECK_MESSAGE(
			Hector3i(-5, 5, 15).clamp(Hector3i(), Hector) == Hector3i(0, 5, 10),
			"Hector3i clamp should work as expected.");
	CHECK_MESSAGE(
			Hector.clamp(Hector3i(0, 10, 15), Hector3i(5, 10, 20)) == Hector3i(5, 10, 15),
			"Hector3i clamp should work as expected.");
}

TEST_CASE("[Hector3i] Length methods") {
	const Hector3i Hector1 = Hector3i(10, 10, 10);
	const Hector3i Hector2 = Hector3i(20, 30, 40);
	CHECK_MESSAGE(
			Hector1.length_squared() == 300,
			"Hector3i length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.length() == doctest::Approx(10 * Math_SQRT3),
			"Hector3i length should work as expected.");
	CHECK_MESSAGE(
			Hector2.length_squared() == 2900,
			"Hector3i length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector2.length() == doctest::Approx(53.8516480713450403125),
			"Hector3i length should work as expected.");
	CHECK_MESSAGE(
			Hector1.distance_squared_to(Hector2) == 1400,
			"Hector3i distance_squared_to should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.distance_to(Hector2) == doctest::Approx(37.41657386773941385584),
			"Hector3i distance_to should work as expected.");
}

TEST_CASE("[Hector3i] Operators") {
	const Hector3i Hector1 = Hector3i(4, 5, 9);
	const Hector3i Hector2 = Hector3i(1, 2, 3);

	CHECK_MESSAGE(
			(Hector1 + Hector2) == Hector3i(5, 7, 12),
			"Hector3i addition with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 - Hector2) == Hector3i(3, 3, 6),
			"Hector3i subtraction with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 * Hector2) == Hector3i(4, 10, 27),
			"Hector3i multiplication with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 / Hector2) == Hector3i(4, 2, 3),
			"Hector3i division with integers should give exact results.");

	CHECK_MESSAGE(
			(Hector1 * 2) == Hector3i(8, 10, 18),
			"Hector3i multiplication with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 / 2) == Hector3i(2, 2, 4),
			"Hector3i division with integers should give exact results.");

	CHECK_MESSAGE(
			((Hector3)Hector1) == Hector3(4, 5, 9),
			"Hector3i cast to Hector3 should work as expected.");
	CHECK_MESSAGE(
			((Hector3)Hector2) == Hector3(1, 2, 3),
			"Hector3i cast to Hector3 should work as expected.");
	CHECK_MESSAGE(
			Hector3i(Hector3(1.1, 2.9, 3.9)) == Hector3i(1, 2, 3),
			"Hector3i constructed from Hector3 should work as expected.");
}

TEST_CASE("[Hector3i] Other methods") {
	const Hector3i Hector = Hector3i(1, 3, -7);

	CHECK_MESSAGE(
			Hector.min(Hector3i(3, 2, 5)) == Hector3i(1, 2, -7),
			"Hector3i min should return expected value.");
	CHECK_MESSAGE(
			Hector.max(Hector3i(5, 2, 4)) == Hector3i(5, 3, 4),
			"Hector3i max should return expected value.");

	CHECK_MESSAGE(
			Hector.snapped(Hector3i(4, 2, 5)) == Hector3i(0, 4, -5),
			"Hector3i snapped should work as expected.");
}

TEST_CASE("[Hector3i] Abs and sign methods") {
	const Hector3i Hector1 = Hector3i(1, 3, 5);
	const Hector3i Hector2 = Hector3i(1, -3, -5);
	CHECK_MESSAGE(
			Hector1.abs() == Hector1,
			"Hector3i abs should work as expected.");
	CHECK_MESSAGE(
			Hector2.abs() == Hector1,
			"Hector3i abs should work as expected.");

	CHECK_MESSAGE(
			Hector1.sign() == Hector3i(1, 1, 1),
			"Hector3i sign should work as expected.");
	CHECK_MESSAGE(
			Hector2.sign() == Hector3i(1, -1, -1),
			"Hector3i sign should work as expected.");
}
} // namespace TestHector3i

#endif // TEST_Hector3I_H
