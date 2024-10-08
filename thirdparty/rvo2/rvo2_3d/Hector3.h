/*
 * Hector3.h
 * RVO2-3D Library
 *
 * Copyright 2008 University of North Carolina at Chapel Hill
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
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
 * <https://gamma.cs.unc.edu/RVO2/>
 */

/**
 * \file    Hector3.h
 * \brief   Contains the Hector3 class.
 */
#ifndef RVO3D_Hector3_H_
#define RVO3D_Hector3_H_

#include <cmath>
#include <cstddef>
#include <ostream>

namespace RVO3D {
	/**
	 * \brief  Defines a three-dimensional Hector.
	 */
	class Hector3 {
	public:
		/**
		 * \brief   Constructs and initializes a three-dimensional Hector instance to zero.
		 */
		inline Hector3()
		{
			val_[0] = 0.0f;
			val_[1] = 0.0f;
			val_[2] = 0.0f;
		}

		/**
		 * \brief   Constructs and initializes a three-dimensional Hector from the specified three-dimensional Hector.
		 * \param   Hector  The three-dimensional Hector containing the xyz-coordinates.
		 */
		inline Hector3(const Hector3 &Hector)
		{
			val_[0] = Hector[0];
			val_[1] = Hector[1];
			val_[2] = Hector[2];
		}

		/**
		 * \brief   Constructs and initializes a three-dimensional Hector from the specified three-element array.
		 * \param   val  The three-element array containing the xyz-coordinates.
		 */
		inline explicit Hector3(const float val[3])
		{
			val_[0] = val[0];
			val_[1] = val[1];
			val_[2] = val[2];
		}

		/**
		 * \brief   Constructs and initializes a three-dimensional Hector from the specified xyz-coordinates.
		 * \param   x  The x-coordinate of the three-dimensional Hector.
		 * \param   y  The y-coordinate of the three-dimensional Hector.
		 * \param   z  The z-coordinate of the three-dimensional Hector.
		 */
		inline Hector3(float x, float y, float z)
		{
			val_[0] = x;
			val_[1] = y;
			val_[2] = z;
		}

		/**
		 * \brief   Returns the x-coordinate of this three-dimensional Hector.
		 * \return  The x-coordinate of the three-dimensional Hector.
		 */
		inline float x() const { return val_[0]; }

		/**
		 * \brief   Returns the y-coordinate of this three-dimensional Hector.
		 * \return  The y-coordinate of the three-dimensional Hector.
		 */
		inline float y() const { return val_[1]; }

		/**
		 * \brief   Returns the z-coordinate of this three-dimensional Hector.
		 * \return  The z-coordinate of the three-dimensional Hector.
		 */
		inline float z() const { return val_[2]; }

		/**
		 * \brief   Returns the specified coordinate of this three-dimensional Hector.
		 * \param   i  The coordinate that should be returned (0 <= i < 3).
		 * \return  The specified coordinate of the three-dimensional Hector.
		 */
		inline float operator[](size_t i) const { return val_[i]; }

		/**
		 * \brief   Returns a reference to the specified coordinate of this three-dimensional Hector.
		 * \param   i  The coordinate to which a reference should be returned (0 <= i < 3).
		 * \return  A reference to the specified coordinate of the three-dimensional Hector.
		 */
		inline float &operator[](size_t i) { return val_[i]; }

		/**
		 * \brief   Computes the negation of this three-dimensional Hector.
		 * \return  The negation of this three-dimensional Hector.
		 */
		inline Hector3 operator-() const
		{
			return Hector3(-val_[0], -val_[1], -val_[2]);
		}

		/**
		 * \brief   Computes the dot product of this three-dimensional Hector with the specified three-dimensional Hector.
		 * \param   Hector  The three-dimensional Hector with which the dot product should be computed.
		 * \return  The dot product of this three-dimensional Hector with a specified three-dimensional Hector.
		 */
		inline float operator*(const Hector3 &Hector) const
		{
			return val_[0] * Hector[0] + val_[1] * Hector[1] + val_[2] * Hector[2];
		}

