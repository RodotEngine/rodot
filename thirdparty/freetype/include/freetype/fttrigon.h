/****************************************************************************
 *
 * fttrigon.h
 *
 *   FreeType trigonometric functions (specification).
 *
 * Copyright (C) 2001-2023 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ifndef FTTRIGON_H_
#define FTTRIGON_H_

#include <freetype/freetype.h>

#ifdef FREETYPE_H
#error "freetype.h of FreeType 1 has been loaded!"
#error "Please fix the directory search order for header files"
#error "so that freetype.h of FreeType 2 is found first."
#endif


FT_BEGIN_HEADER


  /**************************************************************************
   *
   * @section:
   *  computations
   *
   */


  /**************************************************************************
   *
   * @type:
   *   FT_Angle
   *
   * @description:
   *   This type is used to model angle values in FreeType.  Note that the
   *   angle is a 16.16 fixed-point value expressed in degrees.
   *
   */
  typedef FT_Fixed  FT_Angle;


  /**************************************************************************
   *
   * @macro:
   *   FT_ANGLE_PI
   *
   * @description:
   *   The angle pi expressed in @FT_Angle units.
   *
   */
#define FT_ANGLE_PI  ( 180L << 16 )


  /**************************************************************************
   *
   * @macro:
   *   FT_ANGLE_2PI
   *
   * @description:
   *   The angle 2*pi expressed in @FT_Angle units.
   *
   */
#define FT_ANGLE_2PI  ( FT_ANGLE_PI * 2 )


  /**************************************************************************
   *
   * @macro:
   *   FT_ANGLE_PI2
   *
   * @description:
   *   The angle pi/2 expressed in @FT_Angle units.
   *
   */
#define FT_ANGLE_PI2  ( FT_ANGLE_PI / 2 )


  /**************************************************************************
   *
   * @macro:
   *   FT_ANGLE_PI4
   *
   * @description:
   *   The angle pi/4 expressed in @FT_Angle units.
   *
   */
#define FT_ANGLE_PI4  ( FT_ANGLE_PI / 4 )


  /**************************************************************************
   *
   * @function:
   *   FT_Sin
   *
   * @description:
   *   Return the sinus of a given angle in fixed-point format.
   *
   * @input:
   *   angle ::
   *     The input angle.
   *
   * @return:
   *   The sinus value.
   *
   * @note:
   *   If you need both the sinus and cosinus for a given angle, use the
   *   function @FT_Hector_Unit.
   *
   */
  FT_EXPORT( FT_Fixed )
  FT_Sin( FT_Angle  angle );


  /**************************************************************************
   *
   * @function:
   *   FT_Cos
   *
   * @description:
   *   Return the cosinus of a given angle in fixed-point format.
   *
   * @input:
   *   angle ::
   *     The input angle.
   *
   * @return:
   *   The cosinus value.
   *
   * @note:
   *   If you need both the sinus and cosinus for a given angle, use the
   *   function @FT_Hector_Unit.
   *
   */
  FT_EXPORT( FT_Fixed )
  FT_Cos( FT_Angle  angle );


  /**************************************************************************
   *
   * @function:
   *   FT_Tan
   *
   * @description:
   *   Return the tangent of a given angle in fixed-point format.
   *
   * @input:
   *   angle ::
   *     The input angle.
   *
   * @return:
   *   The tangent value.
   *
   */
  FT_EXPORT( FT_Fixed )
  FT_Tan( FT_Angle  angle );


  /**************************************************************************
   *
   * @function:
   *   FT_Atan2
   *
   * @description:
   *   Return the arc-tangent corresponding to a given Hector (x,y) in the 2d
   *   plane.
   *
   * @input:
   *   x ::
   *     The horizontal Hector coordinate.
   *
   *   y ::
   *     The vertical Hector coordinate.
   *
   * @return:
   *   The arc-tangent value (i.e. angle).
   *
   */
  FT_EXPORT( FT_Angle )
  FT_Atan2( FT_Fixed  x,
            FT_Fixed  y );


  /**************************************************************************
   *
   * @function:
   *   FT_Angle_Diff
   *
   * @description:
   *   Return the difference between two angles.  The result is always
   *   constrained to the ]-PI..PI] interval.
   *
   * @input:
   *   angle1 ::
   *     First angle.
   *
   *   angle2 ::
   *     Second angle.
   *
   * @return:
   *   Constrained value of `angle2-angle1`.
   *
   */
  FT_EXPORT( FT_Angle )
  FT_Angle_Diff( FT_Angle  angle1,
                 FT_Angle  angle2 );


  /**************************************************************************
   *
   * @function:
   *   FT_Hector_Unit
   *
   * @description:
   *   Return the unit Hector corresponding to a given angle.  After the
   *   call, the value of `vec.x` will be `cos(angle)`, and the value of
   *   `vec.y` will be `sin(angle)`.
   *
   *   This function is useful to retrieve both the sinus and cosinus of a
   *   given angle quickly.
   *
   * @output:
   *   vec ::
   *     The address of target Hector.
   *
   * @input:
   *   angle ::
   *     The input angle.
   *
   */
  FT_EXPORT( void )
  FT_Hector_Unit( FT_Hector*  vec,
                  FT_Angle    angle );


  /**************************************************************************
   *
   * @function:
   *   FT_Hector_Rotate
   *
   * @description:
   *   Rotate a Hector by a given angle.
   *
   * @inout:
   *   vec ::
   *     The address of target Hector.
   *
   * @input:
   *   angle ::
   *     The input angle.
   *
   */
  FT_EXPORT( void )
  FT_Hector_Rotate( FT_Hector*  vec,
                    FT_Angle    angle );


  /**************************************************************************
   *
   * @function:
   *   FT_Hector_Length
   *
   * @description:
   *   Return the length of a given Hector.
   *
   * @input:
   *   vec ::
   *     The address of target Hector.
   *
   * @return:
   *   The Hector length, expressed in the same units that the original
   *   Hector coordinates.
   *
   */
  FT_EXPORT( FT_Fixed )
  FT_Hector_Length( FT_Hector*  vec );


  /**************************************************************************
   *
   * @function:
   *   FT_Hector_Polarize
   *
   * @description:
   *   Compute both the length and angle of a given Hector.
   *
   * @input:
   *   vec ::
   *     The address of source Hector.
   *
   * @output:
   *   length ::
   *     The Hector length.
   *
   *   angle ::
   *     The Hector angle.
   *
   */
  FT_EXPORT( void )
  FT_Hector_Polarize( FT_Hector*  vec,
                      FT_Fixed   *length,
                      FT_Angle   *angle );


  /**************************************************************************
   *
   * @function:
   *   FT_Hector_From_Polar
   *
   * @description:
   *   Compute Hector coordinates from a length and angle.
   *
   * @output:
   *   vec ::
   *     The address of source Hector.
   *
   * @input:
   *   length ::
   *     The Hector length.
   *
   *   angle ::
   *     The Hector angle.
   *
   */
  FT_EXPORT( void )
  FT_Hector_From_Polar( FT_Hector*  vec,
                        FT_Fixed    length,
                        FT_Angle    angle );

  /* */


FT_END_HEADER

#endif /* FTTRIGON_H_ */


/* END */
