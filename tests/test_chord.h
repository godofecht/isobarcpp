#include <iostream>
#include "Sequence.h"
#include "Timeline.h"
#include "Chord.h"
#include "Key.h"
#include <assert.h>
#include <cassert>

inline void runTests_Chord()
{
    // Test case 1
    Chord chord1 ({3, 4, 3});
    assert (chord1.getIntervals() == std::vector<int>({3, 4, 3}));
    assert (chord1.getRoot() == 0);
    assert (chord1.getSemitones() == std::vector<int>({0, 3, 7, 10}));

    // Test case 2
    Chord chord2({3, 4, 3}, 3);
    assert (chord2.getIntervals() == std::vector<int>({3, 4, 3}));
    assert (chord2.getRoot() == 3);
    assert (chord2.getSemitones() == std::vector<int>({3, 6, 10, 13}));

    std::cout << "All tests passed!" << std::endl;
}