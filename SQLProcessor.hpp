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
#include "CreateTableStatement.hpp"
#include "Database.hpp"
#include "DescribeTableStatement.hpp"
#include "DropTableStatement.hpp"
#include "InsertTableStatement.hpp"
#include "MessageViewer.hpp"
#include "Row.hpp"
#include "SQLStatement.hpp"
#include "ShowTableStatement.hpp"
#include "Tokenizer.hpp"
#include "keywords.hpp"

namespace ECE141 {

class Statement;
class DBProcessor;  // define this later...
class Entity;
class Database;
using RowVectors = std::vector<Row>;
// Should this processor have it's own Statement types?

class SQLProcessor : public CmdProcessor {
   public:
    SQLProcessor(std::ostream &anOutput, Database **aCurrentActiveDbPtr);
    virtual ~SQLProcessor();

    CmdProcessor *recognizes(Tokenizer &aTokenizer) override;
    Statement    *makeStatement(Tokenizer    &aTokenizer,
                                StatusResult &aResult) override;

    StatusResult  run(Statement *aStmt) override;
    // StatusResult  run(SQLStatement *aStmt);
    // We don't DO the work here, but we can help route cmds...
    StatusResult createTable(Entity *anEntity);
    StatusResult describeTable(const std::string &aName);
    StatusResult dropTable(const std::string &aName);
    StatusResult insertTable(const std::string &aName);
    StatusResult showTables();
    Statement   *handleSqlStatements(Tokenizer &aTokenizer);

   protected:
    Database    **currentActiveDbPtr;
    Keywords      keywordStatement;
    MessageViewer SQLMessageHandler;
    RowVectors    theRowData;

    // do you need other data members?
};

}  // namespace ECE141
#endif /* SQLProcessor_hpp */
