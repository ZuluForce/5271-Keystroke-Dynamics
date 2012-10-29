from abstract_partitioner import AbstractPartitioner

class FixedPartitioner(AbstractPartitioner):
    def partitionProfiles(self, fly_times, press_times):
        print("FixedPartioner partitionProfiles method")
