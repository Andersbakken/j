#include "Generator.h"
#include "J.h"
#include "NinjaGenerator.h"
#include <dlfcn.h>
#include <memory>
#include <rct/Path.h>
#include <rct/Process.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    std::auto_ptr<Generator> generator(new NinjaGenerator);
    Path file;
    Path output = generator->defaultOutput();
    List<String> args;
    for (int i=1; i<argc; ++i) {
        Path p = argv[i];
        if (p.isFile()) {
            file = p;
        } else {
            args << p;
        }
    }
    if (file.isEmpty()) {
        error("Nothing to do");
        return 1;
    }
    Path so = tmpnam(0);
    Path includeJ = J_ROOT;
    includeJ.prepend("-I");
    includeJ += "/src";
    Path includeRct = J_ROOT;
    includeRct.prepend("-I");
    includeRct += "/rct/src";
    const int ret = system(String::format<256>("/usr/bin/g++ -shared -rdynamic -fpic %s %s %s %s -o %s",
                                               includeJ.constData(), includeRct.constData(),
                                               file.constData(), String::join(args, " ").constData(),
                                               so.constData()).constData());
    if (ret) {
        error("Failed to compile %d", ret);
        return 1;
    } else {
        void *lib = dlopen(so.constData(), RTLD_LAZY);
        if (!lib) {
            error("Can't open resulting library %s", so.constData());
            return 2;
        }
        typedef void (*Build)(J *);
        Build build = reinterpret_cast<Build>(dlsym(lib, "build"));
        if (!build) {
            error("Can't dlsym \"build\"");
            return 3;
        }
        J j;
        build(&j);
        generator->generate(j, output);
        // printf("Got shit %p %s\n", lib, dlerror());
        unlink(so.constData());
    }
    return 0;
}
