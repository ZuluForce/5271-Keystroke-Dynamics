#!/bin/bash

LOGDIR="log"

echo "Gathering information on per user reduced profiles"
python analysis/reduced_profile_info.py data/*.json > ${LOGDIR}/reduced_profiles.txt

echo "Gathering information on fixed partition profiles"
python analysis/reduced_profile_info.py data/partitions/main/*.json > ${LOGDIR}/fixed_partition_profiles.txt

echo "Gathering information on scaled partition profiles"
python analysis/reduced_profile_info.py data/partitions/main_scaled/*.json > ${LOGDIR}/scaled_partition_profiles.txt

echo "Gathering meta information on the set of fix partitions"
python analysis/partition_set_info.py data/partitions/main > ${LOGDIR}/fixed_partition_info.txt

echo "Gathering meta information on the set of scaled partitions"
python analysis/partition_set_info.py data/partitions/main_scaled > ${LOGDIR}/scaled_partition_info.txt

echo "Gathering average fly time for all ditto profiles (fixed partition)"
python analysis/inspectDittoProfile.py short data/partitions/main/*.ditto > ${LOGDIR}/fixed_ditto_info.txt

echo "Gathering average fly time for all ditto profiles (scaled partition)"
python analysis/inspectDittoProfile.py short data/partitions/main_scaled/*.ditto > ${LOGDIR}/sclaed_ditto_info.txt
