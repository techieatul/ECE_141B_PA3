//
//  Database.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//
#pragma once
#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <map>
#include <string>
#include <fstream>

#include "Storage.hpp"

namespace ECE141 {

class Database {
   public:
    Database(const std::string aName, CreateDB);
    Database(const std::string aName, OpenDB);
    //Atul added to access protected member name
    std::string getDbName(){return name;}
    virtual ~Database();

    StatusResult dump(std::ostream &anOutput);
    Storage& getStorage(){return storage;}
    StatusResult openDB();
    StatusResult closeDB();
    uint32_t getBlockCount(){return blockCount;}
    void setBlockCount(uint32_t &aBlockCount){blockCount=aBlockCount;}
    void setEntityMap(std::string& aName, uint32_t &aBlocNum);
    uint32_t getEntityFromMap(std::string aKey){return idx_map[aKey];}
    void setEntityId(uint32_t &aId) {entity_id=aId;}
    uint32_t getEntityId() {return entity_id;}
    void removeEntityFromMap(const std::string &aName){idx_map.erase(aName);}
    void setChange(bool aValue){changed=aValue;}
    bool checkEntityInMap(const std::string aName){return idx_map.find(aName)!=idx_map.end();}
    std::map<std::string,uint32_t>& getIdxMap(){return idx_map;}
   protected:
    std::string name;
    std::fstream stream;  // low level stream used by storage...
	Storage storage;
    uint32_t blockCount = 0;
    std::map<std::string,uint32_t> idx_map;
    uint32_t entity_id;

    bool changed;  // might be helpful, or ignore if you prefer.
};

}  // namespace ECE141
#endif /* Database_hpp */
