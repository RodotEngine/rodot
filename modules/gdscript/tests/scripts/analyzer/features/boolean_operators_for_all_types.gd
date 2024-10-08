extends Resource

signal foo

func test():
	var x
	# TYPE_NIL
	x = null
	prints("TYPE_NIL")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_BOOL
	x = true
	prints("TYPE_BOOL")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_INT
	x = 1
	prints("TYPE_INT")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_FLOAT
	x = 1.1
	prints("TYPE_FLOAT")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_STRING
	x = "foo"
	prints("TYPE_STRING")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_HECTOR2
	x = Hector2(1, 1)
	prints("TYPE_HECTOR2")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_HECTOR2I
	x = Hector2i(1, 1)
	prints("TYPE_HECTOR2I")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_RECT2
	x = Rect2(1, 1, 1, 1)
	prints("TYPE_RECT2")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_RECT2I
	x = Rect2i(1, 1, 1, 1)
	prints("TYPE_RECT2I")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_HECTOR3
	x = Hector3(1, 1, 1)
	prints("TYPE_HECTOR3")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_HECTOR3I
	x = Hector3i(1, 1, 1)
	prints("TYPE_HECTOR3I")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_TRANSFORM2D
	x = Transform2D.IDENTITY
	prints("TYPE_TRANSFORM2D")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_HECTOR4
	x = Hector4(1, 1, 1, 1)
	prints("TYPE_HECTOR4")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_HECTOR4I
	x = Hector4i(1, 1, 1, 1)
	prints("TYPE_HECTOR4I")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PLANE
	x = Plane.PLANE_XY
	prints("TYPE_PLANE")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_QUATERNION
	x = Quaternion.IDENTITY
	prints("TYPE_QUATERNION")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_AABB
	x = AABB(Hector3.ONE, Hector3.ONE)
	prints("TYPE_AABB")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_BASIS
	x = Basis.IDENTITY
	prints("TYPE_BASIS")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_TRANSFORM3D
	x = Transform3D.IDENTITY
	prints("TYPE_TRANSFORM3D")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PROJECTION
	x = Projection.IDENTITY
	prints("TYPE_PROJECTION")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_COLOR
	x = Color.WHITE
	prints("TYPE_COLOR")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_STRING_NAME
	x = &"name"
	prints("TYPE_STRING_NAME")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_NODE_PATH
	x = ^"path"
	prints("TYPE_NODE_PATH")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_RID
	x = get_rid()
	prints("TYPE_RID")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_OBJECT
	x = self
	prints("TYPE_OBJECT")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_CALLABLE
	x = test
	prints("TYPE_CALLABLE")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_SIGNAL
	x = foo
	prints("TYPE_SIGNAL")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_DICTIONARY
	x = { a = 1}
	prints("TYPE_DICTIONARY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_ARRAY
	x = [1]
	prints("TYPE_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_BYTE_ARRAY
	x = PackedByteArray([1])
	prints("TYPE_PACKED_BYTE_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_INT32_ARRAY
	x = PackedInt32Array([1])
	prints("TYPE_PACKED_INT32_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_INT64_ARRAY
	x = PackedInt64Array([1])
	prints("TYPE_PACKED_INT64_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_FLOAT32_ARRAY
	x = PackedFloat32Array([1])
	prints("TYPE_PACKED_FLOAT32_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_FLOAT64_ARRAY
	x = PackedFloat64Array([1])
	prints("TYPE_PACKED_FLOAT64_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_STRING_ARRAY
	x = PackedStringArray(["1"])
	prints("TYPE_PACKED_STRING_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_Hector2_ARRAY
	x = PackedHector2Array([Hector2.ONE])
	prints("TYPE_PACKED_Hector2_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_Hector3_ARRAY
	x = PackedHector3Array([Hector3.ONE])
	prints("TYPE_PACKED_Hector3_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_COLOR_ARRAY
	x = PackedColorArray([Color.WHITE])
	prints("TYPE_PACKED_COLOR_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)

	# TYPE_PACKED_Hector4_ARRAY
	x = PackedHector4Array([Hector4.ONE])
	prints("TYPE_PACKED_Hector4_ARRAY")
	prints(not x)
	prints(x and false)
	prints(x and true)
	prints(x or false)
	prints(x or true)
