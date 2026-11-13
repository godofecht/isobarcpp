#pragma once

#include "../Scale.h"

//External includes
#include "doctest.h"

TEST_CASE("Scale negative degrees and notes")
{
    Scale major; // 0 2 4 5 7 9 11

    // Degrees below 0 step down into the octave below, as in isobar's Python.
    CHECK(major.get(-1) == -1);
    CHECK(major.get(-2) == -3);
    CHECK(major.get(-7) == -12);
    CHECK(major.get(-8) == -13);
    CHECK(major.get(6) == 11);
    CHECK(major.get(7) == 12);

    // Pitch classes wrap the same way below 0: -1 is B, -12 is C.
    CHECK(major.contains(-1));
    CHECK(major.contains(-12));
    CHECK(!major.contains(-2)); // A#
    CHECK(!major.contains(-11)); // C#

    // Moving a note down an octave moves its index down by one scale length.
    for (int note = 0; note < 12; ++note)
    {
        CHECK(major.indexOf(note - 12) == major.indexOf(note) - 7);
    }
}
