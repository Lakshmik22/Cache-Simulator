#pragma once
#include <stdio.h>
 
//Enumeration for different cache replacement policies
enum {FIFO, LRU, Optimal, Clock, Clock2, LIFO};

// Constants defining maximum sizes for cache configuration
enum{MAX_BLOCKSIZE = 1024, MAX_ASSOCIATIVTY = 8, MAX_SETS = 8};

// Structure representing a single block within a cache set
struct block{
    bool valid = false;
    bool clockbit = false; //write's "1" if this block gets accessed
    int  clockbit2 = 0;// "add" 1 if this block is accessed
    size_t tag;
    int timestamp; //last access time for LRU
    int timestamp_retrieval; //Access Time for FIFO & LIFO
};

//Cache Set that holds multiple blocks
struct set{
    block blocks[MAX_ASSOCIATIVTY]; 
    int   clockhand = 0 ; //Starts at block 0
};

class cache{
    public:
    bool allBlocksUsed(); //checks if all the blocks in the Cache are used

    void read(size_t ,FILE* trace);  //reads the cache from the trace file
    int  replacementAlgorithm (int setNumber,FILE* trace); //chooses a block to replace based on the 

    bool debug = false ;
    int numAlgorithm;
    //intitalize variables
    int tagBits;
    int blockOffsetBits;
    int setIndexBits; 
    int totalSize;
    int totalBlocks;
    int writeMode;
    int associativity;
    int totalHits = 0;
    int totalMisses = 0;
    int totalColdMisses = 0;
    int totalCapacityMisses= 0;
    int totalConflictMisses = 0;
    
    int time ;//time counter for LRU

    set sets[MAX_SETS];
};

