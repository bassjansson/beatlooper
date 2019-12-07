#!/bin/bash

# exit when any command fails
set -e

# cd to directory of script
cd "${0%/*}"

# build program
make fractions-generator
echo

# run program
./fractions-generator > ../src/Fractions.h
echo Fractions generated.
