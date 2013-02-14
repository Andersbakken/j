#ifndef J_h
#define J_h

#include <rct/String.h>
#include <rct/List.h>
#include <rct/Path.h>
#include <rct/Log.h>

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

inline Log operator<<(Log log, const Source &source)
{
    String out = "Source(";
    out += source.sourceFile();
    if (!source.flags().isEmpty()) {
        out += ", ";
        if (source.flagsMode() == Source::IncludeStandard) {
            out += "include: ";
        } else {
            out += "exclude: ";
        }
        out += String::join(source.flags(), " ");
    }
    out += ")";
    log << out;
    return log;
}

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
    Type type() const { return mType; }
    String name() const { return mName; }
private:
    String mName;
    Type mType;
    List<Source> mSources;
    List<String> mDependencies;
};

inline Log operator<<(Log log, const Target &target)
{
    String out = "Target(" + target.name() + ", ";
    out += target.type() == Target::Library ? "lib, " : "app, ";
    log << out;
    log << target.sources() << "dependencies" << target.dependencies();
    return log;
}

class J
{
public:
    List<Target> targets;
};

inline Log operator<<(Log log, const J &j)
{
    log << j.targets;
    return log;
}

#endif
