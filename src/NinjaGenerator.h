#ifndef NinjaGenerator_h
#define NinjaGenerator_h

#include "Generator.h"

class NinjaGenerator : public Generator
{
public:
    virtual bool generate(const J &j, const Path &output)
    {
        error() << j;
        return true;
    }

    virtual String defaultOutput() const { return "build.ninja"; }
};

#endif
