#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <random>

namespace isobar
{
class Chord
{
public:
    // Static map to store Chord objects by name
    static std::unordered_map<std::string, Chord> dict;

    // Constructor allowing vector-only initialization
    Chord(const std::vector<int>& intervals, int root = 0, const std::string& name = "unnamed chord")
        : intervals(intervals), root(root), name(name)
    {
        if (dict.find(name) == dict.end())
        {
            dict[name] = *this;
        }
    }

    // Default constructor for flexibility
    Chord() : intervals({}), root(0), name("unnamed chord") {}

    // Accessor for intervals
    const std::vector<int>& getIntervals() const
    {
        return intervals;
    }

    // Accessor for root
    int getRoot() const
    {
        return root;
    }

    // Accessor for semitones
    std::vector<int> getSemitones() const
    {
        std::vector<int> semitoneList = { root };
        for (size_t i = 0; i < intervals.size(); ++i)
        {
            int sum = std::accumulate(intervals.begin(), intervals.begin() + i + 1, root);
            semitoneList.push_back(sum);
        }
        return semitoneList;
    }

    // Get a string representation of the chord
    std::string toString() const
    {
        std::string result = name + " [";
        std::vector<int> semitoneList = getSemitones();
        for (size_t i = 0; i < semitoneList.size(); ++i)
        {
            result += std::to_string(semitoneList[i]);
            if (i != semitoneList.size() - 1)
            {
                result += ",";
            }
        }
        result += "]";
        return result;
    }

    // Get a Chord by name
    static Chord byName(const std::string& name)
    {
        return dict[name];
    }

    // Generate a random Chord
    static Chord random()
    {
        auto it = dict.begin();
        std::advance(it, rand() % dict.size()); // Random key from dict
        Chord c = it->second;
        c.root = rand() % 13; // Random root [0, 12]
        return c;
    }

    // Generate an arbitrary Chord
    static Chord arbitrary(const std::string& name = "chord")
    {
        std::vector<int> intervalsPoss = { 2, 3, 3, 4, 4, 5, 6 };
        std::vector<int> intervals;
        int top = rand() % (18 - 12 + 1) + 12; // Random top [12, 18]
        int n = 0;

        while (true)
        {
            int interval = intervalsPoss[rand() % intervalsPoss.size()];
            n += interval;
            if (n > top)
                break;
            intervals.push_back(interval);
        }

        return Chord(intervals, 0, name);
    }

private:
    std::vector<int> intervals; // Intervals of the chord
    int root;                   // Root note of the chord
    std::string name;           // Name of the chord
};

//%%%% SCRATCH_PAD %%%%

// Static map definition
// std::unordered_map<std::string, Chord> Chord::dict;


// Define Chord objects
// const Chord major({ 4, 3, 5 }, 0, "major");
// const Chord minor({ 3, 4, 5 }, 0, "minor");
// const Chord diminished({ 3, 3, 6 }, 0, "diminished");
// const Chord augmented({ 4, 4, 4 }, 0, "augmented");
// const Chord sus4({ 5, 2, 5 }, 0, "sus4");
// const Chord sus2({ 7, 2, 5 }, 0, "sus2");

    std::unordered_map<std::string, Chord> Chord::dict;
}