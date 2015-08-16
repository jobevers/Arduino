#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <support.h>


TEST_CASE ("Reverse a list") {
    SECTION ("When the list has an odd number of elements") {
        uint8_t arr[] = {1, 2, 3};
        uint8_t rev_arr[] = {3, 2, 1};
        reverse(arr, 3);
        for (int i=0; i<3; i++) {
            REQUIRE (arr[i] == rev_arr[i]);
        }
    }
    
    SECTION ("When the list has an even number of elements") {
        uint8_t arr[] = {1, 2, 3, 4};
        uint8_t rev_arr[] = {4, 3, 2, 1};
        reverse(arr, 4);
        for (int i=0; i<4; i++) {
            REQUIRE (arr[i] == rev_arr[i]);
        }
    }
}


TEST_CASE ("Making small patterns") {
    uint8_t offset[12];
    uint8_t rows[] = {1, 0};
    uint8_t cols[] = {0, 1, 2};
    SECTION ("Using an idx offset") {
        makePattern(offset, rows, 2, cols, 3, idxOffset);
        for (int i=0; i<12; i++) {
            REQUIRE (offset[i] == i);
        }        
    }

    SECTION ("Using a column offset") {
        makePattern(offset, rows, 2, cols, 3, colOffset);
        uint8_t expected[] = {0, 0, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0};
        for (int i=0; i<12; i++) {
            REQUIRE (offset[i] == expected[i]);
        }
    }

    SECTION ("Using a row offset") {
        makePattern(offset, rows, 2, cols, 3, rowOffset);
        uint8_t expected[] = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
        for (int i=0; i<12; i++) {
            REQUIRE (offset[i] == expected[i]);
        }
    }
}

TEST_CASE ("Making full left patterns") {
    uint8_t offset[50];
    uint8_t rows[] = {4, 3, 2, 1, 0};
    uint8_t cols[] = {0, 1, 2, 3, 4};
    SECTION ("Using a row offset") {
        makePattern(offset, rows, 5, cols, 5, rowOffset);
        uint8_t expected[] = {
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
        };
        for (int i=0; i<50; i++) {
            CHECK (offset[i] == expected[i]);
        }        
    }

    SECTION ("Using a midRow Offset") {
        makePattern(offset, rows, 5, cols, 5, midRowOffset);
        uint8_t expected[] = {
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
        };
        for (int i=0; i<50; i++) {
            CHECK (offset[i] == expected[i]);
        }        
    }

    SECTION ("Using a midColumn Offset") {
        makePattern(offset, rows, 5, cols, 5, midColOffset);
        uint8_t expected[] = {
            4, 4, 4, 4, 4, 3, 3, 3, 3, 3,
            2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
            3, 3, 3, 3, 3, 4, 4, 4, 4, 4,
        };
        for (int i=0; i<50; i++) {
            CHECK (offset[i] == expected[i]);
        }        
    }
}

TEST_CASE ("Making full right patterns") {
    uint8_t offset[50];
    uint8_t rows[] = {4, 3, 2, 1, 0};
    // Note that the columns are different than in 'full left patterns'
    uint8_t cols[] = {5, 6, 7, 8, 9};
    SECTION ("Using a row offset") {
        makePattern(offset, rows, 5, cols, 5, rowOffset);
        uint8_t expected[] = {
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
            4, 3, 2, 1, 0, 0, 1, 2, 3, 4,
        };
        for (int i=0; i<50; i++) {
            CHECK (offset[i] == expected[i]);
        }        
    }

    SECTION ("Using a midRow Offset") {
        makePattern(offset, rows, 5, cols, 5, midRowOffset);
        uint8_t expected[] = {
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
            2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
        };
        for (int i=0; i<50; i++) {
            CHECK (offset[i] == expected[i]);
        }        
    }

    SECTION ("Using a midColumn Offset") {
        makePattern(offset, rows, 5, cols, 5, midColOffset);
        uint8_t expected[] = {
            0, 0, 0, 0, 0,
            1, 1, 1, 1, 1,
            2, 2, 2, 2, 2,
            3, 3, 3, 3, 3,
            4, 4, 4, 4, 4,
            4, 4, 4, 4, 4,
            3, 3, 3, 3, 3,
            2, 2, 2, 2, 2,
            1, 1, 1, 1, 1,
            0, 0, 0, 0, 0,
        };
        for (int i=0; i<50; i++) {
            CHECK (offset[i] == expected[i]);
        }        
    }
}


