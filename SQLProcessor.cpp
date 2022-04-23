//
//  SQLProcessor.cpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//

#include "SQLProcessor.hpp"
#include "Tokenizer.hpp"
#include "SQLStatement.hpp"
#include "Entity.hpp"
#include "Attribute.hpp"
#include "Database.hpp"
#include "Config.hpp"
#include <sstream>
#include <unordered_set>
namespace ECE141 {


  SQLProcessor::SQLProcessor(std::ostream &anOutput,Database** aCurrentActiveDbPtr)
    :  CmdProcessor(anOutput),currentActiveDbPtr(aCurrentActiveDbPtr) {
  }

  SQLProcessor::~SQLProcessor() {}



  bool checkCreateTable(Tokenizer aTokenizer) {
    //size_t theCurrIdx = aTokenizer.getIndex();
    Token theCreateToken{TokenType::keyword, Keywords::create_kw, Operators::unknown_op, "create"};
    Token theTableToken{TokenType::keyword, Keywords::table_kw, Operators::unknown_op, "table"};
    Token theTableNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "table_name"};
    Token theOpenParanToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, "("};

    std::vector<Token> SQLVector;
    SQLVector.push_back(theCreateToken);
    SQLVector.push_back(theTableToken);
    SQLVector.push_back(theTableNameToken);
    SQLVector.push_back(theOpenParanToken);

    for (size_t i = 0; i < SQLVector.size(); ++i) {
        if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) || (SQLVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    // check if we have ) and ;

    // while(aTokenizer.more() && aTokenizer.current().data!=";"){
    //   aTokenizer.next();
    // }
    aTokenizer.skipTo(';');

    if(aTokenizer.tokenAt(aTokenizer.getIndex()-1).data[0]!=')' || aTokenizer.current().data[0]!=';'){
      return false;
    }

    return true;
    
  }

  bool checkShowTable(Tokenizer aTokenizer){
    Token theShowToken{TokenType::keyword, Keywords::show_kw, Operators::unknown_op, "show"};
    Token theTablesToken{TokenType::keyword, Keywords::tables_kw, Operators::unknown_op, "tables"};
    Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};

    std::vector<Token> SQLVector;
    SQLVector.push_back(theShowToken);
    SQLVector.push_back(theTablesToken);
    SQLVector.push_back(theSemiColonToken);

    for (size_t i = 0; i < SQLVector.size(); ++i) {
        if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) || (SQLVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;

  }

  

  bool checkDropTable(Tokenizer aTokenizer){

    Token theDropToken{TokenType::keyword, Keywords::drop_kw, Operators::unknown_op, "drop"};
    Token theTableToken{TokenType::keyword, Keywords::table_kw, Operators::unknown_op, "table"};
    Token theTableNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "table_name"};
    Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};

    std::vector<Token> SQLVector;
    SQLVector.push_back(theDropToken);
    SQLVector.push_back(theTableToken);
    SQLVector.push_back(theTableNameToken);
    SQLVector.push_back(theSemiColonToken);

    for (size_t i = 0; i < SQLVector.size(); ++i) {
        if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) || (SQLVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;

  }

  bool checkDescribeTable(Tokenizer aTokenizer){
    Token theDescribeToken{TokenType::keyword, Keywords::describe_kw, Operators::unknown_op, "describe"};
    Token theTableNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "table_name"};
    Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};

    std::vector<Token> SQLVector;
    SQLVector.push_back(theDescribeToken);
    SQLVector.push_back(theTableNameToken);
    SQLVector.push_back(theSemiColonToken);

    for (size_t i = 0; i < SQLVector.size(); ++i) {
        if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) || (SQLVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;

  }

  CmdProcessor* SQLProcessor::recognizes(Tokenizer &aTokenizer) {
    switch (aTokenizer.current().keyword) {
        case Keywords::create_kw:
            if (checkCreateTable(aTokenizer)) {
                SQLProcessor::keywordStatement = Keywords::create_kw;
                return this;
            }
            break;
        case Keywords::show_kw:
            if (checkShowTable(aTokenizer)) {
                SQLProcessor::keywordStatement = Keywords::show_kw;
                return this;
            }
            break;
        case Keywords::describe_kw:
            if (checkDescribeTable(aTokenizer)) {
                SQLProcessor::keywordStatement = Keywords::describe_kw;
                return this;
            }
            break;
        case Keywords::drop_kw:
            if (checkDropTable(aTokenizer)) {
                SQLProcessor::keywordStatement = Keywords::drop_kw;
                return this;
            }
            break;
  
    }




    return nullptr;
  }

