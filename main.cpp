// from isobar import *
// import logging

// def main():
//     series = PRange(60, 73, 1)
//     timeline = Timeline(120)

//     timeline.schedule({
//         "note": series,
//         "duration": 1
//     })

//     timeline.run()


// if __name__ == "__main__":
//     logging.basicConfig(level=logging.INFO, format="[%(asctime)s] %(message)s")
//     main()

#include <iostream>
#include "Pattern.h"
#include "Timeline.h"
#include "Chord.h"
#include "Key.h"
#include <assert.h>


inline void test_chord()
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

inline void hello_world()
{
    auto series = PRange (60, 73, 1);
    auto timeline = std::make_unique<Timeline> (120, 480);
}

#include <cassert>

inline void testKeyDefaults()
{
    Key a;
    assert(a.get(0) == 0);
    assert(a.get(1) == 2);
}

inline void testKeyEquality()
{
//     Key a;
//     Scale customScale({0, 2, 4, 5, 7, 9, 11}, "custom");
//     Key b(0, &customScale);
//     assert(a == b);

//     Scale minorScale({0, 2, 3, 5, 7, 9, 11}, "minor");
//     Key c(0, &minorScale);
//     assert(a != c);

//     Key d(1, &customScale);
//     assert(a != d);
}

inline void testKeyInvalid()
{
    try
    {
        Key a("X", "major");
        assert(false); // Should not reach here
    }
    catch (const std::invalid_argument&) {}

    try
    {
        Key b("H", "minor");
        assert(false); // Should not reach here
    }
    catch (const std::invalid_argument&) {}

    try
    {
        Key c("C", "mundo");
        assert(false); // Should not reach here
    }
    catch (const std::invalid_argument&) {}
}

inline void testKeyGet()
{
    Key a("C", "major");
    // assert(a.get(0) == a[0]);
    // assert(a.get(1) == a[1]);
    // assert(a.get(2) == a[2]);
    // assert(a.get(7) == a[7]);
    // assert(a.get(-1) == a[-1]);
}

inline void testKeyContains()
{
    Key a("C", "major");
    assert(a.contains(0));
    assert(!a.contains(1));
    assert(a.contains(2));
    assert(!a.contains(3));
    assert(a.contains(4));
    assert(a.contains(12));
    assert(!a.contains(13));
    assert(a.contains(-1)); // Representing a rest
}

inline void testKeySemitones()
{
    Key a("C", "minor");
    std::vector<int> expected = {0, 2, 3, 5, 7, 8, 10};
    assert(a.semitones() == expected);
}

inline void testKeyNearestNote()
{
    Key a("C", new Scale({0, 2, 3, 5, 7, 9}));
    assert(a.nearestNote(-3) == -3);
    assert(a.nearestNote(-2) == -3);
    assert(a.nearestNote(-1) == 0);
    assert(a.nearestNote(0) == 0);
    assert(a.nearestNote(1) == 0);
    assert(a.nearestNote(2) == 2);
    assert(a.nearestNote(3) == 3);
    assert(a.nearestNote(4) == 3);
    assert(a.nearestNote(5) == 5);
    assert(a.nearestNote(6) == 5);
    assert(a.nearestNote(7) == 7);
    assert(a.nearestNote(8) == 7);
    assert(a.nearestNote(9) == 9);
    assert(a.nearestNote(10) == 9);
    assert(a.nearestNote(11) == 12);
}

inline void testKeyDistance()
{
    Key a("C", "minor");
    Key b("C", "major");
    assert(a.distance(b) == 3);
}

inline void testKeyRandom()
{
    Key a = Key::random();
    assert(a.getTonic() >= 0 && a.getTonic() < 12);
    assert(!a.semitones().empty());
    assert(a.semitones().size() <= 12);
    for (int note : a.semitones())
    {
        assert(note >= 0);
    }
}

int main()
{
    testKeyDefaults();
    testKeyEquality();
    testKeyInvalid();
    testKeyGet();
    testKeyContains();
    testKeySemitones();
    testKeyNearestNote();
    testKeyDistance();
    testKeyRandom();

    return 0; // If no assertions fail, program exits normally
}
