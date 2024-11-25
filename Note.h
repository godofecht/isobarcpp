#ifndef NOTE_H
#define NOTE_H

#include <string>
#include <vector>
#include <stdexcept>

class Note
{
public:
    static const std::vector<std::string>& names()
    {
        static const std::vector<std::string> noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        return noteNames;
    }

    Note(int note = 60, int velocity = 64, double duration = 1.0)
        : note(note), velocity(velocity), duration(duration)
    {
    }

    ~Note() = default;

    std::string toString() const
    {
        if (velocity == 0)
        {
            return "rest";
        }

        return midiNoteToNoteName(note);
    }

    static Note rest()
    {
        return Note(0, 0, 0.0);
    }

    static const std::vector<std::string>& all()
    {
        return names();
    }

private:
    int note;
    int velocity;
    double duration;

    std::string midiNoteToNoteName(int midiNote) const
    {
        const auto& noteNames = names();
        if (midiNote < 0 || midiNote > 127)
        {
            throw std::out_of_range("MIDI note must be in the range 0-127");
        }

        int octave = (midiNote / 12) - 1;
        int noteIndex = midiNote % 12;
        return noteNames[noteIndex] + std::to_string(octave);
    }
};

#endif // NOTE_H
