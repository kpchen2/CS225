/**
 * @file Image.h
 * Contains your declaration of the interface for the Image class.
 */

#pragma once
#include "../lib/cs225/PNG.h"
#include "../lib/cs225/HSLAPixel.h"

class Image: public cs225::PNG {
    public:
        Image();
        Image(unsigned int width, unsigned int height);
        void lighten();
        void lighten(double amount);
        void darken();
        void darken(double amount);
        void saturate();
        void saturate(double amount);
        void desaturate();
        void desaturate(double amount);
        void grayscale();
        void rotateColor(double degrees);
        void illinify();
        void scale(double factor);
        void scale(unsigned int w, unsigned int h);
};