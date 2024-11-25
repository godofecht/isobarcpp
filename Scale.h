#ifndef SCALE_H
#define SCALE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <numeric>

class Scale
{
public:
    static std::unordered_map<std::string, Scale*>& scaleDict()
    {
        static std::unordered_map<std::string, Scale*> dict;
        return dict;
    }

    Scale(const std::vector<int>& semitones = {0, 2, 4, 5, 7, 9, 11},
          const std::string& name = "unnamed scale",
          int octaveSize = 12)
        : semitones(semitones), name(name), octaveSize(octaveSize)
    {
        if (scaleDict().find(name) == scaleDict().end())
        {
            scaleDict()[name] = this;
        }
        weights = std::vector<double>(semitones.size(), 1.0 / semitones.size());
    }

    ~Scale()
    {
        scaleDict().erase(name);
    }

    std::string getName() const
    {
        return name;
    }

    int getOctaveSize() const
    {
        return octaveSize;
    }

    int get(int n) const
    {
        int octave = n / semitones.size();
        int degree = n % semitones.size();
        int semitone = semitones[degree];
        return (octaveSize * octave) + semitone;
    }

    Scale* copy() const
    {
        return new Scale(semitones, name, octaveSize);
    }

    void change()
    {
        int i = randomInt(0, semitones.size() - 1);
        int j = randomInt(0, semitones.size() - 1);
        if (i != j)
        {
            std::swap(semitones[i], semitones[j]);
        }
    }

    void shuffle()
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(semitones.begin(), semitones.end(), g);
    }

    int indexOf(int note) const
    {
        int octave = note / octaveSize;
        int index = octave * semitones.size();
        note %= octaveSize;

        auto it = std::upper_bound(semitones.begin(), semitones.end(), note);
        index += std::distance(semitones.begin(), it);
        return index;
    }

    bool contains(int semitone) const
    {
        semitone %= octaveSize;
        return std::find(semitones.begin(), semitones.end(), semitone) != semitones.end();
    }

    static Scale* byName(const std::string& name)
    {
        if (scaleDict().find(name) != scaleDict().end())
        {
            return scaleDict()[name];
        }
        throw std::invalid_argument("Unknown scale name");
    }

    static Scale* randomScale()
    {
        if (scaleDict().empty())
        {
            throw std::runtime_error("No scales available");
        }
        auto it = scaleDict().begin();
        std::advance(it, randomInt(0, scaleDict().size() - 1));
        return it->second;
    }

    int randomNote() const
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<> dist(weights.begin(), weights.end());
        return semitones[dist(gen)];
    }

    static std::vector<Scale*> all()
    {
        std::vector<Scale*> scales;
        for (const auto& pair : scaleDict())
        {
            scales.push_back(pair.second);
        }
        return scales;
    }

    std::vector<int> getSemitones()
    {
        return semitones;
    }

protected:
    std::vector<int> semitones;
    std::vector<double> weights;
    std::string name;
    int octaveSize;

    static int randomInt(int min, int max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }
};

class WeightedScale : public Scale
{
public:
    WeightedScale(const std::vector<int>& semitones = {0, 2, 4, 5, 7, 9, 11},
                  const std::vector<double>& weights = std::vector<double>(7, 1.0 / 7),
                  const std::string& name = "major",
                  int octaveSize = 12)
        : Scale(semitones, name, octaveSize)
    {
        this->weights = weights;
        if (scaleDict().find(name) == scaleDict().end())
        {
            scaleDict()[name] = this;
        }
    }

    std::string toString() const
    {
        std::string result = name + " [ ";
        for (size_t i = 0; i < semitones.size(); ++i)
        {
            result += std::to_string(semitones[i]) + "(" + std::to_string(weights[i]) + ") ";
        }
        result += "]";
        return result;
    }

    static WeightedScale* fromOrder(const std::vector<int>& notes, const std::string& name = "unnamed scale", int octaveSize = 12)
    {
        std::vector<int> normalizedNotes = notes;
        std::for_each(normalizedNotes.begin(), normalizedNotes.end(), [octaveSize](int& note) { note %= octaveSize; });

        std::vector<double> noteWeights(notes.size());
        std::iota(noteWeights.rbegin(), noteWeights.rend(), 1); // descending weights
        double weightSum = std::accumulate(noteWeights.begin(), noteWeights.end(), 0.0);
        for (auto& weight : noteWeights)
        {
            weight /= weightSum;
        }

        return new WeightedScale(normalizedNotes, noteWeights, name, octaveSize);
    }
};

#endif // SCALE_H
