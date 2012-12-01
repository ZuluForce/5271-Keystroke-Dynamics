import sys
from abstract_partitioner import AbstractPartitioner

class FixedPartitioner(AbstractPartitioner):
    def __init__(self, nonfixed=False):
        self.partionSize = 10  # Groups of people withing 10 wpm
        self.lowestPartitionWPM = 20  #No group will have less than 20 wpm
        self.lowestPartitionNum = 2
        
        self.slidingPartitions = nonfixed
        self.slidingWPM = [25, 40, 55, 65, 75, 85, 95, 100, 105, 108, 115, 120]


    def partitionProfiles(self, profiles):
        partition_profiles = {}
        user_to_partition = {}
        
        # Iterate through each person and put them into a partition
        for user,data in profiles.items():
            wpm = int(data['wpm'])
            partition = self.getPartitionNumFromWPM(wpm);
            
            user_to_partition[user] = partition
            
        # Iterate through the users in each group and find the one with the
        # lowest wpm to be the partition's profile
        for user, partition in user_to_partition.items():
            if partition in partition_profiles:
                currentPartitionWPM = partition_profiles[partition]
                userWPM = profiles[user]['wpm']
                
                # We should add some logic for a variance check. We should prefer
                # a profile that has the same WPM but less variance in times
                if userWPM < currentPartitionWPM:
                    self.setPartitionProfile(partition_profiles, profiles[user], partition)
                    partition_profiles[partition]['source'] = user
            else:
                # This is the first user encountered for this partition so just
                # add their data as the partition's profile
                self.setPartitionProfile(partition_profiles, profiles[user], partition)
                partition_profiles[partition]['source'] = user
                
        if self.slidingPartitions:
            user_to_partition['partitioner'] = "scaled"
        else:
            user_to_partition['partitioner'] = "fixed"
                
        return (partition_profiles, user_to_partition)
            
    
    def getPartitionNumFromWPM(self, wpm):
        if not self.slidingPartitions:
            for index, maxWPM in enumerate(self.slidingWPM):
                if wpm < maxWPM:
                    return index
                
            ## Means their typing speed is beyond what we have defined
            return len(self.slidingWPM)
        else:
            partition = int((wpm / self.partionSize) + 1)
            return max(partition, self.lowestPartitionNum)


    def setPartitionProfile(self, partitionDict, profile, number):
        if number not in partitionDict:
            partitionDict[number] = {}

        if 'text' not in profile:
            print("Cannot set profile for partition. Provided profile not in expected format (no text section)")
            sys.exit(2)

        partitionDict[number] = profile['text']
        #partitionDict[number]['fly_times'] = profile['text']['fly_times']
        #partitionDict[number]['press_times'] = profile['text']['press_times']
        partitionDict[number]['wpm'] = profile['wpm']
        
    def __call__(self):
        return self