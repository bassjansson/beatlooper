#!/bin/bash

# exit when any command fails
set -e

# cd to directory of script
cd "${0%/*}"

# build program
cd ./src
make touch
make
echo

# run program
../bin/beatlooper $1 $2 $3 $4
