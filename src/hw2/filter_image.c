#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float sum = 0;
    for (int i = 0; i < im.w*im.h*im.c; i++) {
        sum += im.data[i];
    }
    for (int i = 0; i < im.w*im.h*im.c; i++) {
        im.data[i] /= sum;
    }
}

image make_box_filter(int w)
{
    // TODO
    image box_filter = make_image(w, w, 1);
    for (int i = 0; i < box_filter.w*box_filter.h*box_filter.c; i++) {
        box_filter.data[i] = 1;
    }
    l1_normalize(box_filter);
    return box_filter;
}

//Calculate convolution result for a single pixel in an image.
//The result is equal to the weighted sum of the filter and the area surrounding
//the pixel. The filter is positioned so that (x,y,c) is in
//the center of the filter.
float convolve_pixel(image im, image filter, int x, int y, int c)
{
    float sum = 0;
    int filter_c = c;
    if (filter.c == 1) filter_c = 0;
    for (int i = 0; i < filter.w; i++) {
        for (int j = 0; j < filter.h; j++) {
            int dx = -filter.w/2 + i;
            int dy = -filter.h/2 + j;
            sum += get_pixel(im, x+dx, y+dy, c) * get_pixel(filter, i, j, filter_c);
        }
    }
    return sum;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == im.c || filter.c == 1);
    int new_c = 1;
    if (preserve)
        new_c = im.c;
    image filtered = make_image(im.w, im.h, new_c); //all pixels are set to 0

    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            for (int c = 0; c < im.c; c++) {
                float val = convolve_pixel(im, filter, x, y, c);
                if (preserve) {
                    set_pixel(filtered, x, y, c, val);
                } else {
                    float prev_channels_sum = get_pixel(filtered, x, y, 0);
                    set_pixel(filtered, x, y, 0, prev_channels_sum + val);
                }
            }
        }
    }
    return filtered;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {
        0, -1, 0,
        -1, 4, -1,
        0, -1, 0
    };
    for (int i = 0; i < 9; i++) {
        filter.data[i] = values[i];
    }
    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {
        0, -1, 0,
        -1, 5, -1,
        0, -1, 0
    };
    for (int i = 0; i < 9; i++) {
        filter.data[i] = values[i];
    }
    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {
        -2, -1, 0,
        -1, 1, 1,
        0, 1, 2
    };
    for (int i = 0; i < 9; i++) {
        filter.data[i] = values[i];
    }
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: Preserve should be used for sharpen and emboss. We should use it for high pass if we care for edge color.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: All of the filters need to be clamped. Aplying any of the filters can result in values outside of the 0-1 range.
//

image make_gaussian_filter(float sigma)
{
    // TODO
    int size = ceil(6 * sigma);
    if (size % 2 == 0)
        size++;
    int center = size / 2;
    image filter = make_image(size, size, 1);
    for (int x = -size/2; x <= size/2; x++) {
        for (int y = -size/2; y <= size/2; y++) {
            float val = 1 / (TWOPI*sigma*sigma) * exp(-(x*x + y*y) / (2*sigma*sigma));
            set_pixel(filter, center+x, center+y, 0, val);
        }
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image new = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w*a.h*a.c; i++) {
        new.data[i] = a.data[i] + b.data[i];
    }
    return new;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image new = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w*a.h*a.c; i++) {
        new.data[i] = a.data[i] - b.data[i];
    }
    return new;
}

image make_gx_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    for (int i = 0; i < 9; i++) {
        filter.data[i] = values[i];
    }
    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1
    };
    for (int i = 0; i < 9; i++) {
        filter.data[i] = values[i];
    }
    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float min = INFINITY, max = -INFINITY, range;
    for (int i = 0; i < im.w*im.h*im.c; i++) {
        if (im.data[i] > max)
            max = im.data[i];
        if (im.data[i] < min)
            min = im.data[i];
    }
    
    range = max - min;
    for (int i = 0; i < im.w*im.h*im.c; i++) {
        if (range == 0)
            im.data[i] = 0;
        else
            im.data[i] = (im.data[i] - min) / range;
    }
}

image *sobel_image(image im)
{
    // TODO
    image* sobel = calloc(2, sizeof(image));
    sobel[0] = convolve_image(im, make_gx_filter(), 0);
    sobel[1] = convolve_image(im, make_gy_filter(), 0);
    
    float magnitude, direction, gx, gy;
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            gx = get_pixel(sobel[0], x, y, 0);
            gy = get_pixel(sobel[1], x, y, 0);
            magnitude = sqrt(gx*gx + gy*gy);
            direction = atan2(gy, gx);
            set_pixel(sobel[0], x, y, 0, magnitude);
            set_pixel(sobel[1], x, y, 0, direction);
        }
    }
    return sobel;
}

image colorize_sobel(image im)
{
    // TODO
    image* sobel = sobel_image(im);
    feature_normalize(sobel[0]);
    feature_normalize(sobel[1]);
    float magnitude, direction;
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            magnitude = get_pixel(sobel[0], x, y, 0);
            direction = get_pixel(sobel[1], x, y, 0);
            set_pixel(im, x, y, 0, direction);
            set_pixel(im, x, y, 1, magnitude);
            set_pixel(im, x, y, 2, magnitude);
        }
    }
    hsv_to_rgb(im);
    return im;
}
