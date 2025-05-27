#include <string>
#include <cstring>
#include "cache.h"

//FIFO Replacement Alogrithm
int replacementFIFO (set* s,int blockCount) {
    int oldest = 0 ; //Default
    for (int b = 0;b < blockCount;b ++) {
        if (s->blocks[b].timestamp_retrieval < s->blocks[oldest].timestamp_retrieval) {
            oldest = b ;
        }
    }
    return oldest;
}

//LRU Replacement Algorithm
int replacementLRU(set* s,int blockCount) {
    int oldest = 0 ; //Default
    for (int b = 0;b < blockCount;b ++) {
        if (s->blocks[b].timestamp < s->blocks[oldest].timestamp) {
            oldest = b ;
        }
    }
    
    return oldest ;
}
//For the Optimal Cache Replacement Algorithm
int nextReference (size_t addr_group,int blockOffsetBits,FILE* trace) {
    char buffer[100] ;
    int next = -1 ;
    int step = 0 ;
    int start_pos = ftell (trace) ;
    char *valid = fgets(buffer, 100, trace);
    while (valid) {
        step ++ ;
        char* addr_s = strtok(valid," /t/n");
        size_t addr = strtoull(addr_s, nullptr, 16);
 
        if (addr_group == ((addr >> blockOffsetBits)<<blockOffsetBits)) {
            next = step ;
            break ;
        }
        valid = fgets(buffer, 100, trace);
    }
    
    fseek (trace,start_pos,SEEK_SET) ; //Reset
    return next ;
}
size_t addressGroup  (set* s,cache* c,int b) {
    int setNumber = s - c->sets ; //array of sets
    size_t addr_suffix = setNumber << c->blockOffsetBits ; //Change the tag based on which block we are considering
    return (s->blocks[b].tag << (c->blockOffsetBits + c->setIndexBits)) | addr_suffix ;
}

//Optimal Replacement Algorithm
int replacementOptimal (set* s,cache* c,FILE* trace) {
    int longestOut = 0 ;//default
    int longestOutReference = 0 ;
    for (int b = 0;b < c->associativity;b ++) {
        size_t addr_group = addressGroup (s,c,b) ;
        int nr = nextReference (addr_group,c->blockOffsetBits,trace) ;
        if (nr == -1) {
            //the block is never referenced again
            longestOut = b ;
            longestOutReference = nr ;
            break ;
        }
        else if (nr > longestOutReference) {
            //block is referenced again but later than the current victim block
            longestOut= b ;
            longestOutReference = nr ;
        }
    } 
    return longestOut;   
}

//Clock Replacement Algorithm (1-Bit Clock)
int replacementClock (set* s,int blockCount) {
    int choice = -1 ;
    while (choice == -1) {//repeat at most once
        for (int i = 0;i < blockCount;i ++) {
           int h = s->clockhand ;
           if (s->blocks[h].clockbit == 0) { choice = h ; break ; }
           else s->blocks[h].clockbit = 0 ;
           s->clockhand = (s->clockhand + 1) % blockCount ; //Loop around this
        }
    }
    s->clockhand = (s->clockhand + 1) % blockCount ; //Clear reference bit and update clockhand
    return choice ;   
}

//Clock2 Replacement Algorithm (2-bit Clock)
int replacementClock2 (set* s,int blockCount) {
    int choice = -1 ;
    while (choice == -1) { //Repeat only once
        for (int i = 0;i < blockCount;i ++) {
           int h = s->clockhand ;
           if (s->blocks[h].clockbit2 == 0) { choice = h ; break ; }
           else s->blocks[h].clockbit2 = s->blocks[h].clockbit2 - 1 ;

           s->clockhand = (s->clockhand + 1) % blockCount ; //loop around this
        }
    }
    s->clockhand = (s->clockhand + 1) % blockCount ;
    return choice ;   
}

//LIFO Replacement Algorithm
int replacementLIFO (set* s,int blockCount) {
    int newest = 0 ;//default
    for (int b = 0;b < blockCount;b ++) {
        if (s->blocks[b].timestamp_retrieval > s->blocks[newest].timestamp_retrieval) {
            newest = b ;
        }
    }
    return newest ;
}


//Select the correct replacement algorithm based on the configuration
int cache::replacementAlgorithm (int setNumber,FILE* trace) {
    switch (numAlgorithm) {
        case FIFO: return replacementFIFO (& sets[setNumber],associativity) ;
        case LRU:  return replacementLRU (& sets[setNumber],associativity) ;
        case Optimal:  return replacementOptimal (& sets[setNumber],this,trace) ;
        case Clock:  return replacementClock (& sets[setNumber],associativity) ;
        case Clock2:  return replacementClock2 (& sets[setNumber],associativity) ;
        case LIFO:  return replacementLIFO (& sets[setNumber],associativity) ;
        default: throw std::string ("should never get here") ;
    }
}
