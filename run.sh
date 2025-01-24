#!/bin/bash
EXECUTABLE=build/flux
BUILD_SCRIPT=./build-debug.sh

(test -f $EXECUTABLE) || ${BUILD_SCRIPT}
${EXECUTABLE}