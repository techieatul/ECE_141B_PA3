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
   protected:
    std::string name;
    std::fstream stream;  // low level stream used by storage...
	Storage storage;
    uint32_t blockCount = 0;

    bool changed;  // might be helpful, or ignore if you prefer.
};

}  // namespace ECE141
#endif /* Database_hpp */
