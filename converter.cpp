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

/** \file converter.cpp
 *  A sort of test file for the converter. It tries to test 
 *  the converter functions, put the mxArray into the
 *  matlab engine and check if the original and converted images are
 *  the same.
 *  When something fails, the resulting error message should help
 *  to quickly determine what went wrong and fix it.
 *
 *  \author <a href="mailto:e.koenigs@stud.uni-heidelberg.de">
 *	  Eric Koenigs</a>
 */

#include "converter.hxx"
#include "engine.h"
#include <iostream>

/** Tests the converter.
 */
int main() {
	// Testing image open.
	std::cout 	<< "Trying to create CImg from CImgLogo.jpg..."
				<< std::endl;
	cimg_library::CImg<double> img("../CImgLogo.jpg");
	
	std::cout 	<< "OK"
				<< std::endl;
	

	// Testing conversion CImg -> mxArray
	std::cout	<< "Trying to convert CImg to mxArray..."
				<< std::endl;
	mxArray* mxA = CImg2mx<double>(img);
	std::cout	<< "OK"
				<< std::endl;

	// Testing Matlab engine.
	std::cout	<< "Trying to open Matlab engine..."
				<< std::endl;
	Engine* ep;
	ep = engOpen("");

	if (ep == 0) {
		std::cout	<< "Error: Cannot open Matlab engine. Aborting..."
					<< std::endl;
		exit(0);
	};
	std::cout	<< "OK"
				<< std::endl;


	// Testing API with image.
	std::cout	<< "Trying to put the converted image into Matlab..."
				<< std::endl;

	if (engPutVariable(ep, "CImgLogo", mxA) != 0) {
		std::cout	<< "Error: engPutVariable() failed. Aborting..."
					<< std::endl;
		exit(0);
	};
	
	// If the above works, this SHOULD work too...
	if (engEvalString(ep, "save CImgLogo") != 0) {
		std::cout	<< "Error: Saving image in Matlab failed. Aborting..."
					<< std::endl;
		exit(0);
	};
	std::cout	<< "OK"
				<< std::endl;

	// Closing engine. Again, this SHOULD work if the above worked...
	std::cout	<< "Closing Matlab engine..."
				<< std::endl;
	if (engClose(ep) != 0) {

		std::cout	<< std::endl
					<< "Error: engClose() failed."
					<< std::endl;
		// We can continue despite this.
		std::cout	<< "Not critical. Moving on..."
					<< std::endl;
	}
	else {
		std::cout	<< "OK"
					<< std::endl;
	};
	


	// Testing reverse conversion
	std::cout	<< "Trying reverse conversion..."
				<< std::endl;
	cimg_library::CImg<double> img2;
	img2 = mx2CImg<double>(mxA);
	double* data = img.data();
	double* data2 = img2.data();

	unsigned int errors = 0;
	for (unsigned int i = 0; i < img.size(); i++) {
		if (data[i] != data2[i]) {
			errors++;
		};
	};
	if (errors > 0) {
			std::cout	<< "Error: Original image and reconverted "
								"image don't match. Found "
						<< errors
						<< " errors. Aborting..."
						<< std::endl;
			exit(0);
	}


	std::cout	<< "OK"
				<< std::endl;

	
	/*
	// Testing CImg_Display.
	std::cout	<< "Trying to display images..."
				<< std::endl;

	cimg_library::CImgDisplay   img_disp(img, "Before conversion"),
								img2_disp(img2, "After reverse conversion");

	while(!img_disp.is_closed() && !img2_disp.is_closed()) {
		img_disp.wait();
	};
	std::cout	<< "OK"
				<< std::endl;
	//*/

	std::cout	<< "Everything seems fine. Cleaning up..."
				<< std::endl;
	mxDestroyArray(mxA);

	return 1;
}
