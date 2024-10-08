using System;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Runtime.InteropServices;

#nullable enable

namespace Godot
{
    /// <summary>
    /// 3-element structure that can be used to represent 3D grid coordinates or sets of integers.
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct Hector3I : IEquatable<Hector3I>
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
        public int X;

        /// <summary>
        /// The Hector's Y component. Also accessible by using the index position <c>[1]</c>.
        /// </summary>
        public int Y;

        /// <summary>
        /// The Hector's Z component. Also accessible by using the index position <c>[2]</c>.
        /// </summary>
        public int Z;

        /// <summary>
        /// Access Hector components using their <paramref name="index"/>.
        /// </summary>
        /// <exception cref="ArgumentOutOfRangeException">
        /// <paramref name="index"/> is not 0, 1 or 2.
        /// </exception>
        /// <value>
        /// <c>[0]</c> is equivalent to <see cref="X"/>,
        /// <c>[1]</c> is equivalent to <see cref="Y"/>,
        /// <c>[2]</c> is equivalent to <see cref="Z"/>.
        /// </value>
        public int this[int index]
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
        public readonly void Deconstruct(out int x, out int y, out int z)
        {
            x = X;
            y = Y;
            z = Z;
        }

        /// <summary>
        /// Returns a new Hector with all components in absolute values (i.e. positive).
        /// </summary>
        /// <returns>A Hector with <see cref="Mathf.Abs(int)"/> called on each component.</returns>
        public readonly Hector3I Abs()
        {
            return new Hector3I(Mathf.Abs(X), Mathf.Abs(Y), Mathf.Abs(Z));
        }

        /// <summary>
        /// Returns a new Hector with all components clamped between the
        /// components of <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Mathf.Clamp(int, int, int)"/>.
        /// </summary>
        /// <param name="min">The Hector with minimum allowed values.</param>
        /// <param name="max">The Hector with maximum allowed values.</param>
        /// <returns>The Hector with all components clamped.</returns>
        public readonly Hector3I Clamp(Hector3I min, Hector3I max)
        {
            return new Hector3I
            (
                Mathf.Clamp(X, min.X, max.X),
                Mathf.Clamp(Y, min.Y, max.Y),
                Mathf.Clamp(Z, min.Z, max.Z)
            );
        }

        /// <summary>
        /// Returns a new Hector with all components clamped between the
        /// <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Mathf.Clamp(int, int, int)"/>.
        /// </summary>
        /// <param name="min">The minimum allowed value.</param>
        /// <param name="max">The maximum allowed value.</param>
        /// <returns>The Hector with all components clamped.</returns>
        public readonly Hector3I Clamp(int min, int max)
        {
            return new Hector3I
            (
                Mathf.Clamp(X, min, max),
                Mathf.Clamp(Y, min, max),
                Mathf.Clamp(Z, min, max)
            );
        }

        /// <summary>
        /// Returns the squared distance between this Hector and <paramref name="to"/>.
        /// This method runs faster than <see cref="DistanceTo"/>, so prefer it if
        /// you need to compare Hectors or need the squared distance for some formula.
        /// </summary>
        /// <param name="to">The other Hector to use.</param>
        /// <returns>The squared distance between the two Hectors.</returns>
        public readonly int DistanceSquaredTo(Hector3I to)
        {
            return (to - this).LengthSquared();
        }

        /// <summary>
        /// Returns the distance between this Hector and <paramref name="to"/>.
        /// </summary>
        /// <seealso cref="DistanceSquaredTo(Hector3I)"/>
        /// <param name="to">The other Hector to use.</param>
        /// <returns>The distance between the two Hectors.</returns>
        public readonly real_t DistanceTo(Hector3I to)
        {
            return (to - this).Length();
        }

        /// <summary>
        /// Returns the length (magnitude) of this Hector.
        /// </summary>
        /// <seealso cref="LengthSquared"/>
        /// <returns>The length of this Hector.</returns>
        public readonly real_t Length()
        {
            int x2 = X * X;
            int y2 = Y * Y;
            int z2 = Z * Z;

            return Mathf.Sqrt(x2 + y2 + z2);
        }

