//
//  Storage.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "Storage.hpp"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <sstream>
#include <vector>

#include "Config.hpp"

namespace ECE141 {

// storage class?
Storage::Storage(std::iostream &aStream) : BlockIO(aStream) {}

StatusResult Storage::freeBlocks(const std::string &aName,uint32_t &aBlockCount,size_t &aCount){
    Block theBlock;
    for(size_t i = 0; i<aBlockCount;i++){
        BlockIO::readBlock(i,theBlock);
        if(theBlock.header.theTitle == aName){
            aCount++;
            theBlock.header.type = 'F';
            BlockIO::writeBlock(i,theBlock);
            //writeBlock(theBlock);
           // memset(theBlock.payload,0,kPayloadSize);
        }
    }
    if(aCount==0){
        return StatusResult(Errors::unknownTable);
    }
    return StatusResult(Errors::noError);
}

StatusResult Storage::getTables(uint32_t &aBlockCount,std::vector<std::string>& TableVec){
    Block theBlock;
    for(size_t i = 0; i<aBlockCount;i++){
        BlockIO::readBlock(i,theBlock);
        if(theBlock.header.type=='E'){
            TableVec.push_back(theBlock.header.theTitle);
        }
        
    }

    return StatusResult(Errors::noError);
}

StatusResult Storage::getTableByName(uint32_t &aBlockCount,const std::string &aName, Block &aBlock){
    for(size_t i = 0; i<aBlockCount;i++){
        BlockIO::readBlock(i,aBlock);
        if(aBlock.header.theTitle==aName && aBlock.header.type=='E'){
            return StatusResult(Errors::noError);
        }
    }

    return StatusResult(Errors::unknownTable);
}

}  // namespace ECE141
