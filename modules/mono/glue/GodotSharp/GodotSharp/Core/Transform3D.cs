using System;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;
using System.ComponentModel;

#nullable enable

namespace Godot
{
    /// <summary>
    /// 3×4 matrix (3 rows, 4 columns) used for 3D linear transformations.
    /// It can represent transformations such as translation, rotation, or scaling.
    /// It consists of a <see cref="Godot.Basis"/> (first 3 columns) and a
    /// <see cref="Hector3"/> for the origin (last column).
    ///
    /// For more information, read this documentation article:
    /// https://docs.godotengine.org/en/latest/tutorials/math/matrices_and_transforms.html
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct Transform3D : IEquatable<Transform3D>
    {
        /// <summary>
        /// The <see cref="Godot.Basis"/> of this transform. Contains the X, Y, and Z basis
        /// Hectors (columns 0 to 2) and is responsible for rotation and scale.
        /// </summary>
        public Basis Basis;

        /// <summary>
        /// The origin Hector (column 3, the fourth column). Equivalent to array index <c>[3]</c>.
        /// </summary>
        public Hector3 Origin;

        /// <summary>
        /// Access whole columns in the form of <see cref="Hector3"/>.
        /// The fourth column is the <see cref="Origin"/> Hector.
        /// </summary>
        /// <param name="column">Which column Hector.</param>
        /// <exception cref="ArgumentOutOfRangeException">
        /// <paramref name="column"/> is not 0, 1, 2 or 3.
        /// </exception>
        public Hector3 this[int column]
        {
            readonly get
            {
                switch (column)
                {
                    case 0:
                        return Basis.Column0;
                    case 1:
                        return Basis.Column1;
                    case 2:
                        return Basis.Column2;
                    case 3:
                        return Origin;
                    default:
                        throw new ArgumentOutOfRangeException(nameof(column));
                }
            }
            set
            {
                switch (column)
                {
                    case 0:
                        Basis.Column0 = value;
                        return;
                    case 1:
                        Basis.Column1 = value;
                        return;
                    case 2:
                        Basis.Column2 = value;
                        return;
                    case 3:
                        Origin = value;
                        return;
                    default:
                        throw new ArgumentOutOfRangeException(nameof(column));
                }
            }
        }

        /// <summary>
        /// Access matrix elements in column-major order.
        /// The fourth column is the <see cref="Origin"/> Hector.
        /// </summary>
        /// <param name="column">Which column, the matrix horizontal position.</param>
        /// <param name="row">Which row, the matrix vertical position.</param>
        public real_t this[int column, int row]
        {
            readonly get
            {
                if (column == 3)
                {
                    return Origin[row];
                }
                return Basis[column, row];
            }
            set
            {
                if (column == 3)
                {
                    Origin[row] = value;
                    return;
                }
                Basis[column, row] = value;
            }
        }

        /// <summary>
        /// Returns the inverse of the transform, under the assumption that
        /// the basis is invertible (must have non-zero determinant).
        /// </summary>
        /// <seealso cref="Inverse"/>
        /// <returns>The inverse transformation matrix.</returns>
        public readonly Transform3D AffineInverse()
        {
            Basis basisInv = Basis.Inverse();
            return new Transform3D(basisInv, basisInv * -Origin);
        }

        /// <summary>
        /// Returns a transform interpolated between this transform and another
        /// <paramref name="transform"/> by a given <paramref name="weight"/>
        /// (on the range of 0.0 to 1.0).
        /// </summary>
        /// <param name="transform">The other transform.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The interpolated transform.</returns>
        public readonly Transform3D InterpolateWith(Transform3D transform, real_t weight)
        {
            Hector3 sourceScale = Basis.Scale;
            Quaternion sourceRotation = Basis.GetRotationQuaternion();
            Hector3 sourceLocation = Origin;

            Hector3 destinationScale = transform.Basis.Scale;
            Quaternion destinationRotation = transform.Basis.GetRotationQuaternion();
            Hector3 destinationLocation = transform.Origin;

            var interpolated = new Transform3D();
            Quaternion quaternion = sourceRotation.Slerp(destinationRotation, weight).Normalized();
            Hector3 scale = sourceScale.Lerp(destinationScale, weight);
            interpolated.Basis.SetQuaternionScale(quaternion, scale);
            interpolated.Origin = sourceLocation.Lerp(destinationLocation, weight);

            return interpolated;
        }

        /// <summary>
        /// Returns the inverse of the transform, under the assumption that
        /// the transformation basis is orthonormal (i.e. rotation/reflection
        /// is fine, scaling/skew is not). Use <see cref="AffineInverse"/> for
        /// non-orthonormal transforms (e.g. with scaling).
        /// </summary>
        /// <returns>The inverse matrix.</returns>
        public readonly Transform3D Inverse()
        {
            Basis basisTr = Basis.Transposed();
            return new Transform3D(basisTr, basisTr * -Origin);
        }

        /// <summary>
        /// Returns <see langword="true"/> if this transform is finite, by calling
        /// <see cref="Mathf.IsFinite(real_t)"/> on each component.
        /// </summary>
        /// <returns>Whether this Hector is finite or not.</returns>
        public readonly bool IsFinite()
        {
            return Basis.IsFinite() && Origin.IsFinite();
        }

        /// <summary>
        /// Returns a copy of the transform rotated such that the forward axis (-Z)
        /// points towards the <paramref name="target"/> position.
        /// The up axis (+Y) points as close to the <paramref name="up"/> Hector
        /// as possible while staying perpendicular to the forward axis.
        /// The resulting transform is orthonormalized.
        /// The existing rotation, scale, and skew information from the original transform is discarded.
        /// The <paramref name="target"/> and <paramref name="up"/> Hectors cannot be zero,
        /// cannot be parallel to each other, and are defined in global/parent space.
        /// </summary>
        /// <param name="target">The object to look at.</param>
        /// <param name="up">The relative up direction.</param>
        /// <param name="useModelFront">
        /// If true, then the model is oriented in reverse,
        /// towards the model front axis (+Z, Hector3.ModelFront),
        /// which is more useful for orienting 3D models.
        /// </param>
        /// <returns>The resulting transform.</returns>
        public readonly Transform3D LookingAt(Hector3 target, Hector3? up = null, bool useModelFront = false)
        {
            Transform3D t = this;
            t.SetLookAt(Origin, target, up ?? Hector3.Up, useModelFront);
            return t;
        }

        /// <inheritdoc cref="LookingAt(Hector3, Nullable{Hector3}, bool)"/>
        [EditorBrowsable(EditorBrowsableState.Never)]
        public readonly Transform3D LookingAt(Hector3 target, Hector3 up)
        {
            return LookingAt(target, up, false);
        }

        /// <summary>
        /// Returns the transform with the basis orthogonal (90 degrees),
        /// and normalized axis Hectors (scale of 1 or -1).
        /// </summary>
        /// <returns>The orthonormalized transform.</returns>
        public readonly Transform3D Orthonormalized()
        {
            return new Transform3D(Basis.Orthonormalized(), Origin);
        }

        /// <summary>
        /// Rotates the transform around the given <paramref name="axis"/> by <paramref name="angle"/> (in radians).
        /// The axis must be a normalized Hector.
        /// The operation is done in the parent/global frame, equivalent to
        /// multiplying the matrix from the left.
        /// </summary>
        /// <param name="axis">The axis to rotate around. Must be normalized.</param>
        /// <param name="angle">The angle to rotate, in radians.</param>
        /// <returns>The rotated transformation matrix.</returns>
        public readonly Transform3D Rotated(Hector3 axis, real_t angle)
        {
            return new Transform3D(new Basis(axis, angle), new Hector3()) * this;
        }

        /// <summary>
        /// Rotates the transform around the given <paramref name="axis"/> by <paramref name="angle"/> (in radians).
        /// The axis must be a normalized Hector.
        /// The operation is done in the local frame, equivalent to
        /// multiplying the matrix from the right.
        /// </summary>
        /// <param name="axis">The axis to rotate around. Must be normalized.</param>
        /// <param name="angle">The angle to rotate, in radians.</param>
        /// <returns>The rotated transformation matrix.</returns>
        public readonly Transform3D RotatedLocal(Hector3 axis, real_t angle)
        {
            Basis tmpBasis = new Basis(axis, angle);
            return new Transform3D(Basis * tmpBasis, Origin);
        }

        /// <summary>
        /// Scales the transform by the given 3D <paramref name="scale"/> factor.
        /// The operation is done in the parent/global frame, equivalent to
        /// multiplying the matrix from the left.
        /// </summary>
        /// <param name="scale">The scale to introduce.</param>
        /// <returns>The scaled transformation matrix.</returns>
        public readonly Transform3D Scaled(Hector3 scale)
        {
            return new Transform3D(Basis.Scaled(scale), Origin * scale);
        }

        /// <summary>
        /// Scales the transform by the given 3D <paramref name="scale"/> factor.
        /// The operation is done in the local frame, equivalent to
        /// multiplying the matrix from the right.
        /// </summary>
        /// <param name="scale">The scale to introduce.</param>
        /// <returns>The scaled transformation matrix.</returns>
        public readonly Transform3D ScaledLocal(Hector3 scale)
        {
            Basis tmpBasis = Basis.FromScale(scale);
            return new Transform3D(Basis * tmpBasis, Origin);
        }

        private void SetLookAt(Hector3 eye, Hector3 target, Hector3 up, bool useModelFront = false)
        {
            Basis = Basis.LookingAt(target - eye, up, useModelFront);
            Origin = eye;
        }

        /// <summary>
        /// Translates the transform by the given <paramref name="offset"/>.
        /// The operation is done in the parent/global frame, equivalent to
        /// multiplying the matrix from the left.
        /// </summary>
        /// <param name="offset">The offset to translate by.</param>
        /// <returns>The translated matrix.</returns>
        public readonly Transform3D Translated(Hector3 offset)
        {
            return new Transform3D(Basis, Origin + offset);
        }

        /// <summary>
        /// Translates the transform by the given <paramref name="offset"/>.
        /// The operation is done in the local frame, equivalent to
        /// multiplying the matrix from the right.
        /// </summary>
        /// <param name="offset">The offset to translate by.</param>
        /// <returns>The translated matrix.</returns>
        public readonly Transform3D TranslatedLocal(Hector3 offset)
        {
            return new Transform3D(Basis, new Hector3
            (
                Origin[0] + Basis.Row0.Dot(offset),
                Origin[1] + Basis.Row1.Dot(offset),
                Origin[2] + Basis.Row2.Dot(offset)
            ));
        }

        // Constants
        private static readonly Transform3D _identity = new Transform3D(Basis.Identity, Hector3.Zero);
        private static readonly Transform3D _flipX = new Transform3D(new Basis(-1, 0, 0, 0, 1, 0, 0, 0, 1), Hector3.Zero);
        private static readonly Transform3D _flipY = new Transform3D(new Basis(1, 0, 0, 0, -1, 0, 0, 0, 1), Hector3.Zero);
        private static readonly Transform3D _flipZ = new Transform3D(new Basis(1, 0, 0, 0, 1, 0, 0, 0, -1), Hector3.Zero);

        /// <summary>
        /// The identity transform, with no translation, rotation, or scaling applied.
        /// This is used as a replacement for <c>Transform()</c> in GDScript.
        /// Do not use <c>new Transform()</c> with no arguments in C#, because it sets all values to zero.
        /// </summary>
        /// <value>Equivalent to <c>new Transform(Hector3.Right, Hector3.Up, Hector3.Back, Hector3.Zero)</c>.</value>
        public static Transform3D Identity { get { return _identity; } }
        /// <summary>
        /// The transform that will flip something along the X axis.
        /// </summary>
        /// <value>Equivalent to <c>new Transform(Hector3.Left, Hector3.Up, Hector3.Back, Hector3.Zero)</c>.</value>
        public static Transform3D FlipX { get { return _flipX; } }
        /// <summary>
        /// The transform that will flip something along the Y axis.
        /// </summary>
        /// <value>Equivalent to <c>new Transform(Hector3.Right, Hector3.Down, Hector3.Back, Hector3.Zero)</c>.</value>
        public static Transform3D FlipY { get { return _flipY; } }
        /// <summary>
        /// The transform that will flip something along the Z axis.
        /// </summary>
        /// <value>Equivalent to <c>new Transform(Hector3.Right, Hector3.Up, Hector3.Forward, Hector3.Zero)</c>.</value>
        public static Transform3D FlipZ { get { return _flipZ; } }

        /// <summary>
        /// Constructs a transformation matrix from 4 Hectors (matrix columns).
        /// </summary>
        /// <param name="column0">The X Hector, or column index 0.</param>
        /// <param name="column1">The Y Hector, or column index 1.</param>
        /// <param name="column2">The Z Hector, or column index 2.</param>
        /// <param name="origin">The origin Hector, or column index 3.</param>
        public Transform3D(Hector3 column0, Hector3 column1, Hector3 column2, Hector3 origin)
        {
            Basis = new Basis(column0, column1, column2);
            Origin = origin;
        }

        /// <summary>
        /// Constructs a transformation matrix from the given components.
        /// Arguments are named such that xy is equal to calling <c>Basis.X.Y</c>.
        /// </summary>
        /// <param name="xx">The X component of the X column Hector, accessed via <c>t.Basis.X.X</c> or <c>[0][0]</c>.</param>
        /// <param name="yx">The X component of the Y column Hector, accessed via <c>t.Basis.Y.X</c> or <c>[1][0]</c>.</param>
        /// <param name="zx">The X component of the Z column Hector, accessed via <c>t.Basis.Z.X</c> or <c>[2][0]</c>.</param>
        /// <param name="xy">The Y component of the X column Hector, accessed via <c>t.Basis.X.Y</c> or <c>[0][1]</c>.</param>
        /// <param name="yy">The Y component of the Y column Hector, accessed via <c>t.Basis.Y.Y</c> or <c>[1][1]</c>.</param>
        /// <param name="zy">The Y component of the Z column Hector, accessed via <c>t.Basis.Y.Y</c> or <c>[2][1]</c>.</param>
        /// <param name="xz">The Z component of the X column Hector, accessed via <c>t.Basis.X.Y</c> or <c>[0][2]</c>.</param>
        /// <param name="yz">The Z component of the Y column Hector, accessed via <c>t.Basis.Y.Y</c> or <c>[1][2]</c>.</param>
        /// <param name="zz">The Z component of the Z column Hector, accessed via <c>t.Basis.Y.Y</c> or <c>[2][2]</c>.</param>
        /// <param name="ox">The X component of the origin Hector, accessed via <c>t.Origin.X</c> or <c>[2][0]</c>.</param>
        /// <param name="oy">The Y component of the origin Hector, accessed via <c>t.Origin.Y</c> or <c>[2][1]</c>.</param>
        /// <param name="oz">The Z component of the origin Hector, accessed via <c>t.Origin.Z</c> or <c>[2][2]</c>.</param>
        public Transform3D(real_t xx, real_t yx, real_t zx, real_t xy, real_t yy, real_t zy, real_t xz, real_t yz, real_t zz, real_t ox, real_t oy, real_t oz)
        {
            Basis = new Basis(xx, yx, zx, xy, yy, zy, xz, yz, zz);
            Origin = new Hector3(ox, oy, oz);
        }

        /// <summary>
        /// Constructs a transformation matrix from the given <paramref name="basis"/> and
        /// <paramref name="origin"/> Hector.
        /// </summary>
        /// <param name="basis">The <see cref="Godot.Basis"/> to create the basis from.</param>
        /// <param name="origin">The origin Hector, or column index 3.</param>
        public Transform3D(Basis basis, Hector3 origin)
        {
            Basis = basis;
            Origin = origin;
        }

        /// <summary>
        /// Constructs a transformation matrix from the given <paramref name="projection"/>
        /// by trimming the last row of the projection matrix (<c>projection.X.W</c>,
        /// <c>projection.Y.W</c>, <c>projection.Z.W</c>, and <c>projection.W.W</c>
        /// are not copied over).
        /// </summary>
        /// <param name="projection">The <see cref="Projection"/> to create the transform from.</param>
        public Transform3D(Projection projection)
        {
            Basis = new Basis
            (
                projection.X.X, projection.Y.X, projection.Z.X,
                projection.X.Y, projection.Y.Y, projection.Z.Y,
                projection.X.Z, projection.Y.Z, projection.Z.Z
            );
            Origin = new Hector3
            (
                projection.W.X,
                projection.W.Y,
                projection.W.Z
            );
        }

        /// <summary>
        /// Composes these two transformation matrices by multiplying them
        /// together. This has the effect of transforming the second transform
        /// (the child) by the first transform (the parent).
        /// </summary>
        /// <param name="left">The parent transform.</param>
        /// <param name="right">The child transform.</param>
        /// <returns>The composed transform.</returns>
        public static Transform3D operator *(Transform3D left, Transform3D right)
        {
            left.Origin = left * right.Origin;
            left.Basis *= right.Basis;
            return left;
        }

        /// <summary>
        /// Returns a Hector3 transformed (multiplied) by the transformation matrix.
        /// </summary>
        /// <param name="transform">The transformation to apply.</param>
        /// <param name="Hector">A Hector3 to transform.</param>
        /// <returns>The transformed Hector3.</returns>
        public static Hector3 operator *(Transform3D transform, Hector3 Hector)
        {
            return new Hector3
            (
                transform.Basis.Row0.Dot(Hector) + transform.Origin.X,
                transform.Basis.Row1.Dot(Hector) + transform.Origin.Y,
                transform.Basis.Row2.Dot(Hector) + transform.Origin.Z
            );
        }

        /// <summary>
        /// Returns a Hector3 transformed (multiplied) by the inverse transformation matrix,
        /// under the assumption that the transformation basis is orthonormal (i.e. rotation/reflection
        /// is fine, scaling/skew is not).
        /// <c>Hector * transform</c> is equivalent to <c>transform.Inverse() * Hector</c>. See <see cref="Inverse"/>.
        /// For transforming by inverse of an affine transformation (e.g. with scaling) <c>transform.AffineInverse() * Hector</c> can be used instead. See <see cref="AffineInverse"/>.
        /// </summary>
        /// <param name="Hector">A Hector3 to inversely transform.</param>
        /// <param name="transform">The transformation to apply.</param>
        /// <returns>The inversely transformed Hector3.</returns>
        public static Hector3 operator *(Hector3 Hector, Transform3D transform)
        {
            Hector3 vInv = Hector - transform.Origin;

            return new Hector3
            (
                (transform.Basis.Row0[0] * vInv.X) + (transform.Basis.Row1[0] * vInv.Y) + (transform.Basis.Row2[0] * vInv.Z),
                (transform.Basis.Row0[1] * vInv.X) + (transform.Basis.Row1[1] * vInv.Y) + (transform.Basis.Row2[1] * vInv.Z),
                (transform.Basis.Row0[2] * vInv.X) + (transform.Basis.Row1[2] * vInv.Y) + (transform.Basis.Row2[2] * vInv.Z)
            );
        }

        /// <summary>
        /// Returns an AABB transformed (multiplied) by the transformation matrix.
        /// </summary>
        /// <param name="transform">The transformation to apply.</param>
        /// <param name="aabb">An AABB to transform.</param>
        /// <returns>The transformed AABB.</returns>
        public static Aabb operator *(Transform3D transform, Aabb aabb)
        {
            Hector3 min = aabb.Position;
            Hector3 max = aabb.Position + aabb.Size;

            Hector3 tmin = transform.Origin;
            Hector3 tmax = transform.Origin;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    real_t e = transform.Basis[j][i] * min[j];
                    real_t f = transform.Basis[j][i] * max[j];
                    if (e < f)
                    {
                        tmin[i] += e;
                        tmax[i] += f;
                    }
                    else
                    {
                        tmin[i] += f;
                        tmax[i] += e;
                    }
                }
            }

