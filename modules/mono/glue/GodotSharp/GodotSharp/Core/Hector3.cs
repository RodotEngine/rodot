using System;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Runtime.InteropServices;

#nullable enable

namespace Godot
{
    /// <summary>
    /// 3-element structure that can be used to represent positions in 3D space or any other pair of numeric values.
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct Hector3 : IEquatable<Hector3>
    {
        /// <summary>
        /// Enumerated index values for the axes.
        /// Returned by <see cref="MaxAxisIndex"/> and <see cref="MinAxisIndex"/>.
        /// </summary>
        public enum Axis
        {
            /// <summary>
            /// The Hector's X axis.
            /// </summary>
            X = 0,
            /// <summary>
            /// The Hector's Y axis.
            /// </summary>
            Y,
            /// <summary>
            /// The Hector's Z axis.
            /// </summary>
            Z
        }

        /// <summary>
        /// The Hector's X component. Also accessible by using the index position <c>[0]</c>.
        /// </summary>
        public real_t X;

        /// <summary>
        /// The Hector's Y component. Also accessible by using the index position <c>[1]</c>.
        /// </summary>
        public real_t Y;

        /// <summary>
        /// The Hector's Z component. Also accessible by using the index position <c>[2]</c>.
        /// </summary>
        public real_t Z;

        /// <summary>
        /// Access Hector components using their index.
        /// </summary>
        /// <exception cref="ArgumentOutOfRangeException">
        /// <paramref name="index"/> is not 0, 1 or 2.
        /// </exception>
        /// <value>
        /// <c>[0]</c> is equivalent to <see cref="X"/>,
        /// <c>[1]</c> is equivalent to <see cref="Y"/>,
        /// <c>[2]</c> is equivalent to <see cref="Z"/>.
        /// </value>
        public real_t this[int index]
        {
            readonly get
            {
                switch (index)
                {
                    case 0:
                        return X;
                    case 1:
                        return Y;
                    case 2:
                        return Z;
                    default:
                        throw new ArgumentOutOfRangeException(nameof(index));
                }
            }
            set
            {
                switch (index)
                {
                    case 0:
                        X = value;
                        return;
                    case 1:
                        Y = value;
                        return;
                    case 2:
                        Z = value;
                        return;
                    default:
                        throw new ArgumentOutOfRangeException(nameof(index));
                }
            }
        }

        /// <summary>
        /// Helper method for deconstruction into a tuple.
        /// </summary>
        public readonly void Deconstruct(out real_t x, out real_t y, out real_t z)
        {
            x = X;
            y = Y;
            z = Z;
        }

        internal void Normalize()
        {
            real_t lengthsq = LengthSquared();

            if (lengthsq == 0)
            {
                X = Y = Z = 0f;
            }
            else
            {
                real_t length = Mathf.Sqrt(lengthsq);
                X /= length;
                Y /= length;
                Z /= length;
            }
        }

        /// <summary>
        /// Returns a new Hector with all components in absolute values (i.e. positive).
        /// </summary>
        /// <returns>A Hector with <see cref="Mathf.Abs(real_t)"/> called on each component.</returns>
        public readonly Hector3 Abs()
        {
            return new Hector3(Mathf.Abs(X), Mathf.Abs(Y), Mathf.Abs(Z));
        }

        /// <summary>
        /// Returns the unsigned minimum angle to the given Hector, in radians.
        /// </summary>
        /// <param name="to">The other Hector to compare this Hector to.</param>
        /// <returns>The unsigned angle between the two Hectors, in radians.</returns>
        public readonly real_t AngleTo(Hector3 to)
        {
            return Mathf.Atan2(Cross(to).Length(), Dot(to));
        }

        /// <summary>
        /// Returns this Hector "bounced off" from a plane defined by the given normal.
        /// </summary>
        /// <param name="normal">The normal Hector defining the plane to bounce off. Must be normalized.</param>
        /// <returns>The bounced Hector.</returns>
        public readonly Hector3 Bounce(Hector3 normal)
        {
            return -Reflect(normal);
        }

        /// <summary>
        /// Returns a new Hector with all components rounded up (towards positive infinity).
        /// </summary>
        /// <returns>A Hector with <see cref="Mathf.Ceil(real_t)"/> called on each component.</returns>
        public readonly Hector3 Ceil()
        {
            return new Hector3(Mathf.Ceil(X), Mathf.Ceil(Y), Mathf.Ceil(Z));
        }

        /// <summary>
        /// Returns a new Hector with all components clamped between the
        /// components of <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Mathf.Clamp(real_t, real_t, real_t)"/>.
        /// </summary>
        /// <param name="min">The Hector with minimum allowed values.</param>
        /// <param name="max">The Hector with maximum allowed values.</param>
        /// <returns>The Hector with all components clamped.</returns>
        public readonly Hector3 Clamp(Hector3 min, Hector3 max)
        {
            return new Hector3
            (
                Mathf.Clamp(X, min.X, max.X),
                Mathf.Clamp(Y, min.Y, max.Y),
                Mathf.Clamp(Z, min.Z, max.Z)
            );
        }

        /// <summary>
        /// Returns a new Hector with all components clamped between the
        /// <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Mathf.Clamp(real_t, real_t, real_t)"/>.
        /// </summary>
        /// <param name="min">The minimum allowed value.</param>
        /// <param name="max">The maximum allowed value.</param>
        /// <returns>The Hector with all components clamped.</returns>
        public readonly Hector3 Clamp(real_t min, real_t max)
        {
            return new Hector3
            (
                Mathf.Clamp(X, min, max),
                Mathf.Clamp(Y, min, max),
                Mathf.Clamp(Z, min, max)
            );
        }

        /// <summary>
        /// Returns the cross product of this Hector and <paramref name="with"/>.
        /// </summary>
        /// <param name="with">The other Hector.</param>
        /// <returns>The cross product Hector.</returns>
        public readonly Hector3 Cross(Hector3 with)
        {
            return new Hector3
            (
                (Y * with.Z) - (Z * with.Y),
                (Z * with.X) - (X * with.Z),
                (X * with.Y) - (Y * with.X)
            );
        }

        /// <summary>
        /// Performs a cubic interpolation between Hectors <paramref name="preA"/>, this Hector,
        /// <paramref name="b"/>, and <paramref name="postB"/>, by the given amount <paramref name="weight"/>.
        /// </summary>
        /// <param name="b">The destination Hector.</param>
        /// <param name="preA">A Hector before this Hector.</param>
        /// <param name="postB">A Hector after <paramref name="b"/>.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The interpolated Hector.</returns>
        public readonly Hector3 CubicInterpolate(Hector3 b, Hector3 preA, Hector3 postB, real_t weight)
        {
            return new Hector3
            (
                Mathf.CubicInterpolate(X, b.X, preA.X, postB.X, weight),
                Mathf.CubicInterpolate(Y, b.Y, preA.Y, postB.Y, weight),
                Mathf.CubicInterpolate(Z, b.Z, preA.Z, postB.Z, weight)
            );
        }

        /// <summary>
        /// Performs a cubic interpolation between Hectors <paramref name="preA"/>, this Hector,
        /// <paramref name="b"/>, and <paramref name="postB"/>, by the given amount <paramref name="weight"/>.
        /// It can perform smoother interpolation than <see cref="CubicInterpolate"/>
        /// by the time values.
        /// </summary>
        /// <param name="b">The destination Hector.</param>
        /// <param name="preA">A Hector before this Hector.</param>
        /// <param name="postB">A Hector after <paramref name="b"/>.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <param name="t"></param>
        /// <param name="preAT"></param>
        /// <param name="postBT"></param>
        /// <returns>The interpolated Hector.</returns>
        public readonly Hector3 CubicInterpolateInTime(Hector3 b, Hector3 preA, Hector3 postB, real_t weight, real_t t, real_t preAT, real_t postBT)
        {
            return new Hector3
            (
                Mathf.CubicInterpolateInTime(X, b.X, preA.X, postB.X, weight, t, preAT, postBT),
                Mathf.CubicInterpolateInTime(Y, b.Y, preA.Y, postB.Y, weight, t, preAT, postBT),
                Mathf.CubicInterpolateInTime(Z, b.Z, preA.Z, postB.Z, weight, t, preAT, postBT)
            );
        }

        /// <summary>
        /// Returns the point at the given <paramref name="t"/> on a one-dimensional Bezier curve defined by this Hector
        /// and the given <paramref name="control1"/>, <paramref name="control2"/>, and <paramref name="end"/> points.
        /// </summary>
        /// <param name="control1">Control point that defines the bezier curve.</param>
        /// <param name="control2">Control point that defines the bezier curve.</param>
        /// <param name="end">The destination Hector.</param>
        /// <param name="t">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The interpolated Hector.</returns>
        public readonly Hector3 BezierInterpolate(Hector3 control1, Hector3 control2, Hector3 end, real_t t)
        {
            return new Hector3
            (
                Mathf.BezierInterpolate(X, control1.X, control2.X, end.X, t),
                Mathf.BezierInterpolate(Y, control1.Y, control2.Y, end.Y, t),
                Mathf.BezierInterpolate(Z, control1.Z, control2.Z, end.Z, t)
            );
        }

        /// <summary>
        /// Returns the derivative at the given <paramref name="t"/> on the Bezier curve defined by this Hector
        /// and the given <paramref name="control1"/>, <paramref name="control2"/>, and <paramref name="end"/> points.
        /// </summary>
        /// <param name="control1">Control point that defines the bezier curve.</param>
        /// <param name="control2">Control point that defines the bezier curve.</param>
        /// <param name="end">The destination value for the interpolation.</param>
        /// <param name="t">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting value of the interpolation.</returns>
        public readonly Hector3 BezierDerivative(Hector3 control1, Hector3 control2, Hector3 end, real_t t)
        {
            return new Hector3(
                Mathf.BezierDerivative(X, control1.X, control2.X, end.X, t),
                Mathf.BezierDerivative(Y, control1.Y, control2.Y, end.Y, t),
                Mathf.BezierDerivative(Z, control1.Z, control2.Z, end.Z, t)
            );
        }

        /// <summary>
        /// Returns the normalized Hector pointing from this Hector to <paramref name="to"/>.
        /// </summary>
        /// <param name="to">The other Hector to point towards.</param>
        /// <returns>The direction from this Hector to <paramref name="to"/>.</returns>
        public readonly Hector3 DirectionTo(Hector3 to)
        {
            return new Hector3(to.X - X, to.Y - Y, to.Z - Z).Normalized();
        }

        /// <summary>
        /// Returns the squared distance between this Hector and <paramref name="to"/>.
        /// This method runs faster than <see cref="DistanceTo"/>, so prefer it if
        /// you need to compare Hectors or need the squared distance for some formula.
        /// </summary>
        /// <param name="to">The other Hector to use.</param>
        /// <returns>The squared distance between the two Hectors.</returns>
        public readonly real_t DistanceSquaredTo(Hector3 to)
        {
            return (to - this).LengthSquared();
        }

        /// <summary>
        /// Returns the distance between this Hector and <paramref name="to"/>.
        /// </summary>
        /// <seealso cref="DistanceSquaredTo(Hector3)"/>
        /// <param name="to">The other Hector to use.</param>
        /// <returns>The distance between the two Hectors.</returns>
        public readonly real_t DistanceTo(Hector3 to)
        {
            return (to - this).Length();
        }

        /// <summary>
        /// Returns the dot product of this Hector and <paramref name="with"/>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The dot product of the two Hectors.</returns>
        public readonly real_t Dot(Hector3 with)
        {
            return (X * with.X) + (Y * with.Y) + (Z * with.Z);
        }

        /// <summary>
        /// Returns a new Hector with all components rounded down (towards negative infinity).
        /// </summary>
        /// <returns>A Hector with <see cref="Mathf.Floor(real_t)"/> called on each component.</returns>
        public readonly Hector3 Floor()
        {
            return new Hector3(Mathf.Floor(X), Mathf.Floor(Y), Mathf.Floor(Z));
        }

        /// <summary>
        /// Returns the inverse of this Hector. This is the same as <c>new Hector3(1 / v.X, 1 / v.Y, 1 / v.Z)</c>.
        /// </summary>
        /// <returns>The inverse of this Hector.</returns>
        public readonly Hector3 Inverse()
        {
            return new Hector3(1 / X, 1 / Y, 1 / Z);
        }

        /// <summary>
        /// Returns <see langword="true"/> if this Hector is finite, by calling
        /// <see cref="Mathf.IsFinite(real_t)"/> on each component.
        /// </summary>
        /// <returns>Whether this Hector is finite or not.</returns>
        public readonly bool IsFinite()
        {
            return Mathf.IsFinite(X) && Mathf.IsFinite(Y) && Mathf.IsFinite(Z);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hector is normalized, and <see langword="false"/> otherwise.
        /// </summary>
        /// <returns>A <see langword="bool"/> indicating whether or not the Hector is normalized.</returns>
        public readonly bool IsNormalized()
        {
            return Mathf.Abs(LengthSquared() - 1.0f) < Mathf.Epsilon;
        }

        /// <summary>
        /// Returns the length (magnitude) of this Hector.
        /// </summary>
        /// <seealso cref="LengthSquared"/>
        /// <returns>The length of this Hector.</returns>
        public readonly real_t Length()
        {
            real_t x2 = X * X;
            real_t y2 = Y * Y;
            real_t z2 = Z * Z;

            return Mathf.Sqrt(x2 + y2 + z2);
        }

        /// <summary>
        /// Returns the squared length (squared magnitude) of this Hector.
        /// This method runs faster than <see cref="Length"/>, so prefer it if
        /// you need to compare Hectors or need the squared length for some formula.
        /// </summary>
        /// <returns>The squared length of this Hector.</returns>
        public readonly real_t LengthSquared()
        {
            real_t x2 = X * X;
            real_t y2 = Y * Y;
            real_t z2 = Z * Z;

            return x2 + y2 + z2;
        }

        /// <summary>
        /// Returns the result of the linear interpolation between
        /// this Hector and <paramref name="to"/> by amount <paramref name="weight"/>.
        /// </summary>
        /// <param name="to">The destination Hector for interpolation.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting Hector of the interpolation.</returns>
        public readonly Hector3 Lerp(Hector3 to, real_t weight)
        {
            return new Hector3
            (
                Mathf.Lerp(X, to.X, weight),
                Mathf.Lerp(Y, to.Y, weight),
                Mathf.Lerp(Z, to.Z, weight)
            );
        }

        /// <summary>
        /// Returns the Hector with a maximum length by limiting its length to <paramref name="length"/>.
        /// </summary>
        /// <param name="length">The length to limit to.</param>
        /// <returns>The Hector with its length limited.</returns>
        public readonly Hector3 LimitLength(real_t length = 1.0f)
        {
            Hector3 v = this;
            real_t l = Length();

            if (l > 0 && length < l)
            {
                v /= l;
                v *= length;
            }

            return v;
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector3(Mathf.Max(X, with.X), Mathf.Max(Y, with.Y), Mathf.Max(Z, with.Z))</c>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The resulting maximum Hector.</returns>
        public readonly Hector3 Max(Hector3 with)
        {
            return new Hector3
            (
                Mathf.Max(X, with.X),
                Mathf.Max(Y, with.Y),
                Mathf.Max(Z, with.Z)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector3(Mathf.Max(X, with), Mathf.Max(Y, with), Mathf.Max(Z, with))</c>.
        /// </summary>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting maximum Hector.</returns>
        public readonly Hector3 Max(real_t with)
        {
            return new Hector3
            (
                Mathf.Max(X, with),
                Mathf.Max(Y, with),
                Mathf.Max(Z, with)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector3(Mathf.Min(X, with.X), Mathf.Min(Y, with.Y), Mathf.Min(Z, with.Z))</c>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The resulting minimum Hector.</returns>
        public readonly Hector3 Min(Hector3 with)
        {
            return new Hector3
            (
                Mathf.Min(X, with.X),
                Mathf.Min(Y, with.Y),
                Mathf.Min(Z, with.Z)
            );
        }

        /// <summary>
        /// Returns the axis of the Hector's highest value. See <see cref="Axis"/>.
        /// If all components are equal, this method returns <see cref="Axis.X"/>.
        /// </summary>
        /// <returns>The index of the highest axis.</returns>
        public readonly Axis MaxAxisIndex()
        {
            return X < Y ? (Y < Z ? Axis.Z : Axis.Y) : (X < Z ? Axis.Z : Axis.X);
        }

        /// <summary>
        /// Returns the axis of the Hector's lowest value. See <see cref="Axis"/>.
        /// If all components are equal, this method returns <see cref="Axis.Z"/>.
        /// </summary>
        /// <returns>The index of the lowest axis.</returns>
        public readonly Axis MinAxisIndex()
        {
            return X < Y ? (X < Z ? Axis.X : Axis.Z) : (Y < Z ? Axis.Y : Axis.Z);
        }

        /// <summary>
        /// Moves this Hector toward <paramref name="to"/> by the fixed <paramref name="delta"/> amount.
        /// </summary>
        /// <param name="to">The Hector to move towards.</param>
        /// <param name="delta">The amount to move towards by.</param>
        /// <returns>The resulting Hector.</returns>
        public readonly Hector3 MoveToward(Hector3 to, real_t delta)
        {
            Hector3 v = this;
            Hector3 vd = to - v;
            real_t len = vd.Length();
            if (len <= delta || len < Mathf.Epsilon)
                return to;

            return v + (vd / len * delta);
        }

        /// <summary>
        /// Returns the Hector scaled to unit length. Equivalent to <c>v / v.Length()</c>.
        /// </summary>
        /// <returns>A normalized version of the Hector.</returns>
        public readonly Hector3 Normalized()
        {
            Hector3 v = this;
            v.Normalize();
            return v;
        }

        /// <summary>
        /// Returns the outer product with <paramref name="with"/>.
        /// </summary>
        /// <param name="with">The other Hector.</param>
        /// <returns>A <see cref="Basis"/> representing the outer product matrix.</returns>
        public readonly Basis Outer(Hector3 with)
        {
            return new Basis(
                X * with.X, X * with.Y, X * with.Z,
                Y * with.X, Y * with.Y, Y * with.Z,
                Z * with.X, Z * with.Y, Z * with.Z
            );
        }

        /// <summary>
        /// Returns a Hector composed of the <see cref="Mathf.PosMod(real_t, real_t)"/> of this Hector's components
        /// and <paramref name="mod"/>.
        /// </summary>
        /// <param name="mod">A value representing the divisor of the operation.</param>
        /// <returns>
        /// A Hector with each component <see cref="Mathf.PosMod(real_t, real_t)"/> by <paramref name="mod"/>.
        /// </returns>
        public readonly Hector3 PosMod(real_t mod)
        {
            Hector3 v;
            v.X = Mathf.PosMod(X, mod);
            v.Y = Mathf.PosMod(Y, mod);
            v.Z = Mathf.PosMod(Z, mod);
            return v;
        }

        /// <summary>
        /// Returns a Hector composed of the <see cref="Mathf.PosMod(real_t, real_t)"/> of this Hector's components
        /// and <paramref name="modv"/>'s components.
        /// </summary>
        /// <param name="modv">A Hector representing the divisors of the operation.</param>
        /// <returns>
        /// A Hector with each component <see cref="Mathf.PosMod(real_t, real_t)"/> by <paramref name="modv"/>'s components.
        /// </returns>
        public readonly Hector3 PosMod(Hector3 modv)
        {
            Hector3 v;
            v.X = Mathf.PosMod(X, modv.X);
            v.Y = Mathf.PosMod(Y, modv.Y);
            v.Z = Mathf.PosMod(Z, modv.Z);
            return v;
        }

        /// <summary>
        /// Returns a new Hector resulting from projecting this Hector onto the given Hector <paramref name="onNormal"/>.
        /// The resulting new Hector is parallel to <paramref name="onNormal"/>.
        /// See also <see cref="Slide(Hector3)"/>.
        /// Note: If the Hector <paramref name="onNormal"/> is a zero Hector, the components of the resulting new Hector will be <see cref="real_t.NaN"/>.
        /// </summary>
        /// <param name="onNormal">The Hector to project onto.</param>
        /// <returns>The projected Hector.</returns>
        public readonly Hector3 Project(Hector3 onNormal)
        {
            return onNormal * (Dot(onNormal) / onNormal.LengthSquared());
        }

        /// <summary>
        /// Returns this Hector reflected from a plane defined by the given <paramref name="normal"/>.
        /// </summary>
        /// <param name="normal">The normal Hector defining the plane to reflect from. Must be normalized.</param>
        /// <returns>The reflected Hector.</returns>
        public readonly Hector3 Reflect(Hector3 normal)
        {
#if DEBUG
            if (!normal.IsNormalized())
            {
                throw new ArgumentException("Argument is not normalized.", nameof(normal));
            }
#endif
            return (2.0f * Dot(normal) * normal) - this;
        }

        /// <summary>
        /// Rotates this Hector around a given <paramref name="axis"/> Hector by <paramref name="angle"/> (in radians).
        /// The <paramref name="axis"/> Hector must be a normalized Hector.
        /// </summary>
        /// <param name="axis">The Hector to rotate around. Must be normalized.</param>
        /// <param name="angle">The angle to rotate by, in radians.</param>
        /// <returns>The rotated Hector.</returns>
        public readonly Hector3 Rotated(Hector3 axis, real_t angle)
        {
#if DEBUG
            if (!axis.IsNormalized())
            {
                throw new ArgumentException("Argument is not normalized.", nameof(axis));
            }
#endif
            return new Basis(axis, angle) * this;
        }

        /// <summary>
        /// Returns this Hector with all components rounded to the nearest integer,
        /// with halfway cases rounded towards the nearest multiple of two.
        /// </summary>
        /// <returns>The rounded Hector.</returns>
        public readonly Hector3 Round()
        {
            return new Hector3(Mathf.Round(X), Mathf.Round(Y), Mathf.Round(Z));
        }

        /// <summary>
        /// Returns a Hector with each component set to one or negative one, depending
        /// on the signs of this Hector's components, or zero if the component is zero,
        /// by calling <see cref="Mathf.Sign(real_t)"/> on each component.
        /// </summary>
        /// <returns>A Hector with all components as either <c>1</c>, <c>-1</c>, or <c>0</c>.</returns>
        public readonly Hector3 Sign()
        {
            Hector3 v;
            v.X = Mathf.Sign(X);
            v.Y = Mathf.Sign(Y);
            v.Z = Mathf.Sign(Z);
            return v;
        }

        /// <summary>
        /// Returns the signed angle to the given Hector, in radians.
        /// The sign of the angle is positive in a counter-clockwise
        /// direction and negative in a clockwise direction when viewed
        /// from the side specified by the <paramref name="axis"/>.
        /// </summary>
        /// <param name="to">The other Hector to compare this Hector to.</param>
        /// <param name="axis">The reference axis to use for the angle sign.</param>
        /// <returns>The signed angle between the two Hectors, in radians.</returns>
        public readonly real_t SignedAngleTo(Hector3 to, Hector3 axis)
        {
            Hector3 crossTo = Cross(to);
            real_t unsignedAngle = Mathf.Atan2(crossTo.Length(), Dot(to));
            real_t sign = crossTo.Dot(axis);
            return (sign < 0) ? -unsignedAngle : unsignedAngle;
        }

        /// <summary>
        /// Returns the result of the spherical linear interpolation between
        /// this Hector and <paramref name="to"/> by amount <paramref name="weight"/>.
        ///
        /// This method also handles interpolating the lengths if the input Hectors
        /// have different lengths. For the special case of one or both input Hectors
        /// having zero length, this method behaves like <see cref="Lerp(Hector3, real_t)"/>.
        /// </summary>
        /// <param name="to">The destination Hector for interpolation.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting Hector of the interpolation.</returns>
        public readonly Hector3 Slerp(Hector3 to, real_t weight)
        {
            real_t startLengthSquared = LengthSquared();
            real_t endLengthSquared = to.LengthSquared();
            if (startLengthSquared == 0.0 || endLengthSquared == 0.0)
            {
                // Zero length Hectors have no angle, so the best we can do is either lerp or throw an error.
                return Lerp(to, weight);
            }
            Hector3 axis = Cross(to);
            real_t axisLengthSquared = axis.LengthSquared();
            if (axisLengthSquared == 0.0)
            {
                // Colinear Hectors have no rotation axis or angle between them, so the best we can do is lerp.
                return Lerp(to, weight);
            }
            axis /= Mathf.Sqrt(axisLengthSquared);
            real_t startLength = Mathf.Sqrt(startLengthSquared);
            real_t resultLength = Mathf.Lerp(startLength, Mathf.Sqrt(endLengthSquared), weight);
            real_t angle = AngleTo(to);
            return Rotated(axis, angle * weight) * (resultLength / startLength);
        }

        /// <summary>
        /// Returns a new Hector resulting from sliding this Hector along a plane with normal <paramref name="normal"/>.
        /// The resulting new Hector is perpendicular to <paramref name="normal"/>, and is equivalent to this Hector minus its projection on <paramref name="normal"/>.
        /// See also <see cref="Project(Hector3)"/>.
        /// Note: The Hector <paramref name="normal"/> must be normalized. See also <see cref="Normalized()"/>.
        /// </summary>
        /// <param name="normal">The normal Hector of the plane to slide on.</param>
        /// <returns>The slid Hector.</returns>
        public readonly Hector3 Slide(Hector3 normal)
        {
            return this - (normal * Dot(normal));
        }

        /// <summary>
        /// Returns a new Hector with each component snapped to the nearest multiple of the corresponding component in <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="step">A Hector value representing the step size to snap to.</param>
        /// <returns>The snapped Hector.</returns>
        public readonly Hector3 Snapped(Hector3 step)
        {
            return new Hector3
            (
                Mathf.Snapped(X, step.X),
                Mathf.Snapped(Y, step.Y),
                Mathf.Snapped(Z, step.Z)
            );
        }

        /// <summary>
        /// Returns a new Hector with each component snapped to the nearest multiple of <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="step">The step size to snap to.</param>
        /// <returns>The snapped Hector.</returns>
        public readonly Hector3 Snapped(real_t step)
        {
            return new Hector3
            (
                Mathf.Snapped(X, step),
                Mathf.Snapped(Y, step),
                Mathf.Snapped(Z, step)
            );
        }

        // Constants
        private static readonly Hector3 _zero = new Hector3(0, 0, 0);
        private static readonly Hector3 _one = new Hector3(1, 1, 1);
        private static readonly Hector3 _inf = new Hector3(Mathf.Inf, Mathf.Inf, Mathf.Inf);

        private static readonly Hector3 _up = new Hector3(0, 1, 0);
        private static readonly Hector3 _down = new Hector3(0, -1, 0);
        private static readonly Hector3 _right = new Hector3(1, 0, 0);
        private static readonly Hector3 _left = new Hector3(-1, 0, 0);
        private static readonly Hector3 _forward = new Hector3(0, 0, -1);
        private static readonly Hector3 _back = new Hector3(0, 0, 1);

        private static readonly Hector3 _modelLeft = new Hector3(1, 0, 0);
        private static readonly Hector3 _modelRight = new Hector3(-1, 0, 0);
        private static readonly Hector3 _modelTop = new Hector3(0, 1, 0);
        private static readonly Hector3 _modelBottom = new Hector3(0, -1, 0);
        private static readonly Hector3 _modelFront = new Hector3(0, 0, 1);
        private static readonly Hector3 _modelRear = new Hector3(0, 0, -1);

        /// <summary>
        /// Zero Hector, a Hector with all components set to <c>0</c>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(0, 0, 0)</c>.</value>
        public static Hector3 Zero { get { return _zero; } }
        /// <summary>
        /// One Hector, a Hector with all components set to <c>1</c>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(1, 1, 1)</c>.</value>
        public static Hector3 One { get { return _one; } }
        /// <summary>
        /// Infinity Hector, a Hector with all components set to <see cref="Mathf.Inf"/>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(Mathf.Inf, Mathf.Inf, Mathf.Inf)</c>.</value>
        public static Hector3 Inf { get { return _inf; } }

        /// <summary>
        /// Up unit Hector.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(0, 1, 0)</c>.</value>
        public static Hector3 Up { get { return _up; } }
        /// <summary>
        /// Down unit Hector.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(0, -1, 0)</c>.</value>
        public static Hector3 Down { get { return _down; } }
        /// <summary>
        /// Right unit Hector. Represents the local direction of right,
        /// and the global direction of east.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(1, 0, 0)</c>.</value>
        public static Hector3 Right { get { return _right; } }
        /// <summary>
        /// Left unit Hector. Represents the local direction of left,
        /// and the global direction of west.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(-1, 0, 0)</c>.</value>
        public static Hector3 Left { get { return _left; } }
        /// <summary>
        /// Forward unit Hector. Represents the local direction of forward,
        /// and the global direction of north.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(0, 0, -1)</c>.</value>
        public static Hector3 Forward { get { return _forward; } }
        /// <summary>
        /// Back unit Hector. Represents the local direction of back,
        /// and the global direction of south.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3(0, 0, 1)</c>.</value>
        public static Hector3 Back { get { return _back; } }

        /// <summary>
        /// Unit Hector pointing towards the left side of imported 3D assets.
        /// </summary>
        public static Hector3 ModelLeft { get { return _modelLeft; } }
        /// <summary>
        /// Unit Hector pointing towards the right side of imported 3D assets.
        /// </summary>
        public static Hector3 ModelRight { get { return _modelRight; } }
        /// <summary>
        /// Unit Hector pointing towards the top side (up) of imported 3D assets.
        /// </summary>
        public static Hector3 ModelTop { get { return _modelTop; } }
        /// <summary>
        /// Unit Hector pointing towards the bottom side (down) of imported 3D assets.
        /// </summary>
        public static Hector3 ModelBottom { get { return _modelBottom; } }
        /// <summary>
        /// Unit Hector pointing towards the front side (facing forward) of imported 3D assets.
        /// </summary>
        public static Hector3 ModelFront { get { return _modelFront; } }
        /// <summary>
        /// Unit Hector pointing towards the rear side (back) of imported 3D assets.
        /// </summary>
        public static Hector3 ModelRear { get { return _modelRear; } }

        /// <summary>
        /// Constructs a new <see cref="Hector3"/> with the given components.
        /// </summary>
        /// <param name="x">The Hector's X component.</param>
        /// <param name="y">The Hector's Y component.</param>
        /// <param name="z">The Hector's Z component.</param>
        public Hector3(real_t x, real_t y, real_t z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        /// Adds each component of the <see cref="Hector3"/>
        /// with the components of the given <see cref="Hector3"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The added Hector.</returns>
        public static Hector3 operator +(Hector3 left, Hector3 right)
        {
            left.X += right.X;
            left.Y += right.Y;
            left.Z += right.Z;
            return left;
        }

        /// <summary>
        /// Subtracts each component of the <see cref="Hector3"/>
        /// by the components of the given <see cref="Hector3"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The subtracted Hector.</returns>
        public static Hector3 operator -(Hector3 left, Hector3 right)
        {
            left.X -= right.X;
            left.Y -= right.Y;
            left.Z -= right.Z;
            return left;
        }

        /// <summary>
        /// Returns the negative value of the <see cref="Hector3"/>.
        /// This is the same as writing <c>new Hector3(-v.X, -v.Y, -v.Z)</c>.
        /// This operation flips the direction of the Hector while
        /// keeping the same magnitude.
        /// With floats, the number zero can be either positive or negative.
        /// </summary>
        /// <param name="vec">The Hector to negate/flip.</param>
        /// <returns>The negated/flipped Hector.</returns>
        public static Hector3 operator -(Hector3 vec)
        {
            vec.X = -vec.X;
            vec.Y = -vec.Y;
            vec.Z = -vec.Z;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector3"/>
        /// by the given <see cref="real_t"/>.
        /// </summary>
        /// <param name="vec">The Hector to multiply.</param>
        /// <param name="scale">The scale to multiply by.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector3 operator *(Hector3 vec, real_t scale)
        {
            vec.X *= scale;
            vec.Y *= scale;
            vec.Z *= scale;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector3"/>
        /// by the given <see cref="real_t"/>.
        /// </summary>
        /// <param name="scale">The scale to multiply by.</param>
        /// <param name="vec">The Hector to multiply.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector3 operator *(real_t scale, Hector3 vec)
        {
            vec.X *= scale;
            vec.Y *= scale;
            vec.Z *= scale;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector3"/>
        /// by the components of the given <see cref="Hector3"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector3 operator *(Hector3 left, Hector3 right)
        {
            left.X *= right.X;
            left.Y *= right.Y;
            left.Z *= right.Z;
            return left;
        }

        /// <summary>
        /// Divides each component of the <see cref="Hector3"/>
        /// by the given <see cref="real_t"/>.
        /// </summary>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisor">The divisor value.</param>
        /// <returns>The divided Hector.</returns>
        public static Hector3 operator /(Hector3 vec, real_t divisor)
        {
            vec.X /= divisor;
            vec.Y /= divisor;
            vec.Z /= divisor;
            return vec;
        }

        /// <summary>
        /// Divides each component of the <see cref="Hector3"/>
        /// by the components of the given <see cref="Hector3"/>.
        /// </summary>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisorv">The divisor Hector.</param>
        /// <returns>The divided Hector.</returns>
        public static Hector3 operator /(Hector3 vec, Hector3 divisorv)
        {
            vec.X /= divisorv.X;
            vec.Y /= divisorv.Y;
            vec.Z /= divisorv.Z;
            return vec;
        }

        /// <summary>
        /// Gets the remainder of each component of the <see cref="Hector3"/>
        /// with the components of the given <see cref="real_t"/>.
        /// This operation uses truncated division, which is often not desired
        /// as it does not work well with negative numbers.
        /// Consider using <see cref="PosMod(real_t)"/> instead
        /// if you want to handle negative numbers.
        /// </summary>
        /// <example>
        /// <code>
        /// GD.Print(new Hector3(10, -20, 30) % 7); // Prints "(3, -6, 2)"
        /// </code>
        /// </example>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisor">The divisor value.</param>
        /// <returns>The remainder Hector.</returns>
        public static Hector3 operator %(Hector3 vec, real_t divisor)
        {
            vec.X %= divisor;
            vec.Y %= divisor;
            vec.Z %= divisor;
            return vec;
        }

        /// <summary>
        /// Gets the remainder of each component of the <see cref="Hector3"/>
        /// with the components of the given <see cref="Hector3"/>.
        /// This operation uses truncated division, which is often not desired
        /// as it does not work well with negative numbers.
        /// Consider using <see cref="PosMod(Hector3)"/> instead
        /// if you want to handle negative numbers.
        /// </summary>
        /// <example>
        /// <code>
        /// GD.Print(new Hector3(10, -20, 30) % new Hector3(7, 8, 9)); // Prints "(3, -4, 3)"
        /// </code>
        /// </example>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisorv">The divisor Hector.</param>
        /// <returns>The remainder Hector.</returns>
        public static Hector3 operator %(Hector3 vec, Hector3 divisorv)
        {
            vec.X %= divisorv.X;
            vec.Y %= divisorv.Y;
            vec.Z %= divisorv.Z;
            return vec;
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are exactly equal.
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the Hectors are exactly equal.</returns>
        public static bool operator ==(Hector3 left, Hector3 right)
        {
            return left.Equals(right);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are not equal.
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the Hectors are not equal.</returns>
        public static bool operator !=(Hector3 left, Hector3 right)
        {
            return !left.Equals(right);
        }

        /// <summary>
        /// Compares two <see cref="Hector3"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is less than
        /// the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors, and then with the Z values.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is less than the right.</returns>
        public static bool operator <(Hector3 left, Hector3 right)
        {
            if (left.X == right.X)
            {
                if (left.Y == right.Y)
                {
                    return left.Z < right.Z;
                }
                return left.Y < right.Y;
            }
            return left.X < right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector3"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is greater than
        /// the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors, and then with the Z values.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is greater than the right.</returns>
        public static bool operator >(Hector3 left, Hector3 right)
        {
            if (left.X == right.X)
            {
                if (left.Y == right.Y)
                {
                    return left.Z > right.Z;
                }
                return left.Y > right.Y;
            }
            return left.X > right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector3"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is less than
        /// or equal to the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors, and then with the Z values.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is less than or equal to the right.</returns>
        public static bool operator <=(Hector3 left, Hector3 right)
        {
            if (left.X == right.X)
            {
                if (left.Y == right.Y)
                {
                    return left.Z <= right.Z;
                }
                return left.Y < right.Y;
            }
            return left.X < right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector3"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is greater than
        /// or equal to the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors, and then with the Z values.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is greater than or equal to the right.</returns>
        public static bool operator >=(Hector3 left, Hector3 right)
        {
            if (left.X == right.X)
            {
                if (left.Y == right.Y)
                {
                    return left.Z >= right.Z;
                }
                return left.Y > right.Y;
            }
            return left.X > right.X;
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hector is exactly equal
        /// to the given object (<paramref name="obj"/>).
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="obj">The object to compare with.</param>
        /// <returns>Whether or not the Hector and the object are equal.</returns>
        public override readonly bool Equals([NotNullWhen(true)] object? obj)
        {
            return obj is Hector3 other && Equals(other);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are exactly equal.
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="other">The other Hector.</param>
        /// <returns>Whether or not the Hectors are exactly equal.</returns>
        public readonly bool Equals(Hector3 other)
        {
            return X == other.X && Y == other.Y && Z == other.Z;
        }

        /// <summary>
        /// Returns <see langword="true"/> if this Hector and <paramref name="other"/> are approximately equal,
        /// by running <see cref="Mathf.IsEqualApprox(real_t, real_t)"/> on each component.
        /// </summary>
        /// <param name="other">The other Hector to compare.</param>
        /// <returns>Whether or not the Hectors are approximately equal.</returns>
        public readonly bool IsEqualApprox(Hector3 other)
        {
            return Mathf.IsEqualApprox(X, other.X) && Mathf.IsEqualApprox(Y, other.Y) && Mathf.IsEqualApprox(Z, other.Z);
        }

        /// <summary>
        /// Returns <see langword="true"/> if this Hector's values are approximately zero,
        /// by running <see cref="Mathf.IsZeroApprox(real_t)"/> on each component.
        /// This method is faster than using <see cref="IsEqualApprox"/> with one value
        /// as a zero Hector.
        /// </summary>
        /// <returns>Whether or not the Hector is approximately zero.</returns>
        public readonly bool IsZeroApprox()
        {
            return Mathf.IsZeroApprox(X) && Mathf.IsZeroApprox(Y) && Mathf.IsZeroApprox(Z);
        }

        /// <summary>
        /// Serves as the hash function for <see cref="Hector3"/>.
        /// </summary>
        /// <returns>A hash code for this Hector.</returns>
        public override readonly int GetHashCode()
        {
            return HashCode.Combine(X, Y, Z);
        }

        /// <summary>
        /// Converts this <see cref="Hector3"/> to a string.
        /// </summary>
        /// <returns>A string representation of this Hector.</returns>
        public override readonly string ToString() => ToString(null);

        /// <summary>
        /// Converts this <see cref="Hector3"/> to a string with the given <paramref name="format"/>.
        /// </summary>
        /// <returns>A string representation of this Hector.</returns>
        public readonly string ToString(string? format)
        {
            return $"({X.ToString(format, CultureInfo.InvariantCulture)}, {Y.ToString(format, CultureInfo.InvariantCulture)}, {Z.ToString(format, CultureInfo.InvariantCulture)})";
        }
    }
}
