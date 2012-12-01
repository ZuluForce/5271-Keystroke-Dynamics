#!/bin/bash

function copyToBin() {
  echo "Copying generated profiles to the ditto bin"
  cp -v data/partitions/main/*.ditto ../bin

  mkdir ../bin/scaled
  cp -v data/partitions/main_scaled/*.ditto ../bin/scaled/
}

DO_REDUCE=1
DO_PARTITION=1

for var in "$@"
do
  if [[ $var == "skip" ]]; then
    copyToBin
    exit 1
  fi

  if [[ $var == "no-reduce" ]]; then
    DO_REDUCE=0
  fi

  if [[ $var == "no-partition" ]]; then
    DO_PARTITION=0
  fi
done

# Reduce
if [[ $DO_REDUCE -gt 0 ]]; then
  echo "Reducing profiles from database"
  python reducer/db_to_fs.py -o data
  if [[ $? -ne 0 ]]; then
    echo "Failed to reduce profiles"
    exit 1
  fi
fi

if [[ $DO_PARTITION -gt 0 ]]; then
  # Partition
  echo "Partitioning profiles using the fixed partitioner"
  python partitioner/partition.py -pt fixed -o data/partitions --pname main data/*.json

  if [[ $? -ne 0 ]]; then
	echo "Failed to partition reduced profiles (fixed)"
	exit 1
  fi

  echo "Partitioning profiles using the scaled partitioner"
  python partitioner/partition.py -pt scaled -o data/partitions --pname main_scaled data/*.json

  if [[ $? -ne 0 ]]; then
	echo "Failed to partition reduced profiles (scaled)"
	exit 1
  fi
fi

# Convert to ditto
echo "Converting profiles to ditto format"
python json_to_ditto.py --ignore_unmapped -o data/partitions/main data/partitions/main/*.json

if [[ $? -ne 0 ]]; then
	echo "Failed to convert json profiles to ditto"
	exit 1
fi

echo "Converting profiles from scaled partitioning to ditto format"
python json_to_ditto.py --ignore_unmapped -o data/partitions/main data/partitions/main_scaled/*.json

if [[ $? -ne 0 ]]; then
	echo "Failed to convert json profiles to ditto (sclaed partition profiles)"
	exit 1
fi

# Print partition info
python analysis/partition_set_info.py data/partitions/main
python analysis/partition_set_info.py data/partitions/main_scaled

if [[ $# -gt 0 && $1 == "deploy" ]]; then
  copyToBin
fi
