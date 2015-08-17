#include <avr/pgmspace.h>

#include "ColorMap.h"
#include "progmem.h"
#include "support.h"
#include "constants.h"

ColorMap::ColorMap(uint8_t my_color): my_color(my_color) {}

CHSV RainbowColorMap::color(uint8_t input) {
    uint8_t hue = input * 12;
    return CHSV(hue, 255, 255);
}

CHSV FadeToWhiteColorMap::color(uint8_t input) {
    uint8_t saturation = pgm_read_byte_near(wrappedSixCycle + input);
    return CHSV(my_color, saturation, 255);
}

CHSV FadeToBlackColorMap::color(uint8_t input) {
    uint8_t value = pgm_read_byte_near(wrappedSixCycle + input);
    return CHSV(my_color, 255, value);
}

CHSV ComplementaryColorMapByHue::color(uint8_t input) {
    uint8_t hue = pgm_read_byte_near(wrappedSixCycle + input) / 2 + my_color;
    return CHSV(hue, 255, 255);
}

CHSV ComplementaryColorMapBySaturation::color(uint8_t input) {
    uint8_t currentColor = pickOption(input, my_color, complementary_color, colorCutoffs, nCutoffs);
    uint8_t saturation = pgm_read_byte_near(wrappedSixCycle + input);
    return CHSV(currentColor, saturation, 255);
}

CHSV ComplementaryColorMapByValue::color(uint8_t input) {
    uint8_t currentColor = pickOption(input, my_color, complementary_color, colorCutoffs, nCutoffs);
    uint8_t hue = pgm_read_byte_near(wrappedSixCycle + input);
    return CHSV(currentColor, 255, hue);
}

ColorMap* createColorMap(uint8_t choice, uint8_t color) {
    switch (choice) {
    case 0:
        return new RainbowColorMap(color);
    case 1:
        return new FadeToWhiteColorMap(color);
    case 2:
        return new FadeToBlackColorMap(color);
    case 3:
        return new ComplementaryColorMapByHue(color);
    case 4:
        return new ComplementaryColorMapBySaturation(color);
    case 5:
        return new ComplementaryColorMapByValue(color);
    }
};
