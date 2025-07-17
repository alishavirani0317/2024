#include "Image.h"

void Image::lighten() {
    lighten(0.1);
}

void Image::lighten(double amount) {
    // iterate over colums 
    for (unsigned x = 0; x < width(); ++x) {
        //iterate over rows
        for (unsigned int y = 0; y < height(); ++y) {
            // //get pixel at (x,y)
            // cs225::HSLAPixel & pixel = getPixel(x,y);
            //increase luminance amt
            getPixel(x,y).l += amount;
            //make sure luminance isnt over 1.0
            if (getPixel(x,y).l > 1.0) {
                getPixel(x,y).l = 1.0;
            }
        }
    }

}

void Image::darken() {
    darken(0.1);
}

void Image::darken(double amount) {
    //iterate over cols
    for (unsigned x = 0; x < width(); ++x) {
        //iterate over rows
        for (unsigned y = 0; y < height(); ++y) {
            //get pix at (x,y)
            // HSLAPixel & pixel = getPixel(x,y);
            //decrease luminance by amount
            getPixel(x,y).l -= amount;
            //making sure doesnt go < 0.0
            if (getPixel(x,y).l < 0.0) {
                getPixel(x,y).l = 0.0;
            }
        }
    }
}

void Image::saturate() {
    saturate(0.1);

}

void Image::saturate(double amount) {
    for(unsigned x = 0; x < width(); ++x) {
        for (unsigned y = 0; y < height(); ++y) {
            // HSLAPixel & pixel = getPixel(x,y);
            getPixel(x,y).s += amount;
            if (getPixel(x,y).s > 1.0) {
                getPixel(x,y).s = 1.0;
            }
        }
    }
}

void Image::desaturate() {
    desaturate(0.1);
}

void Image::desaturate(double amount) {
    for(unsigned x = 0; x < width(); ++x) {
        for (unsigned y = 0; y < height(); ++y) {
            // HSLAPixel & pixel = getPixel(x,y);
            getPixel(x,y).s -= amount;
            if (getPixel(x,y).s < 0.0) {
                getPixel(x,y).s = 0.0;
            }
        }
    }
}

void Image::grayscale() {
    for (unsigned x = 0; x < width(); ++x) {
        for (unsigned y = 0; y < height(); ++y) {
            // HSLAPixel & pixel = getPixel(x,y);
            getPixel(x,y).s = 0;
        }
    }
}

void Image::rotateColor(double degrees) {
    for (unsigned x = 0; x < width(); ++x) {
        for (unsigned y = 0; y < height(); ++y) {
            // HSLAPixel & pixel = getPixel(x,y);
            getPixel(x,y).h += degrees;
            if (getPixel(x,y).h> 360) {
                getPixel(x,y).h -= 360;
            }
            if (getPixel(x,y).h < 0) {
                getPixel(x,y).h += 360;
            }
        }
    }
}

void Image::illinify() {
    const double illiniOr = 11.0;
    const double illiniBl = 216.0;
    for (unsigned x = 0; x < width(); ++x) {
        for (unsigned y = 0; y < height(); ++y) {
            // HSLAPixel & pixel = getPixel(x,y);
            double currHue = getPixel(x,y).h;
            double distanceOr = currHue - illiniOr;
            //shortest distance to illini orange on hue
            if (distanceOr < 0) {
                //change to get positive distanec
                distanceOr += 360;
            } 
            if (distanceOr > 180) {
                //shortest distance 
                distanceOr -= 360; 
            }
            double distanceBl = currHue - illiniBl;
            if (distanceBl < 0) {
                distanceBl += 360;
            } 
            if (distanceBl > 180) {
                distanceBl -= 360; 
            }
            if (distanceOr < distanceBl) {
                getPixel(x,y).h = illiniOr;
            } else {
                getPixel(x,y).h = illiniBl;
            }
        }
    }
}

void  Image::scale(double factor) {
    //get new width and height based on scale factor
    // what is the type of newWidth and newHeight supposed to be
    // std::cout << width() * factor << std::endl
    unsigned newWidth = static_cast<unsigned>(width() * factor);
    // std::cout << newWidth << std::endl
    unsigned newHeight = static_cast <unsigned>(height() * factor);
    //make an image w new dimensions
    Image newImage(newWidth, newHeight);
    //scale each pic to new size
    for (unsigned x = 0; x < newWidth; ++x) {
        for (unsigned y = 0; y < newHeight; ++y) {
             unsigned oldX = static_cast<unsigned>(x / factor);
            unsigned oldY = static_cast<unsigned>(y / factor);
            newImage.getPixel(x,y) = getPixel(oldX, oldY);
        }
    }
    *this = newImage;

}

void  Image::scale(unsigned w, unsigned h) {
    double wFactor = static_cast<double>(w);
    wFactor = wFactor / (double) width();
    double hFactor = static_cast<double>(h);
    hFactor =  hFactor / (double) height();
    double factor;
    //// Choose the smaller factor to ensure the image fits
    if (wFactor > hFactor) {
        factor = hFactor;
    } else {
        factor = wFactor;
    }
    scale(factor);
} 