/**************************************************************************/
/*  godot_shape_3d.h                                                      */
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

#ifndef GODOT_SHAPE_3D_H
#define GODOT_SHAPE_3D_H

#include "core/math/geometry_3d.h"
#include "core/templates/local_Hector.h"
#include "servers/physics_server_3d.h"

class GodotShape3D;

class GodotShapeOwner3D {
public:
	virtual void _shape_changed() = 0;
	virtual void remove_shape(GodotShape3D *p_shape) = 0;

	virtual ~GodotShapeOwner3D() {}
};

class GodotShape3D {
	RID self;
	AABB aabb;
	bool configured = false;
	real_t custom_bias = 0.0;

	HashMap<GodotShapeOwner3D *, int> owners;

protected:
	void configure(const AABB &p_aabb);

public:
	enum FeatureType {
		FEATURE_POINT,
		FEATURE_EDGE,
		FEATURE_FACE,
		FEATURE_CIRCLE,
	};

	virtual real_t get_volume() const { return aabb.get_volume(); }

	_FORCE_INLINE_ void set_self(const RID &p_self) { self = p_self; }
	_FORCE_INLINE_ RID get_self() const { return self; }

	virtual PhysicsServer3D::ShapeType get_type() const = 0;

	_FORCE_INLINE_ const AABB &get_aabb() const { return aabb; }
	_FORCE_INLINE_ bool is_configured() const { return configured; }

	virtual bool is_concave() const { return false; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const = 0;
	virtual Hector3 get_support(const Hector3 &p_normal) const;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const = 0;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const = 0;
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_point, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const = 0;
	virtual bool intersect_point(const Hector3 &p_point) const = 0;
	virtual Hector3 get_moment_of_inertia(real_t p_mass) const = 0;

	virtual void set_data(const Variant &p_data) = 0;
	virtual Variant get_data() const = 0;

	_FORCE_INLINE_ void set_custom_bias(real_t p_bias) { custom_bias = p_bias; }
	_FORCE_INLINE_ real_t get_custom_bias() const { return custom_bias; }

	void add_owner(GodotShapeOwner3D *p_owner);
	void remove_owner(GodotShapeOwner3D *p_owner);
	bool is_owner(GodotShapeOwner3D *p_owner) const;
	const HashMap<GodotShapeOwner3D *, int> &get_owners() const;

	GodotShape3D() {}
	virtual ~GodotShape3D();
};

class GodotConcaveShape3D : public GodotShape3D {
public:
	virtual bool is_concave() const override { return true; }
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override { r_amount = 0; }

	// Returns true to stop the query.
	typedef bool (*QueryCallback)(void *p_userdata, GodotShape3D *p_convex);

	virtual void cull(const AABB &p_local_aabb, QueryCallback p_callback, void *p_userdata, bool p_invert_backface_collision) const = 0;

	GodotConcaveShape3D() {}
};

class GodotWorldBoundaryShape3D : public GodotShape3D {
	Plane plane;

	void _setup(const Plane &p_plane);

public:
	Plane get_plane() const;

	virtual real_t get_volume() const override { return INFINITY; }
	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_WORLD_BOUNDARY; }
	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override { r_amount = 0; }

	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;
	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotWorldBoundaryShape3D();
};

class GodotSeparationRayShape3D : public GodotShape3D {
	real_t length = 1.0;
	bool slide_on_slope = false;

	void _setup(real_t p_length, bool p_slide_on_slope);

public:
	real_t get_length() const;
	bool get_slide_on_slope() const;

	virtual real_t get_volume() const override { return 0.0; }
	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_SEPARATION_RAY; }
	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override;

	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotSeparationRayShape3D();
};

class GodotSphereShape3D : public GodotShape3D {
	real_t radius = 0.0;

	void _setup(real_t p_radius);

public:
	real_t get_radius() const;

	virtual real_t get_volume() const override { return 4.0 / 3.0 * Math_PI * radius * radius * radius; }

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_SPHERE; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override;
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotSphereShape3D();
};

