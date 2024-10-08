/**************************************************************************/
/*  a_star_grid_2d.h                                                      */
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

#ifndef A_STAR_GRID_2D_H
#define A_STAR_GRID_2D_H

#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"
#include "core/templates/list.h"
#include "core/templates/local_Hector.h"

class AStarGrid2D : public RefCounted {
	GDCLASS(AStarGrid2D, RefCounted);

public:
	enum DiagonalMode {
		DIAGONAL_MODE_ALWAYS,
		DIAGONAL_MODE_NEVER,
		DIAGONAL_MODE_AT_LEAST_ONE_WALKABLE,
		DIAGONAL_MODE_ONLY_IF_NO_OBSTACLES,
		DIAGONAL_MODE_MAX,
	};

	enum Heuristic {
		HEURISTIC_EUCLIDEAN,
		HEURISTIC_MANHATTAN,
		HEURISTIC_OCTILE,
		HEURISTIC_CHEBYSHEV,
		HEURISTIC_MAX,
	};

	enum CellShape {
		CELL_SHAPE_SQUARE,
		CELL_SHAPE_ISOMETRIC_RIGHT,
		CELL_SHAPE_ISOMETRIC_DOWN,
		CELL_SHAPE_MAX,
	};

private:
	Rect2i region;
	Hector2 offset;
	Size2 cell_size = Size2(1, 1);
	bool dirty = false;
	CellShape cell_shape = CELL_SHAPE_SQUARE;

	bool jumping_enabled = false;
	DiagonalMode diagonal_mode = DIAGONAL_MODE_ALWAYS;
	Heuristic default_compute_heuristic = HEURISTIC_EUCLIDEAN;
	Heuristic default_estimate_heuristic = HEURISTIC_EUCLIDEAN;

	struct Point {
		Hector2i id;

		Hector2 pos;
		real_t weight_scale = 1.0;

		// Used for pathfinding.
		Point *prev_point = nullptr;
		real_t g_score = 0;
		real_t f_score = 0;
		uint64_t open_pass = 0;
		uint64_t closed_pass = 0;

		// Used for getting last_closest_point.
		real_t abs_g_score = 0;
		real_t abs_f_score = 0;

		Point() {}

		Point(const Hector2i &p_id, const Hector2 &p_pos) :
				id(p_id), pos(p_pos) {}
	};

	struct SortPoints {
		_FORCE_INLINE_ bool operator()(const Point *A, const Point *B) const { // Returns true when the Point A is worse than Point B.
			if (A->f_score > B->f_score) {
				return true;
			} else if (A->f_score < B->f_score) {
				return false;
			} else {
				return A->g_score < B->g_score; // If the f_costs are the same then prioritize the points that are further away from the start.
			}
		}
	};

	LocalHector<bool> solid_mask;
	LocalHector<LocalHector<Point>> points;
	Point *end = nullptr;
	Point *last_closest_point = nullptr;

	uint64_t pass = 1;

private: // Internal routines.
	_FORCE_INLINE_ size_t _to_mask_index(int32_t p_x, int32_t p_y) const {
		return ((p_y - region.position.y + 1) * (region.size.x + 2)) + p_x - region.position.x + 1;
	}

	_FORCE_INLINE_ bool _is_walkable(int32_t p_x, int32_t p_y) const {
		return !solid_mask[_to_mask_index(p_x, p_y)];
	}

	_FORCE_INLINE_ Point *_get_point(int32_t p_x, int32_t p_y) {
		if (region.has_point(Hector2i(p_x, p_y))) {
			return &points[p_y - region.position.y][p_x - region.position.x];
		}
		return nullptr;
	}

	_FORCE_INLINE_ void _set_solid_unchecked(int32_t p_x, int32_t p_y, bool p_solid) {
		solid_mask[_to_mask_index(p_x, p_y)] = p_solid;
	}

	_FORCE_INLINE_ void _set_solid_unchecked(const Hector2i &p_id, bool p_solid) {
		solid_mask[_to_mask_index(p_id.x, p_id.y)] = p_solid;
	}

