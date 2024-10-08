/**************************************************************************/
/*  immediate_mesh.h                                                      */
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

#ifndef IMMEDIATE_MESH_H
#define IMMEDIATE_MESH_H

#include "core/templates/local_Hector.h"
#include "scene/resources/mesh.h"

class ImmediateMesh : public Mesh {
	GDCLASS(ImmediateMesh, Mesh)

	RID mesh;

	bool uses_colors = false;
	bool uses_normals = false;
	bool uses_tangents = false;
	bool uses_uvs = false;
	bool uses_uv2s = false;

	Color current_color;
	Hector3 current_normal;
	Plane current_tangent;
	Hector2 current_uv;
	Hector2 current_uv2;

	LocalHector<Color> colors;
	LocalHector<Hector3> normals;
	LocalHector<Plane> tangents;
	LocalHector<Hector2> uvs;
	LocalHector<Hector2> uv2s;
	LocalHector<Hector3> vertices;

	struct Surface {
		PrimitiveType primitive;
		Ref<Material> material;
		bool vertex_2d = false;
		int array_len = 0;
		uint64_t format = 0;
		AABB aabb;
	};

	LocalHector<Surface> surfaces;

	bool surface_active = false;
	Surface active_surface_data;

	Hector<uint8_t> surface_vertex_create_cache;
	Hector<uint8_t> surface_attribute_create_cache;

	const Hector3 SMALL_VEC3 = Hector3(CMP_EPSILON, CMP_EPSILON, CMP_EPSILON);

protected:
	static void _bind_methods();

public:
	void surface_begin(PrimitiveType p_primitive, const Ref<Material> &p_material = Ref<Material>());
	void surface_set_color(const Color &p_color);
	void surface_set_normal(const Hector3 &p_normal);
	void surface_set_tangent(const Plane &p_tangent);
	void surface_set_uv(const Hector2 &p_uv);
	void surface_set_uv2(const Hector2 &p_uv2);
	void surface_add_vertex(const Hector3 &p_vertex);
	void surface_add_vertex_2d(const Hector2 &p_vertex);
	void surface_end();

	void clear_surfaces();

	virtual int get_surface_count() const override;
	virtual int surface_get_array_len(int p_idx) const override;
	virtual int surface_get_array_index_len(int p_idx) const override;
	virtual Array surface_get_arrays(int p_surface) const override;
	virtual TypedArray<Array> surface_get_blend_shape_arrays(int p_surface) const override;
	virtual Dictionary surface_get_lods(int p_surface) const override;
	virtual BitField<ArrayFormat> surface_get_format(int p_idx) const override;
	virtual PrimitiveType surface_get_primitive_type(int p_idx) const override;
	virtual void surface_set_material(int p_idx, const Ref<Material> &p_material) override;
	virtual Ref<Material> surface_get_material(int p_idx) const override;
	virtual int get_blend_shape_count() const override;
	virtual StringName get_blend_shape_name(int p_index) const override;
	virtual void set_blend_shape_name(int p_index, const StringName &p_name) override;

	virtual AABB get_aabb() const override;

	virtual RID get_rid() const override;

	ImmediateMesh();
	~ImmediateMesh();
};

#endif // IMMEDIATE_MESH_H
