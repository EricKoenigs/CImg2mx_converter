/* Copyright (C) 2011 Eric Koenigs
 *
 * This program is free software under the terms of
 * the WTFPL v2, and thus GPL-compatible.
 *
 *		   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *				   Version 2, December 2004
 * 
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 * 
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *  
 *			DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *  
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

/** \file converter.h
 *  Declaration of the parameter functions CImgToMx, MxToCImg and
 *  getClassID.
 *
 *  \author <a href="mailto:e.koenigs@stud.uni-heidelberg.de">
 *	  Eric Koenigs</a>
 */

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
mxArray* CImgToMx (
		cimg_library::CImg<T> img);

template <typename T>
mxArray* CImgListToMx (
		cimg_library::CImgList<T> list);


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
cimg_library::CImg<T> MxToCImg (
		const mxArray* mxA);


/// Gets the Class identifier.
/** This template function returns the mxClassID for the template
 *  type. Currently, it's only defined for the following types:
 *
 *	  int		<-->	mxINT32_CLASS<br>
 *	  float		<-->	mxSINGLE_CLASS<br>
 *	  double	<-->	mxDOUBLE_CLASS<br>
 *
 *  \see CImgToMx()
 *  \see MxToCImg()
 *
 *  \return The mxClassID for the template type.
 */
template <typename T>
mxClassID getClassID ();
