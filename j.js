// j.js


// output:
// var target = {
//     name:"test",
//     SOURCES: ['main.cpp', 'foo.cpp', {source:test.cpp, CXXFLAGS:"-g $CXXFLAGS"}, {source:utenstandardflags.cpp, CXXFLAGS:"-O3"} ],
//     CXXFLAGS: "-O2",
//     type:"library"
// }
// syntax:

SOURCES += "main.cpp foo.cpp";
CXXFLAGS = "-02";
TYPE = "library";
NAME = "test";


