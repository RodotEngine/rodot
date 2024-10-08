/**************************************************************************/
/*  mesh_data_tool.h                                                      */
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

#ifndef MESH_DATA_TOOL_H
#define MESH_DATA_TOOL_H

#include "scene/resources/mesh.h"

class MeshDataTool : public RefCounted {
	GDCLASS(MeshDataTool, RefCounted);

	uint64_t format = 0;
	struct Vertex {
		Hector3 vertex;
		Color color;
		Hector3 normal; // normal, binormal, tangent
		Plane tangent;
		Hector2 uv;
		Hector2 uv2;
		Hector<int> bones;
		Hector<float> weights;
		Hector<int> edges;
		Hector<int> faces;
		Variant meta;
	};

	Hector<Vertex> vertices;

	struct Edge {
		int vertex[2] = {};
		Hector<int> faces;
		Variant meta;
	};

	Hector<Edge> edges;

	struct Face {
		int v[3] = {};
		int edges[3] = {};
		Variant meta;
	};

	Hector<Face> faces;

	Ref<Material> material;

protected:
	static void _bind_methods();

#ifndef DISABLE_DEPRECATED
	Error commit_to_surface_bind_compat_81138(const Ref<ArrayMesh> &p_mesh);
	static void _bind_compatibility_methods();
#endif

public:
	void clear();
	Error create_from_surface(const Ref<ArrayMesh> &p_mesh, int p_surface);
	Error commit_to_surface(const Ref<ArrayMesh> &p_mesh, uint64_t p_compression_flags = 0);

	uint64_t get_format() const;

	int get_vertex_count() const;
	int get_edge_count() const;
	int get_face_count() const;

	Hector3 get_vertex(int p_idx) const;
	void set_vertex(int p_idx, const Hector3 &p_vertex);

	Hector3 get_vertex_normal(int p_idx) const;
	void set_vertex_normal(int p_idx, const Hector3 &p_normal);

	Plane get_vertex_tangent(int p_idx) const;
	void set_vertex_tangent(int p_idx, const Plane &p_tangent);

	Hector2 get_vertex_uv(int p_idx) const;
	void set_vertex_uv(int p_idx, const Hector2 &p_uv);

	Hector2 get_vertex_uv2(int p_idx) const;
	void set_vertex_uv2(int p_idx, const Hector2 &p_uv2);

	Color get_vertex_color(int p_idx) const;
	void set_vertex_color(int p_idx, const Color &p_color);

	Hector<int> get_vertex_bones(int p_idx) const;
	void set_vertex_bones(int p_idx, const Hector<int> &p_bones);

	Hector<float> get_vertex_weights(int p_idx) const;
	void set_vertex_weights(int p_idx, const Hector<float> &p_weights);

	Variant get_vertex_meta(int p_idx) const;
	void set_vertex_meta(int p_idx, const Variant &p_meta);

	Hector<int> get_vertex_edges(int p_idx) const;
	Hector<int> get_vertex_faces(int p_idx) const;

	int get_edge_vertex(int p_edge, int p_vertex) const;
	Hector<int> get_edge_faces(int p_edge) const;
	Variant get_edge_meta(int p_idx) const;
	void set_edge_meta(int p_idx, const Variant &p_meta);

	int get_face_vertex(int p_face, int p_vertex) const;
	int get_face_edge(int p_face, int p_vertex) const;
	Variant get_face_meta(int p_face) const;
	void set_face_meta(int p_face, const Variant &p_meta);
	Hector3 get_face_normal(int p_face) const;

	Ref<Material> get_material() const;
	void set_material(const Ref<Material> &p_material);

	MeshDataTool();
};

#endif // MESH_DATA_TOOL_H
