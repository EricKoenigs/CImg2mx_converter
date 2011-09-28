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

/** \file converter.hxx
 *  Implementation of the parameter functions CImg2mx, mx2CImg and
 *  getClassID.
 *
 *  \author <a href="mailto:e.koenigs@stud.uni-heidelberg.de">
 *		Eric Koenigs</a>
 */

#include "converter.h"

template <typename T>
mxArray* CImg2mx(
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
		img.height(),		// Dimension 2
		img.depth(),		// Dimension 3
		img.spectrum()		// Dimension 4
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
cimg_library::CImg<T> mx2CImg(
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
	// See CImg2mx for explanation.
	img.transpose();

	return img;
}


/// Template implementation for int
template <>
mxClassID getClassID<int> () {
	return mxINT32_CLASS;
}
/// Template implementation for float
template <>
mxClassID getClassID<float> () {
	return mxSINGLE_CLASS;
}
/// Template implementation for double
template <>
mxClassID getClassID<double> () {
	return mxDOUBLE_CLASS;
}



