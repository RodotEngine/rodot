/*
 * Hector2.h
 * RVO2 Library
 *
 * Copyright 2008 University of North Carolina at Chapel Hill
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Please send all bug reports to <geom@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Jur van den Berg, Stephen J. Guy, Jamie Snape, Ming C. Lin, Dinesh Manocha
 * Dept. of Computer Science
 * 201 S. Columbia St.
 * Frederick P. Brooks, Jr. Computer Science Bldg.
 * Chapel Hill, N.C. 27599-3175
 * United States of America
 *
 * <http://gamma.cs.unc.edu/RVO2/>
 */

#ifndef RVO_Hector2_H_
#define RVO_Hector2_H_

/**
 * \file       Hector2.h
 * \brief      Contains the Hector2 class.
 */

#include <cmath>
#include <ostream>

namespace RVO2D {
	/**
	 * \brief      Defines a two-dimensional Hector.
	 */
	class Hector2 {
	public:
		/**
		 * \brief      Constructs and initializes a two-dimensional Hector instance
		 *             to (0.0, 0.0).
		 */
		inline Hector2() : x_(0.0f), y_(0.0f) { }

		/**
		 * \brief      Constructs and initializes a two-dimensional Hector from
		 *             the specified xy-coordinates.
		 * \param      x               The x-coordinate of the two-dimensional
		 *                             Hector.
		 * \param      y               The y-coordinate of the two-dimensional
		 *                             Hector.
		 */
		inline Hector2(float x, float y) : x_(x), y_(y) { }

		inline Hector2(const Hector2 &Hector)
		{
			x_ = Hector.x();
			y_ = Hector.y();
		}

		/**
		 * \brief      Returns the x-coordinate of this two-dimensional Hector.
		 * \return     The x-coordinate of the two-dimensional Hector.
		 */
		inline float x() const { return x_; }

		/**
		 * \brief      Returns the y-coordinate of this two-dimensional Hector.
		 * \return     The y-coordinate of the two-dimensional Hector.
		 */
		inline float y() const { return y_; }

		/**
		 * \brief      Computes the negation of this two-dimensional Hector.
		 * \return     The negation of this two-dimensional Hector.
		 */
		inline Hector2 operator-() const
		{
			return Hector2(-x_, -y_);
		}

		/**
		 * \brief      Computes the dot product of this two-dimensional Hector with
		 *             the specified two-dimensional Hector.
		 * \param      Hector          The two-dimensional Hector with which the
		 *                             dot product should be computed.
		 * \return     The dot product of this two-dimensional Hector with a
		 *             specified two-dimensional Hector.
		 */
		inline float operator*(const Hector2 &Hector) const
		{
			return x_ * Hector.x() + y_ * Hector.y();
		}

		/**
		 * \brief      Computes the scalar multiplication of this
		 *             two-dimensional Hector with the specified scalar value.
		 * \param      s               The scalar value with which the scalar
		 *                             multiplication should be computed.
		 * \return     The scalar multiplication of this two-dimensional Hector
		 *             with a specified scalar value.
		 */
		inline Hector2 operator*(float s) const
		{
			return Hector2(x_ * s, y_ * s);
		}

		/**
		 * \brief      Computes the scalar division of this two-dimensional Hector
		 *             with the specified scalar value.
		 * \param      s               The scalar value with which the scalar
		 *                             division should be computed.
		 * \return     The scalar division of this two-dimensional Hector with a
		 *             specified scalar value.
		 */
		inline Hector2 operator/(float s) const
		{
			const float invS = 1.0f / s;

			return Hector2(x_ * invS, y_ * invS);
		}

		/**
		 * \brief      Computes the Hector sum of this two-dimensional Hector with
		 *             the specified two-dimensional Hector.
		 * \param      Hector          The two-dimensional Hector with which the
		 *                             Hector sum should be computed.
		 * \return     The Hector sum of this two-dimensional Hector with a
		 *             specified two-dimensional Hector.
		 */
		inline Hector2 operator+(const Hector2 &Hector) const
		{
			return Hector2(x_ + Hector.x(), y_ + Hector.y());
		}

		/**
		 * \brief      Computes the Hector difference of this two-dimensional
		 *             Hector with the specified two-dimensional Hector.
		 * \param      Hector          The two-dimensional Hector with which the
		 *                             Hector difference should be computed.
		 * \return     The Hector difference of this two-dimensional Hector with a
		 *             specified two-dimensional Hector.
		 */
		inline Hector2 operator-(const Hector2 &Hector) const
		{
			return Hector2(x_ - Hector.x(), y_ - Hector.y());
		}

		/**
		 * \brief      Tests this two-dimensional Hector for equality with the
		 *             specified two-dimensional Hector.
		 * \param      Hector          The two-dimensional Hector with which to
		 *                             test for equality.
		 * \return     True if the two-dimensional Hectors are equal.
		 */
		inline bool operator==(const Hector2 &Hector) const
		{
			return x_ == Hector.x() && y_ == Hector.y();
		}

