//
//  BlockIO.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "BlockIO.hpp"
#include <iostream>
#include <cstring>

namespace ECE141 {

Block::Block(BlockType aType):header(aType) {}

Block::Block(const Block &aCopy) {
    std::cout<<"I am called"<<std::endl;
    *this = aCopy;
}

Block &Block::operator=(const Block &aCopy) {
    std::memcpy(payload, aCopy.payload, kPayloadSize);
    header = aCopy.header;
    return *this;
}

StatusResult Block::write(std::ostream &aStream) {
    //aStream.write(payload, kBlockSize);

    return StatusResult(Errors::noError);
    
    
}

StatusResult Block::read(std::istream &aStream) {
    aStream.read(payload, kBlockSize);
    return StatusResult{Errors::noError};
}

//---------------------------------------------------

BlockIO::BlockIO(std::iostream &aStream) : stream(aStream) {}

// USE: write data a given block (after seek) ---------------------------------------
StatusResult BlockIO::writeBlock(uint32_t aBlockNum, Block &aBlock) {
	
        stream.seekp(aBlockNum*kBlockSize,std::ios::beg);
       // std::cout<<stream.tellp()<<std::endl;
        stream.write(reinterpret_cast<char*>(&aBlock), kBlockSize);
        stream.flush();
        return StatusResult{Errors::noError};

	

    // if (aBlock.header.type == (char) BlockType::entity_block) {
    //    // go to the end
    //     stream.seekp(aBlockNum*kBlockSize,std::ios::beg);
    //     stream.write(reinterpret_cast<char*>(&aBlock), kBlockSize);
    //     stream.flush();
    //     return StatusResult{Errors::noError};

	// }
    return StatusResult{writeError};
}

// USE: read data a given block (after seek) ---------------------------------------
StatusResult BlockIO::readBlock(uint32_t aBlockNum, Block &aBlock) {
    
    size_t theCurrentPos = stream.tellg();
    stream.seekg(aBlockNum * kBlockSize,std::ios::beg);
    stream.read(reinterpret_cast<char*>(&aBlock), kPayloadSize);
    stream.seekg(theCurrentPos,stream.beg);

}

StatusResult BlockIO::createMetaBlock(Block &aBlock) {
	return StatusResult{writeError};
}

// StatusResult createMetaBlock() {
//     Block *theMetaBlock = new Block(BlockType::meta_block);
//     //StatusResult theCreateResult = writeBlock(0, *theMetaBlock);
//     //stream.close();
// 	return StatusResult{writeError};
// }

// USE: count blocks in file ---------------------------------------
uint32_t BlockIO::getBlockCount() {
    return blockCount;  // What should this be?
}

}  // namespace ECE141