class GodotBoxShape3D : public GodotShape3D {
	Hector3 half_extents;
	void _setup(const Hector3 &p_half_extents);

public:
	_FORCE_INLINE_ Hector3 get_half_extents() const { return half_extents; }
	virtual real_t get_volume() const override { return 8 * half_extents.x * half_extents.y * half_extents.z; }

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_BOX; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override;
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotBoxShape3D();
};

class GodotCapsuleShape3D : public GodotShape3D {
	real_t height = 0.0;
	real_t radius = 0.0;

	void _setup(real_t p_height, real_t p_radius);

public:
	_FORCE_INLINE_ real_t get_height() const { return height; }
	_FORCE_INLINE_ real_t get_radius() const { return radius; }

	virtual real_t get_volume() const override { return 4.0 / 3.0 * Math_PI * radius * radius * radius + (height - radius * 2.0) * Math_PI * radius * radius; }

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_CAPSULE; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override;
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotCapsuleShape3D();
};

class GodotCylinderShape3D : public GodotShape3D {
	real_t height = 0.0;
	real_t radius = 0.0;

	void _setup(real_t p_height, real_t p_radius);

public:
	_FORCE_INLINE_ real_t get_height() const { return height; }
	_FORCE_INLINE_ real_t get_radius() const { return radius; }

	virtual real_t get_volume() const override { return height * Math_PI * radius * radius; }

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_CYLINDER; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override;
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotCylinderShape3D();
};

struct GodotConvexPolygonShape3D : public GodotShape3D {
	Geometry3D::MeshData mesh;
	LocalHector<int> extreme_vertices;
	LocalHector<LocalHector<int>> vertex_neighbors;

	void _setup(const Hector<Hector3> &p_vertices);

public:
	const Geometry3D::MeshData &get_mesh() const { return mesh; }

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_CONVEX_POLYGON; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override;
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotConvexPolygonShape3D();
};

struct _Volume_BVH;
struct GodotFaceShape3D;

struct GodotConcavePolygonShape3D : public GodotConcaveShape3D {
	// always a trimesh

	struct Face {
		Hector3 normal;
		int indices[3] = {};
	};

	Hector<Face> faces;
	Hector<Hector3> vertices;

	struct BVH {
		AABB aabb;
		int left = 0;
		int right = 0;

		int face_index = 0;
	};

	Hector<BVH> bvh;

	struct _CullParams {
		AABB aabb;
		QueryCallback callback = nullptr;
		void *userdata = nullptr;
		const Face *faces = nullptr;
		const Hector3 *vertices = nullptr;
		const BVH *bvh = nullptr;
		GodotFaceShape3D *face = nullptr;
	};

	struct _SegmentCullParams {
		Hector3 from;
		Hector3 to;
		Hector3 dir;
		const Face *faces = nullptr;
		const Hector3 *vertices = nullptr;
		const BVH *bvh = nullptr;
		GodotFaceShape3D *face = nullptr;

		Hector3 result;
		Hector3 normal;
		int face_index = -1;
		real_t min_d = 1e20;
		int collisions = 0;
	};

	bool backface_collision = false;

	void _cull_segment(int p_idx, _SegmentCullParams *p_params) const;
	bool _cull(int p_idx, _CullParams *p_params) const;

	void _fill_bvh(_Volume_BVH *p_bvh_tree, BVH *p_bvh_array, int &p_idx);

	void _setup(const Hector<Hector3> &p_faces, bool p_backface_collision);

public:
	Hector<Hector3> get_faces() const;

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_CONCAVE_POLYGON; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;

	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;

	virtual void cull(const AABB &p_local_aabb, QueryCallback p_callback, void *p_userdata, bool p_invert_backface_collision) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotConcavePolygonShape3D();
};

struct GodotHeightMapShape3D : public GodotConcaveShape3D {
	Hector<real_t> heights;
	int width = 0;
	int depth = 0;
	Hector3 local_origin;

	// Accelerator.
	struct Range {
		real_t min = 0.0;
		real_t max = 0.0;
	};
	LocalHector<Range> bounds_grid;
	int bounds_grid_width = 0;
	int bounds_grid_depth = 0;

	static const int BOUNDS_CHUNK_SIZE = 16;

