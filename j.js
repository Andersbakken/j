// j.js

var fs = require('fs');
var path = require('path');

var project;
var generator = "ninja";
var output = "./build.ninja";

for (var i=2; i<process.argv.length; ++i) {
    var arg = process.argv[i];
    if (arg == "-h" || arg == "--help") {
        console.log("Usage: j [options] projectfile.js\n" +
                    "Options:\n" +
                    "  --help|-h:                       Display this help\n" +
                    "  --output|-o [arg]                Output to this file (default ./build.ninja|./Makefile)\n" +
                    "  --generator|-g [generator]       Use this generator (default is ninja)\n");
        project = undefined;
        break;
    } else if (arg == "--output" || arg == "-o") {
        output = process.argv[++i];
    } else if (arg == "--generator" || arg == "-g") {
        generator = process.argv[++i];
    } else if (!project) {
        project = arg;
    } else {
        throw "Not sure what to do about " + arg;
    }
}

var builds = { targets: [],
               cxxflags: "",
               cflags: "",
               ldflags: "" };

function addTarget(target)
{
    if (typeof target.sources == "string")
        target.sources = target.sources.split(/[ ,]+/);
    builds.targets.push(target);
    // console.log(JSON.stringify(builds));
}
function addLibrary(target)
{
    target.type = "library";
    addTarget(target);
}

function addApplication(target)
{
    target.type = "application";
    addTarget(target);
}

if (project)
    build(project, output, generator);


function build(file, output, generator)
{
    var gen = require('./generators/' + generator);
    if (!gen || !gen.generate || typeof gen.generate != "function")
        throw "Invalid generator " + generator;
    var CXXFLAGS = "";
    var CFLAGS = "";
    var LDFLAGS = "";
    var contents = fs.readFileSync(file).toString();
    try {
        var ret = eval(contents);
    } catch (err) {
        console.log("Caught exception in " + file + " " + err.toString());
        return false;
    }
    if (typeof CXXFLAGS == "string" && CXXFLAGS.length)
        builds.cxxflags = CXXFLAGS;
    if (typeof CFLAGS == "string" && CFLAGS.length)
        builds.cxxflags = CFLAGS;
    if (typeof LDFLAGS == "string" && LDFLAGS.length)
        builds.cxxflags = LDFLAGS;
    var out = gen.generate(builds);
    if (typeof out == "string" && out.length) {
        if (fs.statSync(output).isDirectory())
            output += '/' + gen.fileName;

        fs.writeFileSync(output, out);
        console.log("Wrote output to " + output);
    }
}
