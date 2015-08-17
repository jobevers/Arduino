#ifndef constants_h
#define constants_h

#include <stdint.h>

const uint8_t nRows = 5;
const uint8_t nCols = 5;
const uint8_t midRows[] = {2, 1, 0, 1, 2};
const uint8_t midCols[] = {4, 3, 2, 1, 0, 0, 1, 2, 3, 4};

// this is generated from the make_c_constants.py script
// It goes from 0->256->0 six times
// This is generated due to general difficulty in getting
// integer math to wrap nicely; in making the space/computation
// tradeoff, this is choosing to use more space.
const uint8_t wrappedSixCycle[] = {
      0,  12,  24,  36,  48,  60,  72,  84,  96, 108, 120, 132, 144, 156, 168, 180,
    192, 204, 216, 228, 240, 252, 247, 235, 223, 211, 199, 187, 175, 163, 151, 139,
    127, 115, 103,  91,  79,  67,  55,  43,  31,  19,   7,   5,  17,  29,  41,  53,
     65,  77,  89, 101, 113, 125, 137, 149, 161, 173, 185, 197, 209, 221, 233, 245,
    254, 242, 230, 218, 206, 194, 182, 170, 158, 146, 134, 122, 110,  98,  86,  74,
     62,  50,  38,  26,  14,   2,  10,  22,  34,  46,  58,  70,  82,  94, 106, 118,
    130, 142, 154, 166, 178, 190, 202, 214, 226, 238, 250, 249, 237, 225, 213, 201,
    189, 177, 165, 153, 141, 129, 117, 105,  93,  81,  69,  57,  45,  33,  21,   9,
      3,  15,  27,  39,  51,  63,  75,  87,  99, 111, 123, 135, 147, 159, 171, 183,
    195, 207, 219, 231, 243, 255, 244, 232, 220, 208, 196, 184, 172, 160, 148, 136,
    124, 112, 100,  88,  76,  64,  52,  40,  28,  16,   4,   8,  20,  32,  44,  56,
     68,  80,  92, 104, 116, 128, 140, 152, 164, 176, 188, 200, 212, 224, 236, 248,
    251, 239, 227, 215, 203, 191, 179, 167, 155, 143, 131, 119, 107,  95,  83,  71,
     59,  47,  35,  23,  11,   1,  13,  25,  37,  49,  61,  73,  85,  97, 109, 121,
    133, 145, 157, 169, 181, 193, 205, 217, 229, 241, 253, 246, 234, 222, 210, 198,
    186, 174, 162, 150, 138, 126, 114, 102,  90,  78,  66,  54,  42,  30,  18,   6,
};

// want to switch every 42.667 but, this is good enough
const uint8_t nCutoffs = 5;
const uint8_t colorCutoffs[] = {42, 85, 127, 170, 213};
#endif
