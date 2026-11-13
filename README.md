# isobarcpp

A C++ implementation of Daniel Jones' isobar algorithmic music library.

## What this is

[isobar](https://github.com/ideoforms/isobar) is a Python library by Daniel Jones, described by its author as "a Python package for creating and manipulating musical patterns, designed for use in algorithmic composition, generative music and sonification". Algorithmic composition means writing rules that generate music, rather than writing every note by hand. isobarcpp is an early start on the same ideas in C++.

So far it has the music theory building blocks: notes, chords, scales and keys, plus number patterns that a composition could step through. It does not make any sound or MIDI yet. When you build it, you get a test program that checks these building blocks and reports which checks pass.

## What you can learn from it

Music Technology and Music:

- How scales, keys and chords can be described with numbers. A C major scale is the semitone steps `0, 2, 4, 5, 7, 9, 11` above C.
- How MIDI note numbers work: middle C is 60, and each semitone adds 1.
- What a pattern is in algorithmic music: a rule that produces the next value each time you ask, such as "count up by 2" or "repeat this list".

Computer Science:

- Classes and inheritance in C++: every pattern type shares one `Pattern` interface with `reset()` and `next()`.
- Unit testing with [doctest](https://github.com/doctest/doctest): small checks that prove each piece of code does what it should.
- Building a C++ project with CMake.

## Run it

### On your own computer

**You will need:**

- [Git](https://git-scm.com/downloads).
- [CMake](https://cmake.org/download/) 3.10 or later, a tool that sets up a C++ build for your computer.
- A C++ compiler:
  - Windows: [Visual Studio 2022 Community](https://visualstudio.microsoft.com/vs/community/) with the "Desktop development with C++" workload.
  - macOS: [Xcode](https://developer.apple.com/xcode/) or its Command Line Tools.
  - Linux: `g++` or `clang++`.

The tests depend on doctest, which this repo includes as a git submodule (a link to another repository, fetched along with this one).

Steps:

1. Download the code and the doctest submodule:

   ```bash
   git clone --recurse-submodules https://github.com/godofecht/isobarcpp.git
   cd isobarcpp
   ```

   If you already cloned without `--recurse-submodules`, run `git submodule update --init` inside the folder.

2. Build and run the tests. These are the commands from the original README:

   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ctest -C Debug -V
   ```

When the test program runs, it ends with this summary:

```text
[doctest] test cases: 11 | 11 passed | 0 failed | 0 skipped
[doctest] assertions: 50 | 50 passed | 0 failed |
[doctest] Status: SUCCESS!
```

## How it works

| File or folder | What is in it |
|---|---|
| `Note.h` | A MIDI note with velocity and duration. `toString()` turns note 60 into "C4". |
| `Chord.h` | A chord as a root and a list of intervals. `getSemitones()` turns intervals `{3, 4, 3}` into notes `{0, 3, 7, 10}`. |
| `Scale.h` | A scale as semitones within an octave. Includes `major` and `minor`. |
| `Key.h` | A key: a tonic note plus a scale. Can name notes, check if a note is in the key, and find the nearest note in the key. |
| `Pattern.h` | The base class every pattern shares, with `reset()` and `next()`. |
| `Sequence.h` | Pattern types: `PSequence`, `PSeries`, `PRange`, `PGeom`, `PImpulse` and `PLoop`. |
| `Timeline.h` | Early work on a clock and tracks for playing patterns over time. |
| `main.cpp` | The `BasicProgram` target. It is empty for now. |
| `tests/` | The doctest tests: `test_keys.h` and `test_chord.h`. |
| `CMakeLists.txt` | Builds `BasicProgram` and `tests`, and registers the tests with CTest. |

## Ideas to try

1. Add a chord test. In `tests/test_chord.h`, check that `isobar::Chord({4, 3})` gives semitones `{0, 4, 7}`, a major triad.
2. Add more scales to `scaleDict()` in `Scale.h`, such as the pentatonic scale `{0, 2, 4, 7, 9}`, and a test for it in `tests/test_keys.h`.
3. Make `main.cpp` print something. Create a `PSeries(60, 2, 8)` from `Sequence.h`, call `next()` eight times, and print each value as a note name with `Note((int) value).toString()` from `Note.h`. You should see a whole-tone scale starting on C4.
4. In C++, `-1 % 12` is `-1`, while in Python (which isobar is written in) it is `11`. `Key.h` handles this with its own `floorDiv` and `floorMod`. `Scale.h` still uses plain `%`, so `Scale::get` goes wrong for a negative degree. Write a test that shows it, then fix it the same way.
5. Ambitious: finish `Timeline.h` so a `Timeline` owns a `Clock` and plays a pattern at a set tempo, printing each note as it plays.

## Technical details

- Test framework: doctest 2.4.11, pinned as the submodule `external/doctest` at commit `ae7a135`.
- `tests/main.cpp` defines `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`, so doctest supplies `main()`. CTest runs the test executable with `--success`, which prints passing checks as well as failing ones.
- The code compiles as C++11. `CMakeLists.txt` does not set a C++ standard, so the compiler's default is used.
- `CMakeLists.txt` also defines a custom target, `run`, that builds and runs `BasicProgram`.
- The pinned doctest asks for CMake 2.8, which CMake 4 refuses. `CMakeLists.txt` sets `CMAKE_POLICY_VERSION_MINIMUM` to 3.5 before adding it, so it still configures. CMake prints a deprecation warning about doctest, which is harmless.
- `.vscode/tasks.json` has a "Run CTest Debug" task that runs `ctest --output-on-failure --build-config Debug` from `build/`.

## Project status

From the original README: "Right now, a ton of scaffolding has been written but the tests don't actually pass. The next step is to iterate through each test and debug."

All 11 test cases now pass. The last one to be fixed was "Key nearest note", in November 2026. The library is still scaffolding. Known problems:

- "Chord Test 1" uses C `assert` instead of doctest `CHECK`, so doctest does not count its checks.
- `Timeline` never creates its `Clock`, so calling `Timeline::start()` will crash.
- `main.cpp` is empty, and nothing produces sound or MIDI yet.

## Licence

You can use, copy and change this code for any purpose, as long as you keep the copyright notice. See [LICENSE](LICENSE) (MIT).

doctest and isobar are both MIT licensed and keep their own copyright. No isobar code is copied here. See [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md).

## Credits

- Based on the design of [isobar](https://github.com/ideoforms/isobar) by Daniel Jones.
- Tests use [doctest](https://github.com/doctest/doctest).
- Written by Abhishek Shivakumar.
