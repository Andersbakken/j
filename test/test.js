CXXFLAGS += "-I/usr/include/biff/";

addLibrary({name:"biff",
            sources:"lib.cpp",
            cxxflags:"-g -I."});
addApplication({ sources:"main.cpp,foo.cpp",
                 name:"appsilon",
                 ldflags:"-lbiff",
                 cxxflags:"-g -O3" });
