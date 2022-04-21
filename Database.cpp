//
//  Database.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "Database.hpp"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstring>

#include "Config.hpp"

namespace ECE141 {

Database::Database(const std::string aName, CreateDB)
    : name(aName), changed(true), storage(stream) {
    std::string theDbPath = Config::getDBPath(aName);
    stream.clear();
    stream.open(theDbPath.c_str(), std::fstream::out | std::fstream::in | std::fstream::app);
    
    Block *theMetaBlock = new Block(BlockType::meta_block);
    theMetaBlock->header.theBlockId = 0;
    theMetaBlock->header.type = 'M';
    theMetaBlock->header.theEntityId = 0;
    strcpy(theMetaBlock->header.theTitle,this->name.c_str());
    //std::string theDBName = "Name:" + this->name;
    
    //strcpy(theMetaBlock->payload,theDBName.c_str()); // Copy the DB name  to payload char array
    StatusResult theCreateResult = storage.writeBlock(0, *theMetaBlock);

    // Increase the block count
    Database::blockCount++;
    
}

Database::Database(const std::string aName, OpenDB)
    : name(aName), changed(false), storage(stream) {
        //Atul added
    std::string theDbPath = Config::getDBPath(name);
    stream.clear();
    stream.open(theDbPath.c_str(), std::fstream::in | std::fstream::out);
    stream.seekg(0,std::ios::end);
    size_t theFileSize = stream.tellg();
    size_t theNumBlocks = theFileSize/ECE141::kBlockSize;
    this->blockCount = theNumBlocks;
    stream.seekg(0,std::ios::beg);
    // open for read/write
}

Database::~Database() {
    if (changed) {
        
    }
    //std::cout<<"Destructor called "<<std::endl;
    stream.close();
}

// USE: Call this to dump the db for debug purposes...
StatusResult Database::dump(std::ostream &anOutput) {
    // Database

    for (auto i=0; i<blockCount; i++) {
        Block theTempBlock(BlockType::meta_block);
        storage.readBlock(i, theTempBlock);
        anOutput << theTempBlock.header.type << "|" << i << "|" << "ExtraInfo";
    }

    return StatusResult{noError};
}

StatusResult Database::openDB(){
    this->stream.open(Config::getDBPath(this->getDbName()).c_str(), std::fstream::out | std::fstream::in | std::fstream::app);

}

StatusResult Database::closeDB(){
    this->stream.close();
}

}  // namespace ECE141
