#!/bin/bash

function followSymlink()
{
    python -c "import os, sys; print os.path.realpath(\"$1\")"
}

jdotjs=`followSymlink $BASH_SOURCE | sed -e 's,j$,j.js,'`

nodejs $jdotjs "$@"