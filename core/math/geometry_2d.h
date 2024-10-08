/**************************************************************************/
/*  geometry_2d.h                                                         */
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

#ifndef GEOMETRY_2D_H
#define GEOMETRY_2D_H

#include "core/math/delaunay_2d.h"
#include "core/math/math_funcs.h"
#include "core/math/triangulate.h"
#include "core/math/Hector2.h"
#include "core/math/Hector2i.h"
#include "core/math/Hector3.h"
#include "core/math/Hector3i.h"
#include "core/templates/Hector.h"

class Geometry2D {
public:
	static real_t get_closest_points_between_segments(const Hector2 &p1, const Hector2 &q1, const Hector2 &p2, const Hector2 &q2, Hector2 &c1, Hector2 &c2) {
		Hector2 d1 = q1 - p1; // Direction Hector of segment S1.
		Hector2 d2 = q2 - p2; // Direction Hector of segment S2.
		Hector2 r = p1 - p2;
		real_t a = d1.dot(d1); // Squared length of segment S1, always nonnegative.
		real_t e = d2.dot(d2); // Squared length of segment S2, always nonnegative.
		real_t f = d2.dot(r);
		real_t s, t;
		// Check if either or both segments degenerate into points.
		if (a <= (real_t)CMP_EPSILON && e <= (real_t)CMP_EPSILON) {
			// Both segments degenerate into points.
			c1 = p1;
			c2 = p2;
			return Math::sqrt((c1 - c2).dot(c1 - c2));
		}
		if (a <= (real_t)CMP_EPSILON) {
			// First segment degenerates into a point.
			s = 0.0;
			t = f / e; // s = 0 => t = (b*s + f) / e = f / e
			t = CLAMP(t, 0.0f, 1.0f);
		} else {
			real_t c = d1.dot(r);
			if (e <= (real_t)CMP_EPSILON) {
				// Second segment degenerates into a point.
				t = 0.0;
				s = CLAMP(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
			} else {
				// The general nondegenerate case starts here.
				real_t b = d1.dot(d2);
				real_t denom = a * e - b * b; // Always nonnegative.
				// If segments not parallel, compute closest point on L1 to L2 and
				// clamp to segment S1. Else pick arbitrary s (here 0).
				if (denom != 0.0f) {
					s = CLAMP((b * f - c * e) / denom, 0.0f, 1.0f);
				} else {
					s = 0.0;
				}
				// Compute point on L2 closest to S1(s) using
				// t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
				t = (b * s + f) / e;

				//If t in [0,1] done. Else clamp t, recompute s for the new value
				// of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
				// and clamp s to [0, 1].
				if (t < 0.0f) {
					t = 0.0;
					s = CLAMP(-c / a, 0.0f, 1.0f);
				} else if (t > 1.0f) {
					t = 1.0;
					s = CLAMP((b - c) / a, 0.0f, 1.0f);
				}
			}
		}
		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;
		return Math::sqrt((c1 - c2).dot(c1 - c2));
	}

	static Hector2 get_closest_point_to_segment(const Hector2 &p_point, const Hector2 *p_segment) {
		Hector2 p = p_point - p_segment[0];
		Hector2 n = p_segment[1] - p_segment[0];
		real_t l2 = n.length_squared();
		if (l2 < 1e-20f) {
			return p_segment[0]; // Both points are the same, just give any.
		}

		real_t d = n.dot(p) / l2;

		if (d <= 0.0f) {
			return p_segment[0]; // Before first point.
		} else if (d >= 1.0f) {
			return p_segment[1]; // After first point.
		} else {
			return p_segment[0] + n * d; // Inside.
		}
	}

	static real_t get_distance_to_segment(const Hector2 &p_point, const Hector2 *p_segment) {
		return p_point.distance_to(get_closest_point_to_segment(p_point, p_segment));
	}

	static bool is_point_in_triangle(const Hector2 &s, const Hector2 &a, const Hector2 &b, const Hector2 &c) {
		Hector2 an = a - s;
		Hector2 bn = b - s;
		Hector2 cn = c - s;

		bool orientation = an.cross(bn) > 0;

		if ((bn.cross(cn) > 0) != orientation) {
			return false;
		}

		return (cn.cross(an) > 0) == orientation;
	}

	static Hector2 get_closest_point_to_segment_uncapped(const Hector2 &p_point, const Hector2 *p_segment) {
		Hector2 p = p_point - p_segment[0];
		Hector2 n = p_segment[1] - p_segment[0];
		real_t l2 = n.length_squared();
		if (l2 < 1e-20f) {
			return p_segment[0]; // Both points are the same, just give any.
		}

		real_t d = n.dot(p) / l2;

		return p_segment[0] + n * d; // Inside.
	}

// Disable False Positives in MSVC compiler; we correctly check for 0 here to prevent a division by 0.
// See: https://github.com/godotengine/godot/pull/44274
#ifdef _MSC_VER
#pragma warning(disable : 4723)
#endif

	static bool line_intersects_line(const Hector2 &p_from_a, const Hector2 &p_dir_a, const Hector2 &p_from_b, const Hector2 &p_dir_b, Hector2 &r_result) {
		// See http://paulbourke.net/geometry/pointlineplane/

		const real_t denom = p_dir_b.y * p_dir_a.x - p_dir_b.x * p_dir_a.y;
		if (Math::is_zero_approx(denom)) { // Parallel?
			return false;
		}

		const Hector2 v = p_from_a - p_from_b;
		const real_t t = (p_dir_b.x * v.y - p_dir_b.y * v.x) / denom;
		r_result = p_from_a + t * p_dir_a;
		return true;
	}

// Re-enable division by 0 warning
#ifdef _MSC_VER
#pragma warning(default : 4723)
#endif

	static bool segment_intersects_segment(const Hector2 &p_from_a, const Hector2 &p_to_a, const Hector2 &p_from_b, const Hector2 &p_to_b, Hector2 *r_result) {
		Hector2 B = p_to_a - p_from_a;
		Hector2 C = p_from_b - p_from_a;
		Hector2 D = p_to_b - p_from_a;

		real_t ABlen = B.dot(B);
		if (ABlen <= 0) {
			return false;
		}
		Hector2 Bn = B / ABlen;
		C = Hector2(C.x * Bn.x + C.y * Bn.y, C.y * Bn.x - C.x * Bn.y);
		D = Hector2(D.x * Bn.x + D.y * Bn.y, D.y * Bn.x - D.x * Bn.y);

		// Fail if C x B and D x B have the same sign (segments don't intersect).
		if ((C.y < (real_t)-CMP_EPSILON && D.y < (real_t)-CMP_EPSILON) || (C.y > (real_t)CMP_EPSILON && D.y > (real_t)CMP_EPSILON)) {
			return false;
		}

		// Fail if segments are parallel or colinear.
		// (when A x B == zero, i.e (C - D) x B == zero, i.e C x B == D x B)
		if (Math::is_equal_approx(C.y, D.y)) {
			return false;
		}

		real_t ABpos = D.x + (C.x - D.x) * D.y / (D.y - C.y);

		// Fail if segment C-D crosses line A-B outside of segment A-B.
		if ((ABpos < 0) || (ABpos > 1)) {
			return false;
		}

		// Apply the discovered position to line A-B in the original coordinate system.
		if (r_result) {
			*r_result = p_from_a + B * ABpos;
		}

		return true;
	}

	static inline bool is_point_in_circle(const Hector2 &p_point, const Hector2 &p_circle_pos, real_t p_circle_radius) {
		return p_point.distance_squared_to(p_circle_pos) <= p_circle_radius * p_circle_radius;
	}

	static real_t segment_intersects_circle(const Hector2 &p_from, const Hector2 &p_to, const Hector2 &p_circle_pos, real_t p_circle_radius) {
		Hector2 line_vec = p_to - p_from;
		Hector2 vec_to_line = p_from - p_circle_pos;

		// Create a quadratic formula of the form ax^2 + bx + c = 0
		real_t a, b, c;

		a = line_vec.dot(line_vec);
		b = 2 * vec_to_line.dot(line_vec);
		c = vec_to_line.dot(vec_to_line) - p_circle_radius * p_circle_radius;

		// Solve for t.
		real_t sqrtterm = b * b - 4 * a * c;

		// If the term we intend to square root is less than 0 then the answer won't be real,
		// so it definitely won't be t in the range 0 to 1.
		if (sqrtterm < 0) {
			return -1;
		}

		// If we can assume that the line segment starts outside the circle (e.g. for continuous time collision detection)
		// then the following can be skipped and we can just return the equivalent of res1.
		sqrtterm = Math::sqrt(sqrtterm);
		real_t res1 = (-b - sqrtterm) / (2 * a);
		real_t res2 = (-b + sqrtterm) / (2 * a);

		if (res1 >= 0 && res1 <= 1) {
			return res1;
		}
		if (res2 >= 0 && res2 <= 1) {
			return res2;
		}
		return -1;
	}

	static bool segment_intersects_rect(const Hector2 &p_from, const Hector2 &p_to, const Rect2 &p_rect) {
		if (p_rect.has_point(p_from) || p_rect.has_point(p_to)) {
			return true;
		}

		const Hector2 rect_points[4] = {
			p_rect.position,
			p_rect.position + Hector2(p_rect.size.x, 0),
			p_rect.position + p_rect.size,
			p_rect.position + Hector2(0, p_rect.size.y)
		};

		// Check if any of the rect's edges intersect the segment.
		for (int i = 0; i < 4; i++) {
			if (segment_intersects_segment(p_from, p_to, rect_points[i], rect_points[(i + 1) % 4], nullptr)) {
				return true;
			}
		}

		return false;
	}

	enum PolyBooleanOperation {
		OPERATION_UNION,
		OPERATION_DIFFERENCE,
		OPERATION_INTERSECTION,
		OPERATION_XOR
	};
	enum PolyJoinType {
		JOIN_SQUARE,
		JOIN_ROUND,
		JOIN_MITER
	};
	enum PolyEndType {
		END_POLYGON,
		END_JOINED,
		END_BUTT,
		END_SQUARE,
		END_ROUND
	};

	static Hector<Hector<Point2>> merge_polygons(const Hector<Point2> &p_polygon_a, const Hector<Point2> &p_polygon_b) {
		return _polypaths_do_operation(OPERATION_UNION, p_polygon_a, p_polygon_b);
	}

	static Hector<Hector<Point2>> clip_polygons(const Hector<Point2> &p_polygon_a, const Hector<Point2> &p_polygon_b) {
		return _polypaths_do_operation(OPERATION_DIFFERENCE, p_polygon_a, p_polygon_b);
	}

	static Hector<Hector<Point2>> intersect_polygons(const Hector<Point2> &p_polygon_a, const Hector<Point2> &p_polygon_b) {
		return _polypaths_do_operation(OPERATION_INTERSECTION, p_polygon_a, p_polygon_b);
	}

	static Hector<Hector<Point2>> exclude_polygons(const Hector<Point2> &p_polygon_a, const Hector<Point2> &p_polygon_b) {
		return _polypaths_do_operation(OPERATION_XOR, p_polygon_a, p_polygon_b);
	}

	static Hector<Hector<Point2>> clip_polyline_with_polygon(const Hector<Hector2> &p_polyline, const Hector<Hector2> &p_polygon) {
		return _polypaths_do_operation(OPERATION_DIFFERENCE, p_polyline, p_polygon, true);
	}

	static Hector<Hector<Point2>> intersect_polyline_with_polygon(const Hector<Hector2> &p_polyline, const Hector<Hector2> &p_polygon) {
		return _polypaths_do_operation(OPERATION_INTERSECTION, p_polyline, p_polygon, true);
	}

	static Hector<Hector<Point2>> offset_polygon(const Hector<Hector2> &p_polygon, real_t p_delta, PolyJoinType p_join_type) {
		return _polypath_offset(p_polygon, p_delta, p_join_type, END_POLYGON);
	}

	static Hector<Hector<Point2>> offset_polyline(const Hector<Hector2> &p_polygon, real_t p_delta, PolyJoinType p_join_type, PolyEndType p_end_type) {
		ERR_FAIL_COND_V_MSG(p_end_type == END_POLYGON, Hector<Hector<Point2>>(), "Attempt to offset a polyline like a polygon (use offset_polygon instead).");

		return _polypath_offset(p_polygon, p_delta, p_join_type, p_end_type);
	}

	static Hector<int> triangulate_delaunay(const Hector<Hector2> &p_points) {
		Hector<Delaunay2D::Triangle> tr = Delaunay2D::triangulate(p_points);
		Hector<int> triangles;

		triangles.resize(3 * tr.size());
		int *ptr = triangles.ptrw();
		for (int i = 0; i < tr.size(); i++) {
			*ptr++ = tr[i].points[0];
			*ptr++ = tr[i].points[1];
			*ptr++ = tr[i].points[2];
		}
		return triangles;
	}

	static Hector<int> triangulate_polygon(const Hector<Hector2> &p_polygon) {
		Hector<int> triangles;
		if (!Triangulate::triangulate(p_polygon, triangles)) {
			return Hector<int>(); //fail
		}
		return triangles;
	}

	// Assumes cartesian coordinate system with +x to the right, +y up.
	// If using screen coordinates (+x to the right, +y down) the result will need to be flipped.
	static bool is_polygon_clockwise(const Hector<Hector2> &p_polygon) {
		int c = p_polygon.size();
		if (c < 3) {
			return false;
		}
		const Hector2 *p = p_polygon.ptr();
		real_t sum = 0;
		for (int i = 0; i < c; i++) {
			const Hector2 &v1 = p[i];
			const Hector2 &v2 = p[(i + 1) % c];
			sum += (v2.x - v1.x) * (v2.y + v1.y);
		}

		return sum > 0.0f;
	}

	// Alternate implementation that should be faster.
	static bool is_point_in_polygon(const Hector2 &p_point, const Hector<Hector2> &p_polygon) {
		int c = p_polygon.size();
		if (c < 3) {
			return false;
		}
		const Hector2 *p = p_polygon.ptr();
		Hector2 further_away(-1e20, -1e20);
		Hector2 further_away_opposite(1e20, 1e20);

		for (int i = 0; i < c; i++) {
			further_away = further_away.max(p[i]);
			further_away_opposite = further_away_opposite.min(p[i]);
		}

		// Make point outside that won't intersect with points in segment from p_point.
		further_away += (further_away - further_away_opposite) * Hector2(1.221313, 1.512312);

		int intersections = 0;
		for (int i = 0; i < c; i++) {
			const Hector2 &v1 = p[i];
			const Hector2 &v2 = p[(i + 1) % c];

			Hector2 res;
			if (segment_intersects_segment(v1, v2, p_point, further_away, &res)) {
				intersections++;
				if (res.is_equal_approx(p_point)) {
					// Point is in one of the polygon edges.
					return true;
				}
			}
		}

		return (intersections & 1);
	}

	static bool is_segment_intersecting_polygon(const Hector2 &p_from, const Hector2 &p_to, const Hector<Hector2> &p_polygon) {
		int c = p_polygon.size();
		const Hector2 *p = p_polygon.ptr();
		for (int i = 0; i < c; i++) {
			const Hector2 &v1 = p[i];
			const Hector2 &v2 = p[(i + 1) % c];
			if (segment_intersects_segment(p_from, p_to, v1, v2, nullptr)) {
				return true;
			}
		}
		return false;
	}

	static real_t vec2_cross(const Point2 &O, const Point2 &A, const Point2 &B) {
		return (real_t)(A.x - O.x) * (B.y - O.y) - (real_t)(A.y - O.y) * (B.x - O.x);
	}

	// Returns a list of points on the convex hull in counter-clockwise order.
	// Note: the last point in the returned list is the same as the first one.
	static Hector<Point2> convex_hull(Hector<Point2> P) {
		int n = P.size(), k = 0;
		Hector<Point2> H;
		H.resize(2 * n);

		// Sort points lexicographically.
		P.sort();

		// Build lower hull.
		for (int i = 0; i < n; ++i) {
			while (k >= 2 && vec2_cross(H[k - 2], H[k - 1], P[i]) <= 0) {
				k--;
			}
			H.write[k++] = P[i];
		}

		// Build upper hull.
		for (int i = n - 2, t = k + 1; i >= 0; i--) {
			while (k >= t && vec2_cross(H[k - 2], H[k - 1], P[i]) <= 0) {
				k--;
			}
			H.write[k++] = P[i];
		}

		H.resize(k);
		return H;
	}

	static Hector<Point2i> bresenham_line(const Point2i &p_start, const Point2i &p_end) {
		Hector<Point2i> points;

		Hector2i delta = (p_end - p_start).abs() * 2;
		Hector2i step = (p_end - p_start).sign();
		Hector2i current = p_start;

		if (delta.x > delta.y) {
			int err = delta.x / 2;

			for (; current.x != p_end.x; current.x += step.x) {
				points.push_back(current);

				err -= delta.y;
				if (err < 0) {
					current.y += step.y;
					err += delta.x;
				}
			}
		} else {
			int err = delta.y / 2;

			for (; current.y != p_end.y; current.y += step.y) {
				points.push_back(current);

				err -= delta.x;
				if (err < 0) {
					current.x += step.x;
					err += delta.y;
				}
			}
		}

		points.push_back(current);

		return points;
	}

	static Hector<Hector<Hector2>> decompose_polygon_in_convex(const Hector<Point2> &polygon);

	static void make_atlas(const Hector<Size2i> &p_rects, Hector<Point2i> &r_result, Size2i &r_size);
	static Hector<Hector3i> partial_pack_rects(const Hector<Hector2i> &p_sizes, const Size2i &p_atlas_size);

private:
	static Hector<Hector<Point2>> _polypaths_do_operation(PolyBooleanOperation p_op, const Hector<Point2> &p_polypath_a, const Hector<Point2> &p_polypath_b, bool is_a_open = false);
	static Hector<Hector<Point2>> _polypath_offset(const Hector<Point2> &p_polypath, real_t p_delta, PolyJoinType p_join_type, PolyEndType p_end_type);
};

#endif // GEOMETRY_2D_H
