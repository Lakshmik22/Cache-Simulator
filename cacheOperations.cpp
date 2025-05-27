#include "cache.h"
#include <stdio.h>
#include <string>
//size_t is = integer type defined to be the size of pointers on the active platform

int min (int x,int y) {
    if (x < y) return x ;
    else return y ;
}
//returns the tag of block b and checks if block is invalid
std::string tag (set* s,int b) {
    if (!s->blocks[b].valid) return std::string ("----") ;
    else {
        char bufr[20] ;
        sprintf (bufr,"%04X",s->blocks[b].tag) ;
        return std::string (bufr) ;
    }
}
// Returns a formatted string of tags for the given number of blocks in a set
std::string print (set* s,int blocks) {
    char bufr[100] ;
    switch (blocks) {
        case 1: return tag (s,0) ;
        case 2: return tag (s,0) + " " + tag (s,1) ;
        case 3: return tag (s,0) + " " + tag (s,1) + " " + tag (s,2) ;
        case 4: return tag (s,0) + " " + tag (s,1) + " " + tag (s,2) + " " + tag (s,3) ;
        default: return tag (s,0) + " " + tag (s,1) + " " + tag (s,2) + " " + tag (s,3) + " ...." ;
    }
}
// Logs the result of a cache read attempt (hit/miss/eviction)
void report (size_t readAddress,cache* c,bool hit,size_t evict_tag) {
    size_t tagAndIndex = readAddress >> c->blockOffsetBits ;//gets just the tag:index
    size_t tag = tagAndIndex >> c->setIndexBits;//Gets the tag part
    size_t tagInPlace = tag << c->setIndexBits;//tag + 0000 ....
    size_t index = tagAndIndex ^ tagInPlace;//ie 0x32BA429f329
                                        
    std::string set_image = print (&c->sets[index],c->associativity) ;
    
    if (hit) {
        printf ("%08X => TAG{%08X} SET[%02d] HIT                     [%s]\n"
            ,readAddress,tag,index,set_image.c_str()) ;
    }
    else if (evict_tag == -1) {
        printf ("%08X => TAG{%08X} SET[%02d] MISS                    [%s]\n"
            ,readAddress,tag,index,set_image.c_str()) ;
    }
    else {
        printf ("%08X => TAG{%08X} SET[%02d] MISS (EVICT %08X)   [%s]\n"
            ,readAddress,tag,index,evict_tag,set_image.c_str()) ;
    }
}
// Checks if all blocks in all sets of the cache are currently used
bool cache::allBlocksUsed () {
    int numSets = (totalBlocks /associativity) ;
    for (int s = 0;s < numSets;s ++) {
        for (int a = 0;a < associativity;a ++) {
            if (!sets[s].blocks[a].valid) return false ;
        }
    }
    return true ;
}
// Performs a read from the cache using the specified memory address and trace
void cache::read(size_t readAddress,FILE* trace){
    size_t tag;
    size_t setIndex;
    
    size_t orig = readAddress ;
    readAddress = readAddress >> blockOffsetBits;//gets the tag:index
    size_t setIndexMask = ((1<<setIndexBits) -1);                                     
    setIndex = readAddress & setIndexMask;//Remove the tag part
    tag = readAddress >> setIndexBits;//Get the tag part

   //Loops over the blocks in a set, searching for a hit
   int open_block = -1 ;//default is non-positive integer
   for (int b = 0;b < associativity;b ++) { //number of blocks per set
       if (sets[setIndex].blocks[b].valid) {
           if (sets[setIndex].blocks[b].tag == tag) {
               //hit
               sets[setIndex].blocks[b].timestamp = time ;
               sets[setIndex].blocks[b].clockbit  = 1    ;
               sets[setIndex].blocks[b].clockbit2 = min (sets[setIndex].blocks[b].clockbit2 + 1,2) ;//add 1, stop at "2" meaning 2 bits are now set
               totalHits ++ ;
               if (debug) {
                   report (orig,this,true,-1) ;
               }
               return ;//leave function early
           }
           //else keep searching
       }
       else {
           open_block = b ; //Remembers the index of the first empty block
       }
   }
   //No hit found, its a miss
   totalMisses ++ ;


   if (open_block != -1) {
       //We have found an open block (cold miss)
       if (debug) {
           report (orig,this,false,-1) ;
       }
       sets[setIndex].blocks[open_block].tag = tag ;
       sets[setIndex].blocks[open_block].valid = true ;
       sets[setIndex].blocks[open_block].timestamp = time ;
       sets[setIndex].blocks[open_block].timestamp_retrieval = time ;
       sets[setIndex].blocks[open_block].clockbit  = 1    ;
       sets[setIndex].blocks[open_block].clockbit2 = 1    ;
       //Fetch bytes from main memory and place them in this block
       totalColdMisses ++ ;

   }
   else {
       // Conflict/capacity miss: replacement required
       int    replace_block = replacementAlgorithm (setIndex,trace) ;//Depends on the specific replacement algorithm
       size_t evict_tag     = sets[setIndex].blocks[replace_block].tag ;
       if (debug) {
           report (orig,this,false,evict_tag) ;
       }
       sets[setIndex].blocks[replace_block].tag = tag ;
       sets[setIndex].blocks[replace_block].timestamp = time ;
       sets[setIndex].blocks[replace_block].timestamp_retrieval = time ;
       sets[setIndex].blocks[replace_block].clockbit  = 1 ;
       sets[setIndex].blocks[replace_block].clockbit2 = 1 ;
       //Fetch bytes from main memory and place them in this block
       if (allBlocksUsed()) { totalCapacityMisses ++ ; }
       else { totalConflictMisses ++ ; }

   }
}
