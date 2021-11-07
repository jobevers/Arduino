#include <assert.h>

#include "constants.h"
#include "support.h"


void reverse(uint8_t arr[], int length) {
  uint8_t temp;
  for (int i=0; i<length/2; i++) {
    int end = length - 1 - i;
    temp = arr[i];
    arr[i] = arr[end];
    arr[end] = temp;
  }
}


// offset needs to be of length 2 * nCols * nRows;
void makePattern(
        uint8_t offset[],
        uint8_t rows[], uint8_t nRows, uint8_t cols[], uint8_t nCols,
        uint8_t(*fn)(uint8_t idx, uint8_t row, uint8_t col)) {
    // The number of columns must be odd in order to match up with
    // how the panels should be wired.
    assert (nCols % 2 == 1);
    // The outer loop is one for the front and one for the back
    int idx=0;
    for (int fb=0; fb<2; fb++) {  
        for (int col_idx=0; col_idx<nCols; col_idx++) {
            int col = cols[col_idx];
            for (int row_idx=0; row_idx<nRows; row_idx++) {
                offset[idx] = fn(idx, rows[row_idx], col);
                idx++;
            }
            reverse(rows, nRows);
        }
        reverse(cols, nCols);
    }
}

uint8_t nullOffset(uint8_t idx, uint8_t row, uint8_t col) {
    return 0;
}

uint8_t idxOffset(uint8_t idx, uint8_t row, uint8_t col) {
    return idx;
}

uint8_t rowOffset(uint8_t idx, uint8_t row, uint8_t col) {
    return row;
}

uint8_t colOffset(uint8_t idx, uint8_t row, uint8_t col) {
    return col;
}

uint8_t diagOffset(uint8_t idx, uint8_t row, uint8_t col) {
    return row + col;
};

uint8_t midRowOffset(uint8_t idx, uint8_t row, uint8_t col) {
    return midRows[row];
}

uint8_t midColOffset(uint8_t idx, uint8_t row, uint8_t col) {
    return midCols[col];
}

uint8_t midDiagOffset(uint8_t idx, uint8_t row, uint8_t col) {
    return midRowOffset(idx, row, col) + midColOffset(idx, row, col);
}

uint8_t pickOption(
        uint8_t input, uint8_t optionA, uint8_t optionB,
        uint8_t cutoffs[], uint8_t nCutoffs) {
    bool option = true;
    for (int i=0; i<nCutoffs; i++) {
        if (input <= cutoffs[i]) {
            break;
        }
        option = !option;
    }
    return option ? optionA : optionB;
}

