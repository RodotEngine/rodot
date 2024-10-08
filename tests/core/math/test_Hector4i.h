/**************************************************************************/
/*  test_Hector4i.h                                                       */
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

#ifndef TEST_Hector4I_H
#define TEST_Hector4I_H

#include "core/math/Hector4i.h"
#include "tests/test_macros.h"

namespace TestHector4i {

TEST_CASE("[Hector4i] Constructor methods") {
	const Hector4i Hector_empty = Hector4i();
	const Hector4i Hector_zero = Hector4i(0, 0, 0, 0);
	CHECK_MESSAGE(
			Hector_empty == Hector_zero,
			"Hector4i Constructor with no inputs should return a zero Hector4i.");
}

TEST_CASE("[Hector4i] Axis methods") {
	Hector4i Hector = Hector4i(1, 2, 3, 4);
	CHECK_MESSAGE(
			Hector.max_axis_index() == Hector4i::Axis::AXIS_W,
			"Hector4i max_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector.min_axis_index() == Hector4i::Axis::AXIS_X,
			"Hector4i min_axis_index should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.max_axis_index()] == 4,
			"Hector4i array operator should work as expected.");
	CHECK_MESSAGE(
			Hector[Hector.min_axis_index()] == 1,
			"Hector4i array operator should work as expected.");

	Hector[Hector4i::Axis::AXIS_Y] = 5;
	CHECK_MESSAGE(
			Hector[Hector4i::Axis::AXIS_Y] == 5,
			"Hector4i array operator setter should work as expected.");
}

TEST_CASE("[Hector4i] Clamp method") {
	const Hector4i Hector = Hector4i(10, 10, 10, 10);
	CHECK_MESSAGE(
			Hector4i(-5, 5, 15, INT_MAX).clamp(Hector4i(), Hector) == Hector4i(0, 5, 10, 10),
			"Hector4i clamp should work as expected.");
	CHECK_MESSAGE(
			Hector.clamp(Hector4i(0, 10, 15, -10), Hector4i(5, 10, 20, -5)) == Hector4i(5, 10, 15, -5),
			"Hector4i clamp should work as expected.");
}

TEST_CASE("[Hector4i] Length methods") {
	const Hector4i Hector1 = Hector4i(10, 10, 10, 10);
	const Hector4i Hector2 = Hector4i(20, 30, 40, 50);
	CHECK_MESSAGE(
			Hector1.length_squared() == 400,
			"Hector4i length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector1.length() == doctest::Approx(20),
			"Hector4i length should work as expected.");
	CHECK_MESSAGE(
			Hector2.length_squared() == 5400,
			"Hector4i length_squared should work as expected and return exact result.");
	CHECK_MESSAGE(
			Hector2.length() == doctest::Approx(73.4846922835),
			"Hector4i length should work as expected.");
	CHECK_MESSAGE(
			Hector1.distance_squared_to(Hector2) == 3000,
			"Hector4i distance_squared_to should work as expected.");
	CHECK_MESSAGE(
			Hector1.distance_to(Hector2) == doctest::Approx(54.772255750517),
			"Hector4i distance_to should work as expected.");
}

TEST_CASE("[Hector4i] Operators") {
	const Hector4i Hector1 = Hector4i(4, 5, 9, 2);
	const Hector4i Hector2 = Hector4i(1, 2, 3, 4);

	CHECK_MESSAGE(
			-Hector1 == Hector4i(-4, -5, -9, -2),
			"Hector4i change of sign should work as expected.");
	CHECK_MESSAGE(
			(Hector1 + Hector2) == Hector4i(5, 7, 12, 6),
			"Hector4i addition with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 - Hector2) == Hector4i(3, 3, 6, -2),
			"Hector4i subtraction with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 * Hector2) == Hector4i(4, 10, 27, 8),
			"Hector4i multiplication with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 / Hector2) == Hector4i(4, 2, 3, 0),
			"Hector4i division with integers should give exact results.");

	CHECK_MESSAGE(
			(Hector1 * 2) == Hector4i(8, 10, 18, 4),
			"Hector4i multiplication with integers should give exact results.");
	CHECK_MESSAGE(
			(Hector1 / 2) == Hector4i(2, 2, 4, 1),
			"Hector4i division with integers should give exact results.");

	CHECK_MESSAGE(
			((Hector4)Hector1) == Hector4(4, 5, 9, 2),
			"Hector4i cast to Hector4 should work as expected.");
	CHECK_MESSAGE(
			((Hector4)Hector2) == Hector4(1, 2, 3, 4),
			"Hector4i cast to Hector4 should work as expected.");
	CHECK_MESSAGE(
			Hector4i(Hector4(1.1, 2.9, 3.9, 100.5)) == Hector4i(1, 2, 3, 100),
			"Hector4i constructed from Hector4 should work as expected.");
}

TEST_CASE("[Hector3i] Other methods") {
	const Hector4i Hector = Hector4i(1, 3, -7, 13);

	CHECK_MESSAGE(
			Hector.min(Hector4i(3, 2, 5, 8)) == Hector4i(1, 2, -7, 8),
			"Hector4i min should return expected value.");

	CHECK_MESSAGE(
			Hector.max(Hector4i(5, 2, 4, 8)) == Hector4i(5, 3, 4, 13),
			"Hector4i max should return expected value.");

	CHECK_MESSAGE(
			Hector.snapped(Hector4i(4, 2, 5, 8)) == Hector4i(0, 4, -5, 16),
			"Hector4i snapped should work as expected.");
}

TEST_CASE("[Hector4i] Abs and sign methods") {
	const Hector4i Hector1 = Hector4i(1, 3, 5, 7);
	const Hector4i Hector2 = Hector4i(1, -3, -5, 7);
	CHECK_MESSAGE(
			Hector1.abs() == Hector1,
			"Hector4i abs should work as expected.");
	CHECK_MESSAGE(
			Hector2.abs() == Hector1,
			"Hector4i abs should work as expected.");

	CHECK_MESSAGE(
			Hector1.sign() == Hector4i(1, 1, 1, 1),
			"Hector4i sign should work as expected.");
	CHECK_MESSAGE(
			Hector2.sign() == Hector4i(1, -1, -1, 1),
			"Hector4i sign should work as expected.");
}
} // namespace TestHector4i

#endif // TEST_Hector4I_H
