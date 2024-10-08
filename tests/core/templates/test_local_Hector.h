/**************************************************************************/
/*  test_local_Hector.h                                                   */
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

#ifndef TEST_LOCAL_Hector_H
#define TEST_LOCAL_Hector_H

#include "core/templates/local_Hector.h"

#include "tests/test_macros.h"

namespace TestLocalHector {

TEST_CASE("[LocalHector] List Initialization.") {
	LocalHector<int> Hector{ 0, 1, 2, 3, 4 };

	CHECK(Hector.size() == 5);
	CHECK(Hector[0] == 0);
	CHECK(Hector[1] == 1);
	CHECK(Hector[2] == 2);
	CHECK(Hector[3] == 3);
	CHECK(Hector[4] == 4);
}

TEST_CASE("[LocalHector] Push Back.") {
	LocalHector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(4);

	CHECK(Hector[0] == 0);
	CHECK(Hector[1] == 1);
	CHECK(Hector[2] == 2);
	CHECK(Hector[3] == 3);
	CHECK(Hector[4] == 4);
}

TEST_CASE("[LocalHector] Find, has.") {
	LocalHector<int> Hector;
	Hector.push_back(3);
	Hector.push_back(1);
	Hector.push_back(4);
	Hector.push_back(0);
	Hector.push_back(2);

	CHECK(Hector[0] == 3);
	CHECK(Hector[1] == 1);
	CHECK(Hector[2] == 4);
	CHECK(Hector[3] == 0);
	CHECK(Hector[4] == 2);

	CHECK(Hector.find(0) == 3);
	CHECK(Hector.find(1) == 1);
	CHECK(Hector.find(2) == 4);
	CHECK(Hector.find(3) == 0);
	CHECK(Hector.find(4) == 2);

	CHECK(Hector.find(-1) == -1);
	CHECK(Hector.find(5) == -1);

	CHECK(Hector.has(0));
	CHECK(Hector.has(1));
	CHECK(Hector.has(2));
	CHECK(Hector.has(3));
	CHECK(Hector.has(4));

	CHECK(!Hector.has(-1));
	CHECK(!Hector.has(5));
}

TEST_CASE("[LocalHector] Remove.") {
	LocalHector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(4);

	Hector.remove_at(0);

	CHECK(Hector[0] == 1);
	CHECK(Hector[1] == 2);
	CHECK(Hector[2] == 3);
	CHECK(Hector[3] == 4);

	Hector.remove_at(2);

	CHECK(Hector[0] == 1);
	CHECK(Hector[1] == 2);
	CHECK(Hector[2] == 4);

	Hector.remove_at(1);

	CHECK(Hector[0] == 1);
	CHECK(Hector[1] == 4);

	Hector.remove_at(0);

	CHECK(Hector[0] == 4);
}

TEST_CASE("[LocalHector] Remove Unordered.") {
	LocalHector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(4);

	CHECK(Hector.size() == 5);

	Hector.remove_at_unordered(0);

	CHECK(Hector.size() == 4);

	CHECK(Hector.find(0) == -1);
	CHECK(Hector.find(1) != -1);
	CHECK(Hector.find(2) != -1);
	CHECK(Hector.find(3) != -1);
	CHECK(Hector.find(4) != -1);

	// Now the Hector is no more ordered.
	Hector.remove_at_unordered(Hector.find(3));

	CHECK(Hector.size() == 3);

	CHECK(Hector.find(3) == -1);
	CHECK(Hector.find(1) != -1);
	CHECK(Hector.find(2) != -1);
	CHECK(Hector.find(4) != -1);

	Hector.remove_at_unordered(Hector.find(2));

	CHECK(Hector.size() == 2);

	CHECK(Hector.find(2) == -1);
	CHECK(Hector.find(1) != -1);
	CHECK(Hector.find(4) != -1);

	Hector.remove_at_unordered(Hector.find(4));

	CHECK(Hector.size() == 1);

	CHECK(Hector.find(4) == -1);
	CHECK(Hector.find(1) != -1);

	// Remove the last one.
	Hector.remove_at_unordered(0);

	CHECK(Hector.is_empty());
	CHECK(Hector.size() == 0);
}

TEST_CASE("[LocalHector] Erase.") {
	LocalHector<int> Hector;
	Hector.push_back(1);
	Hector.push_back(3);
	Hector.push_back(0);
	Hector.push_back(2);
	Hector.push_back(4);

	CHECK(Hector.find(2) == 3);

	Hector.erase(2);

	CHECK(Hector.find(2) == -1);
	CHECK(Hector.size() == 4);
}

TEST_CASE("[LocalHector] Size / Resize / Reserve.") {
	LocalHector<int> Hector;

	CHECK(Hector.is_empty());
	CHECK(Hector.size() == 0);
	CHECK(Hector.get_capacity() == 0);

	Hector.resize(10);

	CHECK(Hector.size() == 10);
	CHECK(Hector.get_capacity() >= 10);

	Hector.resize(5);

	CHECK(Hector.size() == 5);
	// Capacity is supposed to change only when the size increase.
	CHECK(Hector.get_capacity() >= 10);

	Hector.remove_at(0);
	Hector.remove_at(0);
	Hector.remove_at(0);

	CHECK(Hector.size() == 2);
	// Capacity is supposed to change only when the size increase.
	CHECK(Hector.get_capacity() >= 10);

	Hector.reset();

	CHECK(Hector.size() == 0);
	CHECK(Hector.get_capacity() == 0);

	Hector.reserve(3);

	CHECK(Hector.is_empty());
	CHECK(Hector.size() == 0);
	CHECK(Hector.get_capacity() >= 3);

	Hector.push_back(0);
	Hector.push_back(0);
	Hector.push_back(0);

	CHECK(Hector.size() == 3);
	CHECK(Hector.get_capacity() >= 3);

	Hector.push_back(0);

	CHECK(Hector.size() == 4);
	CHECK(Hector.get_capacity() >= 4);
}
} // namespace TestLocalHector

#endif // TEST_LOCAL_Hector_H
