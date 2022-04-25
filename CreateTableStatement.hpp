#pragma once
#include <iostream>
#include <string>

#include "SQLStatement.hpp"
#include "Statement.hpp"
namespace ECE141 {
class CreateTableStatement : public SQLStatement {
   public:
    CreateTableStatement(Keywords aStatementType = Keywords::unknown_kw) : SQLStatement::SQLStatement(aStatementType) {}
    ~CreateTableStatement(){};
};

}  // namespace ECE141