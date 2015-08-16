#include "ColorMap.h"


CHSV ColorMap::color(int input) {
  uint8_t hue = input * 12;
  return CHSV(hue, 255, 255);
}

