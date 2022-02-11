#!/bin/bash
#
# Validate compilation of each FEATURE from 0 to NUM_FEATURES on a
# Linux/MacOS/FreeBSD host machine using EpoxyDuino. This allow us to catch
# compile-time errors quickly and in the GitHub Actions CI using 'make' instead
# of compiling to the target platform.

set -eu

PROGRAM_NAME='MemoryBenchmark.ino'
NUM_FEATURES=9  # excluding FEATURE_BASELINE
temp_out_file=

function cleanup() {
    if [[ "$temp_out_file" != '' ]]; then
        rm -f $temp_out_file
    fi
    sed -i -e "s/#define FEATURE [0-9]*/#define FEATURE 0/" $PROGRAM_NAME
}

function create_temp_file() {
    temp_out_file=$(mktemp /tmp/memory_benchmark.epoxy.XXXXXX)
}

function validate() {
    for feature in $(seq 0 $NUM_FEATURES); do
        echo "Validating FEATURE $feature using EpoxyDuino"
        sed -i -e "s/#define FEATURE [0-9]*/#define FEATURE $feature/" \
            $PROGRAM_NAME

        # Run make, redirecting output to temp file. If error, print the output.
        if ! make 2>&1 > $temp_out_file; then
            cat $temp_out_file
            exit 1
        fi
    done
}

trap "cleanup" EXIT

create_temp_file
validate
