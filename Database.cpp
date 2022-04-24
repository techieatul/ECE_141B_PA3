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
    : name(aName), changed(true), storage(stream),entity_id(0) {
    std::string theDbPath = Config::getDBPath(aName);
    stream.clear();
    stream.open(theDbPath.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
    stream.close();
    stream.open(theDbPath.c_str(), std::fstream::binary | std::fstream::binary | std::fstream::in | std::fstream::out);
    
    Block theMetaBlock(BlockType::meta_block);
    theMetaBlock.header.theBlockId = 0;
    theMetaBlock.header.type = 'M';
    theMetaBlock.header.theEntityId = this->entity_id;
    strcpy(theMetaBlock.header.theTitle,this->name.c_str());
    //std::string theDBName = "Name:" + this->name;
    
    //strcpy(theMetaBlock->payload,theDBName.c_str()); // Copy the DB name  to payload char array

    // Make the entity_id 1
    this->setEntityId(1);

    StatusResult theCreateResult = storage.writeBlock(0, theMetaBlock);

    // Increase the block count
    Database::blockCount++;
    
}

Database::Database(const std::string aName, OpenDB)
    : name(aName), changed(false), storage(stream) {
        //Atul added
    std::string theDbPath = Config::getDBPath(name);
    stream.clear();
    stream.open(theDbPath.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out);
    stream.seekg(0,std::ios::end);
    size_t theFileSize = stream.tellg();
    size_t theNumBlocks = theFileSize/ECE141::kBlockSize;
    this->blockCount = theNumBlocks;
    stream.seekg(0,std::ios::beg);
    StatusResult theStatus = storage.decodeMetaBlock(this->idx_map,this->entity_id);
    // decode the meta block and populate the below variables
    // idx_map, entity_id
    // open for read/write
}

Database::~Database() {
    if (changed) {
        //encode the below variables into Block 0
       // idx_map, entity_id
       

       std::map<std::string,uint32_t> theMap = this->idx_map;
       uint32_t theEntityId = this->entity_id;
       Block theBlock = storage.encodeMetaBlock(theMap,theEntityId);
       
       //stream.seekp(0,stream.beg);
       //stream.clear();
       StatusResult theStatus = storage.writeBlock(0,theBlock);
    }
    stream.close();
    //std::cout<<"Destructor called "<<std::endl;
    
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


void Database::setEntityMap(std::string& aName, uint32_t &aBlocNum){
    this->idx_map[aName] = aBlocNum;
}


StatusResult Database::openDB(){
    this->stream.open(Config::getDBPath(this->getDbName()).c_str(), std::fstream::out | std::fstream::in | std::fstream::app);

}

StatusResult Database::closeDB(){
    this->stream.close();
}

}  // namespace ECE141
