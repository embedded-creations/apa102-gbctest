//
// test program orignially from blink1
// https://github.com/todbot/blink1/tree/master/hardware/firmware/rgb_funcs_test
//
//

// teensy 3.1 pinout
// DATA 11
// CLK  13

#include <SPI.h>


uint16_t led16bit_r;
uint16_t led16bit_g;
uint16_t led16bit_b;

#define setRGBOut(r,g,b) {led16bit_r = r; led16bit_g = g; led16bit_b = b; }

#include "color_funcs.h"

rgbuint16_t c1 = {0,0,0};
rgbuint16_t c2;
int steps = 1000;


void setup() {
    SPI.begin();
    rgb_setCurr( &c1 );
}

uint8_t brightness = 10;

void loop() {
  int i;

    c2.r = random(0xFFFF);
    c2.g = random(0xFFFF);
    c2.b = random(0xFFFF);

    c2.r = (c2.r * brightness) / 256;
    c2.g = (c2.g * brightness) / 256;
    c2.b = (c2.b * brightness) / 256;

    rgb_setDest( &c2, steps );  

    for( int j=0; j<steps+1; j++ ) {
        for(i=0; i<4; i++) SPI.transfer(0);    // Start-frame marker

        // first LED in chain with 8-bit color
        SPI.transfer(0xFF);                //  Pixel start with max global brightness
        SPI.transfer(led16bit_r >> 8); // R,G,B
        SPI.transfer(led16bit_g >> 8);
        SPI.transfer(led16bit_b >> 8);

        uint8_t globalbrightness;
        uint8_t localshift;
        uint16_t value = led16bit_r | led16bit_g | led16bit_b;

        if(value & 0x8000) {
            globalbrightness = 0x1F;
            localshift = 8;
        } else if(value & 0x4000) {
            globalbrightness = 0x10;
            localshift = 7;
        } else if(value & 0x2000) {
            globalbrightness = 0x08;
            localshift = 6;
        } else if(value & 0x1000) {
            globalbrightness = 0x04;
            localshift = 5;
        } else if(value & 0x0800) {
            globalbrightness = 0x02;
            localshift = 4;
        } else {
            globalbrightness = 0x01;
            localshift = 3;
        }

        // drive second pixel with 8-bit color plus 5-bit global brightness
        SPI.transfer(0xE0 | globalbrightness);                //  Pixel start with global brightness
        SPI.transfer(led16bit_r >> localshift); // R,G,B
        SPI.transfer(led16bit_g >> localshift);
        SPI.transfer(led16bit_b >> localshift);

        for(i=0; i<1; i++) SPI.transfer(0xFF); // End-frame marker 

        rgb_updateCurrent();

        // 10ms delay = 100Hz
        delay(10);
    }
}
