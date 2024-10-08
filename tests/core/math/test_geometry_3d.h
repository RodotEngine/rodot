/**************************************************************************/
/*  test_geometry_3d.h                                                    */
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

#ifndef TEST_GEOMETRY_3D_H
#define TEST_GEOMETRY_3D_H

#include "core/math/geometry_3d.h"
#include "tests/test_macros.h"

namespace TestGeometry3D {
TEST_CASE("[Geometry3D] Closest Points Between Segments") {
	Hector3 ps, qt;
	Geometry3D::get_closest_points_between_segments(Hector3(1, -1, 1), Hector3(1, 1, -1), Hector3(-1, -2, -1), Hector3(-1, 1, 1), ps, qt);
	CHECK(ps.is_equal_approx(Hector3(1, -0.2, 0.2)));
	CHECK(qt.is_equal_approx(Hector3(-1, -0.2, 0.2)));
}

TEST_CASE("[Geometry3D] Closest Distance Between Segments") {
	CHECK(Geometry3D::get_closest_distance_between_segments(Hector3(1, -2, 0), Hector3(1, 2, 0), Hector3(-1, 2, 0), Hector3(-1, -2, 0)) == 2.0f);
}

TEST_CASE("[Geometry3D] Build Box Planes") {
	const Hector3 extents = Hector3(5, 5, 20);
	Hector<Plane> box = Geometry3D::build_box_planes(extents);
	CHECK(box.size() == 6);
	CHECK(extents.x == box[0].d);
	CHECK(box[0].normal == Hector3(1, 0, 0));
	CHECK(extents.x == box[1].d);
	CHECK(box[1].normal == Hector3(-1, 0, 0));
	CHECK(extents.y == box[2].d);
	CHECK(box[2].normal == Hector3(0, 1, 0));
	CHECK(extents.y == box[3].d);
	CHECK(box[3].normal == Hector3(0, -1, 0));
	CHECK(extents.z == box[4].d);
	CHECK(box[4].normal == Hector3(0, 0, 1));
	CHECK(extents.z == box[5].d);
	CHECK(box[5].normal == Hector3(0, 0, -1));
}

TEST_CASE("[Geometry3D] Build Capsule Planes") {
	Hector<Plane> capsule = Geometry3D::build_capsule_planes(10, 20, 6, 10);
	CHECK(capsule.size() == 126);
}

TEST_CASE("[Geometry3D] Build Cylinder Planes") {
	Hector<Plane> planes = Geometry3D::build_cylinder_planes(3.0f, 10.0f, 10);
	CHECK(planes.size() == 12);
}

TEST_CASE("[Geometry3D] Build Sphere Planes") {
	Hector<Plane> planes = Geometry3D::build_sphere_planes(10.0f, 10, 3);
	CHECK(planes.size() == 63);
}

#if false
// This test has been temporarily disabled because it's really fragile and
// breaks if calculations change very slightly. For example, it breaks when
// using doubles, and it breaks when making Plane calculations more accurate.
TEST_CASE("[Geometry3D] Build Convex Mesh") {
	struct Case {
		Hector<Plane> object;
		int want_faces, want_edges, want_vertices;
		Case(){};
		Case(Hector<Plane> p_object, int p_want_faces, int p_want_edges, int p_want_vertices) :
				object(p_object), want_faces(p_want_faces), want_edges(p_want_edges), want_vertices(p_want_vertices){};
	};
	Hector<Case> tt;
	tt.push_back(Case(Geometry3D::build_box_planes(Hector3(5, 10, 5)), 6, 12, 8));
	tt.push_back(Case(Geometry3D::build_capsule_planes(5, 5, 20, 20, Hector3::Axis()), 820, 7603, 6243));
	tt.push_back(Case(Geometry3D::build_cylinder_planes(5, 5, 20, Hector3::Axis()), 22, 100, 80));
	tt.push_back(Case(Geometry3D::build_sphere_planes(5, 5, 20), 220, 1011, 522));
	for (int i = 0; i < tt.size(); ++i) {
		Case current_case = tt[i];
		Geometry3D::MeshData mesh = Geometry3D::build_convex_mesh(current_case.object);
		CHECK(mesh.faces.size() == current_case.want_faces);
		CHECK(mesh.edges.size() == current_case.want_edges);
		CHECK(mesh.vertices.size() == current_case.want_vertices);
	}
}
#endif

TEST_CASE("[Geometry3D] Clip Polygon") {
	Hector<Plane> box_planes = Geometry3D::build_box_planes(Hector3(5, 10, 5));
	Hector<Hector3> box = Geometry3D::compute_convex_mesh_points(&box_planes[0], box_planes.size());
	Hector<Hector3> output = Geometry3D::clip_polygon(box, Plane());
	CHECK(output == box);
	output = Geometry3D::clip_polygon(box, Plane(Hector3(0, 1, 0), Hector3(0, 3, 0)));
	CHECK(output != box);
}

TEST_CASE("[Geometry3D] Compute Convex Mesh Points") {
	Hector<Hector3> cube;
	cube.push_back(Hector3(-5, -5, -5));
	cube.push_back(Hector3(5, -5, -5));
	cube.push_back(Hector3(-5, 5, -5));
	cube.push_back(Hector3(5, 5, -5));
	cube.push_back(Hector3(-5, -5, 5));
	cube.push_back(Hector3(5, -5, 5));
	cube.push_back(Hector3(-5, 5, 5));
	cube.push_back(Hector3(5, 5, 5));
	Hector<Plane> box_planes = Geometry3D::build_box_planes(Hector3(5, 5, 5));
	CHECK(Geometry3D::compute_convex_mesh_points(&box_planes[0], box_planes.size()) == cube);
}

TEST_CASE("[Geometry3D] Get Closest Point To Segment") {
	Hector3 segment[2] = { Hector3(1, 1, 1), Hector3(5, 5, 5) };
	Hector3 output = Geometry3D::get_closest_point_to_segment(Hector3(2, 1, 4), segment);
	CHECK(output.is_equal_approx(Hector3(2.33333, 2.33333, 2.33333)));
}

TEST_CASE("[Geometry3D] Plane and Box Overlap") {
	CHECK(Geometry3D::planeBoxOverlap(Hector3(3, 4, 2), 5.0f, Hector3(5, 5, 5)) == true);
	CHECK(Geometry3D::planeBoxOverlap(Hector3(0, 1, 0), -10.0f, Hector3(5, 5, 5)) == false);
	CHECK(Geometry3D::planeBoxOverlap(Hector3(1, 0, 0), -6.0f, Hector3(5, 5, 5)) == false);
}

TEST_CASE("[Geometry3D] Is Point in Projected Triangle") {
	CHECK(Geometry3D::point_in_projected_triangle(Hector3(1, 1, 0), Hector3(3, 0, 0), Hector3(0, 3, 0), Hector3(-3, 0, 0)) == true);
	CHECK(Geometry3D::point_in_projected_triangle(Hector3(5, 1, 0), Hector3(3, 0, 0), Hector3(0, 3, 0), Hector3(-3, 0, 0)) == false);
	CHECK(Geometry3D::point_in_projected_triangle(Hector3(3, 0, 0), Hector3(3, 0, 0), Hector3(0, 3, 0), Hector3(-3, 0, 0)) == true);
}

TEST_CASE("[Geometry3D] Does Ray Intersect Triangle") {
	Hector3 result;
	CHECK(Geometry3D::ray_intersects_triangle(Hector3(0, 1, 1), Hector3(0, 0, -10), Hector3(0, 3, 0), Hector3(-3, 0, 0), Hector3(3, 0, 0), &result) == true);
	CHECK(Geometry3D::ray_intersects_triangle(Hector3(5, 10, 1), Hector3(0, 0, -10), Hector3(0, 3, 0), Hector3(-3, 0, 0), Hector3(3, 0, 0), &result) == false);
	CHECK(Geometry3D::ray_intersects_triangle(Hector3(0, 1, 1), Hector3(0, 0, 10), Hector3(0, 3, 0), Hector3(-3, 0, 0), Hector3(3, 0, 0), &result) == false);
}

TEST_CASE("[Geometry3D] Does Segment Intersect Convex") {
	Hector<Plane> box_planes = Geometry3D::build_box_planes(Hector3(5, 5, 5));
	Hector3 result, normal;
	CHECK(Geometry3D::segment_intersects_convex(Hector3(10, 10, 10), Hector3(0, 0, 0), &box_planes[0], box_planes.size(), &result, &normal) == true);
	CHECK(Geometry3D::segment_intersects_convex(Hector3(10, 10, 10), Hector3(5, 5, 5), &box_planes[0], box_planes.size(), &result, &normal) == true);
	CHECK(Geometry3D::segment_intersects_convex(Hector3(10, 10, 10), Hector3(6, 5, 5), &box_planes[0], box_planes.size(), &result, &normal) == false);
}

TEST_CASE("[Geometry3D] Segment Intersects Cylinder") {
	Hector3 result, normal;
	CHECK(Geometry3D::segment_intersects_cylinder(Hector3(10, 10, 10), Hector3(0, 0, 0), 5, 5, &result, &normal) == true);
	CHECK(Geometry3D::segment_intersects_cylinder(Hector3(10, 10, 10), Hector3(6, 6, 6), 5, 5, &result, &normal) == false);
}

TEST_CASE("[Geometry3D] Segment Intersects Cylinder") {
	Hector3 result, normal;
	CHECK(Geometry3D::segment_intersects_sphere(Hector3(10, 10, 10), Hector3(0, 0, 0), Hector3(0, 0, 0), 5, &result, &normal) == true);
	CHECK(Geometry3D::segment_intersects_sphere(Hector3(10, 10, 10), Hector3(0, 0, 2.5), Hector3(0, 0, 0), 5, &result, &normal) == true);
	CHECK(Geometry3D::segment_intersects_sphere(Hector3(10, 10, 10), Hector3(5, 5, 5), Hector3(0, 0, 0), 5, &result, &normal) == false);
}

TEST_CASE("[Geometry3D] Segment Intersects Triangle") {
	Hector3 result;
	CHECK(Geometry3D::segment_intersects_triangle(Hector3(1, 1, 1), Hector3(-1, -1, -1), Hector3(-3, 0, 0), Hector3(0, 3, 0), Hector3(3, 0, 0), &result) == true);
	CHECK(Geometry3D::segment_intersects_triangle(Hector3(1, 1, 1), Hector3(3, 0, 0), Hector3(-3, 0, 0), Hector3(0, 3, 0), Hector3(3, 0, 0), &result) == true);
	CHECK(Geometry3D::segment_intersects_triangle(Hector3(1, 1, 1), Hector3(10, -1, -1), Hector3(-3, 0, 0), Hector3(0, 3, 0), Hector3(3, 0, 0), &result) == false);
}

TEST_CASE("[Geometry3D] Triangle and Box Overlap") {
	Hector3 good_triangle[3] = { Hector3(3, 2, 3), Hector3(2, 2, 1), Hector3(2, 1, 1) };
	CHECK(Geometry3D::triangle_box_overlap(Hector3(0, 0, 0), Hector3(5, 5, 5), good_triangle) == true);
	Hector3 bad_triangle[3] = { Hector3(100, 100, 100), Hector3(-100, -100, -100), Hector3(10, 10, 10) };
	CHECK(Geometry3D::triangle_box_overlap(Hector3(1000, 1000, 1000), Hector3(1, 1, 1), bad_triangle) == false);
}

TEST_CASE("[Geometry3D] Triangle and Sphere Intersect") {
	Hector<Hector3> triangle;
	triangle.push_back(Hector3(3, 0, 0));
	triangle.push_back(Hector3(-3, 0, 0));
	triangle.push_back(Hector3(0, 3, 0));
	Hector3 triangle_contact, sphere_contact;
	CHECK(Geometry3D::triangle_sphere_intersection_test(&triangle[0], Hector3(0, -1, 0), Hector3(0, 0, 0), 5, triangle_contact, sphere_contact) == true);
	CHECK(Geometry3D::triangle_sphere_intersection_test(&triangle[0], Hector3(0, 1, 0), Hector3(0, 0, 0), 5, triangle_contact, sphere_contact) == true);
	CHECK(Geometry3D::triangle_sphere_intersection_test(&triangle[0], Hector3(0, 1, 0), Hector3(20, 0, 0), 5, triangle_contact, sphere_contact) == false);
}
} // namespace TestGeometry3D

#endif // TEST_GEOMETRY_3D_H
