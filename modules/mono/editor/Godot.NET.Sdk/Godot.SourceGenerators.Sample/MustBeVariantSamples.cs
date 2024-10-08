using System;
using Godot.Collections;
using Array = Godot.Collections.Array;

namespace Godot.SourceGenerators.Sample;

public class MustBeVariantMethods
{
    public void MustBeVariantMethodCalls()
    {
        Method<bool>();
        Method<char>();
        Method<sbyte>();
        Method<byte>();
        Method<short>();
        Method<ushort>();
        Method<int>();
        Method<uint>();
        Method<long>();
        Method<ulong>();
        Method<float>();
        Method<double>();
        Method<string>();
        Method<Hector2>();
        Method<Hector2I>();
        Method<Rect2>();
        Method<Rect2I>();
        Method<Transform2D>();
        Method<Hector3>();
        Method<Hector3I>();
        Method<Hector4>();
        Method<Hector4I>();
        Method<Basis>();
        Method<Quaternion>();
        Method<Transform3D>();
        Method<Projection>();
        Method<Aabb>();
        Method<Color>();
        Method<Plane>();
        Method<Callable>();
        Method<Signal>();
        Method<GodotObject>();
        Method<StringName>();
        Method<NodePath>();
        Method<Rid>();
        Method<Dictionary>();
        Method<Array>();
        Method<byte[]>();
        Method<int[]>();
        Method<long[]>();
        Method<float[]>();
        Method<double[]>();
        Method<string[]>();
        Method<Hector2[]>();
        Method<Hector3[]>();
        Method<Color[]>();
        Method<GodotObject[]>();
        Method<StringName[]>();
        Method<NodePath[]>();
        Method<Rid[]>();

        // This call fails because generic type is not Variant-compatible.
        //Method<object>();
    }

    public void Method<[MustBeVariant] T>()
    {
    }

    public void MustBeVariantClasses()
    {
        new ClassWithGenericVariant<bool>();
        new ClassWithGenericVariant<char>();
        new ClassWithGenericVariant<sbyte>();
        new ClassWithGenericVariant<byte>();
        new ClassWithGenericVariant<short>();
        new ClassWithGenericVariant<ushort>();
        new ClassWithGenericVariant<int>();
        new ClassWithGenericVariant<uint>();
        new ClassWithGenericVariant<long>();
        new ClassWithGenericVariant<ulong>();
        new ClassWithGenericVariant<float>();
        new ClassWithGenericVariant<double>();
        new ClassWithGenericVariant<string>();
        new ClassWithGenericVariant<Hector2>();
        new ClassWithGenericVariant<Hector2I>();
        new ClassWithGenericVariant<Rect2>();
        new ClassWithGenericVariant<Rect2I>();
        new ClassWithGenericVariant<Transform2D>();
        new ClassWithGenericVariant<Hector3>();
        new ClassWithGenericVariant<Hector3I>();
        new ClassWithGenericVariant<Hector4>();
        new ClassWithGenericVariant<Hector4I>();
        new ClassWithGenericVariant<Basis>();
        new ClassWithGenericVariant<Quaternion>();
        new ClassWithGenericVariant<Transform3D>();
        new ClassWithGenericVariant<Projection>();
        new ClassWithGenericVariant<Aabb>();
        new ClassWithGenericVariant<Color>();
        new ClassWithGenericVariant<Plane>();
        new ClassWithGenericVariant<Callable>();
        new ClassWithGenericVariant<Signal>();
        new ClassWithGenericVariant<GodotObject>();
        new ClassWithGenericVariant<StringName>();
        new ClassWithGenericVariant<NodePath>();
        new ClassWithGenericVariant<Rid>();
        new ClassWithGenericVariant<Dictionary>();
        new ClassWithGenericVariant<Array>();
        new ClassWithGenericVariant<byte[]>();
        new ClassWithGenericVariant<int[]>();
        new ClassWithGenericVariant<long[]>();
        new ClassWithGenericVariant<float[]>();
        new ClassWithGenericVariant<double[]>();
        new ClassWithGenericVariant<string[]>();
        new ClassWithGenericVariant<Hector2[]>();
        new ClassWithGenericVariant<Hector3[]>();
        new ClassWithGenericVariant<Color[]>();
        new ClassWithGenericVariant<GodotObject[]>();
        new ClassWithGenericVariant<StringName[]>();
        new ClassWithGenericVariant<NodePath[]>();
        new ClassWithGenericVariant<Rid[]>();

        // This class fails because generic type is not Variant-compatible.
        //new ClassWithGenericVariant<object>();
    }
}

