//
//  DBProcessor.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "DBProcessor.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Application.hpp"
#include "Config.hpp"
#include "DBProcessor.hpp"
#include "Database.hpp"
#include "FolderReader.hpp"
#include "FolderView.hpp"
#include "Helpers.hpp"
#include "ShowsTablesView.hpp"

namespace ECE141 {

// like a typedef!!!
using StmtFactory = Statement *(*)(DBProcessor *aProc,
                                   Tokenizer &aTokenize);

DBProcessor::DBProcessor(std::ostream &anOutput,Database** aCurrentActiveDbPtr)
    : CmdProcessor(anOutput),DBMessageHandler(),currentActiveDbPtr(aCurrentActiveDbPtr) {
}

DBProcessor::~DBProcessor() {
}

// Function to check if tokenized tokens represent CREATE DATABASE DB_NAME;
bool checkCreateDB(Tokenizer aTokenizer) {
    Token theCreateToken{TokenType::keyword, Keywords::create_kw, Operators::unknown_op, "create"};
    Token theDBToken{TokenType::keyword, Keywords::database_kw, Operators::unknown_op, "database"};
    Token theDBNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "DB_NAME"};
    Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
    std::vector<Token> DBVector;
    DBVector.push_back(theCreateToken);
    DBVector.push_back(theDBToken);
    DBVector.push_back(theDBNameToken);
    DBVector.push_back(theSemiColonToken);
    size_t theVecSize = DBVector.size();

    size_t theCount = 0;
    // Lambda function to count tokens till ;
    std::function<void()> theCountTillSemi = [&theCount, &aTokenizer]() {
        size_t theCurrIdx = aTokenizer.getIndex();
        while (aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data != ";") {
            theCount++;
            theCurrIdx++;
        }
    };

    // Calling the function
    theCountTillSemi();

