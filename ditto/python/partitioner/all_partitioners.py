from fixed_partitioner import FixedPartitioner

partitioners = {
        "fixed" : FixedPartitioner,
        "scaled" : FixedPartitioner(nonfixed=True),
        "other" : FixedPartitioner
        }
