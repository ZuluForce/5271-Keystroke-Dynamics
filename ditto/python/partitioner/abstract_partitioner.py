

class AbstractPartitioner:
    """
    This class defines the basic interface for a partitioner.
    """

    def partitionProfiles(self, fly_times, press_times):
        """
        Args:
            fly_times : A map of profiles to the nested maps (dictionaries) that hold a given
                profile's fly times. The keys for the main dictionary shall identify a profile
                uniquely since they will be used later to identify the partitions.

            press_times: A map of profiles to a map of the profile's press times.

        Return:
            A map of partitons to a list of the profiles in that partition. The profiles will
            be identified by the keys used in the fly_time and press_time dictionaries.
        """

        print("This is unimplemented partitionProfiles method. This class should be extended.")
        return
