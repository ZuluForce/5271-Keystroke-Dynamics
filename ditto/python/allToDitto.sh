#!/bin/bash

# Reduce
python reducer/db_to_fs.py -o data

# Partition
python partitioner/partition.py -o data/partitions --pname main data/*.json

# Convert to ditto
python json_to_ditto.py -o data/partitions/main data/partitions/main/*.json

# Print partition info
python analysis/partition_set_info.py data/partitions/main
