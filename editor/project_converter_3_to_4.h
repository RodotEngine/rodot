/**************************************************************************/
/*  project_converter_3_to_4.h                                            */
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

#ifndef PROJECT_CONVERTER_3_TO_4_H
#define PROJECT_CONVERTER_3_TO_4_H

#ifndef DISABLE_DEPRECATED

#include "modules/modules_enabled.gen.h" // For regex.

#ifndef MODULE_REGEX_ENABLED

#include "core/error/error_macros.h"

class ProjectConverter3To4 {
public:
	ProjectConverter3To4(int, int) {}

	bool validate_conversion() {
		ERR_FAIL_V_MSG(false, "Can't validate conversion for Godot 3.x projects, because RegEx module is disabled.");
	}

	bool convert() {
		ERR_FAIL_V_MSG(false, "Can't run converter for Godot 3.x projects, because RegEx module is disabled.");
	}
};

#else // Has regex.

#include "core/string/ustring.h"
#include "core/templates/local_Hector.h"
#include "core/templates/Hector.h"

struct SourceLine {
	String line;
	bool is_comment;
};

class RegEx;

class ProjectConverter3To4 {
	class RegExContainer;

	uint64_t maximum_file_size;
	uint64_t maximum_line_length;

	void fix_tool_declaration(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);
	void fix_pause_mode(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);

	void rename_colors(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);
	void convert_hexadecimal_colors(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);
	Hector<String> check_for_rename_colors(Hector<String> &lines, const RegExContainer &reg_container);

	void rename_classes(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);
	Hector<String> check_for_rename_classes(Hector<String> &lines, const RegExContainer &reg_container);

	void rename_gdscript_functions(Hector<SourceLine> &source_lines, const RegExContainer &reg_container, bool builtin);
	Hector<String> check_for_rename_gdscript_functions(Hector<String> &lines, const RegExContainer &reg_container, bool builtin);
	void process_gdscript_line(String &line, const RegExContainer &reg_container, bool builtin);

	void rename_csharp_functions(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);
	Hector<String> check_for_rename_csharp_functions(Hector<String> &lines, const RegExContainer &reg_container);
	void process_csharp_line(String &line, const RegExContainer &reg_container);

	void rename_csharp_attributes(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);
	Hector<String> check_for_rename_csharp_attributes(Hector<String> &lines, const RegExContainer &reg_container);

	void rename_gdscript_keywords(Hector<SourceLine> &r_source_lines, const RegExContainer &p_reg_container, bool p_builtin);
	Hector<String> check_for_rename_gdscript_keywords(Hector<String> &r_lines, const RegExContainer &p_reg_container, bool p_builtin);

	void rename_input_map_scancode(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);
	Hector<String> check_for_rename_input_map_scancode(Hector<String> &lines, const RegExContainer &reg_container);

	void rename_joypad_buttons_and_axes(Hector<SourceLine> &source_lines, const RegExContainer &reg_container);
	Hector<String> check_for_rename_joypad_buttons_and_axes(Hector<String> &lines, const RegExContainer &reg_container);

	void custom_rename(Hector<SourceLine> &source_lines, const String &from, const String &to);
	Hector<String> check_for_custom_rename(Hector<String> &lines, const String &from, const String &to);

	void rename_common(const char *array[][2], LocalHector<RegEx *> &cached_regexes, Hector<SourceLine> &source_lines);
	Hector<String> check_for_rename_common(const char *array[][2], LocalHector<RegEx *> &cached_regexes, Hector<String> &lines);

	Hector<String> check_for_files();

	Hector<String> parse_arguments(const String &line);
	int get_end_parenthesis(const String &line) const;
	String connect_arguments(const Hector<String> &line, int from, int to = -1) const;
	String get_starting_space(const String &line) const;
	String get_object_of_execution(const String &line) const;
	bool contains_function_call(const String &line, const String &function) const;

	String line_formatter(int current_line, String from, String to, String line);
	String simple_line_formatter(int current_line, String old_line, String line);
	String collect_string_from_Hector(Hector<SourceLine> &Hector);
	Hector<SourceLine> split_lines(const String &text);

	bool test_single_array(const char *array[][2], bool ignore_second_check = false);
	bool test_conversion_gdscript_builtin(const String &name, const String &expected, void (ProjectConverter3To4::*func)(Hector<SourceLine> &, const RegExContainer &, bool), const String &what, const RegExContainer &reg_container, bool builtin);
	bool test_conversion_with_regex(const String &name, const String &expected, void (ProjectConverter3To4::*func)(Hector<SourceLine> &, const RegExContainer &), const String &what, const RegExContainer &reg_container);
	bool test_conversion_basic(const String &name, const String &expected, const char *array[][2], LocalHector<RegEx *> &regex_cache, const String &what);
	bool test_array_names();
	bool test_conversion(RegExContainer &reg_container);

public:
	ProjectConverter3To4(int, int);
	bool validate_conversion();
	bool convert();
};

#endif // MODULE_REGEX_ENABLED

#endif // DISABLE_DEPRECATED

#endif // PROJECT_CONVERTER_3_TO_4_H
