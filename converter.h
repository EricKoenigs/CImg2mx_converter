#include "CImg.h"
#include "matrix.h"  
#include <stdexcept>
#include <iostream>

template <typename T>
mxArray* CImgToMx (
        cimg_library::CImg<T> img);

template <typename T>
mxClassID getClassID ();


template <typename T>
cimg_library::CImg<T> MxToCImg (
        const mxArray* mx);