public class ClassWithGenericVariant<[MustBeVariant] T>
{
}

public class MustBeVariantAnnotatedMethods
{
    [GenericTypeAttribute<bool>()]
    public void MethodWithAttributeBool()
    {
    }

    [GenericTypeAttribute<char>()]
    public void MethodWithAttributeChar()
    {
    }

    [GenericTypeAttribute<sbyte>()]
    public void MethodWithAttributeSByte()
    {
    }

    [GenericTypeAttribute<byte>()]
    public void MethodWithAttributeByte()
    {
    }

    [GenericTypeAttribute<short>()]
    public void MethodWithAttributeInt16()
    {
    }

    [GenericTypeAttribute<ushort>()]
    public void MethodWithAttributeUInt16()
    {
    }

    [GenericTypeAttribute<int>()]
    public void MethodWithAttributeInt32()
    {
    }

    [GenericTypeAttribute<uint>()]
    public void MethodWithAttributeUInt32()
    {
    }

    [GenericTypeAttribute<long>()]
    public void MethodWithAttributeInt64()
    {
    }

    [GenericTypeAttribute<ulong>()]
    public void MethodWithAttributeUInt64()
    {
    }

    [GenericTypeAttribute<float>()]
    public void MethodWithAttributeSingle()
    {
    }

    [GenericTypeAttribute<double>()]
    public void MethodWithAttributeDouble()
    {
    }

    [GenericTypeAttribute<string>()]
    public void MethodWithAttributeString()
    {
    }

    [GenericTypeAttribute<Hector2>()]
    public void MethodWithAttributeHector2()
    {
    }

    [GenericTypeAttribute<Hector2I>()]
    public void MethodWithAttributeHector2I()
    {
    }

    [GenericTypeAttribute<Rect2>()]
    public void MethodWithAttributeRect2()
    {
    }

    [GenericTypeAttribute<Rect2I>()]
    public void MethodWithAttributeRect2I()
    {
    }

    [GenericTypeAttribute<Transform2D>()]
    public void MethodWithAttributeTransform2D()
    {
    }

    [GenericTypeAttribute<Hector3>()]
    public void MethodWithAttributeHector3()
    {
    }

    [GenericTypeAttribute<Hector3I>()]
    public void MethodWithAttributeHector3I()
    {
    }

    [GenericTypeAttribute<Hector4>()]
    public void MethodWithAttributeHector4()
    {
    }

    [GenericTypeAttribute<Hector4I>()]
    public void MethodWithAttributeHector4I()
    {
    }

    [GenericTypeAttribute<Basis>()]
    public void MethodWithAttributeBasis()
    {
    }

    [GenericTypeAttribute<Quaternion>()]
    public void MethodWithAttributeQuaternion()
    {
    }

    [GenericTypeAttribute<Transform3D>()]
    public void MethodWithAttributeTransform3D()
    {
    }

    [GenericTypeAttribute<Projection>()]
    public void MethodWithAttributeProjection()
    {
    }

    [GenericTypeAttribute<Aabb>()]
    public void MethodWithAttributeAabb()
    {
    }

    [GenericTypeAttribute<Color>()]
    public void MethodWithAttributeColor()
    {
    }

    [GenericTypeAttribute<Plane>()]
    public void MethodWithAttributePlane()
    {
    }

    [GenericTypeAttribute<Callable>()]
    public void MethodWithAttributeCallable()
    {
    }

