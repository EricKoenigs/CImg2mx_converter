/* Copyright (C) 2011 Eric Koenigs
 *
 * This program is free software under the terms of
 * the Do What The Fuck You Want To Public License,
 * and is thus GPL-compatible. It comes with
 * the usual disclaimer about coming without any warranty
 * as well as other legal yadda yadda.
 *
 * See COPYING.txt for a copy of the license, or
 * head to http://sam.zoy.org/wtfpl/COPYING if the
 * file is missing for whatever reason.
 */

/** \file converter.h
 *  Declaration of the parameter functions CImgToMx, MxToCImg and
 *  getClassID.
 *
 *  \author <a href="mailto:e.koenigs@stud.uni-heidelberg.de">
 *	  Eric Koenigs</a>
 */

// CImg needs these to use libjpeg and libpng
#ifndef cimg_use_jpeg
#define cimg_use_jpeg
#endif
#ifndef cimg_use_png
#define cimg_use_png
#endif
#include "CImg.h"
#include "matrix.h"  
#include <stdexcept>


/// CImg to mxArray converter function
/** This function takes a CImg and tries to convert it into a
 *  mxArray of the appropriate type. See getClassID() for all
 *  supported types.
 *
 *  \see getClassID()
 *
 *  \param img A CImg<T> to be converted.
 *
 *  \return A pointer to the converted 4-dimensional mxArray.
 */
template <typename T>
mxArray* CImg2mx (
		cimg_library::CImg<T> img);

/// mxArray to CImg converter function
/** This function takes a mxArray and tries to convert it into a
 *  CImg of the appropriate type. See getClassID() for all
 *  supported types.
 *
 *  It throws invalid_argument errors when the types of the CImg
 *  and the mxArray don't match or if the mxArray doesn't have the
 *  correct amount of dimensions.
 *
 *  \see getClassID()
 *
 *  \param mxA A 4-dimensional mxArray of the appropriate type.
 *
 *  \return The converted CImg<T>.
 */
template <typename T>
cimg_library::CImg<T> mx2CImg (
		const mxArray* mxA);




/// Gets the Class identifier.
/** This template function returns the mxClassID for the template
 *  type. Currently, it's only defined for the following types:
 *
 *	  int		<-->	mxINT32_CLASS<br>
 *	  float		<-->	mxSINGLE_CLASS<br>
 *	  double	<-->	mxDOUBLE_CLASS<br>
 *
 *  \see CImg2mx()
 *  \see mx2CImg()
 *
 *  \return The mxClassID for the template type.
 */
template <typename T>
mxClassID getClassID ();