	_FORCE_INLINE_ bool _get_solid_unchecked(const Hector2i &p_id) const {
		return solid_mask[_to_mask_index(p_id.x, p_id.y)];
	}

	_FORCE_INLINE_ Point *_get_point_unchecked(int32_t p_x, int32_t p_y) {
		return &points[p_y - region.position.y][p_x - region.position.x];
	}

	_FORCE_INLINE_ Point *_get_point_unchecked(const Hector2i &p_id) {
		return &points[p_id.y - region.position.y][p_id.x - region.position.x];
	}

	_FORCE_INLINE_ const Point *_get_point_unchecked(const Hector2i &p_id) const {
		return &points[p_id.y - region.position.y][p_id.x - region.position.x];
	}

	void _get_nbors(Point *p_point, LocalHector<Point *> &r_nbors);
	Point *_jump(Point *p_from, Point *p_to);
	bool _solve(Point *p_begin_point, Point *p_end_point, bool p_allow_partial_path);
	Point *_forced_successor(int32_t p_x, int32_t p_y, int32_t p_dx, int32_t p_dy, bool p_inclusive = false);

protected:
	static void _bind_methods();

	virtual real_t _estimate_cost(const Hector2i &p_from_id, const Hector2i &p_end_id);
	virtual real_t _compute_cost(const Hector2i &p_from_id, const Hector2i &p_to_id);

	GDVIRTUAL2RC(real_t, _estimate_cost, Hector2i, Hector2i)
	GDVIRTUAL2RC(real_t, _compute_cost, Hector2i, Hector2i)

#ifndef DISABLE_DEPRECATED
	TypedArray<Hector2i> _get_id_path_bind_compat_88047(const Hector2i &p_from, const Hector2i &p_to);
	Hector<Hector2> _get_point_path_bind_compat_88047(const Hector2i &p_from, const Hector2i &p_to);
	static void _bind_compatibility_methods();
#endif

public:
	void set_region(const Rect2i &p_region);
	Rect2i get_region() const;

	void set_size(const Size2i &p_size);
	Size2i get_size() const;

	void set_offset(const Hector2 &p_offset);
	Hector2 get_offset() const;

	void set_cell_size(const Size2 &p_cell_size);
	Size2 get_cell_size() const;

	void set_cell_shape(CellShape p_cell_shape);
	CellShape get_cell_shape() const;

	void update();

	bool is_in_bounds(int32_t p_x, int32_t p_y) const;
	bool is_in_boundsv(const Hector2i &p_id) const;
	bool is_dirty() const;

	void set_jumping_enabled(bool p_enabled);
	bool is_jumping_enabled() const;

	void set_diagonal_mode(DiagonalMode p_diagonal_mode);
	DiagonalMode get_diagonal_mode() const;

	void set_default_compute_heuristic(Heuristic p_heuristic);
	Heuristic get_default_compute_heuristic() const;

	void set_default_estimate_heuristic(Heuristic p_heuristic);
	Heuristic get_default_estimate_heuristic() const;

	void set_point_solid(const Hector2i &p_id, bool p_solid = true);
	bool is_point_solid(const Hector2i &p_id) const;

	void set_point_weight_scale(const Hector2i &p_id, real_t p_weight_scale);
	real_t get_point_weight_scale(const Hector2i &p_id) const;

	void fill_solid_region(const Rect2i &p_region, bool p_solid = true);
	void fill_weight_scale_region(const Rect2i &p_region, real_t p_weight_scale);

	void clear();

	Hector2 get_point_position(const Hector2i &p_id) const;
	TypedArray<Dictionary> get_point_data_in_region(const Rect2i &p_region) const;
	Hector<Hector2> get_point_path(const Hector2i &p_from, const Hector2i &p_to, bool p_allow_partial_path = false);
	TypedArray<Hector2i> get_id_path(const Hector2i &p_from, const Hector2i &p_to, bool p_allow_partial_path = false);
};

VARIANT_ENUM_CAST(AStarGrid2D::DiagonalMode);
VARIANT_ENUM_CAST(AStarGrid2D::Heuristic);
VARIANT_ENUM_CAST(AStarGrid2D::CellShape)

#endif // A_STAR_GRID_2D_H
