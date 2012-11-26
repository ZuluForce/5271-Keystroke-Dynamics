#!/bin/bash

# Reduce
python reducer/db_to_fs.py -o data
if [[ $? -ne 0 ]]; then
	echo "Failed to reduce profiles from database"
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