    // If size unequal then return false
    if (theVecSize != theCount+1) {
        return false;
    }
    // bool theReturnVal = true;
    for (size_t i = 0; i < theVecSize; ++i) {
        if ((DBVector.at(i).keyword != aTokenizer.current().keyword) || (DBVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;
}

// Function to check if tokenized tokens represent SHOW DATABASE;
bool checkShowDB(Tokenizer aTokenizer) {
    Token theShowToken{TokenType::keyword, Keywords::show_kw, Operators::unknown_op, "show"};
    Token theDBToken{TokenType::keyword, Keywords::databases_kw, Operators::unknown_op, "databases"};
    Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
    std::vector<Token> theTokenVector;
    theTokenVector.push_back(theShowToken);
    theTokenVector.push_back(theDBToken);
    theTokenVector.push_back(theSemiColonToken);
    size_t theVecSize = theTokenVector.size();

    size_t theCount = 0;
    // Lambda function to count tokens till ;
       std::function<void()> theCountTillSemi = [&theCount,&aTokenizer](){
         size_t theCurrIdx = aTokenizer.getIndex();
         while(aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data!=";"){
           theCount++;
           theCurrIdx++;
         }
       };
       
       // Calling the function
      theCountTillSemi();

    // If size unequal then return false
    if (theVecSize != theCount+1) {
        return false;
    }    

    // bool theReturnVal = true;
    for (size_t i = 0; i < theVecSize; ++i) {
        if ((theTokenVector.at(i).keyword != aTokenizer.current().keyword) || (theTokenVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;
}

// Function to check if tokenized tokens represent USE DB_NAME;
bool checkUseDB(Tokenizer aTokenizer) {
    Token theUseToken{TokenType::keyword, Keywords::use_kw, Operators::unknown_op, "use"};
    Token theDBNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "DB_NAME"};
    Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
    std::vector<Token> DBVector;
    DBVector.push_back(theUseToken);
    DBVector.push_back(theDBNameToken);
    DBVector.push_back(theSemiColonToken);
    size_t theVecSize = DBVector.size();

    size_t theCount = 0;
    // Lambda function to count tokens till ;
       std::function<void()> theCountTillSemi = [&theCount,&aTokenizer](){
         size_t theCurrIdx = aTokenizer.getIndex();
         while(aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data!=";"){
           theCount++;
           theCurrIdx++;
         }
       };
       
       // Calling the function
      theCountTillSemi();

    // If size unequal then return false
    if (theVecSize != theCount+1) {
        return false;
    }

    
    // bool theReturnVal = true;
    for (size_t i = 0; i < theVecSize; ++i) {
        if ((DBVector.at(i).keyword != aTokenizer.current().keyword) || (DBVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;
}

// Function to check if tokenized tokens represent DROP DATABASE DB_NAME;
bool checkDropDB(Tokenizer aTokenizer) {
    Token theDropToken{TokenType::keyword, Keywords::drop_kw, Operators::unknown_op, "drop"};
    Token theDBToken{TokenType::keyword, Keywords::database_kw, Operators::unknown_op, "database"};
    Token theDBNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "DB_NAME"};
    Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
    std::vector<Token> DBVector;
    DBVector.push_back(theDropToken);
    DBVector.push_back(theDBToken);
    DBVector.push_back(theDBNameToken);
    DBVector.push_back(theSemiColonToken);
    size_t theVecSize = DBVector.size();

    size_t theCount = 0;
    // Lambda function to count tokens till ;
       std::function<void()> theCountTillSemi = [&theCount,&aTokenizer](){
         size_t theCurrIdx = aTokenizer.getIndex();
         while(aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data!=";"){
           theCount++;
           theCurrIdx++;
         }
       };
       
       // Calling the function
      theCountTillSemi();

    // If size unequal then return false
    if (theVecSize != theCount+1) {
        return false;
    }


   
    // bool theReturnVal = true;
    for (size_t i = 0; i < theVecSize; ++i) {
        if ((DBVector.at(i).keyword != aTokenizer.current().keyword) || (DBVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;
}

bool checkDumpDB(Tokenizer aTokenizer) {
    Token theDumpToken{TokenType::keyword, Keywords::dump_kw, Operators::unknown_op, "dump"};
    Token theDBToken{TokenType::keyword, Keywords::database_kw, Operators::unknown_op, "database"};
    Token theDBNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "DB_NAME"};
    Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
    std::vector<Token> DBVector;
    DBVector.push_back(theDumpToken);
    DBVector.push_back(theDBToken);
    DBVector.push_back(theDBNameToken);
    DBVector.push_back(theSemiColonToken);
    size_t theVecSize = DBVector.size();

    size_t theCount = 0;
    // Lambda function to count tokens till ;
    std::function<void()> theCountTillSemi = [&theCount, &aTokenizer]() {
        size_t theCurrIdx = aTokenizer.getIndex();
        while (aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data != ";") {
            theCount++;
            theCurrIdx++;
        }
    };

    // Calling the function
    theCountTillSemi();

    // If size unequal then return false
    if (theVecSize != theCount+1) {
        return false;
    }
    // bool theReturnVal = true;
    for (size_t i = 0; i < theVecSize; ++i) {
        if ((DBVector.at(i).keyword != aTokenizer.current().keyword) || (DBVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;
}

// CmdProcessor interface ...
CmdProcessor *DBProcessor::recognizes(Tokenizer &aTokenizer) {
    // Add code to recognise DB commands
    switch (aTokenizer.current().keyword) {
        case Keywords::create_kw:
            if (checkCreateDB(aTokenizer)) {
                DBProcessor::keywordStatement = Keywords::create_kw;
                return this;
            }
            break;
        case Keywords::show_kw:
            // std::cout<<"I came here first"<<std::endl;
            if (checkShowDB(aTokenizer)) {
                DBProcessor::keywordStatement = Keywords::show_kw;
                return this;
            }
            break;
        case Keywords::use_kw:
            if (checkUseDB(aTokenizer)) {
                DBProcessor::keywordStatement = Keywords::use_kw;
                return this;
            }
            break;
        case Keywords::drop_kw:
            if (checkDropDB(aTokenizer)) {
                DBProcessor::keywordStatement = Keywords::drop_kw;
                return this;
            }
            break;
        case Keywords::dump_kw:
            if (checkDumpDB(aTokenizer)) {
                DBProcessor::keywordStatement = Keywords::dump_kw;
                return this;
            }
            break;
    }
    return nullptr;
}

StatusResult DBProcessor::run(Statement *aStmt) {
    // Atul edited
    switch (aStmt->getType()) {
        case Keywords::create_kw:
            if (dbExists(aStmt->getDBName())) {
                return StatusResult(Errors::databaseExists);
            }
            DBProcessor::createDatabase(aStmt->getDBName());
            
            // Atul
            // moved this to createDatabase()


            // delete currentActiveDbPtr;
			// currentActiveDbPtr = new Database(aStmt->getDBName(), CreateDB{});
            // //###################################### 
            // //Below lines giving error.. Why??
            // // char* theMesage = "Query OK, 1 row affected";
            // // DBProcessor::DBMessageHandler.setMessage(theMesage);
            // // DBProcessor::DBMessageHandler.show(std::cout);
            // //######################################
            // std::cout << "Query OK, 1 row affected";
            // std::cout<<" ("<<Config::getTimer().elapsed()<<" secs)"<<std::endl;
            break;

        case Keywords::show_kw:
            DBProcessor::showDatabases();
            break;

        case Keywords::use_kw:
            if (dbExists(aStmt->getDBName())) {
                DBProcessor::useDatabase(aStmt->getDBName());
            } else {
                return StatusResult(Errors::unknownDatabase);
            }
            break;

        case Keywords::drop_kw:
            // std::cout<<"DBName = "<<aStmt->getDBName()<<std::endl;
            if (dbExists(aStmt->getDBName())) {
                std::filesystem::remove(Config::getDBPath(aStmt->getDBName()));

                 //Atul reset active DBPtr if it is the one to be dropped;
                 if((*currentActiveDbPtr)!=nullptr && (*currentActiveDbPtr)->getDbName() == aStmt->getDBName()){
                     delete (*currentActiveDbPtr);
                     (*currentActiveDbPtr) = nullptr;
                 }
                 //Atul added to take use of Views class
                  std::string theMessage = "Query OK, 0 rows affected";
                  DBProcessor::DBMessageHandler.setMessage(theMessage);
                  DBProcessor::DBMessageHandler.show(output);
                
                
            }else{
                return StatusResult(Errors::unknownDatabase);
            }

            break;

        case Keywords::dump_kw:
            dumpDatabase(aStmt->getDBName());
            // if (dumpDatabase(aStmt->getDBName()) == StatusResult(Errors::noError))
            //     std::cout << "Database dumped" << std::endl;
            break;
    }
    return  StatusResult(Errors::noError);
}
// USE: Get DB names and do other stuff based on Statement
void handleDB(Tokenizer &aTokenizer, Statement *aDBStatement) {
    switch (aDBStatement->getType()) {
        case Keywords::create_kw:
            // std::cout<<"I am here"<<std::endl;
            aTokenizer.skipTo(TokenType::identifier);
            aDBStatement->setDBName(aTokenizer.current().data);
            // std::cout<<"DB Name: "<<aTokenizer.current().data<<std::endl;
            break;
        case Keywords::show_kw:
            break;
        case Keywords::drop_kw:
            aTokenizer.skipTo(TokenType::identifier);
            aDBStatement->setDBName(aTokenizer.current().data);
            break;
        case Keywords::use_kw:
            aTokenizer.skipTo(TokenType::identifier);
            aDBStatement->setDBName(aTokenizer.current().data);
            break;
        case Keywords::dump_kw:
            aTokenizer.skipTo(TokenType::identifier);
            aDBStatement->setDBName(aTokenizer.current().data);
            break;
    }
    aTokenizer.skipTo(TokenType::punctuation);
    aTokenizer.next();
    // std::cout<<"Remaining here = "<<aTokenizer.remaining()<<std::endl;
}
// USE: retrieve a statement based on given text input...
Statement *DBProcessor::makeStatement(Tokenizer &aTokenizer,
                                      StatusResult &aResult) {
    Statement *theDBStatement = new Statement(DBProcessor::keywordStatement);
    handleDB(aTokenizer, theDBStatement);
    DBProcessor::keywordStatement = Keywords::unknown_kw;
    return theDBStatement;

    // return nullptr; //means we don't know the statement...
}

//-------------------------------------------------------

Database *DBProcessor::getDatabaseInUse() {
    return *currentActiveDbPtr;
}

bool DBProcessor::dbExists(const std::string &aDBName) {
    // Atul added
    std::string theDBPath = Config::getDBPath(aDBName);
    // std::cout<<"File path  = "<<theDBPath <<" " << aDBName<<std::endl;
    return std::filesystem::exists(theDBPath);
}

StatusResult DBProcessor::createDatabase(const std::string &aName) {
            delete (*currentActiveDbPtr);
			(*currentActiveDbPtr) = new Database(aName, CreateDB{});
            
            //###################################### 
            //Below lines giving error.. Why??
            // char* theMesage = "Query OK, 1 row affected";
            // DBProcessor::DBMessageHandler.setMessage(theMesage);
            // DBProcessor::DBMessageHandler.show(std::cout);
            //######################################
            output << "Query OK, 1 row affected";
            output <<" ("<<Config::getTimer().elapsed()<<" secs)"<<std::endl;
            return StatusResult(Errors::noError);
}

StatusResult DBProcessor::showDatabases() {
    // Atul added

    // Storing all the database names in a vector, It will be like ["Database", <DB1>, <DB2> ....]
    std::vector<std::string> theDBVector;
    theDBVector.push_back("Database");
    size_t theLongestString = theDBVector.at(0).length();
    std::string thePath = Config::getStoragePath();
    FolderReader theFr(thePath.c_str());

    // TODO: Ask Atul about the lambda visitor
    theFr.each(".db", [&theDBVector, &theLongestString](const std::string &aName) {
        theDBVector.push_back(aName);
        theLongestString = std::max(theLongestString, aName.length());
        return true;
    });
    theLongestString+=10;
    DBProcessor::DBMessageHandler.showTableView(output,theDBVector,theLongestString);

    // After message 3 rows in set (0.02 sec)
    std::stringstream theMessage;
    theMessage<<theDBVector.size()-1<<" rows in set"; // Atul changed
    std::string theMsg = std::string(theMessage.str());
    DBProcessor::DBMessageHandler.setMessage(theMsg);
    DBProcessor::DBMessageHandler.show(output);
    return StatusResult(Errors::noError);
}

// USE: call this to perform the dropping of a database (remove the file)...
StatusResult DBProcessor::dropDatabase(const std::string &aName) {
    return StatusResult{unknownDatabase};
}

// USE: DB dump all storage blocks
StatusResult DBProcessor::dumpDatabase(const std::string &aName) {
    std::fstream theDumpStream;
    std::string theDbPath = Config::getDBPath(aName);
    theDumpStream.open(theDbPath,std::ios::in);
    theDumpStream.seekg(0,std::ios::end);
    size_t theFileSize = theDumpStream.tellg();
    size_t theNumBlocks = theFileSize/ECE141::kBlockSize;
    theDumpStream.seekg(0,std::ios::beg);
    Block theHeader;
    // Table View class
    ShowsTablesView theTb(true);
    theTb.addToRow("Type");
    theTb.addToRow("id");
    theTb.addToRow("Extra");
    theTb.endOfRow();

    for(size_t i = 0; i<theNumBlocks;i++){
        theDumpStream.read(reinterpret_cast<char*>(&theHeader),ECE141::kBlockSize);
        char c = theHeader.header.type;
        size_t id = theHeader.header.theBlockId;
        switch(c){
            case 'M':
            theTb.addToRow("Meta");
            break;
            case 'E':
            theTb.addToRow("Entity");
            break;
            case 'D':
            theTb.addToRow("Data");
            break;
            case 'F':
            theTb.addToRow("Free");
            break;
        }
        theTb.addToRow(std::to_string(id));
        theTb.addToRow("");
        theTb.endOfRow();
        theDumpStream.seekg((i+1)*ECE141::kBlockSize,std::ios::beg);


    }

    theDumpStream.close();
    theTb.show(output);
    output<<theNumBlocks-1<<" rows in set ("<<Config::getTimer().elapsed()<<" sec)\n";

    return StatusResult(Errors::noError);
}

// USE: call DB object to be loaded into memory...
StatusResult DBProcessor::useDatabase(const std::string &aName) {
    delete (*DBProcessor::currentActiveDbPtr);
    (*DBProcessor::currentActiveDbPtr) = new Database(aName, OpenDB{});
    //DBProcessor::currentActiveDbPtr = &theUseDb;
    // load the file into memory
    //Storage storage = (*DBProcessor::currentActiveDbPtr)->getStorage();
    //std::fstream theStream = theUseDb->getStream();
    output<<"Database changed"<<std::endl;
    // DBProcessor::DBMessageHandler.setMessage(theMessage);
    // DBProcessor::DBMessageHandler.show(std::cout);
    return StatusResult{ECE141::noError};
}

}  // namespace ECE141
