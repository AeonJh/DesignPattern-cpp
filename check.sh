#!/bin/bash

for test in build/bin/*; do
    if [[ -x "$test" ]]; then
        echo "Running $test"
        "$test"
        if [[ $? -ne 0 ]]; then
            echo "Test $test failed"
            exit 1
        fi
    else
        echo "$test is not executable or does not exist"
    fi
done
