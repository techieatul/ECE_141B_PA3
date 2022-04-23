#pragma once
#include <iostream>
#include "Statement.hpp"
#include "SQLStatement.hpp"
#include <string>
#include "Attribute.hpp"
namespace ECE141{
    class DropTableStatement:public SQLStatement{
        public:
        DropTableStatement(Keywords aStatementType=Keywords::unknown_kw):SQLStatement::SQLStatement(aStatementType){}
        ~DropTableStatement(){};

    };

}