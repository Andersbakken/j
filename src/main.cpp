#include <rct/Path.h>
#include <rct/Process.h>
#include <stdio.h>

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
        char *name = tmpnam(0);
        List<String> args;
        // g++ -c  jtest.cpp -shared -I../src/ -I../rct/src/ -o /tmp/lib.so
        Path includeJ = J_ROOT;
        includeJ.prepend("-I");
        includeJ += "/src";
        Path includeRct = J_ROOT;
        includeRct.prepend("-I");
        includeRct += "/rct/src";
        args << "-shared" << includeJ << includeRct << "-c" << mPath << "-o" << name;
        error() << "g++" << String::join(args, " ");
        error() << start("g++", args);
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
        printf("[%s] %s:%d: void onFinished() [after]\n", __func__, __FILE__, __LINE__);
    }
private:
    Path mPath;
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
