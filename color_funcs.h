//
// color_funcs.h -- color sliding
//
// 2012, Tod E. Kurt, http://todbot.com/blog/
//
//
// also see:
//   http://meyerweb.com/eric/tools/color-blend/ 
//
//

#ifndef RGB_FUNCS_H
#define RGB_FUNCS_H

#include <stdint.h>

// RGB triplet of 16-bit vals for input/output use
typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} rgbuint16_t;

// RGB triplet signed int32s for internal use of 100x scale
// used instead of floating point
typedef struct {
    int32_t r;
    int32_t g;
    int32_t b;
} rgbint32_t;

//
typedef struct {
    rgbuint16_t color;
    uint16_t dmillis; // hundreths of a sec
} patternline_t;

rgbint32_t dest100x;  // the eventual destination color we want to hit
rgbint32_t step100x;  // the amount of to move each tick
rgbint32_t curr100x;  // the current color, times 100 (to lessen int trunc issue)
int stepcnt;

#ifndef setRGBOut
#error "setRGBOut(r,g,b) not defined"
#endif

// set the current color
void rgb_setCurr( rgbuint16_t* newcolor )
{
    curr100x.r = newcolor->r * 100;
    curr100x.g = newcolor->g * 100;
    curr100x.b = newcolor->b * 100;

    dest100x.r = curr100x.r;
    dest100x.g = curr100x.g;
    dest100x.b = curr100x.b;
    stepcnt = 0;

    setRGBOut( newcolor->r, newcolor->g, newcolor->b );
}

// set a new destination color
void rgb_setDest( rgbuint16_t* newcolor, int steps  )
{
    dest100x.r = newcolor->r*100;
    dest100x.g = newcolor->g*100;
    dest100x.b = newcolor->b*100;

    stepcnt = steps+1;

    step100x.r = (dest100x.r - curr100x.r ) / steps;
    step100x.g = (dest100x.g - curr100x.g ) / steps;
    step100x.b = (dest100x.b - curr100x.b ) / steps;
}

// call at every tick
void rgb_updateCurrent(void)
{
    if( !stepcnt ) {
        return;
    }
    stepcnt--;
    if( stepcnt ) {
        curr100x.r += step100x.r;
        curr100x.g += step100x.g;
        curr100x.b += step100x.b;
    } else {
        curr100x.r = dest100x.r;
        curr100x.g = dest100x.g;
        curr100x.b = dest100x.b;
    }

    setRGBOut( curr100x.r/100, curr100x.g/100, curr100x.b/100 );
}

#endif


