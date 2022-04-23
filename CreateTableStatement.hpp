#pragma once
#include <iostream>
#include "Statement.hpp"
#include "SQLStatement.hpp"
#include <string>
namespace ECE141{
    class CreateTableStatement:public SQLStatement {
        public:
        CreateTableStatement(Keywords aStatementType=Keywords::unknown_kw):SQLStatement::SQLStatement(aStatementType){}
        ~CreateTableStatement(){};


    };

}