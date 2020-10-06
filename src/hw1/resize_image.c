#include <math.h>
#include "image.h"

float map_to_old_coordinate(float old_size, float new_size, float coord)
{
    float a = old_size / new_size;
    float b = -0.5 + a*0.5;
    return a*coord + b;
}

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    image new_image = make_image(w, h, im.c);
    // TODO 
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            for (int c = 0; c < im.c; c++) {
                float old_x = map_to_old_coordinate(im.w, w, x);
                float old_y = map_to_old_coordinate(im.h, h, y);
                float value = nn_interpolate(im, old_x, old_y, c);
                set_pixel(new_image, x, y, c, value);
            }
        }
    }
    return new_image;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float p1, p2, p3, p4, q1, q2;
    float x_floor = floor(x), x_ceil = ceil(x);
    float y_floor = floor(y), y_ceil = ceil(y);
    p1 = get_pixel(im, x_floor, y_floor, c);
    p2 = get_pixel(im, x_ceil, y_floor, c);
    p3 = get_pixel(im, x_floor, y_ceil, c);
    p4 = get_pixel(im, x_ceil, y_ceil, c);
    q1 = (y_ceil - y) * p1 + (y - y_floor) * p3;
    q2 = (y_ceil - y) * p2 + (y - y_floor) * p4;
    return (x_ceil - x) * q1 + (x - x_floor) * q2;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image new_image = make_image(w, h, im.c);
    // TODO 
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            for (int c = 0; c < im.c; c++) {
                float old_x = map_to_old_coordinate(im.w, w, x);
                float old_y = map_to_old_coordinate(im.h, h, y);
                float value = bilinear_interpolate(im, old_x, old_y, c);
                set_pixel(new_image, x, y, c, value);
            }
        }
    }
    return new_image;
}