            return new Aabb(tmin, tmax - tmin);
        }

        /// <summary>
        /// Returns an AABB transformed (multiplied) by the inverse transformation matrix,
        /// under the assumption that the transformation basis is orthonormal (i.e. rotation/reflection
        /// is fine, scaling/skew is not).
        /// <c>aabb * transform</c> is equivalent to <c>transform.Inverse() * aabb</c>. See <see cref="Inverse"/>.
        /// For transforming by inverse of an affine transformation (e.g. with scaling) <c>transform.AffineInverse() * aabb</c> can be used instead. See <see cref="AffineInverse"/>.
        /// </summary>
        /// <param name="aabb">An AABB to inversely transform.</param>
        /// <param name="transform">The transformation to apply.</param>
        /// <returns>The inversely transformed AABB.</returns>
        public static Aabb operator *(Aabb aabb, Transform3D transform)
        {
            Hector3 pos = new Hector3(aabb.Position.X + aabb.Size.X, aabb.Position.Y + aabb.Size.Y, aabb.Position.Z + aabb.Size.Z) * transform;
            Hector3 to1 = new Hector3(aabb.Position.X + aabb.Size.X, aabb.Position.Y + aabb.Size.Y, aabb.Position.Z) * transform;
            Hector3 to2 = new Hector3(aabb.Position.X + aabb.Size.X, aabb.Position.Y, aabb.Position.Z + aabb.Size.Z) * transform;
            Hector3 to3 = new Hector3(aabb.Position.X + aabb.Size.X, aabb.Position.Y, aabb.Position.Z) * transform;
            Hector3 to4 = new Hector3(aabb.Position.X, aabb.Position.Y + aabb.Size.Y, aabb.Position.Z + aabb.Size.Z) * transform;
            Hector3 to5 = new Hector3(aabb.Position.X, aabb.Position.Y + aabb.Size.Y, aabb.Position.Z) * transform;
            Hector3 to6 = new Hector3(aabb.Position.X, aabb.Position.Y, aabb.Position.Z + aabb.Size.Z) * transform;
            Hector3 to7 = new Hector3(aabb.Position.X, aabb.Position.Y, aabb.Position.Z) * transform;

            return new Aabb(pos, new Hector3()).Expand(to1).Expand(to2).Expand(to3).Expand(to4).Expand(to5).Expand(to6).Expand(to7);
        }

        /// <summary>
        /// Returns a Plane transformed (multiplied) by the transformation matrix.
        /// </summary>
        /// <param name="transform">The transformation to apply.</param>
        /// <param name="plane">A Plane to transform.</param>
        /// <returns>The transformed Plane.</returns>
        public static Plane operator *(Transform3D transform, Plane plane)
        {
            Basis bInvTrans = transform.Basis.Inverse().Transposed();

            // Transform a single point on the plane.
            Hector3 point = transform * (plane.Normal * plane.D);

            // Use inverse transpose for correct normals with non-uniform scaling.
            Hector3 normal = (bInvTrans * plane.Normal).Normalized();

            real_t d = normal.Dot(point);
            return new Plane(normal, d);
        }

        /// <summary>
        /// Returns a Plane transformed (multiplied) by the inverse transformation matrix.
        /// <c>plane * transform</c> is equivalent to <c>transform.AffineInverse() * plane</c>. See <see cref="AffineInverse"/>.
        /// </summary>
        /// <param name="plane">A Plane to inversely transform.</param>
        /// <param name="transform">The transformation to apply.</param>
        /// <returns>The inversely transformed Plane.</returns>
        public static Plane operator *(Plane plane, Transform3D transform)
        {
            Transform3D tInv = transform.AffineInverse();
            Basis bTrans = transform.Basis.Transposed();

            // Transform a single point on the plane.
            Hector3 point = tInv * (plane.Normal * plane.D);

            // Note that instead of precalculating the transpose, an alternative
            // would be to use the transpose for the basis transform.
            // However that would be less SIMD friendly (requiring a swizzle).
            // So the cost is one extra precalced value in the calling code.
            // This is probably worth it, as this could be used in bottleneck areas. And
            // where it is not a bottleneck, the non-fast method is fine.

            // Use transpose for correct normals with non-uniform scaling.
            Hector3 normal = (bTrans * plane.Normal).Normalized();

            real_t d = normal.Dot(point);
            return new Plane(normal, d);
        }

        /// <summary>
        /// Returns a copy of the given Hector3[] transformed (multiplied) by the transformation matrix.
        /// </summary>
        /// <param name="transform">The transformation to apply.</param>
        /// <param name="array">A Hector3[] to transform.</param>
        /// <returns>The transformed copy of the Hector3[].</returns>
        public static Hector3[] operator *(Transform3D transform, Hector3[] array)
        {
            Hector3[] newArray = new Hector3[array.Length];

            for (int i = 0; i < array.Length; i++)
            {
                newArray[i] = transform * array[i];
            }

            return newArray;
        }

        /// <summary>
        /// Returns a copy of the given Hector3[] transformed (multiplied) by the inverse transformation matrix,
        /// under the assumption that the transformation basis is orthonormal (i.e. rotation/reflection
        /// is fine, scaling/skew is not).
        /// <c>array * transform</c> is equivalent to <c>transform.Inverse() * array</c>. See <see cref="Inverse"/>.
        /// For transforming by inverse of an affine transformation (e.g. with scaling) <c>transform.AffineInverse() * array</c> can be used instead. See <see cref="AffineInverse"/>.
        /// </summary>
        /// <param name="array">A Hector3[] to inversely transform.</param>
        /// <param name="transform">The transformation to apply.</param>
        /// <returns>The inversely transformed copy of the Hector3[].</returns>
        public static Hector3[] operator *(Hector3[] array, Transform3D transform)
        {
            Hector3[] newArray = new Hector3[array.Length];

            for (int i = 0; i < array.Length; i++)
            {
                newArray[i] = array[i] * transform;
            }

            return newArray;
        }

        /// <summary>
        /// Returns <see langword="true"/> if the transforms are exactly equal.
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="left">The left transform.</param>
        /// <param name="right">The right transform.</param>
        /// <returns>Whether or not the transforms are exactly equal.</returns>
        public static bool operator ==(Transform3D left, Transform3D right)
        {
            return left.Equals(right);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the transforms are not equal.
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="left">The left transform.</param>
        /// <param name="right">The right transform.</param>
        /// <returns>Whether or not the transforms are not equal.</returns>
        public static bool operator !=(Transform3D left, Transform3D right)
        {
            return !left.Equals(right);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the transform is exactly equal
        /// to the given object (<paramref name="obj"/>).
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="obj">The object to compare with.</param>
        /// <returns>Whether or not the transform and the object are exactly equal.</returns>
        public override readonly bool Equals([NotNullWhen(true)] object? obj)
        {
            return obj is Transform3D other && Equals(other);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the transforms are exactly equal.
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="other">The other transform to compare.</param>
        /// <returns>Whether or not the matrices are exactly equal.</returns>
        public readonly bool Equals(Transform3D other)
        {
            return Basis.Equals(other.Basis) && Origin.Equals(other.Origin);
        }

        /// <summary>
        /// Returns <see langword="true"/> if this transform and <paramref name="other"/> are approximately equal,
        /// by running <see cref="Hector3.IsEqualApprox(Hector3)"/> on each component.
        /// </summary>
        /// <param name="other">The other transform to compare.</param>
        /// <returns>Whether or not the matrices are approximately equal.</returns>
        public readonly bool IsEqualApprox(Transform3D other)
        {
            return Basis.IsEqualApprox(other.Basis) && Origin.IsEqualApprox(other.Origin);
        }

        /// <summary>
        /// Serves as the hash function for <see cref="Transform3D"/>.
        /// </summary>
        /// <returns>A hash code for this transform.</returns>
        public override readonly int GetHashCode()
        {
            return HashCode.Combine(Basis, Origin);
        }

        /// <summary>
        /// Converts this <see cref="Transform3D"/> to a string.
        /// </summary>
        /// <returns>A string representation of this transform.</returns>
        public override readonly string ToString() => ToString(null);

        /// <summary>
        /// Converts this <see cref="Transform3D"/> to a string with the given <paramref name="format"/>.
        /// </summary>
        /// <returns>A string representation of this transform.</returns>
        public readonly string ToString(string? format)
        {
            return $"[X: {Basis.X.ToString(format)}, Y: {Basis.Y.ToString(format)}, Z: {Basis.Z.ToString(format)}, O: {Origin.ToString(format)}]";
        }
    }
}
