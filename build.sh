#!/bin/bash

rm -r dist
mkdir dist
cd dist

qmake ../click-test.pro
make all

echo
echo "Building complete"
