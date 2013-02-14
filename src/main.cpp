#include <rct/Path.h>
#include <rct/Process.h>
#include <stdio.h>
#include <dlfcn.h>

class Compile : private Process
{
public:
    Compile(const Path &path)
    : mPath(path)
    {
        readyReadStdOut().connect(this, &Compile::onReadyReadStdOut);
        readyReadStdErr().connect(this, &Compile::onReadyReadStdErr);
        finished().connect(this, &Compile::onFinished);
    }

    void run()
    {
        mOutput = tmpnam(0);
        List<String> args;
        // g++ -c  jtest.cpp -shared -I../src/ -I../rct/src/ -o /tmp/lib.so
        Path includeJ = J_ROOT;
        includeJ.prepend("-I");
        includeJ += "/src";
        Path includeRct = J_ROOT;
        includeRct.prepend("-I");
        includeRct += "/rct/src";
        args << "/usr/bin/g++" << "-shared" << "-rdynamic" << "-fpic" << includeJ << includeRct << mPath << "-o" << mOutput;
        // error() << "g++" << String::join(args, " ");
        // error() << start("/usr/bin/g++", args);


    }

    void onReadyReadStdOut()
    {
        error() << "got out" << readAllStdOut();
    }

    void onReadyReadStdErr()
    {
        error() << "got err" << readAllStdErr();
    }

    void onFinished()
    {
        error() << returnCode();
        void *lib = dlopen(mOutput.constData(), RTLD_LAZY);
        printf("Got shit %p %s\n", lib, dlerror());
        printf("[%s] %s:%d: void onFinished() [after]\n", __func__, __FILE__, __LINE__);
    }
private:
    Path mPath;
    Path mOutput;
};

int main(int argc, char **argv)
{
    EventLoop loop;
    for (int i=1; i<argc; ++i) {
        Path p = argv[i];
        if (p.isFile()) {
            Compile *compile = new Compile(p);
            compile->run();
        }
    }
    loop.run();
    return 0;
}
