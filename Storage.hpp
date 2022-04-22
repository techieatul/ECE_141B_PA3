//
//  Storage.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <map>
#include "BlockIO.hpp"
#include "Errors.hpp"

namespace ECE141 {

struct CreateDB {};  // tags for db-open modes...
struct OpenDB {};

// not required -- but we discussed in class...
class Storable {
   public:
    virtual StatusResult encode(std::ostream &anOutput) = 0;
    virtual StatusResult decode(std::istream &anInput) = 0;
};

// USE: A storage class, might be helpful...
class Storage : public BlockIO {
   public:
    Storage(std::iostream &aStream);
    StatusResult freeBlocks(const std::string &aName,uint32_t &aBlockCount);
    StatusResult getTables(uint32_t &aBlockCount,std::vector<std::string>& TableVec);
    StatusResult getTableByName(uint32_t &aBlockCount,const std::string &aName,Block &aBlock);
    Block encodeMetaBlock(std::map<std::string,uint32_t>& anIdxMap, uint32_t &anEntityId);
    StatusResult decodeMetaBlock(std::map<std::string,uint32_t>& anIdxMap, uint32_t &anEntityId);
};

}  // namespace ECE141

#endif /* Storage_hpp */
