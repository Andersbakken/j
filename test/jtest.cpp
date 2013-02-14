#include <J.h>

extern "C" {
void build(J *j)
{
    Target target("foo", Target::Application);
    target.addSource(Source("main.cpp"));
    j->targets.append(target);
}
}
