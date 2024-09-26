#!/bin/bash
line=`find . \( -name "*.cpp" -o -name "*.h" \) -exec cat {} + | wc -l`
echo $line