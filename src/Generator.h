#ifndef Generator_h
#define Generator_h

#include <rct/Path.h>
#include "J.h"

class Generator
{
public:
    Generator() {}
    virtual ~Generator() {}

    virtual bool generate(const J &j, const Path &output) = 0;
    virtual String defaultOutput() const = 0;
};

#endif
