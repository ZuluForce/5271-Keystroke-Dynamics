#!/bin/bash

for var in "$@"
do
  if [[ var == "skip" ]]; then
    copyToBin
    exit -1
  fi
done

# Reduce
python reducer/db_to_fs.py -o data
if [[ $? -ne 0 ]]; then
  echo "Failed to reduce profiles"
  exit -1
fi

# Partition
python partitioner/partition.py -o data/partitions --pname main data/*.json

if [[ $? -ne 0 ]]; then
	echo "Failed to partition reduced profiles"
	exit -1
fi

# Convert to ditto
python json_to_ditto.py --ignore_unmapped -o data/partitions/main data/partitions/main/*.json

if [[ $? -ne 0 ]]; then
	echo "Failed to convert json profiles to ditto"
	exit -1
fi

# Print partition info
python analysis/partition_set_info.py data/partitions/main

if [[ $# -gt 0 && $1 == "deploy" ]]; then
  copyToBin
fi

function copyToBin() {
  echo "Copying generated profiles to the ditto bin"
  cp -v data/partitions/main/*.ditto ../bin
}
