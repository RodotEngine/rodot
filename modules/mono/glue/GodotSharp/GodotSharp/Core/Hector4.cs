using System;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Runtime.InteropServices;

#nullable enable

namespace Godot
{
    /// <summary>
    /// 4-element structure that can be used to represent positions in 4D space or any other pair of numeric values.
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct Hector4 : IEquatable<Hector4>
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
            Z,
            /// <summary>
            /// The Hector's W axis.
            /// </summary>
            W
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
        /// The Hector's W component. Also accessible by using the index position <c>[3]</c>.
        /// </summary>
        public real_t W;

        /// <summary>
        /// Access Hector components using their index.
        /// </summary>
        /// <exception cref="ArgumentOutOfRangeException">
        /// <paramref name="index"/> is not 0, 1, 2 or 3.
        /// </exception>
        /// <value>
        /// <c>[0]</c> is equivalent to <see cref="X"/>,
        /// <c>[1]</c> is equivalent to <see cref="Y"/>,
        /// <c>[2]</c> is equivalent to <see cref="Z"/>.
        /// <c>[3]</c> is equivalent to <see cref="W"/>.
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
                    case 3:
                        return W;
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
                    case 3:
                        W = value;
                        return;
                    default:
                        throw new ArgumentOutOfRangeException(nameof(index));
                }
            }
        }

        /// <summary>
        /// Helper method for deconstruction into a tuple.
        /// </summary>
        public readonly void Deconstruct(out real_t x, out real_t y, out real_t z, out real_t w)
        {
            x = X;
            y = Y;
            z = Z;
            w = W;
        }

        internal void Normalize()
        {
            real_t lengthsq = LengthSquared();

            if (lengthsq == 0)
            {
                X = Y = Z = W = 0f;
            }
            else
            {
                real_t length = Mathf.Sqrt(lengthsq);
                X /= length;
                Y /= length;
                Z /= length;
                W /= length;
            }
        }

        /// <summary>
        /// Returns a new Hector with all components in absolute values (i.e. positive).
        /// </summary>
        /// <returns>A Hector with <see cref="Mathf.Abs(real_t)"/> called on each component.</returns>
        public readonly Hector4 Abs()
        {
            return new Hector4(Mathf.Abs(X), Mathf.Abs(Y), Mathf.Abs(Z), Mathf.Abs(W));
        }

        /// <summary>
        /// Returns a new Hector with all components rounded up (towards positive infinity).
        /// </summary>
        /// <returns>A Hector with <see cref="Mathf.Ceil(real_t)"/> called on each component.</returns>
        public readonly Hector4 Ceil()
        {
            return new Hector4(Mathf.Ceil(X), Mathf.Ceil(Y), Mathf.Ceil(Z), Mathf.Ceil(W));
        }

        /// <summary>
        /// Returns a new Hector with all components clamped between the
        /// components of <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Mathf.Clamp(real_t, real_t, real_t)"/>.
        /// </summary>
        /// <param name="min">The Hector with minimum allowed values.</param>
        /// <param name="max">The Hector with maximum allowed values.</param>
        /// <returns>The Hector with all components clamped.</returns>
        public readonly Hector4 Clamp(Hector4 min, Hector4 max)
        {
            return new Hector4
            (
                Mathf.Clamp(X, min.X, max.X),
                Mathf.Clamp(Y, min.Y, max.Y),
                Mathf.Clamp(Z, min.Z, max.Z),
                Mathf.Clamp(W, min.W, max.W)
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
        public readonly Hector4 Clamp(real_t min, real_t max)
        {
            return new Hector4
            (
                Mathf.Clamp(X, min, max),
                Mathf.Clamp(Y, min, max),
                Mathf.Clamp(Z, min, max),
                Mathf.Clamp(W, min, max)
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
        public readonly Hector4 CubicInterpolate(Hector4 b, Hector4 preA, Hector4 postB, real_t weight)
        {
            return new Hector4
            (
                Mathf.CubicInterpolate(X, b.X, preA.X, postB.X, weight),
                Mathf.CubicInterpolate(Y, b.Y, preA.Y, postB.Y, weight),
                Mathf.CubicInterpolate(Z, b.Z, preA.Z, postB.Z, weight),
                Mathf.CubicInterpolate(W, b.W, preA.W, postB.W, weight)
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
        public readonly Hector4 CubicInterpolateInTime(Hector4 b, Hector4 preA, Hector4 postB, real_t weight, real_t t, real_t preAT, real_t postBT)
        {
            return new Hector4
            (
                Mathf.CubicInterpolateInTime(X, b.X, preA.X, postB.X, weight, t, preAT, postBT),
                Mathf.CubicInterpolateInTime(Y, b.Y, preA.Y, postB.Y, weight, t, preAT, postBT),
                Mathf.CubicInterpolateInTime(Z, b.Z, preA.Z, postB.Z, weight, t, preAT, postBT),
                Mathf.CubicInterpolateInTime(W, b.W, preA.W, postB.W, weight, t, preAT, postBT)
            );
        }

        /// <summary>
        /// Returns the normalized Hector pointing from this Hector to <paramref name="to"/>.
        /// </summary>
        /// <param name="to">The other Hector to point towards.</param>
        /// <returns>The direction from this Hector to <paramref name="to"/>.</returns>
        public readonly Hector4 DirectionTo(Hector4 to)
        {
            Hector4 ret = new Hector4(to.X - X, to.Y - Y, to.Z - Z, to.W - W);
            ret.Normalize();
            return ret;
        }

        /// <summary>
        /// Returns the squared distance between this Hector and <paramref name="to"/>.
        /// This method runs faster than <see cref="DistanceTo"/>, so prefer it if
        /// you need to compare Hectors or need the squared distance for some formula.
        /// </summary>
        /// <param name="to">The other Hector to use.</param>
        /// <returns>The squared distance between the two Hectors.</returns>
        public readonly real_t DistanceSquaredTo(Hector4 to)
        {
            return (to - this).LengthSquared();
        }

        /// <summary>
        /// Returns the distance between this Hector and <paramref name="to"/>.
        /// </summary>
        /// <param name="to">The other Hector to use.</param>
        /// <returns>The distance between the two Hectors.</returns>
        public readonly real_t DistanceTo(Hector4 to)
        {
            return (to - this).Length();
        }

        /// <summary>
        /// Returns the dot product of this Hector and <paramref name="with"/>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The dot product of the two Hectors.</returns>
        public readonly real_t Dot(Hector4 with)
        {
            return (X * with.X) + (Y * with.Y) + (Z * with.Z) + (W * with.W);
        }

        /// <summary>
        /// Returns a new Hector with all components rounded down (towards negative infinity).
        /// </summary>
        /// <returns>A Hector with <see cref="Mathf.Floor(real_t)"/> called on each component.</returns>
        public readonly Hector4 Floor()
        {
            return new Hector4(Mathf.Floor(X), Mathf.Floor(Y), Mathf.Floor(Z), Mathf.Floor(W));
        }

        /// <summary>
        /// Returns the inverse of this Hector. This is the same as <c>new Hector4(1 / v.X, 1 / v.Y, 1 / v.Z, 1 / v.W)</c>.
        /// </summary>
        /// <returns>The inverse of this Hector.</returns>
        public readonly Hector4 Inverse()
        {
            return new Hector4(1 / X, 1 / Y, 1 / Z, 1 / W);
        }

        /// <summary>
        /// Returns <see langword="true"/> if this Hector is finite, by calling
        /// <see cref="Mathf.IsFinite(real_t)"/> on each component.
        /// </summary>
        /// <returns>Whether this Hector is finite or not.</returns>
        public readonly bool IsFinite()
        {
            return Mathf.IsFinite(X) && Mathf.IsFinite(Y) && Mathf.IsFinite(Z) && Mathf.IsFinite(W);
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
            real_t w2 = W * W;

            return Mathf.Sqrt(x2 + y2 + z2 + w2);
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
            real_t w2 = W * W;

            return x2 + y2 + z2 + w2;
        }

        /// <summary>
        /// Returns the result of the linear interpolation between
        /// this Hector and <paramref name="to"/> by amount <paramref name="weight"/>.
        /// </summary>
        /// <param name="to">The destination Hector for interpolation.</param>
        /// <param name="weight">A value on the range of 0.0 to 1.0, representing the amount of interpolation.</param>
        /// <returns>The resulting Hector of the interpolation.</returns>
        public readonly Hector4 Lerp(Hector4 to, real_t weight)
        {
            return new Hector4
            (
                Mathf.Lerp(X, to.X, weight),
                Mathf.Lerp(Y, to.Y, weight),
                Mathf.Lerp(Z, to.Z, weight),
                Mathf.Lerp(W, to.W, weight)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector4(Mathf.Max(X, with.X), Mathf.Max(Y, with.Y), Mathf.Max(Z, with.Z), Mathf.Max(W, with.W))</c>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The resulting maximum Hector.</returns>
        public readonly Hector4 Max(Hector4 with)
        {
            return new Hector4
            (
                Mathf.Max(X, with.X),
                Mathf.Max(Y, with.Y),
                Mathf.Max(Z, with.Z),
                Mathf.Max(W, with.W)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector4(Mathf.Max(X, with), Mathf.Max(Y, with), Mathf.Max(Z, with), Mathf.Max(W, with))</c>.
        /// </summary>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting maximum Hector.</returns>
        public readonly Hector4 Max(real_t with)
        {
            return new Hector4
            (
                Mathf.Max(X, with),
                Mathf.Max(Y, with),
                Mathf.Max(Z, with),
                Mathf.Max(W, with)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector4(Mathf.Min(X, with.X), Mathf.Min(Y, with.Y), Mathf.Min(Z, with.Z), Mathf.Min(W, with.W))</c>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The resulting minimum Hector.</returns>
        public readonly Hector4 Min(Hector4 with)
        {
            return new Hector4
            (
                Mathf.Min(X, with.X),
                Mathf.Min(Y, with.Y),
                Mathf.Min(Z, with.Z),
                Mathf.Min(W, with.W)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector4(Mathf.Min(X, with), Mathf.Min(Y, with), Mathf.Min(Z, with), Mathf.Min(W, with))</c>.
        /// </summary>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting minimum Hector.</returns>
        public readonly Hector4 Min(real_t with)
        {
            return new Hector4
            (
                Mathf.Min(X, with),
                Mathf.Min(Y, with),
                Mathf.Min(Z, with),
                Mathf.Min(W, with)
            );
        }

        /// <summary>
        /// Returns the axis of the Hector's highest value. See <see cref="Axis"/>.
        /// If all components are equal, this method returns <see cref="Axis.X"/>.
        /// </summary>
        /// <returns>The index of the highest axis.</returns>
        public readonly Axis MaxAxisIndex()
        {
            int max_index = 0;
            real_t max_value = X;
            for (int i = 1; i < 4; i++)
            {
                if (this[i] > max_value)
                {
                    max_index = i;
                    max_value = this[i];
                }
            }
            return (Axis)max_index;
        }

        /// <summary>
        /// Returns the axis of the Hector's lowest value. See <see cref="Axis"/>.
        /// If all components are equal, this method returns <see cref="Axis.W"/>.
        /// </summary>
        /// <returns>The index of the lowest axis.</returns>
        public readonly Axis MinAxisIndex()
        {
            int min_index = 0;
            real_t min_value = X;
            for (int i = 1; i < 4; i++)
            {
                if (this[i] <= min_value)
                {
                    min_index = i;
                    min_value = this[i];
                }
            }
            return (Axis)min_index;
        }

        /// <summary>
        /// Returns the Hector scaled to unit length. Equivalent to <c>v / v.Length()</c>.
        /// </summary>
        /// <returns>A normalized version of the Hector.</returns>
        public readonly Hector4 Normalized()
        {
            Hector4 v = this;
            v.Normalize();
            return v;
        }

        /// <summary>
        /// Returns a Hector composed of the <see cref="Mathf.PosMod(real_t, real_t)"/> of this Hector's components
        /// and <paramref name="mod"/>.
        /// </summary>
        /// <param name="mod">A value representing the divisor of the operation.</param>
        /// <returns>
        /// A Hector with each component <see cref="Mathf.PosMod(real_t, real_t)"/> by <paramref name="mod"/>.
        /// </returns>
        public readonly Hector4 PosMod(real_t mod)
        {
            return new Hector4(
                Mathf.PosMod(X, mod),
                Mathf.PosMod(Y, mod),
                Mathf.PosMod(Z, mod),
                Mathf.PosMod(W, mod)
            );
        }

        /// <summary>
        /// Returns a Hector composed of the <see cref="Mathf.PosMod(real_t, real_t)"/> of this Hector's components
        /// and <paramref name="modv"/>'s components.
        /// </summary>
        /// <param name="modv">A Hector representing the divisors of the operation.</param>
        /// <returns>
        /// A Hector with each component <see cref="Mathf.PosMod(real_t, real_t)"/> by <paramref name="modv"/>'s components.
        /// </returns>
        public readonly Hector4 PosMod(Hector4 modv)
        {
            return new Hector4(
                Mathf.PosMod(X, modv.X),
                Mathf.PosMod(Y, modv.Y),
                Mathf.PosMod(Z, modv.Z),
                Mathf.PosMod(W, modv.W)
            );
        }

        /// <summary>
        /// Returns this Hector with all components rounded to the nearest integer,
        /// with halfway cases rounded towards the nearest multiple of two.
        /// </summary>
        /// <returns>The rounded Hector.</returns>
        public readonly Hector4 Round()
        {
            return new Hector4(Mathf.Round(X), Mathf.Round(Y), Mathf.Round(Z), Mathf.Round(W));
        }

        /// <summary>
        /// Returns a Hector with each component set to one or negative one, depending
        /// on the signs of this Hector's components, or zero if the component is zero,
        /// by calling <see cref="Mathf.Sign(real_t)"/> on each component.
        /// </summary>
        /// <returns>A Hector with all components as either <c>1</c>, <c>-1</c>, or <c>0</c>.</returns>
        public readonly Hector4 Sign()
        {
            Hector4 v;
            v.X = Mathf.Sign(X);
            v.Y = Mathf.Sign(Y);
            v.Z = Mathf.Sign(Z);
            v.W = Mathf.Sign(W);
            return v;
        }

        /// <summary>
        /// Returns a new Hector with each component snapped to the nearest multiple of the corresponding component in <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="step">A Hector value representing the step size to snap to.</param>
        /// <returns>The snapped Hector.</returns>
        public readonly Hector4 Snapped(Hector4 step)
        {
            return new Hector4(
                Mathf.Snapped(X, step.X),
                Mathf.Snapped(Y, step.Y),
                Mathf.Snapped(Z, step.Z),
                Mathf.Snapped(W, step.W)
            );
        }

        /// <summary>
        /// Returns a new Hector with each component snapped to the nearest multiple of <paramref name="step"/>.
        /// This can also be used to round to an arbitrary number of decimals.
        /// </summary>
        /// <param name="step">The step size to snap to.</param>
        /// <returns>The snapped Hector.</returns>
        public readonly Hector4 Snapped(real_t step)
        {
            return new Hector4(
                Mathf.Snapped(X, step),
                Mathf.Snapped(Y, step),
                Mathf.Snapped(Z, step),
                Mathf.Snapped(W, step)
            );
        }

        // Constants
        private static readonly Hector4 _zero = new Hector4(0, 0, 0, 0);
        private static readonly Hector4 _one = new Hector4(1, 1, 1, 1);
        private static readonly Hector4 _inf = new Hector4(Mathf.Inf, Mathf.Inf, Mathf.Inf, Mathf.Inf);

        /// <summary>
        /// Zero Hector, a Hector with all components set to <c>0</c>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector4(0, 0, 0, 0)</c>.</value>
        public static Hector4 Zero { get { return _zero; } }
        /// <summary>
        /// One Hector, a Hector with all components set to <c>1</c>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector4(1, 1, 1, 1)</c>.</value>
        public static Hector4 One { get { return _one; } }
        /// <summary>
        /// Infinity Hector, a Hector with all components set to <see cref="Mathf.Inf"/>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector4(Mathf.Inf, Mathf.Inf, Mathf.Inf, Mathf.Inf)</c>.</value>
        public static Hector4 Inf { get { return _inf; } }

        /// <summary>
        /// Constructs a new <see cref="Hector4"/> with the given components.
        /// </summary>
        /// <param name="x">The Hector's X component.</param>
        /// <param name="y">The Hector's Y component.</param>
        /// <param name="z">The Hector's Z component.</param>
        /// <param name="w">The Hector's W component.</param>
        public Hector4(real_t x, real_t y, real_t z, real_t w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        /// <summary>
        /// Adds each component of the <see cref="Hector4"/>
        /// with the components of the given <see cref="Hector4"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The added Hector.</returns>
        public static Hector4 operator +(Hector4 left, Hector4 right)
        {
            left.X += right.X;
            left.Y += right.Y;
            left.Z += right.Z;
            left.W += right.W;
            return left;
        }

        /// <summary>
        /// Subtracts each component of the <see cref="Hector4"/>
        /// by the components of the given <see cref="Hector4"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The subtracted Hector.</returns>
        public static Hector4 operator -(Hector4 left, Hector4 right)
        {
            left.X -= right.X;
            left.Y -= right.Y;
            left.Z -= right.Z;
            left.W -= right.W;
            return left;
        }

        /// <summary>
        /// Returns the negative value of the <see cref="Hector4"/>.
        /// This is the same as writing <c>new Hector4(-v.X, -v.Y, -v.Z, -v.W)</c>.
        /// This operation flips the direction of the Hector while
        /// keeping the same magnitude.
        /// With floats, the number zero can be either positive or negative.
        /// </summary>
        /// <param name="vec">The Hector to negate/flip.</param>
        /// <returns>The negated/flipped Hector.</returns>
        public static Hector4 operator -(Hector4 vec)
        {
            vec.X = -vec.X;
            vec.Y = -vec.Y;
            vec.Z = -vec.Z;
            vec.W = -vec.W;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector4"/>
        /// by the given <see cref="real_t"/>.
        /// </summary>
        /// <param name="vec">The Hector to multiply.</param>
        /// <param name="scale">The scale to multiply by.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector4 operator *(Hector4 vec, real_t scale)
        {
            vec.X *= scale;
            vec.Y *= scale;
            vec.Z *= scale;
            vec.W *= scale;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector4"/>
        /// by the given <see cref="real_t"/>.
        /// </summary>
        /// <param name="scale">The scale to multiply by.</param>
        /// <param name="vec">The Hector to multiply.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector4 operator *(real_t scale, Hector4 vec)
        {
            vec.X *= scale;
            vec.Y *= scale;
            vec.Z *= scale;
            vec.W *= scale;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector4"/>
        /// by the components of the given <see cref="Hector4"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector4 operator *(Hector4 left, Hector4 right)
        {
            left.X *= right.X;
            left.Y *= right.Y;
            left.Z *= right.Z;
            left.W *= right.W;
            return left;
        }

        /// <summary>
        /// Divides each component of the <see cref="Hector4"/>
        /// by the given <see cref="real_t"/>.
        /// </summary>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisor">The divisor value.</param>
        /// <returns>The divided Hector.</returns>
        public static Hector4 operator /(Hector4 vec, real_t divisor)
        {
            vec.X /= divisor;
            vec.Y /= divisor;
            vec.Z /= divisor;
            vec.W /= divisor;
            return vec;
        }

        /// <summary>
        /// Divides each component of the <see cref="Hector4"/>
        /// by the components of the given <see cref="Hector4"/>.
        /// </summary>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisorv">The divisor Hector.</param>
        /// <returns>The divided Hector.</returns>
        public static Hector4 operator /(Hector4 vec, Hector4 divisorv)
        {
            vec.X /= divisorv.X;
            vec.Y /= divisorv.Y;
            vec.Z /= divisorv.Z;
            vec.W /= divisorv.W;
            return vec;
        }

        /// <summary>
        /// Gets the remainder of each component of the <see cref="Hector4"/>
        /// with the components of the given <see cref="real_t"/>.
        /// This operation uses truncated division, which is often not desired
        /// as it does not work well with negative numbers.
        /// Consider using <see cref="PosMod(real_t)"/> instead
        /// if you want to handle negative numbers.
        /// </summary>
        /// <example>
        /// <code>
        /// GD.Print(new Hector4(10, -20, 30, 40) % 7); // Prints "(3, -6, 2, 5)"
        /// </code>
        /// </example>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisor">The divisor value.</param>
        /// <returns>The remainder Hector.</returns>
        public static Hector4 operator %(Hector4 vec, real_t divisor)
        {
            vec.X %= divisor;
            vec.Y %= divisor;
            vec.Z %= divisor;
            vec.W %= divisor;
            return vec;
        }

        /// <summary>
        /// Gets the remainder of each component of the <see cref="Hector4"/>
        /// with the components of the given <see cref="Hector4"/>.
        /// This operation uses truncated division, which is often not desired
        /// as it does not work well with negative numbers.
        /// Consider using <see cref="PosMod(Hector4)"/> instead
        /// if you want to handle negative numbers.
        /// </summary>
        /// <example>
        /// <code>
        /// GD.Print(new Hector4(10, -20, 30, 10) % new Hector4(7, 8, 9, 10)); // Prints "(3, -4, 3, 0)"
        /// </code>
        /// </example>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisorv">The divisor Hector.</param>
        /// <returns>The remainder Hector.</returns>
        public static Hector4 operator %(Hector4 vec, Hector4 divisorv)
        {
            vec.X %= divisorv.X;
            vec.Y %= divisorv.Y;
            vec.Z %= divisorv.Z;
            vec.W %= divisorv.W;
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
        public static bool operator ==(Hector4 left, Hector4 right)
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
        public static bool operator !=(Hector4 left, Hector4 right)
        {
            return !left.Equals(right);
        }

        /// <summary>
        /// Compares two <see cref="Hector4"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is less than
        /// the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y, Z and finally W values of the two Hectors.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is less than the right.</returns>
        public static bool operator <(Hector4 left, Hector4 right)
        {
            if (left.X == right.X)
            {
                if (left.Y == right.Y)
                {
                    if (left.Z == right.Z)
                    {
                        return left.W < right.W;
                    }
                    return left.Z < right.Z;
                }
                return left.Y < right.Y;
            }
            return left.X < right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector4"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is greater than
        /// the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y, Z and finally W values of the two Hectors.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is greater than the right.</returns>
        public static bool operator >(Hector4 left, Hector4 right)
        {
            if (left.X == right.X)
            {
                if (left.Y == right.Y)
                {
                    if (left.Z == right.Z)
                    {
                        return left.W > right.W;
                    }
                    return left.Z > right.Z;
                }
                return left.Y > right.Y;
            }
            return left.X > right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector4"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is less than
        /// or equal to the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y, Z and finally W values of the two Hectors.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is less than or equal to the right.</returns>
        public static bool operator <=(Hector4 left, Hector4 right)
        {
            if (left.X == right.X)
            {
                if (left.Y == right.Y)
                {
                    if (left.Z == right.Z)
                    {
                        return left.W <= right.W;
                    }
                    return left.Z < right.Z;
                }
                return left.Y < right.Y;
            }
            return left.X < right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector4"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is greater than
        /// or equal to the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y, Z and finally W values of the two Hectors.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is greater than or equal to the right.</returns>
        public static bool operator >=(Hector4 left, Hector4 right)
        {
            if (left.X == right.X)
            {
                if (left.Y == right.Y)
                {
                    if (left.Z == right.Z)
                    {
                        return left.W >= right.W;
                    }
                    return left.Z > right.Z;
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
            return obj is Hector4 other && Equals(other);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are exactly equal.
        /// Note: Due to floating-point precision errors, consider using
        /// <see cref="IsEqualApprox"/> instead, which is more reliable.
        /// </summary>
        /// <param name="other">The other Hector.</param>
        /// <returns>Whether or not the Hectors are exactly equal.</returns>
        public readonly bool Equals(Hector4 other)
        {
            return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
        }

        /// <summary>
        /// Returns <see langword="true"/> if this Hector and <paramref name="other"/> are approximately equal,
        /// by running <see cref="Mathf.IsEqualApprox(real_t, real_t)"/> on each component.
        /// </summary>
        /// <param name="other">The other Hector to compare.</param>
        /// <returns>Whether or not the Hectors are approximately equal.</returns>
        public readonly bool IsEqualApprox(Hector4 other)
        {
            return Mathf.IsEqualApprox(X, other.X) && Mathf.IsEqualApprox(Y, other.Y) && Mathf.IsEqualApprox(Z, other.Z) && Mathf.IsEqualApprox(W, other.W);
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
            return Mathf.IsZeroApprox(X) && Mathf.IsZeroApprox(Y) && Mathf.IsZeroApprox(Z) && Mathf.IsZeroApprox(W);
        }

        /// <summary>
        /// Serves as the hash function for <see cref="Hector4"/>.
        /// </summary>
        /// <returns>A hash code for this Hector.</returns>
        public override readonly int GetHashCode()
        {
            return HashCode.Combine(X, Y, Z, W);
        }

        /// <summary>
        /// Converts this <see cref="Hector4"/> to a string.
        /// </summary>
        /// <returns>A string representation of this Hector.</returns>
        public override readonly string ToString() => ToString(null);

        /// <summary>
        /// Converts this <see cref="Hector4"/> to a string with the given <paramref name="format"/>.
        /// </summary>
        /// <returns>A string representation of this Hector.</returns>
        public readonly string ToString(string? format)
        {
            return $"({X.ToString(format, CultureInfo.InvariantCulture)}, {Y.ToString(format, CultureInfo.InvariantCulture)}, {Z.ToString(format, CultureInfo.InvariantCulture)}, {W.ToString(format, CultureInfo.InvariantCulture)})";
        }
    }
}
