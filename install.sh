#!/bin/bash

build_generator="Unix Makefiles"
executable="chip8"
run_tests=false

for arg in "$@"
do
    case "$arg" in
        -r)
            rm -rv build/
            exit 0
            ;;
        -G)
            build_generator="$2"
            shift
            shift
            ;;
        -t)
            run_tests=true
            shift
            ;;

    esac
done

# Create build directory
mkdir -p build/buildfiles

# Generate build files
cd build/buildfiles
cmake ../.. -G "$build_generator"

# Build project
case "$build_generator" in
    "Unix Makefiles")
        make
        ;;
    "Ninja")
        ninja
        ;;
    *)
        echo "Unknown build tool"
        exit 1
        ;;
esac

# Run tests if required
if  [ "$run_tests" = true ];
then
    ctest
fi

# Copy result file to build folder
mv "$executable" ..
