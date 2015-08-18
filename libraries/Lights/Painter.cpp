#include <Arduino.h>
#include <FastLED.h>
#include "Painter.h"
#include "support.h"

void Painter::setOffset(uint8_t offsetIdx) {
  makePattern(offsets, rows, nRows, cols, nCols, allOffsets[offsetIdx]); 
}

void Painter::reset(uint8_t* data) {
    delete cm;
    // createColorMap returns a NEW colorMap, which is why we delete
    // the reference to the previous one;
    cm = createColorMap(data[1], data[2]);
    setOffset(data[0]);
    forward = data[3];
}

void Painter::paint(uint8_t input){
    for (int i=0; i<ledBufferLength; i++) {
        uint8_t color = forward ? offsets[i] + input : offsets[i] - input;
        leds[i] = cm->color(color);
    } 
    FastLED.show();
}

void Painter::wait() {
    delay(cm->delay);
}
