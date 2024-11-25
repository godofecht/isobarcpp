#pragma once

#include "Pattern.h"

#include <vector>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <memory>

// PSequence
class PSequence : public Pattern
{
public:
    PSequence(const std::vector<double>& sequence, int repeats = std::numeric_limits<int>::max())
        : sequence(sequence), repeats(repeats), pos(0), rcount(0)
    {
        if (sequence.empty())
            throw std::invalid_argument("Sequence must not be empty");
    }

    void reset() override
    {
        pos = 0;
        rcount = 0;
    }

    double next() override
    {
        if (sequence.empty() || rcount >= repeats)
            throw std::out_of_range("Sequence exhausted");

        double value = sequence[pos];
        pos++;
        if (pos >= sequence.size())
        {
            pos = 0;
            rcount++;
        }
        return value;
    }

private:
    std::vector<double> sequence;
    int repeats;
    int pos;
    int rcount;
};

// PSeries
class PSeries : public Pattern
{
public:
    PSeries(double start, double step, int length = std::numeric_limits<int>::max())
        : start(start), step(step), length(length), value(start), count(0) {}

    void reset() override
    {
        value = start;
        count = 0;
    }

    double next() override
    {
        if (count >= length)
            throw std::out_of_range("Series exhausted");

        double current = value;
        value += step;
        count++;
        return current;
    }

private:
    double start;
    double step;
    int length;
    double value;
    int count;
};

// PRange
class PRange : public Pattern
{
public:
    PRange(double start, double end, double step)
        : start(start), end(end), step(step), value(start) {}

    void reset() override
    {
        value = start;
    }

    double next() override
    {
        if ((step > 0 && value >= end) || (step < 0 && value <= end))
            throw std::out_of_range("Range exhausted");

        double current = value;
        value += step;
        return current;
    }

private:
    double start;
    double end;
    double step;
    double value;
};

// PGeom
class PGeom : public Pattern
{
public:
    PGeom(double start, double multiply, int length = std::numeric_limits<int>::max())
        : start(start), multiply(multiply), length(length), value(start), count(0) {}

    void reset() override
    {
        value = start;
        count = 0;
    }

    double next() override
    {
        if (count >= length)
            throw std::out_of_range("Geometric series exhausted");

        double current = value;
        value *= multiply;
        count++;
        return current;
    }

private:
    double start;
    double multiply;
    int length;
    double value;
    int count;
};

// PImpulse
class PImpulse : public Pattern
{
public:
    PImpulse(int period) : period(period), pos(0) {}

    void reset() override
    {
        pos = 0;
    }

    double next() override
    {
        int current = (pos == 0) ? 1 : 0;
        pos = (pos + 1) % period;
        return current;
    }

private:
    int period;
    int pos;
};

// PLoop
class PLoop : public Pattern
{
public:
    PLoop(std::shared_ptr<Pattern> pattern, int count = std::numeric_limits<int>::max())
        : pattern(pattern), count(count), loopIndex(0), pos(0), readAll(false) {}

    void reset() override
    {
        pattern->reset();
        loopIndex = 0;
        pos = 0;
        values.clear();
        readAll = false;
    }

    double next() override
    {
        if (!readAll)
        {
            try
            {
                values.push_back(pattern->next());
            }
            catch (const std::out_of_range&)
            {
                readAll = true;
            }
        }

        if (readAll && pos >= values.size())
        {
            if (loopIndex >= count - 1)
                throw std::out_of_range("PLoop exhausted");
            loopIndex++;
            pos = 0;
        }

        return values[pos++];
    }

private:
    std::shared_ptr<Pattern> pattern;
    int count;
    int loopIndex;
    size_t pos;
    bool readAll;
    std::vector<double> values;
};

