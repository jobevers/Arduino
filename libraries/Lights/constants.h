#ifndef constants_h
#define constants_h

#include <stdint.h>

const uint8_t nRows = 5;
const uint8_t nCols = 5;
const uint8_t midRows[] = {2, 1, 0, 1, 2};
const uint8_t midCols[] = {4, 3, 2, 1, 0, 0, 1, 2, 3, 4};

extern const uint8_t nCutoffs;
extern uint8_t colorCutoffs[];
#endif
