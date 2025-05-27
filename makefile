srcs = cacheOperations.cpp main.cpp replacementAlg.cpp

cache-sim-os: $(srcs)
	 g++ -g $(srcs) -o cache-sim-os
    
.: cache-sim-os
