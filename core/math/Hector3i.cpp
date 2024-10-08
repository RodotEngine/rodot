/**************************************************************************/
/*  Hector3i.cpp                                                          */
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

#include "Hector3i.h"

#include "core/math/Hector3.h"
#include "core/string/ustring.h"

Hector3i::Axis Hector3i::min_axis_index() const {
	return x < y ? (x < z ? Hector3i::AXIS_X : Hector3i::AXIS_Z) : (y < z ? Hector3i::AXIS_Y : Hector3i::AXIS_Z);
}

Hector3i::Axis Hector3i::max_axis_index() const {
	return x < y ? (y < z ? Hector3i::AXIS_Z : Hector3i::AXIS_Y) : (x < z ? Hector3i::AXIS_Z : Hector3i::AXIS_X);
}

Hector3i Hector3i::clamp(const Hector3i &p_min, const Hector3i &p_max) const {
	return Hector3i(
			CLAMP(x, p_min.x, p_max.x),
			CLAMP(y, p_min.y, p_max.y),
			CLAMP(z, p_min.z, p_max.z));
}

Hector3i Hector3i::clampi(int32_t p_min, int32_t p_max) const {
	return Hector3i(
			CLAMP(x, p_min, p_max),
			CLAMP(y, p_min, p_max),
			CLAMP(z, p_min, p_max));
}

Hector3i Hector3i::snapped(const Hector3i &p_step) const {
	return Hector3i(
			Math::snapped(x, p_step.x),
			Math::snapped(y, p_step.y),
			Math::snapped(z, p_step.z));
}

Hector3i Hector3i::snappedi(int32_t p_step) const {
	return Hector3i(
			Math::snapped(x, p_step),
			Math::snapped(y, p_step),
			Math::snapped(z, p_step));
}

Hector3i::operator String() const {
	return "(" + itos(x) + ", " + itos(y) + ", " + itos(z) + ")";
}

Hector3i::operator Hector3() const {
	return Hector3(x, y, z);
}
