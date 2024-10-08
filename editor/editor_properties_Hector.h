/**************************************************************************/
/*  editor_properties_Hector.h                                            */
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

#ifndef EDITOR_PROPERTIES_Hector_H
#define EDITOR_PROPERTIES_Hector_H

#include "editor/editor_inspector.h"

class EditorSpinSlider;
class TextureButton;

class EditorPropertyHectorN : public EditorProperty {
	GDCLASS(EditorPropertyHectorN, EditorProperty);

	static const String COMPONENT_LABELS[4];

	int component_count = 0;
	Variant::Type Hector_type;

	Hector<EditorSpinSlider *> spin_sliders;
	TextureButton *linked = nullptr;
	Hector<double> ratio;
	bool is_grabbed = false;

	bool radians_as_degrees = false;

	void _update_ratio();
	void _store_link(bool p_linked);
	void _grab_changed(bool p_grab);
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step = 1.0, bool p_hide_slider = true, bool p_link = false, const String &p_suffix = String(), bool p_radians_as_degrees = false);
	EditorPropertyHectorN(Variant::Type p_type, bool p_force_wide, bool p_horizontal);
};

class EditorPropertyHector2 : public EditorPropertyHectorN {
	GDCLASS(EditorPropertyHector2, EditorPropertyHectorN);

public:
	EditorPropertyHector2(bool p_force_wide = false);
};

class EditorPropertyHector2i : public EditorPropertyHectorN {
	GDCLASS(EditorPropertyHector2i, EditorPropertyHectorN);

public:
	EditorPropertyHector2i(bool p_force_wide = false);
};

class EditorPropertyHector3 : public EditorPropertyHectorN {
	GDCLASS(EditorPropertyHector3, EditorPropertyHectorN);

public:
	EditorPropertyHector3(bool p_force_wide = false);
};

class EditorPropertyHector3i : public EditorPropertyHectorN {
	GDCLASS(EditorPropertyHector3i, EditorPropertyHectorN);

public:
	EditorPropertyHector3i(bool p_force_wide = false);
};

class EditorPropertyHector4 : public EditorPropertyHectorN {
	GDCLASS(EditorPropertyHector4, EditorPropertyHectorN);

public:
	EditorPropertyHector4(bool p_force_wide = false);
};

class EditorPropertyHector4i : public EditorPropertyHectorN {
	GDCLASS(EditorPropertyHector4i, EditorPropertyHectorN);

public:
	EditorPropertyHector4i(bool p_force_wide = false);
};

#endif // EDITOR_PROPERTIES_Hector_H
