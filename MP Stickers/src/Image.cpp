#include "Image.h"
#include "math.h"
#include "../lib/cs225/PNG.h"

Image::Image() {
    resize(0,0);
}

Image::Image(unsigned int width, unsigned int height) {
    resize(width,height);
}

void Image::lighten() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).l >= 0.9) {
                getPixel(i,j).l = 1.0;
            } else {
                getPixel(i,j).l += 0.1;
            }
        }
    }
}

void Image::lighten(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).l >= 1.0-amount) {
                getPixel(i,j).l = 1.0;
            } else {
                getPixel(i,j).l += amount;
            }
        }
    }
}

void Image::darken() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).l <= 0.1) {
                getPixel(i,j).l = 0.0;
            } else {
                getPixel(i,j).l -= 0.1;
            }
        }
    }
}

void Image::darken(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).l <= amount) {
                getPixel(i,j).l = 0.0;
            } else {
                getPixel(i,j).l -= amount;
            }
        }
    }
}

void Image::saturate() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).s >= 0.9) {
                getPixel(i,j).s = 1.0;
            } else {
                getPixel(i,j).s += 0.1;
            }
        }
    }
}

void Image::saturate(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).s >= 1.0-amount) {
                getPixel(i,j).s = 1.0;
            } else {
                getPixel(i,j).s += amount;
            }
        }
    }
}

void Image::desaturate() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).s <= 0.1) {
                getPixel(i,j).s = 0.0;
            } else {
                getPixel(i,j).s -= 0.1;
            }
        }
    }
}

void Image::desaturate(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).s <= amount) {
                getPixel(i,j).s = 0.0;
            } else {
                getPixel(i,j).s -= amount;
            }
        }
    }
}

void Image::grayscale() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            getPixel(i,j).s = 0;
        }
    }
}

void Image::rotateColor(double degrees) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).h >= 360.0-degrees) {
                getPixel(i,j).h += degrees-360.0;
            } else if (getPixel(i,j).h + degrees <= 0) {
                getPixel(i,j).h += 360.0+degrees;
            } else {
                getPixel(i,j).h += degrees;
            }
        }
    }
}

void Image::illinify() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            if (getPixel(i,j).h >= 113.5 && getPixel(i,j).h <= 293.5) {
                getPixel(i,j).h = 216;
            } else {
                getPixel(i,j).h = 11;
            }
        }
    }
}

void Image::scale(double factor) {
    Image im = *this;
    resize(factor*width(), factor*height());

    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            getPixel(i,j) = im.getPixel(i*im.width()/width(), j*im.height()/height());
        }
    }
}

void Image::scale(unsigned w, unsigned h) {
    double ratio = (double)(width())/(double)(height());

    if ((double)(w)/(double)(h) > ratio) {
        scale((double)(h)/height());
    } else {
        scale((double)w/width());
    }
}