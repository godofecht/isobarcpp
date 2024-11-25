#include "Scale.h"
#include "Note.h"

#ifndef KEY_H
#define KEY_H

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>

class Key
{
public:
    Key(int tonic = 0, Scale* scale = Scale::scaleDict()["major"])
        : tonic(tonic), scale(scale)
    {
    }

    Key(const std::string& tonicStr, const std::string& scaleStr)
    {
        tonic = noteNameToMidi(tonicStr);
        scale = Scale::byName(scaleStr);
    }

    Key(const std::string& tonicStr, Scale* scale = Scale::scaleDict()["major"])
        : tonic(noteNameToMidi(tonicStr)), scale(scale)
    {
    }

    bool operator==(const Key& other) const
    {
        return tonic == other.tonic && scale == other.scale;
    }

    bool operator!=(const Key& other) const
    {
        return tonic != other.tonic || scale != other.scale;
    }

    std::string toString() const
    {
        return "Key: " + midiToNoteName(tonic) + " " + scale->getName();
    }

    int get(int degree) const
    {
        if (degree == -1)
        {
            return -1; // Represent rest
        }
        return scale->get(degree) + tonic;
    }

    bool contains(int semitone) const
    {
        if (semitone == -1) // Represent rest
        {
            return true;
        }
        return std::find(semitones().begin(), semitones().end(), semitone % scale->getOctaveSize()) != semitones().end();
    }

    std::vector<int> semitones() const
    {
        std::vector<int> result;
        for (int semitone : scale->getSemitones())
        {
            result.push_back((semitone + tonic) % scale->getOctaveSize());
        }
        std::sort(result.begin(), result.end());
        return result;
    }

    int nearestNote(int note) const
    {
        if (contains(note))
        {
            return note;
        }

        int octave = note / scale->getOctaveSize();
        int pitch = note % scale->getOctaveSize();

        int nearestSemitone = 0;
        int nearestDistance = std::numeric_limits<int>::max();

        for (int semitone : semitones())
        {
            int distance = std::abs(semitone - pitch);
            if (distance > scale->getOctaveSize() / 2)
            {
                distance = scale->getOctaveSize() - distance;
            }
            if (distance < nearestDistance)
            {
                nearestSemitone = semitone;
                nearestDistance = distance;
            }
        }

        return octave * scale->getOctaveSize() + nearestSemitone;
    }

    std::vector<std::pair<int, int>> voiceleading(const Key& other) const
    {
        const std::vector<int>& semisA = semitones();
        const std::vector<int>& semisB = other.semitones();

        std::vector<std::pair<int, int>> leading;

        for (int semiA : semisA)
        {
            int nearestSemiB = 0;
            int minDistance = std::numeric_limits<int>::max();

            for (int semiB : semisB)
            {
                int distance = std::abs(semiA - semiB);
                if (distance > scale->getOctaveSize() / 2)
                {
                    distance = scale->getOctaveSize() - distance;
                }
                if (distance < minDistance)
                {
                    nearestSemiB = semiB;
                    minDistance = distance;
                }
            }

            leading.emplace_back(semiA, nearestSemiB);
        }

        return leading;
    }

    int distance(const Key& other) const
    {
        auto leading = voiceleading(other);
        int totalDistance = 0;

        for (const auto& pair : leading)
        {
            totalDistance += std::abs(pair.first - pair.second);
        }

        return totalDistance;
    }

    std::vector<int> fadeto(const Key& other, double level) const
    {
        const std::vector<int>& semisA = semitones();
        const std::vector<int>& semisB = other.semitones();

        std::vector<int> shared, aOnly, bOnly;

        for (int semiA : semisA)
        {
            if (std::find(semisB.begin(), semisB.end(), semiA) != semisB.end())
            {
                shared.push_back(semiA);
            }
            else
            {
                aOnly.push_back(semiA);
            }
        }

        for (int semiB : semisB)
        {
            if (std::find(semisA.begin(), semisA.end(), semiB) == semisA.end())
            {
                bOnly.push_back(semiB);
            }
        }

        if (level < 0.5)
        {
            level = 1.0 - (level * 2.0);
            int countFromA = static_cast<int>(std::round(level * aOnly.size()));
            shared.insert(shared.end(), aOnly.begin(), aOnly.begin() + countFromA);
        }
        else
        {
            level = 2.0 * (level - 0.5);
            int countFromB = static_cast<int>(std::round(level * bOnly.size()));
            shared.insert(shared.end(), bOnly.begin(), bOnly.begin() + countFromB);
        }

        return shared;
    }

    static Key random()
    {
        int t = randomInt(0, 11);
        Scale* s = Scale::randomScale();
        return Key(t, s);
    }

    int getTonic() { return tonic; }

private:
    int tonic;
    Scale* scale;

    static int randomInt(int min, int max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }

    static int noteNameToMidi(const std::string& name)
    {
        // Implement logic to convert note name to MIDI number
        // Example: "C" -> 0, "C#" -> 1, "D" -> 2, ...
        throw std::logic_error("noteNameToMidi not implemented");
    }

    static std::string midiToNoteName(int midi)
    {
        // Implement logic to convert MIDI number to note name
        // Example: 0 -> "C", 1 -> "C#", 2 -> "D", ...
        throw std::logic_error("midiToNoteName not implemented");
    }
};

#endif // KEY_H
