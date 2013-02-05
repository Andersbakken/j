// j.js


// output:
// var target = {
//     name:"test",
//     SOURCES: ['main.cpp', 'foo.cpp', {source:test.cpp, CXXFLAGS:"-g $CXXFLAGS"}, {source:utenstandardflags.cpp, CXXFLAGS:"-O3"} ],
//     CXXFLAGS: "-O2",
//     type:"library"
// }
// syntax:

console.log("doing shit 1");
SOURCES += "main.cpp foo.cpp";
console.log("doing shit 2");
CXXFLAGS = "-02";
console.log("doing shit 3");
TYPE = "library";
console.log("doing shit 4");
NAME = "test";
console.log("doing shit 5");


