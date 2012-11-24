

class AbstractPartitioner:
    """
    This class defines the basic interface for a partitioner.
    """

    def partitionProfiles(self, profiles):
        """
        Args:
            profiles - The set of profiles for each user. Each user entry in this parameter should contain a
                map with press times, fly times and the calculated words per minute. 

        Return:
            A tuple ({partition profiles}, {users -> partition}). The first part of the tuple maps each
            partition number (1,2,3...) to their corresponding profile. The second piece of the tuple maps
            each user to the partition they are assigned.
        """

        print("This is the unimplemented partitionProfiles method. This class should be extended.")
        return
