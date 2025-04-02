#!/bin/bash

OUTPUT_FILE="tlb_results.txt"

> "$OUTPUT_FILE"

PAGES=2

# 2^14
MAX_PAGES=4096

echo "Running TLB measurements..."

while [ $PAGES -le $MAX_PAGES ]; do
    echo "Testing with $PAGES pages..."

    RESULT=$(./build/tlb $PAGES)

    echo "$PAGES pages | $RESULT" >> "$OUTPUT_FILE"

    PAGES=$((PAGES * 2))
done

echo "Results saved to $OUTPUT_FILE"