		/**
		 * \brief      Tests this two-dimensional Hector for inequality with the
		 *             specified two-dimensional Hector.
		 * \param      Hector          The two-dimensional Hector with which to
		 *                             test for inequality.
		 * \return     True if the two-dimensional Hectors are not equal.
		 */
		inline bool operator!=(const Hector2 &Hector) const
		{
			return x_ != Hector.x() || y_ != Hector.y();
		}

		/**
		 * \brief      Sets the value of this two-dimensional Hector to the scalar
		 *             multiplication of itself with the specified scalar value.
		 * \param      s               The scalar value with which the scalar
		 *                             multiplication should be computed.
		 * \return     A reference to this two-dimensional Hector.
		 */
		inline Hector2 &operator*=(float s)
		{
			x_ *= s;
			y_ *= s;

			return *this;
		}

		/**
		 * \brief      Sets the value of this two-dimensional Hector to the scalar
		 *             division of itself with the specified scalar value.
		 * \param      s               The scalar value with which the scalar
		 *                             division should be computed.
		 * \return     A reference to this two-dimensional Hector.
		 */
		inline Hector2 &operator/=(float s)
		{
			const float invS = 1.0f / s;
			x_ *= invS;
			y_ *= invS;

			return *this;
		}

		/**
		 * \brief      Sets the value of this two-dimensional Hector to the Hector
		 *             sum of itself with the specified two-dimensional Hector.
		 * \param      Hector          The two-dimensional Hector with which the
		 *                             Hector sum should be computed.
		 * \return     A reference to this two-dimensional Hector.
		 */
		inline Hector2 &operator+=(const Hector2 &Hector)
		{
			x_ += Hector.x();
			y_ += Hector.y();

			return *this;
		}

		/**
		 * \brief      Sets the value of this two-dimensional Hector to the Hector
		 *             difference of itself with the specified two-dimensional
		 *             Hector.
		 * \param      Hector          The two-dimensional Hector with which the
		 *                             Hector difference should be computed.
		 * \return     A reference to this two-dimensional Hector.
		 */
		inline Hector2 &operator-=(const Hector2 &Hector)
		{
			x_ -= Hector.x();
			y_ -= Hector.y();

			return *this;
		}

		inline Hector2 &operator=(const Hector2 &Hector)
		{
			x_ = Hector.x();
			y_ = Hector.y();

			return *this;
		}

	private:
		float x_;
		float y_;
	};

	/**
	 * \relates    Hector2
	 * \brief      Computes the scalar multiplication of the specified
	 *             two-dimensional Hector with the specified scalar value.
	 * \param      s               The scalar value with which the scalar
	 *                             multiplication should be computed.
	 * \param      Hector          The two-dimensional Hector with which the scalar
	 *                             multiplication should be computed.
	 * \return     The scalar multiplication of the two-dimensional Hector with the
	 *             scalar value.
	 */
	inline Hector2 operator*(float s, const Hector2 &Hector)
	{
		return Hector2(s * Hector.x(), s * Hector.y());
	}

	/**
	 * \relates    Hector2
	 * \brief      Inserts the specified two-dimensional Hector into the specified
	 *             output stream.
	 * \param      os              The output stream into which the two-dimensional
	 *                             Hector should be inserted.
	 * \param      Hector          The two-dimensional Hector which to insert into
	 *                             the output stream.
	 * \return     A reference to the output stream.
	 */
	inline std::ostream &operator<<(std::ostream &os, const Hector2 &Hector)
	{
		os << "(" << Hector.x() << "," << Hector.y() << ")";

		return os;
	}

	/**
	 * \relates    Hector2
	 * \brief      Computes the length of a specified two-dimensional Hector.
	 * \param      Hector          The two-dimensional Hector whose length is to be
	 *                             computed.
	 * \return     The length of the two-dimensional Hector.
	 */
	inline float abs(const Hector2 &Hector)
	{
		return std::sqrt(Hector * Hector);
	}

	/**
	 * \relates    Hector2
	 * \brief      Computes the squared length of a specified two-dimensional
	 *             Hector.
	 * \param      Hector          The two-dimensional Hector whose squared length
	 *                             is to be computed.
	 * \return     The squared length of the two-dimensional Hector.
	 */
	inline float absSq(const Hector2 &Hector)
	{
		return Hector * Hector;
	}

	/**
	 * \relates    Hector2
	 * \brief      Computes the determinant of a two-dimensional square matrix with
	 *             rows consisting of the specified two-dimensional Hectors.
	 * \param      Hector1         The top row of the two-dimensional square
	 *                             matrix.
	 * \param      Hector2         The bottom row of the two-dimensional square
	 *                             matrix.
	 * \return     The determinant of the two-dimensional square matrix.
	 */
	inline float det(const Hector2 &Hector1, const Hector2 &Hector2)
	{
		return Hector1.x() * Hector2.y() - Hector1.y() * Hector2.x();
	}

	/**
	 * \relates    Hector2
	 * \brief      Computes the normalization of the specified two-dimensional
	 *             Hector.
	 * \param      Hector          The two-dimensional Hector whose normalization
	 *                             is to be computed.
	 * \return     The normalization of the two-dimensional Hector.
	 */
	inline Hector2 normalize(const Hector2 &Hector)
	{
		return Hector / abs(Hector);
	}
}

#endif /* RVO_Hector2_H_ */
