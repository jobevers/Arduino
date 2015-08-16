#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <ColorMap.h>


TEST_CASE ("This is my first test", "[huh]") {
    ColorMap cm;

    REQUIRE (cm.factorial(2) == 2);
}