	_FORCE_INLINE_ const Range &_get_bounds_chunk(int p_x, int p_z) const {
		return bounds_grid[(p_z * bounds_grid_width) + p_x];
	}

	_FORCE_INLINE_ real_t _get_height(int p_x, int p_z) const {
		return heights[(p_z * width) + p_x];
	}

	_FORCE_INLINE_ void _get_point(int p_x, int p_z, Hector3 &r_point) const {
		r_point.x = p_x - 0.5 * (width - 1.0);
		r_point.y = _get_height(p_x, p_z);
		r_point.z = p_z - 0.5 * (depth - 1.0);
	}

	void _get_cell(const Hector3 &p_point, int &r_x, int &r_y, int &r_z) const;

	void _build_accelerator();

	template <typename ProcessFunction>
	bool _intersect_grid_segment(ProcessFunction &p_process, const Hector3 &p_begin, const Hector3 &p_end, int p_width, int p_depth, const Hector3 &offset, Hector3 &r_point, Hector3 &r_normal) const;

	void _setup(const Hector<real_t> &p_heights, int p_width, int p_depth, real_t p_min_height, real_t p_max_height);

public:
	Hector<real_t> get_heights() const;
	int get_width() const;
	int get_depth() const;

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_HEIGHTMAP; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_point, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;

	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;
	virtual void cull(const AABB &p_local_aabb, QueryCallback p_callback, void *p_userdata, bool p_invert_backface_collision) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override;
	virtual Variant get_data() const override;

	GodotHeightMapShape3D();
};

//used internally
struct GodotFaceShape3D : public GodotShape3D {
	Hector3 normal; //cache
	Hector3 vertex[3];
	bool backface_collision = false;
	bool invert_backface_collision = false;

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_CONCAVE_POLYGON; }

	const Hector3 &get_vertex(int p_idx) const { return vertex[p_idx]; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override;
	virtual Hector3 get_support(const Hector3 &p_normal) const override;
	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override;
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override;
	virtual bool intersect_point(const Hector3 &p_point) const override;
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override;

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override;

	virtual void set_data(const Variant &p_data) override {}
	virtual Variant get_data() const override { return Variant(); }

	GodotFaceShape3D();
};

struct GodotMotionShape3D : public GodotShape3D {
	GodotShape3D *shape = nullptr;
	Hector3 motion;

	virtual PhysicsServer3D::ShapeType get_type() const override { return PhysicsServer3D::SHAPE_CONVEX_POLYGON; }

	virtual void project_range(const Hector3 &p_normal, const Transform3D &p_transform, real_t &r_min, real_t &r_max) const override {
		Hector3 cast = p_transform.basis.xform(motion);
		real_t mina, maxa;
		real_t minb, maxb;
		Transform3D ofsb = p_transform;
		ofsb.origin += cast;
		shape->project_range(p_normal, p_transform, mina, maxa);
		shape->project_range(p_normal, ofsb, minb, maxb);
		r_min = MIN(mina, minb);
		r_max = MAX(maxa, maxb);
	}

	virtual Hector3 get_support(const Hector3 &p_normal) const override {
		Hector3 support = shape->get_support(p_normal);
		if (p_normal.dot(motion) > 0) {
			support += motion;
		}
		return support;
	}

	virtual void get_supports(const Hector3 &p_normal, int p_max, Hector3 *r_supports, int &r_amount, FeatureType &r_type) const override { r_amount = 0; }
	virtual bool intersect_segment(const Hector3 &p_begin, const Hector3 &p_end, Hector3 &r_result, Hector3 &r_normal, int &r_face_index, bool p_hit_back_faces) const override { return false; }
	virtual bool intersect_point(const Hector3 &p_point) const override { return false; }
	virtual Hector3 get_closest_point_to(const Hector3 &p_point) const override { return p_point; }

	virtual Hector3 get_moment_of_inertia(real_t p_mass) const override { return Hector3(); }

	virtual void set_data(const Variant &p_data) override {}
	virtual Variant get_data() const override { return Variant(); }

	GodotMotionShape3D() { configure(AABB()); }
};

#endif // GODOT_SHAPE_3D_H
