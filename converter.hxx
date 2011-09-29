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
mxArray* CImgList2mx(
		cimg_library::CImgList<T> list) {
	// Sets the appropriate dimensions for the cell matrix
	mwSize matrixSizeM = 1;				// We only need 1 dimension.
	mwSize matrixSizeN = list.size();

	// Create the cell matrix.
	mxArray* mxCell = mxCreateCellMatrix(
			matrixSizeM,
			matrixSizeN
			);
	
	// Stop. Conversion time.
	mxArray* mxTmp;
	cimg_library::CImg<T> imgTmp;
	for (unsigned int i = 0; i < list.size(); i++) {
		imgTmp = list[i];
		mxTmp = CImg2mx<T>(imgTmp);

		// Matlab indices start at 1, not 0!
		mxSetCell(mxCell, i + 1, mxTmp);

		// Clean up before the next iteration,
		// we don't want nasty memory leaks.
		// mxDestroyArray(mxTmp);
		// Actually I'm not sure about this.
		// Does mxSetCell make a deep copy or rely on the pointer?
		// Risking memory leaks for now.
	};

	return mxCell;
};




template <typename T>
cimg_library::CImg<T> mx2CImg(
		const mxArray* mxA) {
	// Checks if mxA is a numeric array.
	if (!mxIsNumeric(mxA)) {
		throw std::invalid_argument("Conversion failed. "
				"The supplied mxArray is not a numeric array.");
	};
	// Checks if the types match.
	mxClassID mxID = mxGetClassID(mxA);
	mxClassID imgID = getClassID<T>();
	if (mxID != imgID) {
		throw std::invalid_argument("Conversion failed. "
				"The types of the mxArray and the requested "
				"CImg don't match.");
	};

	// Checks if the mxArray has the correct number of dimensions.
	mwSize ndims = mxGetNumberOfDimensions(mxA);
	if (ndims > 4) {
		throw std::invalid_argument(
				"Conversion failed. The supplied mxArray has too "
				"many dimensions. Expected 4 or less.");

	};

	// Gets the size of the dimensions from the mxArray.
	const mwSize* dims;
	dims = mxGetDimensions(mxA);

	// Assigns the dimension 
	const unsigned int width = dims[0];
	const unsigned int height = dims[1];
	// Checks if the image has additional dimensions beyond 2,
	// and sets them to 1 if they don't exist, or to the correct
	// value attained from dims[].
	unsigned int depth;
	unsigned int spectrum;
	if (ndims > 2) {
		depth = dims[2];
	}
	else {
		depth = 1;
	};
	if (ndims > 3) {
		spectrum = dims[3];
	}
	else {
		spectrum = 1;
	};

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



