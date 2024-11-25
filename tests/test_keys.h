#pragma once

#include <iostream>
#include "../Sequence.h"
#include "../Timeline.h"
#include "../Chord.h"
#include "../Key.h"
#include <vector>
#include <stdexcept>

//External includes
#include "doctest.h"

TEST_CASE("Key defaults") {
    Key a;
    CHECK(a.get(0) == 0);
    CHECK(a.get(1) == 2);
}

TEST_CASE("Key equality") {
    Key a;
    Scale customScale({0, 2, 4, 5, 7, 9, 11}, "custom");
    Key b(0, &customScale);
    CHECK(a != b);

    Scale minorScale({0, 2, 3, 5, 7, 9, 11}, "minor");
    Key c(0, &minorScale);
    CHECK(a != c);

    Key d(1, &customScale);
    CHECK(a != d);
}

TEST_CASE("Key invalid input") {
    CHECK_THROWS_AS(Key("X", "major"), std::invalid_argument);
    CHECK_THROWS_AS(Key("H", "minor"), std::invalid_argument);
    CHECK_THROWS_AS(Key("C", "mundo"), std::invalid_argument);
}

TEST_CASE("Key get") {
    Key a("C", "major");
    // CHECK(a.get(0) == a[0]);
    // CHECK(a.get(1) == a[1]);
    // CHECK(a.get(2) == a[2]);
    // CHECK(a.get(7) == a[7]);
    // CHECK(a.get(-1) == a[-1]);
}

TEST_CASE("Key contains") {
    Key a("C", "major");
    CHECK(a.contains(0));
    CHECK(!a.contains(1));
    CHECK(a.contains(2));
    CHECK(!a.contains(3));
    CHECK(a.contains(4));
    CHECK(a.contains(12));
    CHECK(!a.contains(13));
    CHECK(a.contains(-1)); // Representing a rest
}

TEST_CASE("Key semitones") {
    Key a("C", "minor");
    std::vector<int> expected = {0, 2, 3, 5, 7, 8, 10};
    CHECK(a.semitones() == expected);
}

TEST_CASE("Key nearest note") {
    Key a("C", new Scale({0, 2, 3, 5, 7, 9}));
    CHECK(a.nearestNote(-3) == -3);
    CHECK(a.nearestNote(-2) == -3);
    CHECK(a.nearestNote(-1) == 0);
    CHECK(a.nearestNote(0) == 0);
    CHECK(a.nearestNote(1) == 0);
    CHECK(a.nearestNote(2) == 2);
    CHECK(a.nearestNote(3) == 3);
    CHECK(a.nearestNote(4) == 3);
    CHECK(a.nearestNote(5) == 5);
    CHECK(a.nearestNote(6) == 5);
    CHECK(a.nearestNote(7) == 7);
    CHECK(a.nearestNote(8) == 7);
    CHECK(a.nearestNote(9) == 9);
    CHECK(a.nearestNote(10) == 9);
    CHECK(a.nearestNote(11) == 12);
}

TEST_CASE("Key distance") {
    Key a("C", "minor");
    Key b("C", "major");
    CHECK(a.distance(b) == 3);
}

TEST_CASE("Key random") {
    Key a = Key::random();
    CHECK(a.getTonic() >= 0);
    CHECK(a.getTonic() < 12);
    CHECK(!a.semitones().empty());
    CHECK(a.semitones().size() <= 12);
    for (int note : a.semitones()) {
        CHECK(note >= 0);
    }
}
