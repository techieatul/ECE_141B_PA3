//
//  SQLProcessor.hpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//
#pragma once
#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp


#include <stdio.h>
#include "CmdProcessor.hpp"
#include "Tokenizer.hpp"
#include "Database.hpp"
#include "keywords.hpp"
#include "SQLStatement.hpp"
#include "MessageViewer.hpp"
#include "CreateTableStatement.hpp"
#include "DropTableStatement.hpp"
#include "ShowTableStatement.hpp"
#include "DescribeTableStatement.hpp"

namespace ECE141 {

  class Statement;
  class DBProcessor; //define this later...
  class Entity;
  class Database;

  //Should this processor have it's own Statement types?

  class SQLProcessor : public CmdProcessor {
  public:
    
    SQLProcessor(std::ostream &anOutput,Database** aCurrentActiveDbPtr);
    virtual ~SQLProcessor();
    
    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer,
                                StatusResult &aResult) override;
    
    StatusResult  run(Statement *aStmt) override;
    //StatusResult  run(SQLStatement *aStmt);
    //We don't DO the work here, but we can help route cmds...
    StatusResult  createTable(Entity *anEntity);
    StatusResult  describeTable(const std::string &aName);
    StatusResult  dropTable(const std::string &aName);
    StatusResult  showTables();
    Statement*    handleSqlStatements(Tokenizer &aTokenizer);

  protected:
  Database** currentActiveDbPtr;
  Keywords keywordStatement;
  MessageViewer SQLMessageHandler;

    //do you need other data members?
  };

}
#endif /* SQLProcessor_hpp */