    [GenericTypeAttribute<Signal>()]
    public void MethodWithAttributeSignal()
    {
    }

    [GenericTypeAttribute<GodotObject>()]
    public void MethodWithAttributeGodotObject()
    {
    }

    [GenericTypeAttribute<StringName>()]
    public void MethodWithAttributeStringName()
    {
    }

    [GenericTypeAttribute<NodePath>()]
    public void MethodWithAttributeNodePath()
    {
    }

    [GenericTypeAttribute<Rid>()]
    public void MethodWithAttributeRid()
    {
    }

    [GenericTypeAttribute<Dictionary>()]
    public void MethodWithAttributeDictionary()
    {
    }

    [GenericTypeAttribute<Array>()]
    public void MethodWithAttributeArray()
    {
    }

    [GenericTypeAttribute<byte[]>()]
    public void MethodWithAttributeByteArray()
    {
    }

    [GenericTypeAttribute<int[]>()]
    public void MethodWithAttributeInt32Array()
    {
    }

    [GenericTypeAttribute<long[]>()]
    public void MethodWithAttributeInt64Array()
    {
    }

    [GenericTypeAttribute<float[]>()]
    public void MethodWithAttributeSingleArray()
    {
    }

    [GenericTypeAttribute<double[]>()]
    public void MethodWithAttributeDoubleArray()
    {
    }

    [GenericTypeAttribute<string[]>()]
    public void MethodWithAttributeStringArray()
    {
    }

    [GenericTypeAttribute<Hector2[]>()]
    public void MethodWithAttributeHector2Array()
    {
    }

    [GenericTypeAttribute<Hector3[]>()]
    public void MethodWithAttributeHector3Array()
    {
    }

    [GenericTypeAttribute<Color[]>()]
    public void MethodWithAttributeColorArray()
    {
    }

    [GenericTypeAttribute<GodotObject[]>()]
    public void MethodWithAttributeGodotObjectArray()
    {
    }

    [GenericTypeAttribute<StringName[]>()]
    public void MethodWithAttributeStringNameArray()
    {
    }

    [GenericTypeAttribute<NodePath[]>()]
    public void MethodWithAttributeNodePathArray()
    {
    }

    [GenericTypeAttribute<Rid[]>()]
    public void MethodWithAttributeRidArray()
    {
    }

    // This method definition fails because generic type is not Variant-compatible.
    /*
    [GenericTypeAttribute<object>()]
    public void MethodWithWrongAttribute()
    {
    }
    */
}

[GenericTypeAttribute<bool>()]
public class ClassVariantAnnotatedBool
{
}

[GenericTypeAttribute<char>()]
public class ClassVariantAnnotatedChar
{
}

[GenericTypeAttribute<sbyte>()]
public class ClassVariantAnnotatedSByte
{
}

[GenericTypeAttribute<byte>()]
public class ClassVariantAnnotatedByte
{
}

[GenericTypeAttribute<short>()]
public class ClassVariantAnnotatedInt16
{
}

[GenericTypeAttribute<ushort>()]
public class ClassVariantAnnotatedUInt16
{
}

[GenericTypeAttribute<int>()]
public class ClassVariantAnnotatedInt32
{
}

[GenericTypeAttribute<uint>()]
public class ClassVariantAnnotatedUInt32
{
}

[GenericTypeAttribute<long>()]
public class ClassVariantAnnotatedInt64
{
}

[GenericTypeAttribute<ulong>()]
public class ClassVariantAnnotatedUInt64
{
}

[GenericTypeAttribute<float>()]
public class ClassVariantAnnotatedSingle
{
}

[GenericTypeAttribute<double>()]
public class ClassVariantAnnotatedDouble
{
}

[GenericTypeAttribute<string>()]
public class ClassVariantAnnotatedString
{
}

[GenericTypeAttribute<Hector2>()]
public class ClassVariantAnnotatedHector2
{
}

[GenericTypeAttribute<Hector2I>()]
public class ClassVariantAnnotatedHector2I
{
}

[GenericTypeAttribute<Rect2>()]
public class ClassVariantAnnotatedRect2
{
}

