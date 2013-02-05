CXXFLAGS += "-I/usr/include/biff/";
LDFLAGS += "-lbiff";

addLibrary({name:"kaploik",
            sources:"lib.cpp",
            cxxflags:"-g -I."});
addApplication({ sources:"main.cpp,foo.cpp",
                 name:"appsilon",
                 cxxflags:"-g -O3" });
