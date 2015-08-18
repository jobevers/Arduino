
#ifndef Painter_h
#define Painter_h

#include <FastLED.h>
#include "ColorMap.h"
#include "constants.h"


class Painter {
    uint8_t* rows;
    uint8_t* cols;
    CRGB* leds;
    ColorMap* cm;
    uint8_t offsets[ledBufferLength];
    bool forward;
    void setOffset(uint8_t offsetIdx);
public:
    Painter(uint8_t* rows, uint8_t* cols, CRGB* leds): rows(rows), cols(cols), leds(leds){};
    void reset(uint8_t* data);
    void paint(uint8_t input);
    void wait();
};

#endif
