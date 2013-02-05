// j.js

var fs = require('fs');
var path = require('path');
var foo = require('./foo');
foo.foobar(1);

for (var i=2; i<process.argv.length; ++i) {
    if (!build(process.argv[i]))
        break;
}

function build(file)
{
    var SOURCES = "";
    var CXXFLAGS = "";
    var CFLAGS = "";
    var LDFLAGS = "";
    var TYPE = "app";
    var NAME = path.basename(file);
    if (NAME.length > 3 && NAME.substr(NAME.length - 3, 3) == ".js")
        NAME = NAME.substr(0, NAME.length - 3);
    var contents = fs.readFileSync(file).toString();
    try {
        var ret = eval(contents);
    } catch (err) {
        console.log("Caught exception in " + file);
        return false;
    }
    var sources;
    if (typeof SOURCES === "string") {
        sources = SOURCES.split(/ +/);
    } else if (typeof SOURCES == "object") {
        sources = SOURCES;
    }
    TYPE = TYPE.toLowerCase();
    if (TYPE == "app") {
        TYPE = "application";
    } else if (TYPE == "lib") {
        TYPE = "library";
    } else if (TYPE != "library" && TYPE != "application") {
        throw "Invalid target TYPE " + TYPE;
    }

    if (typeof CXXFLAGS == "string")
        CXXFLAGS = CXXFLAGS.split(/ +/);
    if (typeof CFLAGS == "string")
        CFLAGS = CFLAGS.split(/ +/);
    if (typeof LDFLAGS == "string")
        LDFLAGS = LDFLAGS.split(/ +/);

    if (!NAME.length)
        throw "Invalid target NAME";
    var target = { NAME: NAME,
                   SOURCES: sources,
                   CXXFLAGS: CXXFLAGS,
                   CFLAGS: CXXFLAGS,
                   LDFLAGS: CXXFLAGS,
                   TYPE: TYPE };
    console.log(JSON.stringify(target));
}
// fs.readFile(file, [encoding], [callback]);

// output:
// var target = {
//     name:"test",
//     SOURCES: ['main.cpp', 'foo.cpp', {source:test.cpp, CXXFLAGS:"-g $CXXFLAGS"}, {source:utenstandardflags.cpp, CXXFLAGS:"-O3"} ],
//     CXXFLAGS: "-O2",
//     type:"library"
// }
// syntax:

// SOURCES += "main.cpp foo.cpp";
// CXXFLAGS = "-02";
// TYPE = "library";
// NAME = "test";


