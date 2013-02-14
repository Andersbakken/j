#ifndef J_h
#define J_h

#include <rct/String.h>
#include <rct/List.h>
#include <rct/Path.h>

class Source
{
public:
    Source(const Path &file = Path()) : mSource(file) {}

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

class Target
{
public:
    enum Type {
        Library,
        Application
    };
    Target(const String &name = String(), Type type = Library) : mName(name), mType(type) {}
    void addSource(const Source &source) { mSources.append(source); }
    List<Source> sources() const { return mSources; }
    void addDependency(const String &name) { mDependencies.append(name); }
    List<String> dependencies() const { return mDependencies; }
private:
    String mName;
    Type mType;
    List<Source> mSources;
    List<String> mDependencies;
};

class J
{
public:
    List<Target> targets;
};


#endif
