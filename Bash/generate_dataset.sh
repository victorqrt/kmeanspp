#!/bin/bash

rand()
{
    echo $((RANDOM % 1000))
}

SIZE=$1

if [ $# -ne 1 ]; then
    echo -e "Usage:\n    $0 dataset_size"
    exit 1
fi

for ((i=0; i<$1; i++)); do
    echo $(rand),$(rand) >> dataset.csv
done
