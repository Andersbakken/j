#include "Generator.h"
#include "J.h"
#include "NinjaGenerator.h"
#include <dlfcn.h>
#include <getopt.h>
#include <errno.h>
#include <memory>
#include <rct/Path.h>
#include <rct/Process.h>
#include <rct/Rct.h>
#include <stdio.h>

static void usage(FILE *f)
{
    fprintf(f,
            "j [options] [J]\n"
            "  --help|-h                    Display this help\n"
            "  --verbose|-v                 Increase verbosity\n"
            "  --generator|-g [generator]   Pick generator (default is ninja)\n"
            "  --output|-o [output]         Where to write build file (dir or file)\n"
            "  --includepath|-I [path]      To use additional include paths for J\n"
            "  --define|-D [define]         Pass additional define to J\n"
            "  --library|-l [library]       Link J against this library\n"
            "  --library-path|-L [libpath]  Add additional library path for J\n"
            "  --log-file|-O [logfile]      Log to this file\n");
}

int main(int argc, char **argv)
{
    Rct::findApplicationDirPath(*argv);
    option options[] = {
        { "help", no_argument, 0, 'h' },
        { "verbose", no_argument, 0, 'v' },
        { "generator", required_argument, 0, 'g' },
        { "output", required_argument, 0, 'o' },
        { "includepath", required_argument, 0, 'I' },
        { "define", required_argument, 0, 'D' },
        { "library", required_argument, 0, 'l' },
        { "library-path", required_argument, 0, 'L' },
        { "log-file", required_argument, 0, 'O' },
        { 0, no_argument, 0, 0 }
    };
    int logLevel = Error;
    Path output, file, logFile;
    List<String> args;
    std::auto_ptr<Generator> generator(new NinjaGenerator);
    const String shortOptions = Rct::shortOptions(options);
    while (true) {
        const int c = getopt_long(argc, argv, shortOptions.constData(), options, 0);
        if (c == -1)
            break;
        switch (c) {
        case 'h':
            usage(stdout);
            return 0;
        case 'g':
            if (strcmp(optarg, "ninja")) {
                error("Only ninja is supported so far for --generator");
                return 1;
            }
            break;
        case 'O':
            logFile = optarg;
            break;
        case 'v':
            ++logLevel;
            break;
        case 'o':
            output = optarg;
            break;
        case 'I':
        case 'D':
        case 'l':
        case 'L':
            args << String::format<64>("-%c%s", c, optarg);
            break;
        }
    }
    initLogging(logLevel, logFile, 0);
    if (optind + 1 != argc) {
        error("No J!");
        return 1;
    }

    file = argv[optind];
    if (!file.isFile()) {
        error("J doesn't seem to exist [%s]", file.constData());
        return 1;
    }

    char buf[128];
    strcpy(buf, "/tmp/j_XXXXXX.so");
    int fd = mkstemps(buf, 3);
    if (fd <= 0) {
        error("Can't create temporary file %s", strerror(errno));
        return 1;
    }
    close(fd);
    Path so = buf;
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
