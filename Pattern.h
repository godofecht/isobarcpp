#ifndef PATTERN_H
#define PATTERN_H

#include <vector>
#include <stdexcept>
#include <limits>
#include <memory>

class Pattern
{
public:
    virtual ~Pattern() = default;
    virtual void reset() = 0;
    virtual double next() = 0;
};

#endif // PATTERN_H