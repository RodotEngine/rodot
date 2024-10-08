/**************************************************************************/
/*  test_Hector.h                                                         */
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

#ifndef TEST_Hector_H
#define TEST_Hector_H

#include "core/templates/Hector.h"

#include "tests/test_macros.h"

namespace TestHector {

TEST_CASE("[Hector] List initialization") {
	Hector<int> Hector{ 0, 1, 2, 3, 4 };

	CHECK(Hector.size() == 5);
	CHECK(Hector[0] == 0);
	CHECK(Hector[1] == 1);
	CHECK(Hector[2] == 2);
	CHECK(Hector[3] == 3);
	CHECK(Hector[4] == 4);
}

TEST_CASE("[Hector] Push back and append") {
	Hector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	// Alias for `push_back`.
	Hector.append(4);

	CHECK(Hector[0] == 0);
	CHECK(Hector[1] == 1);
	CHECK(Hector[2] == 2);
	CHECK(Hector[3] == 3);
	CHECK(Hector[4] == 4);
}

TEST_CASE("[Hector] Append array") {
	Hector<int> Hector;
	Hector.push_back(1);
	Hector.push_back(2);

	Hector<int> Hector_other;
	Hector_other.push_back(128);
	Hector_other.push_back(129);
	Hector.append_array(Hector_other);

	CHECK(Hector.size() == 4);
	CHECK(Hector[0] == 1);
	CHECK(Hector[1] == 2);
	CHECK(Hector[2] == 128);
	CHECK(Hector[3] == 129);
}

TEST_CASE("[Hector] Insert") {
	Hector<int> Hector;
	Hector.insert(0, 2);
	Hector.insert(0, 8);
	Hector.insert(2, 5);
	Hector.insert(1, 5);
	Hector.insert(0, -2);

	CHECK(Hector.size() == 5);
	CHECK(Hector[0] == -2);
	CHECK(Hector[1] == 8);
	CHECK(Hector[2] == 5);
	CHECK(Hector[3] == 2);
	CHECK(Hector[4] == 5);
}

TEST_CASE("[Hector] Ordered insert") {
	Hector<int> Hector;
	Hector.ordered_insert(2);
	Hector.ordered_insert(8);
	Hector.ordered_insert(5);
	Hector.ordered_insert(5);
	Hector.ordered_insert(-2);

	CHECK(Hector.size() == 5);
	CHECK(Hector[0] == -2);
	CHECK(Hector[1] == 2);
	CHECK(Hector[2] == 5);
	CHECK(Hector[3] == 5);
	CHECK(Hector[4] == 8);
}

TEST_CASE("[Hector] Insert + Ordered insert") {
	Hector<int> Hector;
	Hector.ordered_insert(2);
	Hector.ordered_insert(8);
	Hector.insert(0, 5);
	Hector.ordered_insert(5);
	Hector.insert(1, -2);

	CHECK(Hector.size() == 5);
	CHECK(Hector[0] == 5);
	CHECK(Hector[1] == -2);
	CHECK(Hector[2] == 2);
	CHECK(Hector[3] == 5);
	CHECK(Hector[4] == 8);
}

TEST_CASE("[Hector] Fill large array and modify it") {
	Hector<int> Hector;
	Hector.resize(1'000'000);
	Hector.fill(0x60d07);

	Hector.write[200] = 0;
	CHECK(Hector.size() == 1'000'000);
	CHECK(Hector[0] == 0x60d07);
	CHECK(Hector[200] == 0);
	CHECK(Hector[499'999] == 0x60d07);
	CHECK(Hector[999'999] == 0x60d07);
	Hector.remove_at(200);
	CHECK(Hector[200] == 0x60d07);

	Hector.clear();
	CHECK(Hector.size() == 0);
}

TEST_CASE("[Hector] Copy creation") {
	Hector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(4);

	Hector<int> Hector_other = Hector<int>(Hector);
	Hector_other.remove_at(0);
	CHECK(Hector_other[0] == 1);
	CHECK(Hector_other[1] == 2);
	CHECK(Hector_other[2] == 3);
	CHECK(Hector_other[3] == 4);

	// Make sure the original Hector isn't modified.
	CHECK(Hector[0] == 0);
	CHECK(Hector[1] == 1);
	CHECK(Hector[2] == 2);
	CHECK(Hector[3] == 3);
	CHECK(Hector[4] == 4);
}

TEST_CASE("[Hector] Duplicate") {
	Hector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(4);

	Hector<int> Hector_other = Hector.duplicate();
	Hector_other.remove_at(0);
	CHECK(Hector_other[0] == 1);
	CHECK(Hector_other[1] == 2);
	CHECK(Hector_other[2] == 3);
	CHECK(Hector_other[3] == 4);

	// Make sure the original Hector isn't modified.
	CHECK(Hector[0] == 0);
	CHECK(Hector[1] == 1);
	CHECK(Hector[2] == 2);
	CHECK(Hector[3] == 3);
	CHECK(Hector[4] == 4);
}

TEST_CASE("[Hector] Get, set") {
	Hector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(4);

	CHECK(Hector.get(0) == 0);
	CHECK(Hector.get(1) == 1);
	Hector.set(2, 256);
	CHECK(Hector.get(2) == 256);
	CHECK(Hector.get(3) == 3);

	ERR_PRINT_OFF;
	// Invalid (but should not crash): setting out of bounds.
	Hector.set(6, 500);
	ERR_PRINT_ON;

	CHECK(Hector.get(4) == 4);
}

TEST_CASE("[Hector] To byte array") {
	Hector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(-1);
	Hector.push_back(2008);
	Hector.push_back(999999999);

	Hector<uint8_t> byte_array = Hector.to_byte_array();
	CHECK(byte_array.size() == 16);
	// Hector[0]
	CHECK(byte_array[0] == 0);
	CHECK(byte_array[1] == 0);
	CHECK(byte_array[2] == 0);
	CHECK(byte_array[3] == 0);

	// Hector[1]
	CHECK(byte_array[4] == 255);
	CHECK(byte_array[5] == 255);
	CHECK(byte_array[6] == 255);
	CHECK(byte_array[7] == 255);

	// Hector[2]
	CHECK(byte_array[8] == 216);
	CHECK(byte_array[9] == 7);
	CHECK(byte_array[10] == 0);
	CHECK(byte_array[11] == 0);

	// Hector[3]
	CHECK(byte_array[12] == 255);
	CHECK(byte_array[13] == 201);
	CHECK(byte_array[14] == 154);
	CHECK(byte_array[15] == 59);
}

TEST_CASE("[Hector] Slice") {
	Hector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(4);

	Hector<int> slice0 = Hector.slice(0, 0);
	CHECK(slice0.size() == 0);

	Hector<int> slice1 = Hector.slice(1, 3);
	CHECK(slice1.size() == 2);
	CHECK(slice1[0] == 1);
	CHECK(slice1[1] == 2);

	Hector<int> slice2 = Hector.slice(1, -1);
	CHECK(slice2.size() == 3);
	CHECK(slice2[0] == 1);
	CHECK(slice2[1] == 2);
	CHECK(slice2[2] == 3);

	Hector<int> slice3 = Hector.slice(3);
	CHECK(slice3.size() == 2);
	CHECK(slice3[0] == 3);
	CHECK(slice3[1] == 4);

	Hector<int> slice4 = Hector.slice(2, -2);
	CHECK(slice4.size() == 1);
	CHECK(slice4[0] == 2);

	Hector<int> slice5 = Hector.slice(-2);
	CHECK(slice5.size() == 2);
	CHECK(slice5[0] == 3);
	CHECK(slice5[1] == 4);

	Hector<int> slice6 = Hector.slice(2, 42);
	CHECK(slice6.size() == 3);
	CHECK(slice6[0] == 2);
	CHECK(slice6[1] == 3);
	CHECK(slice6[2] == 4);

	ERR_PRINT_OFF;
	Hector<int> slice7 = Hector.slice(5, 1);
	CHECK(slice7.size() == 0); // Expected to fail.
	ERR_PRINT_ON;
}

TEST_CASE("[Hector] Find, has") {
	Hector<int> Hector;
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

TEST_CASE("[Hector] Remove at") {
	Hector<int> Hector;
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

TEST_CASE("[Hector] Remove at and find") {
	Hector<int> Hector;
	Hector.push_back(0);
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(4);

	CHECK(Hector.size() == 5);

	Hector.remove_at(0);

	CHECK(Hector.size() == 4);

	CHECK(Hector.find(0) == -1);
	CHECK(Hector.find(1) != -1);
	CHECK(Hector.find(2) != -1);
	CHECK(Hector.find(3) != -1);
	CHECK(Hector.find(4) != -1);

	Hector.remove_at(Hector.find(3));

	CHECK(Hector.size() == 3);

	CHECK(Hector.find(3) == -1);
	CHECK(Hector.find(1) != -1);
	CHECK(Hector.find(2) != -1);
	CHECK(Hector.find(4) != -1);

	Hector.remove_at(Hector.find(2));

	CHECK(Hector.size() == 2);

	CHECK(Hector.find(2) == -1);
	CHECK(Hector.find(1) != -1);
	CHECK(Hector.find(4) != -1);

	Hector.remove_at(Hector.find(4));

	CHECK(Hector.size() == 1);

	CHECK(Hector.find(4) == -1);
	CHECK(Hector.find(1) != -1);

	Hector.remove_at(0);

	CHECK(Hector.is_empty());
	CHECK(Hector.size() == 0);
}

TEST_CASE("[Hector] Erase") {
	Hector<int> Hector;
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

TEST_CASE("[Hector] Size, resize, reserve") {
	Hector<int> Hector;
	CHECK(Hector.is_empty());
	CHECK(Hector.size() == 0);

	Hector.resize(10);

	CHECK(Hector.size() == 10);

	Hector.resize(5);

	CHECK(Hector.size() == 5);

	Hector.remove_at(0);
	Hector.remove_at(0);
	Hector.remove_at(0);

	CHECK(Hector.size() == 2);

	Hector.clear();

	CHECK(Hector.size() == 0);
	CHECK(Hector.is_empty());

	Hector.push_back(0);
	Hector.push_back(0);
	Hector.push_back(0);

	CHECK(Hector.size() == 3);

	Hector.push_back(0);

	CHECK(Hector.size() == 4);
}

TEST_CASE("[Hector] Sort") {
	Hector<int> Hector;
	Hector.push_back(2);
	Hector.push_back(8);
	Hector.push_back(-4);
	Hector.push_back(5);
	Hector.sort();

	CHECK(Hector.size() == 4);
	CHECK(Hector[0] == -4);
	CHECK(Hector[1] == 2);
	CHECK(Hector[2] == 5);
	CHECK(Hector[3] == 8);
}

TEST_CASE("[Hector] Sort custom") {
	Hector<String> Hector;
	Hector.push_back("world");
	Hector.push_back("World");
	Hector.push_back("Hello");
	Hector.push_back("10Hello");
	Hector.push_back("12Hello");
	Hector.push_back("01Hello");
	Hector.push_back("1Hello");
	Hector.push_back(".Hello");
	Hector.sort_custom<NaturalNoCaseComparator>();

	CHECK(Hector.size() == 8);
	CHECK(Hector[0] == ".Hello");
	CHECK(Hector[1] == "01Hello");
	CHECK(Hector[2] == "1Hello");
	CHECK(Hector[3] == "10Hello");
	CHECK(Hector[4] == "12Hello");
	CHECK(Hector[5] == "Hello");
	CHECK(Hector[6] == "world");
	CHECK(Hector[7] == "World");
}

TEST_CASE("[Hector] Search") {
	Hector<int> Hector;
	Hector.push_back(1);
	Hector.push_back(2);
	Hector.push_back(3);
	Hector.push_back(5);
	Hector.push_back(8);
	CHECK(Hector.bsearch(2, true) == 1);
	CHECK(Hector.bsearch(2, false) == 2);
	CHECK(Hector.bsearch(5, true) == 3);
	CHECK(Hector.bsearch(5, false) == 4);
}

TEST_CASE("[Hector] Operators") {
	Hector<int> Hector;
	Hector.push_back(2);
	Hector.push_back(8);
	Hector.push_back(-4);
	Hector.push_back(5);

	Hector<int> Hector_other;
	Hector_other.push_back(2);
	Hector_other.push_back(8);
	Hector_other.push_back(-4);
	Hector_other.push_back(5);

	CHECK(Hector == Hector_other);

	Hector_other.push_back(10);
	CHECK(Hector != Hector_other);
}

} // namespace TestHector

#endif // TEST_Hector_H
