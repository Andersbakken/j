CXXFLAGS += "-I/usr/include/biff/";

addLibrary({name:"libbiff.so",
            sources:"lib.cpp",
            cxxflags:"-g -I.",
            ldflags:"-shared"});
addApplication({ sources:"main.cpp,foo.cpp",
                 name:"appsilon",
                 ldflags:"-L. -lbiff",
                 cxxflags:"-g -O3",
                 depends:"libbiff.so"});
