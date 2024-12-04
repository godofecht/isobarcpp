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

    Key(const std::string& tonicStr, const std::string& scaleStr)
    {
        tonic = noteNameToMidi(tonicStr);
        scale = Scale::byName(scaleStr);
    }

    Key(const std::string& tonicStr, Scale* scale = Scale::scaleDict()["major"].get())
        : tonic(noteNameToMidi(tonicStr)), scale(scale)
    {
    }

    explicit Key(int tonic = 0, Scale* scale = Scale::scaleDict()["major"].get())
        : tonic(tonic), scale(scale)
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
        return "Key: " + midiToNoteName (tonic) + " " + scale->getName();
    }

    int operator[] (int degree) const
    {
        if (degree == -1)
        {
            return -1; // Represent rest
        }
        return scale->get(degree) + tonic;        
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
        if (semitone == -1) // Rest
        {
            return true;
        }

        auto sortedSemitones = getSortedSemitones();
        int modSemitone = semitone % scale->getOctaveSize();
        return std::binary_search(sortedSemitones.begin(), sortedSemitones.end(), modSemitone);
    }


    std::vector<int> getSortedSemitones() const
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

        for (int semitone : getSortedSemitones())
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
        const std::vector<int>& semisA = getSortedSemitones();
        const std::vector<int>& semisB = other.getSortedSemitones();

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
        const std::vector<int>& semisA = getSortedSemitones();
        const std::vector<int>& semisB = other.getSortedSemitones();

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

#include <string>
#include <cctype>

    static int extractOctaveFromNoteName(const std::string& name)
    {
        if (name.empty())
        {
            throw std::invalid_argument("Invalid input: Name cannot be empty.");
        }

        // Check if the last character is a digit
        if (std::isdigit(name.back()))
        {
            try
            {
                return std::stoi(name.substr(name.size() - 1));
            }
            catch (const std::exception&)
            {
                throw std::runtime_error("Failed to extract octave from note name.");
            }
        }

        // Default to octave 0 if no digit is found
        return 0;
    }

    static int noteNameToMidi(const std::string& name)
    {
        // Map for note names and their corresponding semitone offset
        static const std::unordered_map<std::string, int> noteToSemitone = {
            {"C", 0}, {"C#", 1}, {"D", 2}, {"D#", 3}, {"E", 4}, {"F", 5},
            {"F#", 6}, {"G", 7}, {"G#", 8}, {"A", 9}, {"A#", 10}, {"B", 11}
        };

        // Extract base note and optional octave number
        std::string note = name.substr (0, 1);
        int octave = extractOctaveFromNoteName (name);

        // Find semitone value for the base note
        auto it = noteToSemitone.find(note);
        if (it == noteToSemitone.end())
        {
            throw std::invalid_argument("Invalid note name: " + name);
        }

        int semitone = it->second;
        return (octave + 1) * 12 + semitone; // MIDI formula
    }

    static std::string midiToNoteName(int midi)
    {
        if (midi < 0 || midi > 127)
        {
            throw std::out_of_range("MIDI number out of valid range: " + std::to_string(midi));
        }

        // Vector for note names
        static const std::vector<std::string> semitoneToNote = {
            "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
        };

        int octave = midi / 12 - 1;         // Calculate octave
        int semitone = midi % 12;          // Find the note within the octave
        return semitoneToNote[semitone] + std::to_string(octave);
    }

};

#endif // KEY_H
