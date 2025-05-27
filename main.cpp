#include <string>
#include <cstring>
#include <stdio.h> 
#include <stdlib.h> 
#include "cache.h"

//checks if string contains digits
bool isInteger (std::string x) {
    for (int i = 0;i < x.size();i ++) {
        if (x[i] < '0' || x[i] > '9') return false ;
    }
    return true ;
}

int log2 (int pow2) {
    int l2 = -1 ;
    while (pow2) {
        l2 ++ ;
        pow2 = pow2 >> 1 ;
    }
    return l2 ;
}
//Main Function
int main (int argn,char** argv) {
    try {
        char buffer[100];
        cache simCache;
        std::string USAGE = "USAGE: ./cache-sim-os <trace-file> <associativity> <cache-size> <replacement-algorithm-#> [-t]\n"
                            "replacement-algorithms:\n"
                            "   0:  FIFO\n"
                            "   1:  LRU\n"
                            "   2:  optimal\n"
                            "   3:  clock\n"
                            "   4:  clock2\n"
                            "   5:  LIFO\n"
                            "options:\n"
                            "   -t      show the tag for every memory access + the image of the set\n" ;
                            
        
        int j = 1 ;
        for (int i = 1;i < argn;i ++) {
            if (argv[i][0] == '-') {//special option to show tag output
                if (argv[i][1] == 't') simCache.debug = true ;
            }
            else {
                argv[j] = argv[i] ;//ignores the "-t" argument
                j ++ ;
            }
        }
        argn = j ; 
    
        if (argn != 5) throw USAGE ; //validates number of arguments
        for (int i = 2;i < argn;i ++) { //checks if argument is integers
            if (!isInteger (argv[i])) throw USAGE ;
        }
        if (atoi (argv[3]) < 0) throw USAGE ; //throw error if cache size is negative
        if (atoi (argv[4]) % 10 < 0) throw USAGE ; 
        if (atoi (argv[4]) % 10 > 5) throw USAGE ;
        
        //Initialize cache configuration
        simCache.totalBlocks   = 8 ;
        simCache.associativity = atoi(argv[2]) ;
        simCache.totalSize = atoi(argv[3]);
        simCache.numAlgorithm = atoi(argv[4]) % 10 ;
        
        //checks for valid block and set configuration
        if (simCache.totalBlocks > simCache.totalSize) {
            throw std::string ("cache can not hold that many blocks") ; //throws error if their is too many blocks
        }
        if (simCache.associativity > simCache.totalBlocks) {
            throw std::string ("set size is larger than total number of blocks") ; //throws error is N is larger than total blocks
        }
        simCache.blockOffsetBits = log2 (simCache.totalSize/simCache.totalBlocks) ;
        simCache.setIndexBits = log2 (simCache.totalBlocks/simCache.associativity) ;
        simCache.time = 0 ;//Simulation clock begins

        //Open the trace file
        FILE* tf = fopen(argv[1], "r"); 
        char *valid = fgets(buffer, 100, tf);
        //Read each line of memory addresses from the trace file
        while(valid){
            char* addr_s = strtok(valid," /t/n"); //Get address string
            size_t addr = strtoull(addr_s, nullptr, 16);//Convert String hex address to Integer
            simCache.read(addr,tf);//All accesses are set to read
            simCache.time ++ ;
            valid = fgets(buffer, 100, tf); //Read the next line
        }
        fclose(tf);//Trace file can be closed
        
        //Calculate Statistics
        int totalAccesses = simCache.totalHits + simCache.totalMisses ;
        int hit_rate = 100.0 * simCache.totalHits /totalAccesses ;
        int miss_rate = 100.0 * simCache.totalMisses /totalAccesses ;
        int blocksize = simCache.totalSize /simCache.totalBlocks ;
        
        //Print Simulation Results
        printf ("cache-blocksize:           %d bytes\n",blocksize) ;
        printf ("hit-rate:                  %d%%\n",hit_rate) ;
        printf ("miss-rate:                 %d%%\n",miss_rate) ;
        printf ("total accesses:            %d\n",totalAccesses) ;
        printf ("total misses:              %d\n",simCache.totalMisses) ;
        printf ("total cold misses:         %d\n",simCache.totalColdMisses) ;
        printf ("total conflict misses:     %d\n",simCache.totalConflictMisses) ;
        printf ("total capacity misses:     %d\n",simCache.totalCapacityMisses) ;

        return 0 ;
    }
    catch (std::string excp) {
        printf ("%s",excp.c_str()) ; //print any error messages
        return 1 ;
    }
}
