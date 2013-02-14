#include <rct/Path.h>
#include <rct/Process.h>
#include "J.h"
#include "Generator.h"
#include <stdio.h>
#include <dlfcn.h>

// class Compile : private Process
// {
// public:
//     Compile(const Path &path)
//     : mPath(path)
//     {
//         readyReadStdOut().connect(this, &Compile::onReadyReadStdOut);
//         readyReadStdErr().connect(this, &Compile::onReadyReadStdErr);
//         finished().connect(this, &Compile::onFinished);
//     }

//     void run()
//     {
//         mOutput = tmpnam(0);
//         List<String> args;
//         // g++ -c  jtest.cpp -shared -I../src/ -I../rct/src/ -o /tmp/lib.so
//         Path includeJ = J_ROOT;
//         includeJ.prepend("-I");
//         includeJ += "/src";
//         Path includeRct = J_ROOT;
//         includeRct.prepend("-I");
//         includeRct += "/rct/src";
//         args << "/usr/bin/g++" << "-shared" << "-rdynamic" << "-fpic" << includeJ << includeRct << mPath << "-o" << mOutput;
//         // error() << "g++" << String::join(args, " ");
//         // error() << start("/usr/bin/g++", args);
//         // int ret = system(
//         error() << start("bash", List<String>() << "-c" << String::format<256>("\"/usr/bin/g++ -shared -rdynamic -fpic %s %s %s -o %s\"",
//                                                                                includeJ.constData(), includeRct.constData(),
//                                                                                mPath.constData(), mOutput.constData()));
//     }

//     void onReadyReadStdOut()
//     {
//         error() << "got out" << readAllStdOut();
//     }

//     void onReadyReadStdErr()
//     {
//         error() << "got err" << readAllStdErr();
//     }

//     void onFinished()
//     {
//         error() << returnCode();
//         void *lib = dlopen(mOutput.constData(), RTLD_LAZY);
//         printf("Got shit %p %s\n", lib, dlerror());
//         printf("[%s] %s:%d: void onFinished() [after]\n", __func__, __FILE__, __LINE__);
//     }
// private:
//     Path mPath;
//     Path mOutput;
// };

int main(int argc, char **argv)
{
    // EventLoop loop;
    auto_ptr<Generator> generator;
    for (int i=1; i<argc; ++i) {
        Path p = argv[i];
        if (p.isFile()) {
            Path output = tmpnam(0);
            Path includeJ = J_ROOT;
            includeJ.prepend("-I");
            includeJ += "/src";
            Path includeRct = J_ROOT;
            includeRct.prepend("-I");
            includeRct += "/rct/src";
            const int ret = system(String::format<256>("/usr/bin/g++ -shared -rdynamic -fpic %s %s %s -o %s",
                                                       includeJ.constData(), includeRct.constData(),
                                                       p.constData(), output.constData()).constData());
            if (ret) {
                error("Failed to compile %d", ret);
                return 1;
            } else {
                void *lib = dlopen(output.constData(), RTLD_LAZY);
                if (!lib) {
                    error("Can't open resulting library %s", output.constData());
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
                generate(j);
                printf("Got shit %p %s\n", lib, dlerror());
            }
            // error() << "got shit" << ret << output;

            // Compile *compile = new Compile(p);
            // compile->run();
            // unlink(output.constData());
        }
    }
    // loop.run();
    return 0;
}
