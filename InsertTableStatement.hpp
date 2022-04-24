#pragma once
#include <iostream>
#include <string>

#include "Attribute.hpp"
#include "SQLStatement.hpp"
#include "Statement.hpp"
namespace ECE141 {
class InsertTableStatement : public SQLStatement {
   public:
    InsertTableStatement(Keywords aStatementType = Keywords::unknown_kw)
        : SQLStatement::SQLStatement(aStatementType) {}
    ~InsertTableStatement(){};
};

}  // namespace ECE141