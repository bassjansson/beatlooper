#!/bin/bash

cd "${0%/*}"
cd ./src
make
../bin/beatlooper