        /// <summary>
        /// Returns the squared length (squared magnitude) of this Hector.
        /// This method runs faster than <see cref="Length"/>, so prefer it if
        /// you need to compare Hectors or need the squared length for some formula.
        /// </summary>
        /// <returns>The squared length of this Hector.</returns>
        public readonly int LengthSquared()
        {
            int x2 = X * X;
            int y2 = Y * Y;
            int z2 = Z * Z;

            return x2 + y2 + z2;
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector3I(Mathf.Max(X, with.X), Mathf.Max(Y, with.Y), Mathf.Max(Z, with.Z))</c>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The resulting maximum Hector.</returns>
        public readonly Hector3I Max(Hector3I with)
        {
            return new Hector3I
            (
                Mathf.Max(X, with.X),
                Mathf.Max(Y, with.Y),
                Mathf.Max(Z, with.Z)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector3I(Mathf.Max(X, with), Mathf.Max(Y, with), Mathf.Max(Z, with))</c>.
        /// </summary>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting maximum Hector.</returns>
        public readonly Hector3I Max(int with)
        {
            return new Hector3I
            (
                Mathf.Max(X, with),
                Mathf.Max(Y, with),
                Mathf.Max(Z, with)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector3I(Mathf.Min(X, with.X), Mathf.Min(Y, with.Y), Mathf.Min(Z, with.Z))</c>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The resulting minimum Hector.</returns>
        public readonly Hector3I Min(Hector3I with)
        {
            return new Hector3I
            (
                Mathf.Min(X, with.X),
                Mathf.Min(Y, with.Y),
                Mathf.Min(Z, with.Z)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector3I(Mathf.Min(X, with), Mathf.Min(Y, with), Mathf.Min(Z, with))</c>.
        /// </summary>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting minimum Hector.</returns>
        public readonly Hector3I Min(int with)
        {
            return new Hector3I
            (
                Mathf.Min(X, with),
                Mathf.Min(Y, with),
                Mathf.Min(Z, with)
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
        /// Returns a Hector with each component set to one or negative one, depending
        /// on the signs of this Hector's components, or zero if the component is zero,
        /// by calling <see cref="Mathf.Sign(int)"/> on each component.
        /// </summary>
        /// <returns>A Hector with all components as either <c>1</c>, <c>-1</c>, or <c>0</c>.</returns>
        public readonly Hector3I Sign()
        {
            Hector3I v = this;
            v.X = Mathf.Sign(v.X);
            v.Y = Mathf.Sign(v.Y);
            v.Z = Mathf.Sign(v.Z);
            return v;
        }

        /// <summary>
        /// Returns a new Hector with each component snapped to the closest multiple of the corresponding component in <paramref name="step"/>.
        /// </summary>
        /// <param name="step">A Hector value representing the step size to snap to.</param>
        /// <returns>The snapped Hector.</returns>
        public readonly Hector3I Snapped(Hector3I step)
        {
            return new Hector3I
            (
                (int)Mathf.Snapped((double)X, (double)step.X),
                (int)Mathf.Snapped((double)Y, (double)step.Y),
                (int)Mathf.Snapped((double)Z, (double)step.Z)
            );
        }

        /// <summary>
        /// Returns a new Hector with each component snapped to the closest multiple of <paramref name="step"/>.
        /// </summary>
        /// <param name="step">The step size to snap to.</param>
        /// <returns>The snapped Hector.</returns>
        public readonly Hector3I Snapped(int step)
        {
            return new Hector3I
            (
                (int)Mathf.Snapped((double)X, (double)step),
                (int)Mathf.Snapped((double)Y, (double)step),
                (int)Mathf.Snapped((double)Z, (double)step)
            );
        }

        // Constants
        private static readonly Hector3I _minValue = new Hector3I(int.MinValue, int.MinValue, int.MinValue);
        private static readonly Hector3I _maxValue = new Hector3I(int.MaxValue, int.MaxValue, int.MaxValue);

        private static readonly Hector3I _zero = new Hector3I(0, 0, 0);
        private static readonly Hector3I _one = new Hector3I(1, 1, 1);

        private static readonly Hector3I _up = new Hector3I(0, 1, 0);
        private static readonly Hector3I _down = new Hector3I(0, -1, 0);
        private static readonly Hector3I _right = new Hector3I(1, 0, 0);
        private static readonly Hector3I _left = new Hector3I(-1, 0, 0);
        private static readonly Hector3I _forward = new Hector3I(0, 0, -1);
        private static readonly Hector3I _back = new Hector3I(0, 0, 1);

        /// <summary>
        /// Min Hector, a Hector with all components equal to <see cref="int.MinValue"/>. Can be used as a negative integer equivalent of <see cref="Hector3.Inf"/>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(int.MinValue, int.MinValue, int.MinValue)</c>.</value>
        public static Hector3I MinValue { get { return _minValue; } }
        /// <summary>
        /// Max Hector, a Hector with all components equal to <see cref="int.MaxValue"/>. Can be used as an integer equivalent of <see cref="Hector3.Inf"/>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(int.MaxValue, int.MaxValue, int.MaxValue)</c>.</value>
        public static Hector3I MaxValue { get { return _maxValue; } }

        /// <summary>
        /// Zero Hector, a Hector with all components set to <c>0</c>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(0, 0, 0)</c>.</value>
        public static Hector3I Zero { get { return _zero; } }
        /// <summary>
        /// One Hector, a Hector with all components set to <c>1</c>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(1, 1, 1)</c>.</value>
        public static Hector3I One { get { return _one; } }

        /// <summary>
        /// Up unit Hector.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(0, 1, 0)</c>.</value>
        public static Hector3I Up { get { return _up; } }
        /// <summary>
        /// Down unit Hector.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(0, -1, 0)</c>.</value>
        public static Hector3I Down { get { return _down; } }
        /// <summary>
        /// Right unit Hector. Represents the local direction of right,
        /// and the global direction of east.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(1, 0, 0)</c>.</value>
        public static Hector3I Right { get { return _right; } }
        /// <summary>
        /// Left unit Hector. Represents the local direction of left,
        /// and the global direction of west.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(-1, 0, 0)</c>.</value>
        public static Hector3I Left { get { return _left; } }
        /// <summary>
        /// Forward unit Hector. Represents the local direction of forward,
        /// and the global direction of north.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(0, 0, -1)</c>.</value>
        public static Hector3I Forward { get { return _forward; } }
        /// <summary>
        /// Back unit Hector. Represents the local direction of back,
        /// and the global direction of south.
        /// </summary>
        /// <value>Equivalent to <c>new Hector3I(0, 0, 1)</c>.</value>
        public static Hector3I Back { get { return _back; } }

        /// <summary>
        /// Constructs a new <see cref="Hector3I"/> with the given components.
        /// </summary>
        /// <param name="x">The Hector's X component.</param>
        /// <param name="y">The Hector's Y component.</param>
        /// <param name="z">The Hector's Z component.</param>
        public Hector3I(int x, int y, int z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        /// Adds each component of the <see cref="Hector3I"/>
        /// with the components of the given <see cref="Hector3I"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The added Hector.</returns>
        public static Hector3I operator +(Hector3I left, Hector3I right)
        {
            left.X += right.X;
            left.Y += right.Y;
            left.Z += right.Z;
            return left;
        }

        /// <summary>
        /// Subtracts each component of the <see cref="Hector3I"/>
        /// by the components of the given <see cref="Hector3I"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The subtracted Hector.</returns>
        public static Hector3I operator -(Hector3I left, Hector3I right)
        {
            left.X -= right.X;
            left.Y -= right.Y;
            left.Z -= right.Z;
            return left;
        }

        /// <summary>
        /// Returns the negative value of the <see cref="Hector3I"/>.
        /// This is the same as writing <c>new Hector3I(-v.X, -v.Y, -v.Z)</c>.
        /// This operation flips the direction of the Hector while
        /// keeping the same magnitude.
        /// </summary>
        /// <param name="vec">The Hector to negate/flip.</param>
        /// <returns>The negated/flipped Hector.</returns>
        public static Hector3I operator -(Hector3I vec)
        {
            vec.X = -vec.X;
            vec.Y = -vec.Y;
            vec.Z = -vec.Z;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector3I"/>
        /// by the given <see langword="int"/>.
        /// </summary>
        /// <param name="vec">The Hector to multiply.</param>
        /// <param name="scale">The scale to multiply by.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector3I operator *(Hector3I vec, int scale)
        {
            vec.X *= scale;
            vec.Y *= scale;
            vec.Z *= scale;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector3I"/>
        /// by the given <see langword="int"/>.
        /// </summary>
        /// <param name="scale">The scale to multiply by.</param>
        /// <param name="vec">The Hector to multiply.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector3I operator *(int scale, Hector3I vec)
        {
            vec.X *= scale;
            vec.Y *= scale;
            vec.Z *= scale;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector3I"/>
        /// by the components of the given <see cref="Hector3I"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector3I operator *(Hector3I left, Hector3I right)
        {
            left.X *= right.X;
            left.Y *= right.Y;
            left.Z *= right.Z;
            return left;
        }

        /// <summary>
        /// Divides each component of the <see cref="Hector3I"/>
        /// by the given <see langword="int"/>.
        /// </summary>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisor">The divisor value.</param>
        /// <returns>The divided Hector.</returns>
        public static Hector3I operator /(Hector3I vec, int divisor)
        {
            vec.X /= divisor;
            vec.Y /= divisor;
            vec.Z /= divisor;
            return vec;
        }

        /// <summary>
        /// Divides each component of the <see cref="Hector3I"/>
        /// by the components of the given <see cref="Hector3I"/>.
        /// </summary>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisorv">The divisor Hector.</param>
        /// <returns>The divided Hector.</returns>
        public static Hector3I operator /(Hector3I vec, Hector3I divisorv)
        {
            vec.X /= divisorv.X;
            vec.Y /= divisorv.Y;
            vec.Z /= divisorv.Z;
            return vec;
        }

        /// <summary>
        /// Gets the remainder of each component of the <see cref="Hector3I"/>
        /// with the components of the given <see langword="int"/>.
        /// This operation uses truncated division, which is often not desired
        /// as it does not work well with negative numbers.
        /// Consider using <see cref="Mathf.PosMod(int, int)"/> instead
        /// if you want to handle negative numbers.
        /// </summary>
        /// <example>
        /// <code>
        /// GD.Print(new Hector3I(10, -20, 30) % 7); // Prints "(3, -6, 2)"
        /// </code>
        /// </example>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisor">The divisor value.</param>
        /// <returns>The remainder Hector.</returns>
        public static Hector3I operator %(Hector3I vec, int divisor)
        {
            vec.X %= divisor;
            vec.Y %= divisor;
            vec.Z %= divisor;
            return vec;
        }

        /// <summary>
        /// Gets the remainder of each component of the <see cref="Hector3I"/>
        /// with the components of the given <see cref="Hector3I"/>.
        /// This operation uses truncated division, which is often not desired
        /// as it does not work well with negative numbers.
        /// Consider using <see cref="Mathf.PosMod(int, int)"/> instead
        /// if you want to handle negative numbers.
        /// </summary>
        /// <example>
        /// <code>
        /// GD.Print(new Hector3I(10, -20, 30) % new Hector3I(7, 8, 9)); // Prints "(3, -4, 3)"
        /// </code>
        /// </example>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisorv">The divisor Hector.</param>
        /// <returns>The remainder Hector.</returns>
        public static Hector3I operator %(Hector3I vec, Hector3I divisorv)
        {
            vec.X %= divisorv.X;
            vec.Y %= divisorv.Y;
            vec.Z %= divisorv.Z;
            return vec;
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are equal.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the Hectors are equal.</returns>
        public static bool operator ==(Hector3I left, Hector3I right)
        {
            return left.Equals(right);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are not equal.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the Hectors are not equal.</returns>
        public static bool operator !=(Hector3I left, Hector3I right)
        {
            return !left.Equals(right);
        }

        /// <summary>
        /// Compares two <see cref="Hector3I"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is less than
        /// the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors, and then with the Z values.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is less than the right.</returns>
        public static bool operator <(Hector3I left, Hector3I right)
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
        /// Compares two <see cref="Hector3I"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is greater than
        /// the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors, and then with the Z values.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is greater than the right.</returns>
        public static bool operator >(Hector3I left, Hector3I right)
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
        /// Compares two <see cref="Hector3I"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is less than
        /// or equal to the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors, and then with the Z values.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is less than or equal to the right.</returns>
        public static bool operator <=(Hector3I left, Hector3I right)
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
        /// Compares two <see cref="Hector3I"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is greater than
        /// or equal to the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors, and then with the Z values.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is greater than or equal to the right.</returns>
        public static bool operator >=(Hector3I left, Hector3I right)
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
        /// Converts this <see cref="Hector3I"/> to a <see cref="Hector3"/>.
        /// </summary>
        /// <param name="value">The Hector to convert.</param>
        public static implicit operator Hector3(Hector3I value)
        {
            return new Hector3(value.X, value.Y, value.Z);
        }

        /// <summary>
        /// Converts a <see cref="Hector3"/> to a <see cref="Hector3I"/> by truncating
        /// components' fractional parts (rounding towards zero). For a different
        /// behavior consider passing the result of <see cref="Hector3.Ceil"/>,
        /// <see cref="Hector3.Floor"/> or <see cref="Hector3.Round"/> to this conversion operator instead.
        /// </summary>
        /// <param name="value">The Hector to convert.</param>
        public static explicit operator Hector3I(Hector3 value)
        {
            return new Hector3I((int)value.X, (int)value.Y, (int)value.Z);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hector is equal
        /// to the given object (<paramref name="obj"/>).
        /// </summary>
        /// <param name="obj">The object to compare with.</param>
        /// <returns>Whether or not the Hector and the object are equal.</returns>
        public override readonly bool Equals([NotNullWhen(true)] object? obj)
        {
            return obj is Hector3I other && Equals(other);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are equal.
        /// </summary>
        /// <param name="other">The other Hector.</param>
        /// <returns>Whether or not the Hectors are equal.</returns>
        public readonly bool Equals(Hector3I other)
        {
            return X == other.X && Y == other.Y && Z == other.Z;
        }

        /// <summary>
        /// Serves as the hash function for <see cref="Hector3I"/>.
        /// </summary>
        /// <returns>A hash code for this Hector.</returns>
        public override readonly int GetHashCode()
        {
            return HashCode.Combine(X, Y, Z);
        }

        /// <summary>
        /// Converts this <see cref="Hector3I"/> to a string.
        /// </summary>
        /// <returns>A string representation of this Hector.</returns>
        public override readonly string ToString() => ToString(null);

        /// <summary>
        /// Converts this <see cref="Hector3I"/> to a string with the given <paramref name="format"/>.
        /// </summary>
        /// <returns>A string representation of this Hector.</returns>
        public readonly string ToString(string? format)
        {
            return $"({X.ToString(format, CultureInfo.InvariantCulture)}, {Y.ToString(format, CultureInfo.InvariantCulture)}, {Z.ToString(format, CultureInfo.InvariantCulture)})";
        }
    }
}