// No longer used. See makeStatement for further comments  
  void handleSQL(Tokenizer &aTokenizer, SQLStatement** aSQLStatement) {
    switch ((*aSQLStatement)->getType()) {
        case Keywords::create_kw:
            aTokenizer.skipTo(TokenType::identifier);
            (*aSQLStatement)->setTableName (aTokenizer.current().data); // here it's table name
            (*aSQLStatement)->createTableStatement(aTokenizer);
            break;
        case Keywords::show_kw:
            //showTableStatement(aTokenizer);
            break;
        case Keywords::drop_kw:
            aTokenizer.skipTo(TokenType::identifier);
            (*aSQLStatement)->setTableName(aTokenizer.current().data); //  here its table name

            break;
        case Keywords::describe_kw:
            aTokenizer.skipTo(TokenType::identifier);
            (*aSQLStatement)->setTableName(aTokenizer.current().data);
            break;
        
    }
    aTokenizer.skipTo(';');
    aTokenizer.next();
}


  Statement* SQLProcessor::makeStatement(Tokenizer &aTokenizer,
                                       StatusResult &aResult) {
     // Atul: Added separate class to create statements for create_table,show_table,drop_table, and describe_table
     // This is handled in handleSqlStatements
    // SQLStatement *theSQLStatement = new SQLStatement(SQLProcessor::keywordStatement);
    // handleSQL(aTokenizer, &theSQLStatement);
    
    Statement *theSQLStatement = SQLProcessor::handleSqlStatements(aTokenizer);
    SQLProcessor::keywordStatement = Keywords::unknown_kw; //resetting for the next command

    // Go to end of the command
    aTokenizer.skipTo(';');
    aTokenizer.next();
    return theSQLStatement;
  }


   Statement* SQLProcessor::handleSqlStatements(Tokenizer &aTokenizer){
     switch(aTokenizer.current().keyword){
           case Keywords::create_kw:{
            CreateTableStatement* theCreateTable = new CreateTableStatement(Keywords::create_kw);
            aTokenizer.skipTo(TokenType::identifier); // Skip to table name
            theCreateTable->setTableName(aTokenizer.current().data);
            theCreateTable->createTableStatement(aTokenizer);
            return theCreateTable;
           }
           
           case Keywords::show_kw:{
            ShowTableStatement* theShowTable = new ShowTableStatement(Keywords::show_kw);
            //theShowTable->setTableName(aTokenizer.current().data);
            return theShowTable;
           }
           case Keywords::describe_kw:{
             DescribeTableStatement* theDescribeTable = new DescribeTableStatement(Keywords::describe_kw);
             aTokenizer.skipTo(TokenType::identifier);
             theDescribeTable->setTableName(aTokenizer.current().data);
             
             return theDescribeTable;
           }
           case Keywords::drop_kw:{
             DropTableStatement* theDropTable = new DropTableStatement(Keywords::drop_kw);
             aTokenizer.skipTo(TokenType::identifier);
             theDropTable->setTableName(aTokenizer.current().data);
             return theDropTable;
           }
            
  

       }
       

    }
  
  StatusResult  SQLProcessor::run(Statement *aStmt) {
    // running which command to run
    SQLStatement *theStatement = dynamic_cast<SQLStatement*>(aStmt);
    StatusResult theStatus(Errors::noError);
    switch (theStatement->getType()){
      
      case Keywords::create_kw:{
        
        std::string theTableName = theStatement->getTableName();

        // 

        Entity* theEntity = new Entity(theStatement->getTableName());
        std::vector<Attribute> theAttr = theStatement->getAttributevector();

        

        for(size_t i = 0; i<theAttr.size();i++){
          theEntity->addAttribute(theAttr.at(i));
        }
        theStatus = createTable(theEntity);
        if(theStatus){
          uint32_t theBlockNum = (*currentActiveDbPtr)->getBlockCount()-1;
          (*currentActiveDbPtr)->setEntityMap(theTableName,theBlockNum);
          uint32_t theEntityId = (*currentActiveDbPtr)->getEntityId()+1;
          (*currentActiveDbPtr)->setEntityId(theEntityId);

        }
        delete theEntity;
        break;
      }
      
      case Keywords::show_kw:
         theStatus = showTables();
         break;

      case Keywords::drop_kw:
      theStatus = dropTable(theStatement->getTableName());
      break;

      case Keywords::describe_kw:
      theStatus = describeTable(theStatement->getTableName());
      break;
    }
    return theStatus;
  }





  StatusResult  SQLProcessor::createTable(Entity *anEntity){
    // creating the table
    // Store the entity as a block
    
    // What all needed to store the entity??
    // Need header and encode the attributes in char payload[], need to decode as well....
    if(*currentActiveDbPtr == nullptr){
      return StatusResult(Errors::noDatabaseSpecified);
    }


    // Checking if the table already exists
    bool theEntityExists = (*currentActiveDbPtr)->checkEntityInMap(anEntity->getName());
    if(theEntityExists){
      return StatusResult(Errors::tableExists);
    }
    // Check if duplicate attributes 
    
    bool theDuplicateAttrCheck = anEntity->checkDuplicateAttr();
    if(theDuplicateAttrCheck){
      return StatusResult(Errors::attributeExists);
    }




    // This part of code handles the blockifying the entity
    Block theConvertedBlock = anEntity->getBlock();
    uint32_t theBlockNum = (*currentActiveDbPtr)->getBlockCount();
    (*currentActiveDbPtr)->getStorage().writeBlock(theBlockNum,theConvertedBlock);
    uint32_t theNewBlockCount = theBlockNum+1; // new block count will old + 1
    (*currentActiveDbPtr)->setBlockCount(theNewBlockCount);
    (*currentActiveDbPtr)->setChange(true);
    output << "Query OK, 1 row affected";
    output <<" ("<<Config::getTimer().elapsed()<<" sec)"<<std::endl;
    return StatusResult(Errors::noError);
  }

  StatusResult  SQLProcessor::describeTable(const std::string &aName){

    // need to write code for decoding
    //uint32_t theBlockNum = (*currentActiveDbPtr)->getBlockCount();
    Block theDescribeBlock;
    uint32_t theBlockNum =  (*currentActiveDbPtr)->getEntityFromMap(aName);
   // (*currentActiveDbPtr)->getStorage().getTableByName(theBlockNum,aName,theDescribeBlock);
    (*currentActiveDbPtr)->getStorage().readBlock(theBlockNum,theDescribeBlock);
    if(theDescribeBlock.header.theTitle==aName){
      // decode the block
      Entity* theEntity = new Entity(aName);
      theEntity->decodeBlock(theDescribeBlock);
      SQLMessageHandler.printAttrTable(output,theEntity->getAttributes());
      output<<theEntity->getAttributes().size()<<" rows in set ("<<Config::getTimer().elapsed()<<" sec.)"<<std::endl;
      delete theEntity;
      
    }
    return StatusResult(Errors::noError);
  }

  StatusResult  SQLProcessor::dropTable(const std::string &aName){

    if(!(*currentActiveDbPtr)->checkEntityInMap(aName)){
      return StatusResult(Errors::unknownTable);
    }
    uint32_t theBlockNum = (*currentActiveDbPtr)->getEntityFromMap(aName);

		StatusResult theStatus = (*currentActiveDbPtr)->getStorage().freeBlocks(aName,theBlockNum);

    if(theStatus){
      (*currentActiveDbPtr)->setChange(true);
      (*currentActiveDbPtr)->removeEntityFromMap(aName);
      output<<"Query OK, 1 rows affected ("<<Config::getTimer().elapsed()<<" sec)"<<"\n";
      
    }
    
    return theStatus;
  }

  StatusResult  SQLProcessor::showTables(){
    std::vector<std::string> theTableVector;
    std::stringstream ss;
    ss<<"Tables_in_"<<(*currentActiveDbPtr)->getDbName();
    theTableVector.push_back(ss.str());
   // uint32_t theBlockNum = (*currentActiveDbPtr)->getBlockCount();
   // (*currentActiveDbPtr)->getStorage().getTables(theBlockNum,theTableVector);
   for(auto const& imap: (*currentActiveDbPtr)->getIdxMap()){
      theTableVector.push_back(imap.first);
   }
    size_t theLongestString = 0;
    for(size_t i = 0; i<theTableVector.size();i++){
      theLongestString = std::max(theLongestString,theTableVector.at(i).length());
    }
    SQLProcessor::SQLMessageHandler.showTableView(output,theTableVector,theLongestString);

    //std::stringstream theMessage;
    //theMessage<<theTableVector.size()-1<<" rows in set"; // Atul changed
    output<<theTableVector.size()-1<<" rows in set ("<<Config::getTimer().elapsed()<<" sec.)"<<"\n";
    //std::string theMsg = std::string(theMessage.str());
    //SQLProcessor::SQLMessageHandler.setMessage(theMsg);
    //SQLProcessor::SQLMessageHandler.show(output);
    return StatusResult(Errors::noError);
  }

}