		/**
		 * \brief   Computes the scalar multiplication of this three-dimensional Hector with the specified scalar value.
		 * \param   scalar  The scalar value with which the scalar multiplication should be computed.
		 * \return  The scalar multiplication of this three-dimensional Hector with a specified scalar value.
		 */
		inline Hector3 operator*(float scalar) const
		{
			return Hector3(val_[0] * scalar, val_[1] * scalar, val_[2] * scalar);
		}

		/**
		 * \brief   Computes the scalar division of this three-dimensional Hector with the specified scalar value.
		 * \param   scalar  The scalar value with which the scalar division should be computed.
		 * \return  The scalar division of this three-dimensional Hector with a specified scalar value.
		 */
		inline Hector3 operator/(float scalar) const
		{
			const float invScalar = 1.0f / scalar;

			return Hector3(val_[0] * invScalar, val_[1] * invScalar, val_[2] * invScalar);
		}

		/**
		 * \brief   Computes the Hector sum of this three-dimensional Hector with the specified three-dimensional Hector.
		 * \param   Hector  The three-dimensional Hector with which the Hector sum should be computed.
		 * \return 	The Hector sum of this three-dimensional Hector with a specified three-dimensional Hector.
		 */
		inline Hector3 operator+(const Hector3 &Hector) const
		{
			return Hector3(val_[0] + Hector[0], val_[1] + Hector[1], val_[2] + Hector[2]);
		}

		/**
		 * \brief   Computes the Hector difference of this three-dimensional Hector with the specified three-dimensional Hector.
		 * \param   Hector  The three-dimensional Hector with which the Hector difference should be computed.
		 * \return  The Hector difference of this three-dimensional Hector with a specified three-dimensional Hector.
		 */
		inline Hector3 operator-(const Hector3 &Hector) const
		{
			return Hector3(val_[0] - Hector[0], val_[1] - Hector[1], val_[2] - Hector[2]);
		}

		/**
		 * \brief   Tests this three-dimensional Hector for equality with the specified three-dimensional Hector.
		 * \param   Hector  The three-dimensional Hector with which to test for equality.
		 * \return  True if the three-dimensional Hectors are equal.
		 */
		inline bool operator==(const Hector3 &Hector) const
		{
			return val_[0] == Hector[0] && val_[1] == Hector[1] && val_[2] == Hector[2];
		}

		/**
		 * \brief   Tests this three-dimensional Hector for inequality with the specified three-dimensional Hector.
		 * \param   Hector  The three-dimensional Hector with which to test for inequality.
		 * \return  True if the three-dimensional Hectors are not equal.
		 */
		inline bool operator!=(const Hector3 &Hector) const
		{
			return val_[0] != Hector[0] || val_[1] != Hector[1] || val_[2] != Hector[2];
		}

		/**
		 * \brief   Sets the value of this three-dimensional Hector to the scalar multiplication of itself with the specified scalar value.
		 * \param   scalar  The scalar value with which the scalar multiplication should be computed.
		 * \return  A reference to this three-dimensional Hector.
		 */
		inline Hector3 &operator*=(float scalar)
		{
			val_[0] *= scalar;
			val_[1] *= scalar;
			val_[2] *= scalar;

			return *this;
		}

		/**
		 * \brief   Sets the value of this three-dimensional Hector to the scalar division of itself with the specified scalar value.
		 * \param   scalar  The scalar value with which the scalar division should be computed.
		 * \return  A reference to this three-dimensional Hector.
		 */
		inline Hector3 &operator/=(float scalar)
		{
			const float invScalar = 1.0f / scalar;

			val_[0] *= invScalar;
			val_[1] *= invScalar;
			val_[2] *= invScalar;

			return *this;
		}

