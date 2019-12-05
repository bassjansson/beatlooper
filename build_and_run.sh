#!/bin/bash

# exit when any command fails
set -e

# cd to directory of script
cd "${0%/*}"

# build program
cd ./src
make touch
make

# run program
../bin/beatlooper
