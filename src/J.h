#ifndef J_h
#define J_h

#include <rct/String.h>
#include <rct/List.h>

class Target
{
public:

};

class Source
{
public:
    Source(const Path &file) : mSource(file) {}

    enum FlagsMode {
        IncludeStandard,
        ExcludeStandard
    };

    void setFlags(const List<String> &extraFlags, FlagsMode mode = IncludeStandard)
    {
        mFlagsMode = mode;
        mFlags = extraFlags;
    }
    Path sourceFile() const { return mSource; }
    List<String> flags() const { return mFlags; }
    FlagsMode flagsMode() const { return mFlagsMode; }
private:
    Path mSource;
    List<Path> mFlags;
    FlagsMode mFlagsMode;
};

class J
{
public:
    List<Target> targets;
};


#endif
