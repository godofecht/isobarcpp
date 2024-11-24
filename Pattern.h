#include <iostream>
#include <vector>
#include <stdexcept>
#include <memory>
#include <string>

class PRange
{
public:
    PRange (double start = 0, double end = 128, double step = 1)
        : start (start), end (end), step (step), currentValue (start)
    {
        if (step == 0)
        {
            throw std::invalid_argument ("Step value cannot be zero.");
        }
    }

    // Reset the range to the starting value
    void reset()
    {
        currentValue = start;
    }

    // Get the next value in the range
    double next()
    {
        if ((step > 0 && currentValue >= end) || (step < 0 && currentValue <= end))
        {
            throw std::out_of_range ("Iteration exceeds range bounds.");
        }

        double returnValue = currentValue;
        currentValue += step;
        return returnValue;
    }

    // Fetch the next N values in the range
    std::vector<double> nextN(size_t count)
    {
        std::vector<double> values;
        for (size_t i = 0; i < count; ++i)
        {
            try
            {
                values.push_back(next());
            }
            catch (const std::out_of_range &)
            {
                break; // Stop iteration when range is exceeded
            }
        }
        return values;
    }

    // Get the current value without advancing
    double value() const
    {
        return currentValue;
    }

    std::string toString() const
    {
        return "PRange(" + std::to_string(start) + ", " + std::to_string(end) + ", " + std::to_string(step) + ")";
    }

private:
    double start;
    double end;
    double step;
    double currentValue; // Internal state to track the current value
};