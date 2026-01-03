#!/bin/bash

set -e

# Note: this shell script is executed during the Docker container's runtime

# run the analyzer on a test case
echo "Running the analyzer..."
cd analyzer/build

# note that we create "output.json" representing the output of the analyzer program
# and copy it to a file named "input.json" representing the file name to the input visualization
# this is to make the divide between the two sides explicit and de-coupled

if [ -d /usr/src/tests ]; then
    ./bin/dsa -extra-arg=-resource-dir=/usr/bin/../lib/clang/21 "/usr/src/tests/$1" -o output.json
else
    ./bin/dsa -extra-arg=-resource-dir=/usr/bin/../lib/clang/21 "/usr/src/$1" -o output.json
fi

# move the json to the correct location
echo "Moving json output..."
rm ../../frontend/input/input.json
mv output.json ../../frontend/input/input.json

# startup vite server
echo "Starting vite server..."
cd ../../frontend
npm run-script dev
