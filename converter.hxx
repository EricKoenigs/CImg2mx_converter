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

/** \file converter.hxx
 *  Implementation of the parameter functions CImgToMx, MxToCImg and
 *  getClassID.
 *
 *  \author <a href="mailto:e.koenigs@stud.uni-heidelberg.de">
 *		Eric Koenigs</a>
 */

#include "converter.h"

template <typename T>
mxArray* CImgToMx(
		cimg_library::CImg<T> img) {
	// Gets the correct matlab class ID for the type.
	mxClassID classid = getClassID<T>();
	
	// A CImg has 4 dimensions
	mwSize ndims = 4;

	// Matlab stores arrays Fortran-style (column wise), which means the
	// image will be transposed 90 degrees to the left. To have the
	// correct image in Matlab, it needs to be transposed to the right
	// first.
	img.transpose();

	// Gets the sizes of all 4 dimensions
	const mwSize dims[] = {
		img.width(),		// Dimension 1
		img.height(),	   // Dimension 2
		img.depth(),		// Dimension 3
		img.spectrum()	  // Dimension 4
	};

	// Create an unpopulated mxArray with the appropriate dimensions and
	// the correct type.
	mxArray* mxA;
	mxA = mxCreateNumericArray(
			ndims,
			dims,
			classid,
			mxREAL		  // Sets the complexity to real.
			);

	// Gets the data pointers of the CImg and the mxArray
	void* pointer = mxGetData(mxA);
	T* data = img.data();

	// MAGIC. A WIZARD DID IT.
	memcpy(pointer, data, img.size()*sizeof(T));

	return mxA;
}


template <typename T>
mxArray* CImgListToMx (
		cimg_library::CImgList<T>) {

	return 0;
}



template <typename T>
cimg_library::CImg<T> MxToCImg(
		const mxArray* mxA) {
	// Check if the types match.
	mxClassID mxID = mxGetClassID(mxA);
	mxClassID imgID = getClassID<T>();
	if (mxID != imgID) {
		throw std::invalid_argument("Conversion failed. "
"The types of the mxArray and the requested CImg don't match.");
	};

	// Check if the mxArray has the correct number of dimensions.
	mwSize ndims = mxGetNumberOfDimensions(mxA);
	if (ndims != 4) {
		throw std::invalid_argument("Conversion failed. The supplied mxArray does not have the correct number of dimensions. Expected 4.");
	};

	// Gets the size of the dimensions from the mxArray.
	const mwSize* dims;
	dims = mxGetDimensions(mxA);

	// Assigns the dimension 
	const unsigned int width = dims[0];
	const unsigned int height = dims[1];
	const unsigned int depth = dims[2];
	const unsigned int spectrum = dims[3];

	// Creates the image.
	cimg_library::CImg<T> img(width, height, depth, spectrum);

	// Gets the data pointers.
	void* pointer = mxGetData(mxA);
	T* data = img.data();

	// MORE MAGIC.
	memcpy(data, pointer, img.size()*sizeof(T));

	// Transposes the image back.
	// See CImgToMx for explanation.
	img.transpose();

	return img;
}



template <>
mxClassID getClassID<int> () {
	return mxINT32_CLASS;
}
template <>
mxClassID getClassID<float> () {
	return mxSINGLE_CLASS;
}
template <>
mxClassID getClassID<double> () {
	return mxDOUBLE_CLASS;
}



