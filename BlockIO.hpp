//
//  BlockIO.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#ifndef BlockIO_hpp
#define BlockIO_hpp

#include <stdio.h>

#include <functional>
#include <iostream>

#include "Errors.hpp"

namespace ECE141 {

enum class BlockType {
    data_block = 'D',
    free_block = 'F',
    entity_block = 'E',
    // other types?
    meta_block = 'M',
    unknown_block = 'U',
};

// a small header that describes the block...
struct BlockHeader {
    BlockHeader(BlockType aType = BlockType::data_block)
        : type(static_cast<char>(aType)) {}

    BlockHeader(const BlockHeader &aCopy) {
        *this = aCopy;
    }

    void empty() {
        type = static_cast<char>(BlockType::free_block);
    }

    BlockHeader &operator=(const BlockHeader &aCopy) {
        type = aCopy.type;
        return *this;
    }

    char type;  // char version of block type
    uint32_t theBlockId;   // This is to store 0 for metablock, Id for entities, auto_incr value for data blocks
    char theTitle[20];
    uint32_t theEntityId;   // This is to store current auto_incr value of an entity, 
                            //and current hightest entity id value to be stored in metablock
                            // to store which entity the Data block ti belong to..
                            

};

const size_t kBlockSize = 1024;
const size_t kPayloadSize = kBlockSize - sizeof(BlockHeader);

// block .................
class Block {
   public:
    Block(BlockType aType = BlockType::data_block);
    Block(const Block &aCopy);

    Block &operator=(const Block &aCopy);

    StatusResult write(std::ostream &aStream);
    StatusResult read(std::istream &aStream);

    BlockHeader header;
    char payload[kPayloadSize];
};

//------------------------------

class BlockIO {
   public:
    BlockIO(std::iostream &aStream);

    uint32_t getBlockCount();

    virtual StatusResult readBlock(uint32_t aBlockNumber, Block &aBlock);
    virtual StatusResult writeBlock(uint32_t aBlockNumber, Block &aBlock);
	virtual StatusResult createMetaBlock(Block &aBlock);

   protected:
    std::iostream &stream;
    uint32_t blockCount;
};

}  // namespace ECE141

#endif /* BlockIO_hpp */
