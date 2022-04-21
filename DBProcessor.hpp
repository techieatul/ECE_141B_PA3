//
//  DBProcessor.hpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//
#pragma once
#ifndef DBProcessor_hpp
#define DBProcessor_hpp


#include <stdio.h>
#include "CmdProcessor.hpp"
#include "Tokenizer.hpp"
#include "Database.hpp"
#include "keywords.hpp"
#include "MessageViewer.hpp"

namespace ECE141 {
  
  class DBStatement;

  class DBProcessor : public CmdProcessor {
  public:
    
    DBProcessor(std::ostream &anOutput, Database** aCurrentActiveDbPtr);
    ~DBProcessor();
    
      //cmd processor interface...
    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer,
                                StatusResult &aResult) override;
    StatusResult  run(Statement *aStmt) override;
               
      //dbProcessor interface...
    StatusResult  createDatabase(const std::string &aName);
    StatusResult  dumpDatabase(const std::string &aName);
    StatusResult  dropDatabase(const std::string &aName);
    StatusResult  showDatabases();
    StatusResult  useDatabase(const std::string &aName);

    bool          dbExists(const std::string &aDBName);
    Database*     getDatabaseInUse();

  protected:
  //stuff?
  // to track the keyword (create_kw or use_kw or drop_kw or show_kw)
     Keywords keywordStatement;
     //Atul
     Database** currentActiveDbPtr; // To have a pointer to pointer to the current active DB// Changed this after PA2
     MessageViewer DBMessageHandler; // To handle all printing task
    
    
    
  };

}
#endif /* DBProcessor_hpp */
