// ninja module

function NinjaInternals()
{
    this._compileRule = function(name, command, flags) {
        return "rule " + name + "\n" +
               "  depfile = $out.d\n" +
               "  command = " + command + " $" + flags + " $in -o $out -MMD -MF $out.d\n";
    };
    this._linkRule = function(name, command, flags) {
        return "rule " + name + "\n" +
            "  command = " + command + " $in -o $out $" + flags + "\n";
    };
    this._build = function(t) {
        var o = [];
        var r = this._guessRule(t.source) || t.rule;

        var tn = t.target || (t.source + ".o");
        if (!r) {
            console.log("Unknown rule for " + t.source);
            return "";
        }
        if (r === "cxx")
            this.linker = "cxxlink";

        o.push("build " + tn + ": " + r + " " + t.source);
        for (var i in t.flags) {
            if (t.flags[i])
                o.push("  " + i + " = " + t.flags[i]);
        }
        return this._string(o);
    };
    this._string = function(o) {
        var str = "";
        for (var i in o)
            str += o[i] + "\n";
        return str;
    };
    this._guessRule = function(source) {
        if (!typeof source === "string")
            return undefined;
        var dot = source.lastIndexOf(".");
        if (dot == -1)
            return undefined;
        var ext = source.substr(dot);
        var rule = this._rules[ext];
        if (!rule)
            return undefined;
        return rule;
    };
    this._rules = { ".cpp": "cxx", ".cxx": "cxx", ".cc": "cxx", ".c": "cc" };
    this.linker = "cclink";
};

module.exports = {
    fileName: "build.ninja",
    generate: function(obj) {
        var nin = new NinjaInternals();

        var i, o = [];
        for (i in obj.variables) {
            o.push(i + " = " + obj.variables[i]);
        }
        if (obj.cxxflags)
            o.push("cxxflags = " + obj.cxxflags);
        if (obj.cflags)
            o.push("cflags = " + obj.cflags);
        if (obj.ldflags)
            o.push("ldflags = " + obj.ldflags);
        var cxx = obj.cxx || "g++";
        var cc = obj.cc || "gcc";
        o.push(nin._compileRule("cxx", cxx, "cxxflags -c"));
        o.push(nin._compileRule("cc", cc, "cflags -c"));
        o.push(nin._linkRule("cclink", cc, "ldflags"));
        o.push(nin._linkRule("cxxlink", cxx, "ldflags"));
        for (i in obj.targets) {
            var t = obj.targets[i];
            var all = "";
            for (var s in t.sources) {
                var source = t.sources[s];
                if (typeof source === "string") {
                    o.push(nin._build({source: source}));
                    all += source + ".o ";
                } else if (typeof source === "object") {
                    o.push(nin._build({source: source.source, flags: { cxxflags: source.cxxflags, cflags: source.cflags }}));
                    all += source.source + ".o ";
                }
            }
            if (t.depends)
                all += "| " + t.depends;
            o.push(nin._build({source: all, flags: { ldflags: t.ldflags }, rule: nin.linker, target: t.name}));
        }
        return nin._string(o);
    }
};

//console.log(module.exports.generate({cxxflags: "-I hepp", targets: [{name: "test", sources: ["a.cpp", {source: "b.cpp", cxxflags: "-I ting" }]}]}));
