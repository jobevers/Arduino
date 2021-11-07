#ifndef ColorMap_h
#define ColorMap_h

#include <FastLED.h>

class ColorMap {
protected:
    uint8_t my_color;
public:
    ColorMap(uint8_t);
    virtual CHSV color (uint8_t);
    int delay = 50; //how much delay the caller should use at each step
};

class RainbowColorMap: public ColorMap {
public:
    RainbowColorMap(uint8_t my_color): ColorMap(my_color){};
    CHSV color (uint8_t);
};

class FadeToWhiteColorMap : public ColorMap {
public:
    FadeToWhiteColorMap(uint8_t my_color): ColorMap(my_color){};
    CHSV color (uint8_t);
};

class FadeToBlackColorMap : public ColorMap {
public:
    FadeToBlackColorMap(uint8_t my_color): ColorMap(my_color){};
    CHSV color (uint8_t);
};

class ComplementaryColorMap : public ColorMap { 
protected:
    uint8_t complementary_color;
public:
    ComplementaryColorMap(uint8_t my_color): ColorMap(my_color), complementary_color(my_color + 128){};
};

class ComplementaryColorMapByHue : public ComplementaryColorMap {
public:
    ComplementaryColorMapByHue(uint8_t my_color): ComplementaryColorMap(my_color){};
    CHSV color (uint8_t);
};

class ComplementaryColorMapBySaturation : public ComplementaryColorMap {
public:
    ComplementaryColorMapBySaturation(uint8_t my_color): ComplementaryColorMap(my_color){};
    CHSV color (uint8_t);
};

class ComplementaryColorMapByValue : public ComplementaryColorMap {
public:
    ComplementaryColorMapByValue(uint8_t my_color): ComplementaryColorMap(my_color){};
    CHSV color (uint8_t);
};

const uint8_t nColorMaps = 10; // setting this to ten gives the rainbow map a 50% chance of being selected
ColorMap* createColorMap(uint8_t choice, uint8_t color);

#endif
