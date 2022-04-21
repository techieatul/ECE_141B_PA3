#include "SQLStatement.hpp"
#include "Tokenizer.hpp"

namespace ECE141{
    SQLStatement::SQLStatement(Keywords aStatementType):Statement(aStatementType){

    }
    void SQLStatement::setTableName(std::string& aName){
        SQLStatement::theTableName = aName;

    }

    std::string SQLStatement::getTableName(){
        return SQLStatement::theTableName;
    }

    bool SQLStatement::createTableStatement(Tokenizer &aTokenizer){
        
		aTokenizer.skipTo('(');
        Attribute theAttr;
		bool theNot = false;
		while (aTokenizer.current().data != ";" && aTokenizer.more()) {

			while (aTokenizer.current().data != "," && aTokenizer.current().data != ")" && aTokenizer.more()){

				if (aTokenizer.current().type == TokenType::identifier) {
					theAttr.setName(aTokenizer.current().data);
				}
				switch(aTokenizer.current().keyword){


                    case Keywords::primary_kw:
						theAttr.setPrimaryKey(true);
						break;

					case Keywords::not_kw:
						theNot = true;
						break;

					case Keywords::null_kw:
						theAttr.setNullable(true);
						if (theNot) { theAttr.setNullable(false);}
						theNot = false;
						break;

					case Keywords::auto_increment_kw:
						theAttr.setAutoIncrement(true);
						break;

					case Keywords::integer_kw:
						theAttr.setDataType(DataTypes::int_type);
						break;

					case Keywords::boolean_kw:
						theAttr.setDataType(DataTypes::bool_type);
						break;

					case Keywords::float_kw:
						theAttr.setDataType(DataTypes::float_type);
						break;

					case Keywords::current_timestamp_kw:
						theAttr.setDataType(DataTypes::datetime_type);
						break;
					
					case Keywords::varchar_kw:
						theAttr.setDataType(DataTypes::varchar_type);
                        aTokenizer.skipTo(TokenType::number);
						theAttr.setSize(stoi(aTokenizer.current().data));
						aTokenizer.next();
						break;

					
				}

				aTokenizer.next();
				

			} 
            this->vectorPush(theAttr);
			theNot = false;
			theAttr.reset();
			aTokenizer.next();
		}
		//aTokenizer.skipTo(';');
        
		
		return true;
  }
}