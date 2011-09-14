#include "converter.hxx"
#include "engine.h"

/* Tests the converter.
 */
int main() {
    // Testing image open.
    std::cout << "Trying to create CImg from CImgLogo.jpg..." << std::endl;
    cimg_library::CImg<double> img("../CImgLogo.jpg");
    std::cout << "OK" << std::endl;
    

    // Testing conversion CImg -> mxArray
    std::cout << "Trying to convert CImg to mxArray..." << std::endl;
    mxArray* mxA = CImgToMx<double>(img);
    std::cout << "OK" << std::endl;
    

    // Testing Matlab engine.
    std::cout << "Trying to open Matlab engine..." << std::endl;
    Engine* ep;
    ep = engOpen("");

    if (ep == 0) {
        std::cout << "Error: Cannot open Matlab engine. Aborting..." << std::endl;
        exit(0);
    };
    std::cout << "OK" << std::endl;


    // Testing API with image.
    std::cout << "Trying to put the converted image into Matlab..." << std::endl;

    if (engPutVariable(ep, "CImgLogo", mxA) != 0) {
        std::cout << "Error: engPutVariable() failed. Aborting..." << std::endl;
        exit(0);
    };
    
    // If the above works, this SHOULD work too...
    if (engEvalString(ep, "save CImgLogo") != 0) {
        std::cout << "Error: Saving image in Matlab failed. Aborting..." << std::endl;
        exit(0);
    };
    std::cout << "OK" << std::endl;

    // Closing engine. Again, this SHOULD work if the above worked...
    std::cout << "Closing Matlab engine..." << std::endl;
    if (engClose(ep) != 0) {

        std::cout << std::endl << "Error: engClose() failed." << std::endl;
        // We can continue despite this.
        std::cout << "Not critical. Moving on..." << std::endl;
    }
    else {
        std::cout << "OK" << std::endl;
    };
    


    // Testing reverse conversion
    std::cout << "Trying reverse conversion..." << std::endl;
    cimg_library::CImg<double> img2;
    img2 = MxToCImg<double>(mxA);
    double* data = img.data();
    double* data2 = img2.data();

    unsigned int errors = 0;
    for (int i = 0; i < img.size(); i++) {
        if (data[i] != data2[i]) {
            errors++;
        };
    };
    if (errors > 0) {
            std::cout << "Error: Original image and reconverted image don't match. Found " << errors << " errors. Aborting..." << std::endl;
            exit(0);
    }


    std::cout << "OK" << std::endl;

    
    // Testing CImg_Display.
    std::cout << "Trying to display images..." << std::endl;

    cimg_library::CImgDisplay   img_disp(img, "Before conversion"),
                                img2_disp(img2, "After reverse conversion");

    while(!img_disp.is_closed() && !img2_disp.is_closed()) {
        img_disp.wait();
    };
    std::cout << "OK" << std::endl;

    std::cout << "Everything seems fine. Cleaning up..." << std::endl;
    mxDestroyArray(mxA);

    return 1;
};
