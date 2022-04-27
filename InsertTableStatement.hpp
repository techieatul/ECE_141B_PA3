#pragma once
#include <iostream>
#include <string>

#include "Attribute.hpp"
#include "Entity.hpp"
#include "Row.hpp"
#include "SQLProcessor.hpp"
#include "SQLStatement.hpp"
#include "Statement.hpp"

namespace ECE141 {
using RowVectors = std::vector<Row>;
class InsertTableStatement : public SQLStatement {
   public:
    InsertTableStatement(Keywords aStatementType, RowVectors *aRowVector, Entity *anEntity)
        : SQLStatement::SQLStatement(aStatementType), rows(aRowVector), entity(anEntity) {}
    ~InsertTableStatement(){};

    bool insertTableStatement(Tokenizer &aTokenizer);
    bool createRow(InsertTableStatement               &aStatement,
                   std::map<std::string, std::string> &aKVList);
    bool makeRowsFromValueLists(Tokenizer  &aTokenizer,
                                StringList &aFields);

   protected:
    RowVectors *rows;
    Entity     *entity;
};

}  // namespace ECE141