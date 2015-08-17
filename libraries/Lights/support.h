
#ifndef support_h
#define support_h

#include <stdint.h>

// Put functions in here that I can test

/*
  Reverse an array in-place
 */
void reverse(uint8_t arr[], int length);


/*
  Creates a pattern (an array of offsets)
 */
void makePattern(
    uint8_t offset[],
    uint8_t rows[], uint8_t nRows, uint8_t cols[], uint8_t nCols,
    uint8_t (*fn)(uint8_t idx, uint8_t row, uint8_t col));

typedef uint8_t (*offsetFn)(uint8_t, uint8_t, uint8_t);
uint8_t nullOffset(uint8_t idx, uint8_t row, uint8_t col);
uint8_t idxOffset(uint8_t idx, uint8_t row, uint8_t col);
uint8_t rowOffset(uint8_t idx, uint8_t row, uint8_t col);
uint8_t colOffset(uint8_t idx, uint8_t row, uint8_t col);
uint8_t diagOffset(uint8_t idx, uint8_t row, uint8_t col);
uint8_t midRowOffset(uint8_t idx, uint8_t row, uint8_t col);
uint8_t midColOffset(uint8_t idx, uint8_t row, uint8_t col);
uint8_t midDiagOffset(uint8_t idx, uint8_t row, uint8_t col);

const int nOffsets = 8;
const offsetFn allOffsets[] = {nullOffset, idxOffset, rowOffset, colOffset, diagOffset, midRowOffset, midColOffset, midDiagOffset};

uint8_t pickOption(uint8_t input, uint8_t optionA, uint8_t optionB, uint8_t cutoffs[], uint8_t nCutoffs);
#endif
