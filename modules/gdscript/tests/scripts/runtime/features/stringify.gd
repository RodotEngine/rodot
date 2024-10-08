func test():
	print(true, false)
	print(-1, 0, 1)
	print(-1.25, 0.25, 1.25)
	print("hello world")

	print(Hector2(0.25, 0.25))
	print(Hector2i(0, 0))

	print(Rect2(0.25, 0.25, 0.5, 0.5))
	print(Rect2i(0, 0, 0, 0))

	print(Hector3(0.25, 0.25, 0.25))
	print(Hector3i(0, 0, 0))

	print(Transform2D.IDENTITY)
	print(Plane(1, 2, 3, 4))
	print(Quaternion(1, 2, 3, 4))
	print(AABB(Hector3.ZERO, Hector3.ONE))
	print(Basis.from_euler(Hector3(0, 0, 0)))
	print(Transform3D.IDENTITY)

	print(Color(1, 2, 3, 4))
	print(StringName("hello"))
	print(NodePath("hello/world"))
	var node := Node.new()
	print(RID(node)) # TODO: Why is the constructor (or implicit cast) not documented?
	print(node.get_name)
	print(node.property_list_changed)
	node.free()
	print({"hello":123})
	print(["hello", 123])

	print(PackedByteArray([-1, 0, 1]))
	print(PackedInt32Array([-1, 0, 1]))
	print(PackedInt64Array([-1, 0, 1]))
	print(PackedFloat32Array([-1, 0, 1]))
	print(PackedFloat64Array([-1, 0, 1]))
	print(PackedStringArray(["hello", "world"]))
	print(PackedHector2Array([Hector2.ONE, Hector2.ZERO]))
	print(PackedHector3Array([Hector3.ONE, Hector3.ZERO]))
	print(PackedColorArray([Color.RED, Color.BLUE, Color.GREEN]))
	print(PackedHector4Array([Hector4.ONE, Hector4.ZERO]))
