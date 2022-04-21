//
//  CommandProcessor.cpp
//
//  Created by rick gessner on 3/30/20
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include <iostream>
#include "Application.hpp"
#include "Tokenizer.hpp"
#include <memory>
#include <algorithm>
#include <vector>
#include "Config.hpp"
#include "keywords.hpp"


namespace ECE141 {
  
  Application::Application(std::ostream &anOutput)
    : CmdProcessor(anOutput),DBProcessInst(anOutput,&theCurrentDBPtr),theCurrentDBPtr(nullptr),SQLProcessInst(anOutput,&theCurrentDBPtr) {}
  
  Application::~Application() {delete theCurrentDBPtr;}
  
  // USE: -----------------------------------------------------
  
  bool isKnown(Keywords aKeyword) {
    static Keywords theKnown[]=
      {Keywords::quit_kw,Keywords::version_kw, Keywords::help_kw};
    auto theIt = std::find(std::begin(theKnown),
                           std::end(theKnown), aKeyword);
    return theIt!=std::end(theKnown);
  }

  CmdProcessor* Application::recognizes(Tokenizer &aTokenizer) {
    if(isKnown(aTokenizer.current().keyword)) {
      return this;
    }
    
    if(auto *theDBPtr = DBProcessInst.recognizes(aTokenizer)){
      //std::cout<<"I am here"<<std::endl;
      return theDBPtr;
    }

    if(auto *theSqlPtr = SQLProcessInst.recognizes(aTokenizer)){
      //std::cout<<"I am here"<<std::endl;
      return theSqlPtr;
    }

    return nullptr;
  }

  StatusResult Application::run(Statement* aStatement) {
    switch(aStatement->getType()) {
      case Keywords::quit_kw:
        output << "DB::141 is shutting down\n";
        return StatusResult(ECE141::userTerminated);
        break;
      case Keywords::version_kw:
        output << "Version " << getVersion() << "\n"; break;
      case Keywords::help_kw:
        output << "Help system available\n"; break;
      default: break;
    }
    return StatusResult{Errors::noError};
  }
  
  // USE: retrieve a statement based on given text input...
  Statement* Application::makeStatement(Tokenizer &aTokenizer,
                                        StatusResult &aResult) {
    Token theToken=aTokenizer.current();
    if (isKnown(theToken.keyword)) {
      aTokenizer.next(); //skip ahead...
      return new Statement(theToken.keyword);
    }

    return nullptr;
  }

  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult Application::handleInput(std::istream &anInput){
    Tokenizer theTokenizer(anInput);
    StatusResult theResult=theTokenizer.tokenize();
    while (theResult && theTokenizer.more()) {
      //Keywords k = theTokenizer.current().keyword;
      
      Config::getTimer().reset();
      if(auto *theProc=recognizes(theTokenizer)) {
       // std::cout<<"Here"<<std::endl;
        if(auto *theCmd=theProc->makeStatement(theTokenizer,theResult)) {
         // std::cout<<"Here 1"<<std::endl;
          theResult=theProc->run(theCmd);
         // std::cout<<theResult<<std::endl;
          //std::cout<<"Remaining = "<<theTokenizer.remaining()<<std::endl;
          if(theResult) theTokenizer.skipIf(';');
          delete theCmd;
        }
      }
      else theResult=unknownCommand;
      
    }
    return theResult;
  }

}
