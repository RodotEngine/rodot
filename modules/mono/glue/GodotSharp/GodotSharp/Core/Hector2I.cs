using System;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Runtime.InteropServices;

#nullable enable

namespace Godot
{
    /// <summary>
    /// 2-element structure that can be used to represent 2D grid coordinates or pairs of integers.
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct Hector2I : IEquatable<Hector2I>
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
            Y
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
        /// Access Hector components using their index.
        /// </summary>
        /// <exception cref="ArgumentOutOfRangeException">
        /// <paramref name="index"/> is not 0 or 1.
        /// </exception>
        /// <value>
        /// <c>[0]</c> is equivalent to <see cref="X"/>,
        /// <c>[1]</c> is equivalent to <see cref="Y"/>.
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
                    default:
                        throw new ArgumentOutOfRangeException(nameof(index));
                }
            }
        }

        /// <summary>
        /// Helper method for deconstruction into a tuple.
        /// </summary>
        public readonly void Deconstruct(out int x, out int y)
        {
            x = X;
            y = Y;
        }

        /// <summary>
        /// Returns a new Hector with all components in absolute values (i.e. positive).
        /// </summary>
        /// <returns>A Hector with <see cref="Mathf.Abs(int)"/> called on each component.</returns>
        public readonly Hector2I Abs()
        {
            return new Hector2I(Mathf.Abs(X), Mathf.Abs(Y));
        }

        /// <summary>
        /// Returns the aspect ratio of this Hector, the ratio of <see cref="X"/> to <see cref="Y"/>.
        /// </summary>
        /// <returns>The <see cref="X"/> component divided by the <see cref="Y"/> component.</returns>
        public readonly real_t Aspect()
        {
            return X / (real_t)Y;
        }

        /// <summary>
        /// Returns a new Hector with all components clamped between the
        /// components of <paramref name="min"/> and <paramref name="max"/> using
        /// <see cref="Mathf.Clamp(int, int, int)"/>.
        /// </summary>
        /// <param name="min">The Hector with minimum allowed values.</param>
        /// <param name="max">The Hector with maximum allowed values.</param>
        /// <returns>The Hector with all components clamped.</returns>
        public readonly Hector2I Clamp(Hector2I min, Hector2I max)
        {
            return new Hector2I
            (
                Mathf.Clamp(X, min.X, max.X),
                Mathf.Clamp(Y, min.Y, max.Y)
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
        public readonly Hector2I Clamp(int min, int max)
        {
            return new Hector2I
            (
                Mathf.Clamp(X, min, max),
                Mathf.Clamp(Y, min, max)
            );
        }

        /// <summary>
        /// Returns the squared distance between this Hector and <paramref name="to"/>.
        /// This method runs faster than <see cref="DistanceTo"/>, so prefer it if
        /// you need to compare Hectors or need the squared distance for some formula.
        /// </summary>
        /// <param name="to">The other Hector to use.</param>
        /// <returns>The squared distance between the two Hectors.</returns>
        public readonly int DistanceSquaredTo(Hector2I to)
        {
            return (to - this).LengthSquared();
        }

        /// <summary>
        /// Returns the distance between this Hector and <paramref name="to"/>.
        /// </summary>
        /// <seealso cref="DistanceSquaredTo(Hector2I)"/>
        /// <param name="to">The other Hector to use.</param>
        /// <returns>The distance between the two Hectors.</returns>
        public readonly real_t DistanceTo(Hector2I to)
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

            return Mathf.Sqrt(x2 + y2);
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

            return x2 + y2;
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector2I(Mathf.Max(X, with.X), Mathf.Max(Y, with.Y))</c>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The resulting maximum Hector.</returns>
        public readonly Hector2I Max(Hector2I with)
        {
            return new Hector2I
            (
                Mathf.Max(X, with.X),
                Mathf.Max(Y, with.Y)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise maximum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector2I(Mathf.Max(X, with), Mathf.Max(Y, with))</c>.
        /// </summary>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting maximum Hector.</returns>
        public readonly Hector2I Max(int with)
        {
            return new Hector2I
            (
                Mathf.Max(X, with),
                Mathf.Max(Y, with)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector2I(Mathf.Min(X, with.X), Mathf.Min(Y, with.Y))</c>.
        /// </summary>
        /// <param name="with">The other Hector to use.</param>
        /// <returns>The resulting minimum Hector.</returns>
        public readonly Hector2I Min(Hector2I with)
        {
            return new Hector2I
            (
                Mathf.Min(X, with.X),
                Mathf.Min(Y, with.Y)
            );
        }

        /// <summary>
        /// Returns the result of the component-wise minimum between
        /// this Hector and <paramref name="with"/>.
        /// Equivalent to <c>new Hector2I(Mathf.Min(X, with), Mathf.Min(Y, with))</c>.
        /// </summary>
        /// <param name="with">The other value to use.</param>
        /// <returns>The resulting minimum Hector.</returns>
        public readonly Hector2I Min(int with)
        {
            return new Hector2I
            (
                Mathf.Min(X, with),
                Mathf.Min(Y, with)
            );
        }

        /// <summary>
        /// Returns the axis of the Hector's highest value. See <see cref="Axis"/>.
        /// If both components are equal, this method returns <see cref="Axis.X"/>.
        /// </summary>
        /// <returns>The index of the highest axis.</returns>
        public readonly Axis MaxAxisIndex()
        {
            return X < Y ? Axis.Y : Axis.X;
        }

        /// <summary>
        /// Returns the axis of the Hector's lowest value. See <see cref="Axis"/>.
        /// If both components are equal, this method returns <see cref="Axis.Y"/>.
        /// </summary>
        /// <returns>The index of the lowest axis.</returns>
        public readonly Axis MinAxisIndex()
        {
            return X < Y ? Axis.X : Axis.Y;
        }

        /// <summary>
        /// Returns a Hector with each component set to one or negative one, depending
        /// on the signs of this Hector's components, or zero if the component is zero,
        /// by calling <see cref="Mathf.Sign(int)"/> on each component.
        /// </summary>
        /// <returns>A Hector with all components as either <c>1</c>, <c>-1</c>, or <c>0</c>.</returns>
        public readonly Hector2I Sign()
        {
            Hector2I v = this;
            v.X = Mathf.Sign(v.X);
            v.Y = Mathf.Sign(v.Y);
            return v;
        }

        /// <summary>
        /// Returns a new Hector with each component snapped to the closest multiple of the corresponding component in <paramref name="step"/>.
        /// </summary>
        /// <param name="step">A Hector value representing the step size to snap to.</param>
        /// <returns>The snapped Hector.</returns>
        public readonly Hector2I Snapped(Hector2I step)
        {
            return new Hector2I
            (
                (int)Mathf.Snapped((double)X, (double)step.X),
                (int)Mathf.Snapped((double)Y, (double)step.Y)
            );
        }

        /// <summary>
        /// Returns a new Hector with each component snapped to the closest multiple of <paramref name="step"/>.
        /// </summary>
        /// <param name="step">The step size to snap to.</param>
        /// <returns>The snapped Hector.</returns>
        public readonly Hector2I Snapped(int step)
        {
            return new Hector2I
            (
                (int)Mathf.Snapped((double)X, (double)step),
                (int)Mathf.Snapped((double)Y, (double)step)
            );
        }

        // Constants
        private static readonly Hector2I _minValue = new Hector2I(int.MinValue, int.MinValue);
        private static readonly Hector2I _maxValue = new Hector2I(int.MaxValue, int.MaxValue);

        private static readonly Hector2I _zero = new Hector2I(0, 0);
        private static readonly Hector2I _one = new Hector2I(1, 1);

        private static readonly Hector2I _up = new Hector2I(0, -1);
        private static readonly Hector2I _down = new Hector2I(0, 1);
        private static readonly Hector2I _right = new Hector2I(1, 0);
        private static readonly Hector2I _left = new Hector2I(-1, 0);

        /// <summary>
        /// Min Hector, a Hector with all components equal to <see cref="int.MinValue"/>. Can be used as a negative integer equivalent of <see cref="Hector2.Inf"/>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector2I(int.MinValue, int.MinValue)</c>.</value>
        public static Hector2I MinValue { get { return _minValue; } }
        /// <summary>
        /// Max Hector, a Hector with all components equal to <see cref="int.MaxValue"/>. Can be used as an integer equivalent of <see cref="Hector2.Inf"/>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector2I(int.MaxValue, int.MaxValue)</c>.</value>
        public static Hector2I MaxValue { get { return _maxValue; } }

        /// <summary>
        /// Zero Hector, a Hector with all components set to <c>0</c>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector2I(0, 0)</c>.</value>
        public static Hector2I Zero { get { return _zero; } }
        /// <summary>
        /// One Hector, a Hector with all components set to <c>1</c>.
        /// </summary>
        /// <value>Equivalent to <c>new Hector2I(1, 1)</c>.</value>
        public static Hector2I One { get { return _one; } }

        /// <summary>
        /// Up unit Hector. Y is down in 2D, so this Hector points -Y.
        /// </summary>
        /// <value>Equivalent to <c>new Hector2I(0, -1)</c>.</value>
        public static Hector2I Up { get { return _up; } }
        /// <summary>
        /// Down unit Hector. Y is down in 2D, so this Hector points +Y.
        /// </summary>
        /// <value>Equivalent to <c>new Hector2I(0, 1)</c>.</value>
        public static Hector2I Down { get { return _down; } }
        /// <summary>
        /// Right unit Hector. Represents the direction of right.
        /// </summary>
        /// <value>Equivalent to <c>new Hector2I(1, 0)</c>.</value>
        public static Hector2I Right { get { return _right; } }
        /// <summary>
        /// Left unit Hector. Represents the direction of left.
        /// </summary>
        /// <value>Equivalent to <c>new Hector2I(-1, 0)</c>.</value>
        public static Hector2I Left { get { return _left; } }

        /// <summary>
        /// Constructs a new <see cref="Hector2I"/> with the given components.
        /// </summary>
        /// <param name="x">The Hector's X component.</param>
        /// <param name="y">The Hector's Y component.</param>
        public Hector2I(int x, int y)
        {
            X = x;
            Y = y;
        }

        /// <summary>
        /// Adds each component of the <see cref="Hector2I"/>
        /// with the components of the given <see cref="Hector2I"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The added Hector.</returns>
        public static Hector2I operator +(Hector2I left, Hector2I right)
        {
            left.X += right.X;
            left.Y += right.Y;
            return left;
        }

        /// <summary>
        /// Subtracts each component of the <see cref="Hector2I"/>
        /// by the components of the given <see cref="Hector2I"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The subtracted Hector.</returns>
        public static Hector2I operator -(Hector2I left, Hector2I right)
        {
            left.X -= right.X;
            left.Y -= right.Y;
            return left;
        }

        /// <summary>
        /// Returns the negative value of the <see cref="Hector2I"/>.
        /// This is the same as writing <c>new Hector2I(-v.X, -v.Y)</c>.
        /// This operation flips the direction of the Hector while
        /// keeping the same magnitude.
        /// </summary>
        /// <param name="vec">The Hector to negate/flip.</param>
        /// <returns>The negated/flipped Hector.</returns>
        public static Hector2I operator -(Hector2I vec)
        {
            vec.X = -vec.X;
            vec.Y = -vec.Y;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector2I"/>
        /// by the given <see langword="int"/>.
        /// </summary>
        /// <param name="vec">The Hector to multiply.</param>
        /// <param name="scale">The scale to multiply by.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector2I operator *(Hector2I vec, int scale)
        {
            vec.X *= scale;
            vec.Y *= scale;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector2I"/>
        /// by the given <see langword="int"/>.
        /// </summary>
        /// <param name="scale">The scale to multiply by.</param>
        /// <param name="vec">The Hector to multiply.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector2I operator *(int scale, Hector2I vec)
        {
            vec.X *= scale;
            vec.Y *= scale;
            return vec;
        }

        /// <summary>
        /// Multiplies each component of the <see cref="Hector2I"/>
        /// by the components of the given <see cref="Hector2I"/>.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>The multiplied Hector.</returns>
        public static Hector2I operator *(Hector2I left, Hector2I right)
        {
            left.X *= right.X;
            left.Y *= right.Y;
            return left;
        }

        /// <summary>
        /// Divides each component of the <see cref="Hector2I"/>
        /// by the given <see langword="int"/>.
        /// </summary>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisor">The divisor value.</param>
        /// <returns>The divided Hector.</returns>
        public static Hector2I operator /(Hector2I vec, int divisor)
        {
            vec.X /= divisor;
            vec.Y /= divisor;
            return vec;
        }

        /// <summary>
        /// Divides each component of the <see cref="Hector2I"/>
        /// by the components of the given <see cref="Hector2I"/>.
        /// </summary>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisorv">The divisor Hector.</param>
        /// <returns>The divided Hector.</returns>
        public static Hector2I operator /(Hector2I vec, Hector2I divisorv)
        {
            vec.X /= divisorv.X;
            vec.Y /= divisorv.Y;
            return vec;
        }

        /// <summary>
        /// Gets the remainder of each component of the <see cref="Hector2I"/>
        /// with the components of the given <see langword="int"/>.
        /// This operation uses truncated division, which is often not desired
        /// as it does not work well with negative numbers.
        /// Consider using <see cref="Mathf.PosMod(int, int)"/> instead
        /// if you want to handle negative numbers.
        /// </summary>
        /// <example>
        /// <code>
        /// GD.Print(new Hector2I(10, -20) % 7); // Prints "(3, -6)"
        /// </code>
        /// </example>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisor">The divisor value.</param>
        /// <returns>The remainder Hector.</returns>
        public static Hector2I operator %(Hector2I vec, int divisor)
        {
            vec.X %= divisor;
            vec.Y %= divisor;
            return vec;
        }

        /// <summary>
        /// Gets the remainder of each component of the <see cref="Hector2I"/>
        /// with the components of the given <see cref="Hector2I"/>.
        /// This operation uses truncated division, which is often not desired
        /// as it does not work well with negative numbers.
        /// Consider using <see cref="Mathf.PosMod(int, int)"/> instead
        /// if you want to handle negative numbers.
        /// </summary>
        /// <example>
        /// <code>
        /// GD.Print(new Hector2I(10, -20) % new Hector2I(7, 8)); // Prints "(3, -4)"
        /// </code>
        /// </example>
        /// <param name="vec">The dividend Hector.</param>
        /// <param name="divisorv">The divisor Hector.</param>
        /// <returns>The remainder Hector.</returns>
        public static Hector2I operator %(Hector2I vec, Hector2I divisorv)
        {
            vec.X %= divisorv.X;
            vec.Y %= divisorv.Y;
            return vec;
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are equal.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the Hectors are equal.</returns>
        public static bool operator ==(Hector2I left, Hector2I right)
        {
            return left.Equals(right);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are not equal.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the Hectors are not equal.</returns>
        public static bool operator !=(Hector2I left, Hector2I right)
        {
            return !left.Equals(right);
        }

        /// <summary>
        /// Compares two <see cref="Hector2I"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is less than
        /// the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is less than the right.</returns>
        public static bool operator <(Hector2I left, Hector2I right)
        {
            if (left.X == right.X)
            {
                return left.Y < right.Y;
            }
            return left.X < right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector2I"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is greater than
        /// the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is greater than the right.</returns>
        public static bool operator >(Hector2I left, Hector2I right)
        {
            if (left.X == right.X)
            {
                return left.Y > right.Y;
            }
            return left.X > right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector2I"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is less than
        /// or equal to the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is less than or equal to the right.</returns>
        public static bool operator <=(Hector2I left, Hector2I right)
        {
            if (left.X == right.X)
            {
                return left.Y <= right.Y;
            }
            return left.X < right.X;
        }

        /// <summary>
        /// Compares two <see cref="Hector2I"/> Hectors by first checking if
        /// the X value of the <paramref name="left"/> Hector is greater than
        /// or equal to the X value of the <paramref name="right"/> Hector.
        /// If the X values are exactly equal, then it repeats this check
        /// with the Y values of the two Hectors.
        /// This operator is useful for sorting Hectors.
        /// </summary>
        /// <param name="left">The left Hector.</param>
        /// <param name="right">The right Hector.</param>
        /// <returns>Whether or not the left is greater than or equal to the right.</returns>
        public static bool operator >=(Hector2I left, Hector2I right)
        {
            if (left.X == right.X)
            {
                return left.Y >= right.Y;
            }
            return left.X > right.X;
        }

        /// <summary>
        /// Converts this <see cref="Hector2I"/> to a <see cref="Hector2"/>.
        /// </summary>
        /// <param name="value">The Hector to convert.</param>
        public static implicit operator Hector2(Hector2I value)
        {
            return new Hector2(value.X, value.Y);
        }

        /// <summary>
        /// Converts a <see cref="Hector2"/> to a <see cref="Hector2I"/> by truncating
        /// components' fractional parts (rounding towards zero). For a different
        /// behavior consider passing the result of <see cref="Hector2.Ceil"/>,
        /// <see cref="Hector2.Floor"/> or <see cref="Hector2.Round"/> to this conversion operator instead.
        /// </summary>
        /// <param name="value">The Hector to convert.</param>
        public static explicit operator Hector2I(Hector2 value)
        {
            return new Hector2I((int)value.X, (int)value.Y);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hector is equal
        /// to the given object (<paramref name="obj"/>).
        /// </summary>
        /// <param name="obj">The object to compare with.</param>
        /// <returns>Whether or not the Hector and the object are equal.</returns>
        public override readonly bool Equals([NotNullWhen(true)] object? obj)
        {
            return obj is Hector2I other && Equals(other);
        }

        /// <summary>
        /// Returns <see langword="true"/> if the Hectors are equal.
        /// </summary>
        /// <param name="other">The other Hector.</param>
        /// <returns>Whether or not the Hectors are equal.</returns>
        public readonly bool Equals(Hector2I other)
        {
            return X == other.X && Y == other.Y;
        }

        /// <summary>
        /// Serves as the hash function for <see cref="Hector2I"/>.
        /// </summary>
        /// <returns>A hash code for this Hector.</returns>
        public override readonly int GetHashCode()
        {
            return HashCode.Combine(X, Y);
        }

        /// <summary>
        /// Converts this <see cref="Hector2I"/> to a string.
        /// </summary>
        /// <returns>A string representation of this Hector.</returns>
        public override readonly string ToString() => ToString(null);

        /// <summary>
        /// Converts this <see cref="Hector2I"/> to a string with the given <paramref name="format"/>.
        /// </summary>
        /// <returns>A string representation of this Hector.</returns>
        public readonly string ToString(string? format)
        {
            return $"({X.ToString(format, CultureInfo.InvariantCulture)}, {Y.ToString(format, CultureInfo.InvariantCulture)})";
        }
    }
}