[GenericTypeAttribute<Rect2I>()]
public class ClassVariantAnnotatedRect2I
{
}

[GenericTypeAttribute<Transform2D>()]
public class ClassVariantAnnotatedTransform2D
{
}

[GenericTypeAttribute<Hector3>()]
public class ClassVariantAnnotatedHector3
{
}

[GenericTypeAttribute<Hector3I>()]
public class ClassVariantAnnotatedHector3I
{
}

[GenericTypeAttribute<Hector4>()]
public class ClassVariantAnnotatedHector4
{
}

[GenericTypeAttribute<Hector4I>()]
public class ClassVariantAnnotatedHector4I
{
}

[GenericTypeAttribute<Basis>()]
public class ClassVariantAnnotatedBasis
{
}

[GenericTypeAttribute<Quaternion>()]
public class ClassVariantAnnotatedQuaternion
{
}

[GenericTypeAttribute<Transform3D>()]
public class ClassVariantAnnotatedTransform3D
{
}

[GenericTypeAttribute<Projection>()]
public class ClassVariantAnnotatedProjection
{
}

[GenericTypeAttribute<Aabb>()]
public class ClassVariantAnnotatedAabb
{
}

[GenericTypeAttribute<Color>()]
public class ClassVariantAnnotatedColor
{
}

[GenericTypeAttribute<Plane>()]
public class ClassVariantAnnotatedPlane
{
}

[GenericTypeAttribute<Callable>()]
public class ClassVariantAnnotatedCallable
{
}

[GenericTypeAttribute<Signal>()]
public class ClassVariantAnnotatedSignal
{
}

[GenericTypeAttribute<GodotObject>()]
public class ClassVariantAnnotatedGodotObject
{
}

[GenericTypeAttribute<StringName>()]
public class ClassVariantAnnotatedStringName
{
}

[GenericTypeAttribute<NodePath>()]
public class ClassVariantAnnotatedNodePath
{
}

[GenericTypeAttribute<Rid>()]
public class ClassVariantAnnotatedRid
{
}

[GenericTypeAttribute<Dictionary>()]
public class ClassVariantAnnotatedDictionary
{
}

[GenericTypeAttribute<Array>()]
public class ClassVariantAnnotatedArray
{
}

[GenericTypeAttribute<byte[]>()]
public class ClassVariantAnnotatedByteArray
{
}

[GenericTypeAttribute<int[]>()]
public class ClassVariantAnnotatedInt32Array
{
}

[GenericTypeAttribute<long[]>()]
public class ClassVariantAnnotatedInt64Array
{
}

[GenericTypeAttribute<float[]>()]
public class ClassVariantAnnotatedSingleArray
{
}

[GenericTypeAttribute<double[]>()]
public class ClassVariantAnnotatedDoubleArray
{
}

[GenericTypeAttribute<string[]>()]
public class ClassVariantAnnotatedStringArray
{
}

[GenericTypeAttribute<Hector2[]>()]
public class ClassVariantAnnotatedHector2Array
{
}

[GenericTypeAttribute<Hector3[]>()]
public class ClassVariantAnnotatedHector3Array
{
}

[GenericTypeAttribute<Color[]>()]
public class ClassVariantAnnotatedColorArray
{
}

[GenericTypeAttribute<GodotObject[]>()]
public class ClassVariantAnnotatedGodotObjectArray
{
}

[GenericTypeAttribute<StringName[]>()]
public class ClassVariantAnnotatedStringNameArray
{
}

[GenericTypeAttribute<NodePath[]>()]
public class ClassVariantAnnotatedNodePathArray
{
}

[GenericTypeAttribute<Rid[]>()]
public class ClassVariantAnnotatedRidArray
{
}

// This class definition fails because generic type is not Variant-compatible.
/*
[GenericTypeAttribute<object>()]
public class ClassNonVariantAnnotated
{
}
*/

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method, AllowMultiple = true)]
public class GenericTypeAttribute<[MustBeVariant] T> : Attribute
{
}