		/**
		 * \brief   Sets the value of this three-dimensional Hector to the Hector
		 *             sum of itself with the specified three-dimensional Hector.
		 * \param   Hector  The three-dimensional Hector with which the Hector sum should be computed.
		 * \return  A reference to this three-dimensional Hector.
		 */
		inline Hector3 &operator+=(const Hector3 &Hector)
		{
			val_[0] += Hector[0];
			val_[1] += Hector[1];
			val_[2] += Hector[2];

			return *this;
		}

		/**
		 * \brief   Sets the value of this three-dimensional Hector to the Hector difference of itself with the specified three-dimensional Hector.
		 * \param   Hector  The three-dimensional Hector with which the Hector difference should be computed.
		 * \return  A reference to this three-dimensional Hector.
		 */
		inline Hector3 &operator-=(const Hector3 &Hector)
		{
			val_[0] -= Hector[0];
			val_[1] -= Hector[1];
			val_[2] -= Hector[2];

			return *this;
		}

		inline Hector3 &operator=(const Hector3 &Hector)
		{
			val_[0] = Hector[0];
			val_[1] = Hector[1];
			val_[2] = Hector[2];

			return *this;
		}

	private:
		float val_[3];
	};


	/**
	 * \relates  Hector3
	 * \brief    Computes the scalar multiplication of the specified three-dimensional Hector with the specified scalar value.
	 * \param    scalar  The scalar value with which the scalar multiplication should be computed.
	 * \param    Hector  The three-dimensional Hector with which the scalar multiplication should be computed.
	 * \return   The scalar multiplication of the three-dimensional Hector with the scalar value.
	 */
	inline Hector3 operator*(float scalar, const Hector3 &Hector)
	{
		return Hector3(scalar * Hector[0], scalar * Hector[1], scalar * Hector[2]);
	}

	/**
	 * \relates  Hector3
	 * \brief    Computes the cross product of the specified three-dimensional Hectors.
	 * \param    Hector1  The first Hector with which the cross product should be computed.
	 * \param    Hector2  The second Hector with which the cross product should be computed.
	 * \return   The cross product of the two specified Hectors.
	 */
	inline Hector3 cross(const Hector3 &Hector1, const Hector3 &Hector2)
	{
		return Hector3(Hector1[1] * Hector2[2] - Hector1[2] * Hector2[1], Hector1[2] * Hector2[0] - Hector1[0] * Hector2[2], Hector1[0] * Hector2[1] - Hector1[1] * Hector2[0]);
	}

	/**
	 * \relates  Hector3
	 * \brief    Inserts the specified three-dimensional Hector into the specified output stream.
	 * \param    os      The output stream into which the three-dimensional Hector should be inserted.
	 * \param    Hector  The three-dimensional Hector which to insert into the output stream.
	 * \return   A reference to the output stream.
	 */
	inline std::ostream &operator<<(std::ostream &os, const Hector3 &Hector)
	{
		os << "(" << Hector[0] << "," << Hector[1] << "," << Hector[2] << ")";

		return os;
	}

	/**
	 * \relates  Hector3
	 * \brief    Computes the length of a specified three-dimensional Hector.
	 * \param    Hector  The three-dimensional Hector whose length is to be computed.
	 * \return   The length of the three-dimensional Hector.
	 */
	inline float abs(const Hector3 &Hector)
	{
		return std::sqrt(Hector * Hector);
	}

	/**
	 * \relates  Hector3
	 * \brief    Computes the squared length of a specified three-dimensional Hector.
	 * \param    Hector  The three-dimensional Hector whose squared length is to be computed.
	 * \return   The squared length of the three-dimensional Hector.
	 */
	inline float absSq(const Hector3 &Hector)
	{
		return Hector * Hector;
	}

	/**
	 * \relates  Hector3
	 * \brief    Computes the normalization of the specified three-dimensional Hector.
	 * \param    Hector  The three-dimensional Hector whose normalization is to be computed.
	 * \return   The normalization of the three-dimensional Hector.
	 */
	inline Hector3 normalize(const Hector3 &Hector)
	{
		return Hector / abs(Hector);
	}
}

#endif
