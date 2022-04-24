//
//  Row.cpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//


#include <stdio.h>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <sstream>
#include <memory>
#include "Row.hpp"
#include "BasicTypes.hpp"
#include "Helpers.hpp"

namespace ECE141 {
  
  Row::Row(uint32_t entityId) {}
  Row::Row(const Row &aRow) {*this=aRow;}

  Row::~Row() {}

  Row& Row::operator=(const Row &aRow) {return *this;}
  bool Row::operator==(Row &aCopy) const {return false;}

  //STUDENT: What other methods do you require?
  void Row::getBlock(Block &aBlock){
    aBlock.header.theEntityId = this->entityId;
    aBlock.header.type = 'D';
    this->encode(aBlock);
    
  }

  void Row::encode(Block &aBlock){
    std::map<int,std::string> KeyValueToString = {{0,"B"},{1,"I"},{2,"D"},{3,"S"}};
    KeyValues theRowData = this->getData();
    std::stringstream ss;
    // Check below from dicussion in slidespace and correct later
    for (auto const& [key, val] : theRowData){
        ss<<key<<" ";
        std::visit([&ss](const auto &elem) { ss << elem << " "; }, val);
        std::string valType = KeyValueToString[val.index()];
        ss<<valType<<" ";
    }

    // Entity id(not null) first_name(nullable) last_name(nullable) email(not null)
    // Insert into users (email) VALUES (... , ....)
    // decode Entity -> AttributeList
    // loop over the fields
    // If field is missing and it is allowed to be null, "NULL"

    ss.read(aBlock.payload,ECE141::kPayloadSize);

  }

  void Row::decode(Block &aBlock){
    std::stringstream theStream;
    theStream.write(aBlock.payload,ECE141::kPayloadSize);
    std::string theKey;
    std::string theValtype;
    std::string theVal;
    std::variant<bool,int,double,std::string> value;
    while(theStream>>theKey){
      theStream>>theVal>>theValtype;
      if(theValtype=="S")
      {
        this->set(theKey,std::string(theVal));

      } else if(theValtype=="I")
      {
        this->set(theKey,Helpers::convertStrToInt(theVal));

      } else if (theValtype=="B")
      {
        this->set(theKey,Helpers::convertStrToBool(theVal));
        
      } else {

        this->set(theKey,Helpers::convertStrToDouble(theVal));

      }
      
    }

  }


  Row& Row::set(const std::string &aKey,const Value &aValue) {
    this->data[aKey] = aValue;
    return *this;
  }
    
}
