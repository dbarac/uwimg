#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    if (x >= im.w)
        x = im.w - 1;
    if (x < 0)
        x = 0;
    if (y >= im.h)
        y = im.h - 1;
    if (y < 0)
        y = 0;
    return *(im.data + c*im.w*im.h + y*im.w + x);
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if (x >= im.w || x < 0 || y >= im.h || y < 0)
        return;
    *(im.data + c*im.w*im.h + y*im.w + x) = v;    
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, sizeof(float)*im.c*im.w*im.h);
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            float red, green, blue;
            red = get_pixel(im, x, y, 0);
            green = get_pixel(im, x, y, 1);
            blue = get_pixel(im, x, y, 2);
            set_pixel(gray, x, y, 0, 0.299*red + 0.587*green + 0.114*blue);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            *(im.data + c*im.w*im.h + y*im.w + x) += v;
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            for (int c = 0; c < im.c; c++) {
                float val = get_pixel(im, x, y, c);
                val = (val > 1.0) ? 1.0 : val;
                val = (val < 0.0) ? 0.0 : val;
                set_pixel(im, x, y, c, val);
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

float max(float a, float b) {
    return (a > b) ? a : b;
}

float min(float a, float b) {
    return (a < b) ? a : b;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            float red, green, blue;
            red = get_pixel(im, x, y, 0);
            green = get_pixel(im, x, y, 1);
            blue = get_pixel(im, x, y, 2);
            
            float value, min, chroma, hue, saturation;
            value = three_way_max(red, green, blue);
            min = three_way_min(red, green, blue);
            chroma = value - min;
            if (value == 0.0) 
                saturation = 0.0;
            else
                saturation = chroma / value;
            
            if (chroma == 0.0) {
                hue = 0.0;
            } else if (value == red) {
                hue = (green - blue) / chroma;
            } else if (value == green) {
                hue = (blue - red) / chroma + 2;
            } else if (value == blue) {
                hue = (red - green) / chroma + 4;
            }
            hue /= 6;
            if (hue < 0)
                hue += 1;

            set_pixel(im, x, y, 0, hue);
            set_pixel(im, x, y, 1, saturation);
            set_pixel(im, x, y, 2, value);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            float value, hue, saturation;
            float red, green, blue;
            hue = get_pixel(im, x, y, 0);
            saturation = get_pixel(im, x, y, 1);
            value = get_pixel(im, x, y, 2);
            
            hue *= 6;
            float k_red = fmod(5 + hue, 6);
            float k_green = fmod(3 + hue, 6);
            float k_blue = fmod(1 + hue, 6);
            red = value - value*saturation*max(0, three_way_min(k_red, 4 - k_red, 1));
            green = value - value*saturation*max(0, three_way_min(k_green, 4 - k_green, 1));
            blue = value - value*saturation*max(0, three_way_min(k_blue, 4 - k_blue, 1));

            set_pixel(im, x, y, 0, red);
            set_pixel(im, x, y, 1, green);
            set_pixel(im, x, y, 2, blue); 
        }
    }
}

void scale_image(image im, int c, float v)
{
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            *(im.data + c*im.w*im.h + y*im.w + x) *= v;
        }
    }
}