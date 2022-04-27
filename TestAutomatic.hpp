//
//  TestAutomatic.hpp
//  RGAssignement1
//
//  Created by rick gessner on 2/26/21.
//

#ifndef TestAutomatic_h
#define TestAutomatic_h

#include <iostream>
#include <sstream>
#include <vector>

#include "AboutUs.hpp"
#include "Application.hpp"
#include "Config.hpp"
#include "Errors.hpp"
#include "Faked.hpp"
#include "FolderReader.hpp"
#include "TestSequencer.hpp"

void showErrors(ECE141::StatusResult &aResult, std::ostream &anOutput) {
    static std::map<ECE141::Errors, std::string> theMessages = {
        {ECE141::illegalIdentifier, "Illegal identifier"},
        {ECE141::unknownIdentifier, "Unknown identifier"},
        {ECE141::databaseExists, "Database exists"},
        {ECE141::tableExists, "Table Exists"},
        {ECE141::syntaxError, "Syntax Error"},
        {ECE141::unknownCommand, "Unknown command"},
        {ECE141::unknownDatabase, "Unknown database"},
        {ECE141::unknownTable, "Unknown table"},
        {ECE141::unknownError, "Unknown error"}};

    std::string theMessage = "Unknown Error";
    if (theMessages.count(aResult.error)) {
        theMessage = theMessages[aResult.error];
    }
    anOutput << "Error (" << aResult.error << ") " << theMessage << "\n";
}

namespace ECE141 {

using StringMap = std::map<std::string, std::string>;
using CountList = std::vector<int>;

void showErrors(ECE141::StatusResult &aResult, std::ostream &anOutput) {
    static std::map<ECE141::Errors, std::string> theMessages = {
        {ECE141::illegalIdentifier, "Illegal identifier"},
        {ECE141::unknownIdentifier, "Unknown identifier"},
        {ECE141::databaseExists, "Database exists"},
        {ECE141::tableExists, "Table Exists"},
        {ECE141::syntaxError, "Syntax Error"},
        {ECE141::unknownCommand, "Unknown command"},
        {ECE141::unknownDatabase, "Unknown database"},
        {ECE141::unknownTable, "Unknown table"},
        {ECE141::unknownError, "Unknown error"}};

    std::string theMessage = "Unknown Error";
    if (theMessages.count(aResult.error)) {
        theMessage = theMessages[aResult.error];
    }
    anOutput << "Error (" << aResult.error << ") "
             << theMessage << "\n";
}

enum Commands { unknown,
                alter,
                createDB,
                createTable,
                describe,
                delet,
                dropDB,
                dropTable,
                dumpDB,
                insert,
                select,
                showDBs,
                showTables,
                update,
                useDB };

struct CommandCount {
    Commands command;
    int      count;
    char     cmp = '=';

    bool     compare(const CommandCount &anItem) {
        if (command == anItem.command) {
            if ('>' == cmp) return anItem.count > count;
            return true;
        }
        return false;
    }
};

using Responses = std::vector<CommandCount>;
using ExpectList = std::initializer_list<CommandCount>;

class Expected {
   public:
    Expected(const ExpectList &aList) : expected(aList) {}

    bool operator==(const Responses &aResponses) {
        size_t theSize = expected.size();
        if (theSize && aResponses.size() == theSize) {
            for (size_t i = 0; i < theSize; i++) {
                if (!expected[i].compare(aResponses[i])) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

   protected:
    Responses expected;
};

class TestAutomatic {
   protected:
    std::ostream &output;

   public:
    TestAutomatic(std::ostream &anOutput) : output(anOutput) {}

    ~TestAutomatic() { std::cout << "Test Version 1.3\n"; }

    void addUsersTable(std::ostream &anOutput) {
        anOutput << "create table Users (";
        anOutput << " id int NOT NULL auto_increment primary key,";
        anOutput << " first_name varchar(50) NOT NULL,";
        anOutput << " last_name varchar(50),";
        anOutput << " zipcode int);\n";
    }

    void addPaymentsTable(std::ostream &anOutput) {
        anOutput << "create table Payments (";
        anOutput << " id int NOT NULL auto_increment primary key,";
        anOutput << " user_id int,";
        anOutput << " paydate int,";
        anOutput << " amount int);\n";
    }

    void addAccountsTable(std::ostream &anOutput) {
        anOutput << "create table Accounts (";
        anOutput << " id int NOT NULL auto_increment primary key,";
        anOutput << " account_type varchar(25) NOT NULL,";
        anOutput << " amount int);\n";
    }

    void addBooksTable(std::ostream &anOutput) {
        anOutput << "create table Books (";
        anOutput << " id int NOT NULL auto_increment primary key,";
        anOutput << " title varchar(25) NOT NULL,";
        anOutput << " subtitle varchar(25),";
        anOutput << " user_id int);\n";
    }

    bool doCompileTest() {
        AboutUs                  theAbout;
        auto                     theCount = theAbout.getTeamSize();
        std::vector<std::string> theNames;
        for (size_t i = 0; i < theCount; i++) {
            if (auto theName = theAbout.getName(i))
                theNames.push_back(*theName);
        }
        return theNames.size() > 0;
    }

    bool doScriptTest(std::istream &anInput, std::ostream &anOutput) {
        ECE141::Application  theApp(anOutput);
        ECE141::StatusResult theResult{};
        std::string          theCommand;

        while (theResult && anInput) {
            std::getline(anInput, theCommand);
            std::stringstream theStream(theCommand);
            anOutput << theCommand << "\n";
            theResult = theApp.handleInput(theStream);
            if (theResult == ECE141::userTerminated) {
                theResult.error = Errors::noError;
                break;
            } else if (!theResult) {
                showErrors(theResult, anOutput);
            }
        }
        return theResult;
    }

    //----------------------------------------------

    bool doAppTest() {
        std::string       theInput("version; help; quit;");
        std::stringstream theStream(theInput);
        std::stringstream theOutput;
        bool              theResult = doScriptTest(theStream, theOutput);
        std::string       temp = theOutput.str();
        output << temp << "\n";

        if (theResult) {
            const char *theLines[] = {
                theInput.c_str(),
                "Version 0.4", "Help system available",
                "DB::141 is shutting down"};

            std::stringstream theStream(temp);
            std::string       theLine;

            for (auto *theNext : theLines) {
                std::getline(theStream, theLine);
                if (theLine != theNext) return false;
            }

            return true;
        }

        return false;
    }

    //---------------------------------------------------

    enum states { wasUnknown,
                  wasAlter,
                  wasCreate,
                  wasDescribe,
                  wasDelete,
                  wasDrop,
                  wasDump,
                  wasInsert,
                  wasSelect,
                  wasShow,
                  wasUpdate,
                  wasUse };

    // validates output of DBCommand test..
    size_t analyzeOutput(std::istream &aStream, Responses &aResults) {
        std::stack<Commands> theStack;  // tracking open state...

        static KWList        createDB{Keywords::create_kw, Keywords::database_kw};
        static KWList        showDBs{Keywords::show_kw, Keywords::databases_kw};
        static KWList        dumpDB{Keywords::dump_kw, Keywords::database_kw};
        static KWList        dropDB{Keywords::drop_kw, Keywords::database_kw};
        static KWList        createTable{Keywords::create_kw, Keywords::table_kw};
        static KWList        showTables{Keywords::show_kw, Keywords::tables_kw};
        static KWList        dropTable{Keywords::drop_kw, Keywords::table_kw};
        static KWList        insertInto{Keywords::insert_kw, Keywords::into_kw};

        Tokenizer            theTokenizer(aStream);
        if (theTokenizer.tokenize()) {
            TestSequencer theSeq(theTokenizer);
            int           theValue{0};
            while (theTokenizer.more()) {
                if (theSeq.clear().nextIs(createDB)) {
                    if (theSeq.skip(2).nextIs({Keywords::query_kw}).skip(2)) {
                        theSeq.getNumber(theValue).skip(7);
                        aResults.push_back({Commands::createDB, theValue});
                    }
                } else if (theSeq.clear().nextIs(showDBs)) {
                    if (theSeq.skip(1)) {
                        theTokenizer.skipTo(TokenType::number);
                        theSeq.getNumber(theValue).skipPast(')');
                        aResults.push_back({Commands::showDBs, theValue});
                    }
                } else if (theSeq.clear().nextIs({Keywords::use_kw})) {
                    static KWList dbChanged{Keywords::database_kw, Keywords::changed_kw};
                    if (theSeq.skip(2).nextIs(dbChanged)) {
                        aResults.push_back({Commands::useDB, 1});
                    }
                } else if (theSeq.clear().nextIs(dropDB)) {
                    if (theSeq.skip(2).nextIs({Keywords::query_kw}).skip(2)) {
                        theSeq.getNumber(theValue).skipPast(')');
                        aResults.push_back({Commands::dropDB, theValue});
                    }
                } else if (theSeq.clear().nextIs(dumpDB)) {
                    if (theTokenizer.skipTo(Keywords::rows_kw)) {
                        auto theToken = theTokenizer.peek(-1);
                        theValue = std::stoi(theToken.data);
                        theSeq.skip(7);
                    }
                    aResults.push_back({Commands::dumpDB, theValue});
                } else if (theSeq.clear().nextIs(createTable)) {
                    if (theSeq.skipPast(';').nextIs({Keywords::query_kw}).skip(2)) {
                        theSeq.getNumber(theValue).skip(7);
                        aResults.push_back({Commands::createTable, theValue});
                    }
                } else if (theSeq.clear().nextIs(showTables)) {
                    theTokenizer.skipTo(TokenType::number);
                    theSeq.getNumber(theValue).skipPast(')');
                    aResults.push_back({Commands::showTables, theValue});
                } else if (theSeq.clear().nextIs(dropTable)) {
                    theTokenizer.skipTo(TokenType::number);
                    theSeq.getNumber(theValue).skipPast(')');
                    aResults.push_back({Commands::dropTable, theValue});
                } else if (theSeq.clear().nextIs(insertInto)) {
                    if (theSeq.skipPast(';').nextIs({Keywords::query_kw}).skip(2)) {
                        theSeq.getNumber(theValue).skip(7);
                        aResults.push_back({Commands::insert, theValue});
                    }
                } else if (theTokenizer.skipIf(Keywords::describe_kw)) {
                    if (theTokenizer.skipTo(Keywords::rows_kw)) {
                        auto theToken = theTokenizer.peek(-1);
                        theValue = std::stoi(theToken.data);
                        aResults.push_back({Commands::describe, theValue});
                        theSeq.clear().skipPast(')');
                    }
                } else {
                    theTokenizer.next();
                }
            }
        }
        return aResults.size();
    }

    using FileList = std::vector<std::string>;

    bool hasFiles(FileList &aFilelist) {
        for (auto theFile : aFilelist) {
            std::string   thePath = Config::getDBPath(theFile);
            std::ifstream theStream(thePath);
            if (!theStream) return false;
        }
        return true;
    }

    std::string getRandomDBName(char aChar) {
        uint32_t theCount = rand() % 99999;
        return std::string("testdb_" + std::to_string(theCount + 1000) + aChar);
    }

    bool compareCounts(CountList &aList1, CountList &aList2, const size_t aPos) {
        bool theResult = true;
        for (size_t i = 0; i < aList1.size(); i++) {
            if (theResult) {
                theResult = i == aPos ? aList1[i] < aList2[i] : aList1[i] == aList2[i];
            }
        }
        return theResult;
    }

    size_t countDBFiles() {
        std::string  thePath = Config::getStoragePath();
        FolderReader theReader(thePath.c_str());
        size_t       theCount{0};
        theReader.each(Config::getDBExtension(),
                       [&](const std::string &aName) {
                           theCount++;
                           return true;
                       });
        return theCount;
    }

    //----------------------------------------------
    bool createFile(const std::string &aPath, const std::string &aName) {
        std::string temp(aPath);
        temp += '/';
        temp += aName;
        std::fstream output(temp, std::fstream::out | std::fstream::trunc);
        output << "some text\n";
        return true;
    }

    //----------------------------------------------

    bool doReaderTest() {
        std::string thePath = Config::getStoragePath();
        createFile(thePath, "test1.txt");

        FolderReader theReader(thePath.c_str());
        size_t       theCount{0};
        theReader.each(".txt", [&](const std::string &aName) {
            theCount++;
            return true;
        });
        return theCount > 0;
    }
    //----------------------------------------------

    bool doDBCommandsTest() {
        std::vector<std::string> theFiles;

        // size_t theDBCount=countDBFiles();

        char theType = 'A';
        theFiles.push_back(getRandomDBName(theType));
        theFiles.push_back(getRandomDBName(theType));
        theFiles.push_back(getRandomDBName(theType));

        std::stringstream theStream1;
        theStream1 << "CREATE DATABASE " + theFiles[0] << ";\n";
        theStream1 << "create database " + theFiles[1] << ";\n";
        theStream1 << "CrEaTe dAtABaSe " + theFiles[2] << ";\n";
        theStream1 << "use " + theFiles[0] << ";\n";
        theStream1 << "shoW databaseS;\n";

        std::string       temp(theStream1.str());
        std::stringstream theInput(temp);
        std::stringstream theOutput1;
        bool              theResult = doScriptTest(theStream1, theOutput1) && hasFiles(theFiles);
        if (theResult) {
            auto temp = theOutput1.str();
            output << temp;  // show user...
            // std::cout << temp;

            Responses theResponses;
            auto      theCount = analyzeOutput(theOutput1, theResponses);

            Expected  theExpected({{Commands::createDB, 1}, {Commands::createDB, 1}, {Commands::createDB, 1}, {Commands::useDB, 0}, {Commands::showDBs, 2, '>'}});

            if (theCount && theExpected == theResponses) {
                std::stringstream theStream2;
                theStream2 << "DRop dataBASE " + theFiles[1] << ";\n";
                theStream2 << "shoW databaseS;\n";
                theStream2 << "dump database " + theFiles[0] << ";\n";
                theStream2 << "drop database " + theFiles[0] << ";\n";
                theStream2 << "drop database " + theFiles[2] << ";\n";

                std::stringstream theOutput2;
                if ((theResult = doScriptTest(theStream2, theOutput2))) {
                    temp = theOutput2.str();
                    output << temp << "\n";  // DEBUG!
                    // std::cout << temp << "\n"; //DEBUG
                    std::stringstream theOutput(temp);

                    int               thePrevDBCount = theResponses[4].count;
                    Responses         theResponses2;

                    if ((theResult = analyzeOutput(theOutput, theResponses2))) {
                        Expected theExpected({{Commands::dropDB, 0}, {Commands::showDBs, thePrevDBCount - 1}, {Commands::dumpDB, 0, '>'}, {Commands::dropDB, 0}, {Commands::dropDB}});

                        if (theExpected == theResponses2) {
                            return true;
                        }
                    }
                }
            } else
                theResult = false;
        }
        return theResult;
    }

    void insertUsers(std::ostream &anOut,
                     size_t anOffset, size_t aLimit) {
        static const char *kUsers[] = {
            " (\"terry\",\"pratchett\",92124)",
            " (\"ian\",\"tregellis\",92123)",
            " (\"jody\",\"taylor\",92120)",
            " (\"stephen\",\"king\",92125)",
            " (\"ted\",\"chiang\",92120)"};

        anOut << "INSERT INTO Users (first_name, last_name, zipcode)";

        size_t      theSize = sizeof(kUsers) / sizeof(char *);
        size_t      theLimit = std::min(theSize, anOffset + aLimit);
        const char *thePrefix = " VALUES";
        for (size_t i = anOffset; i < theLimit; i++) {
            anOut << thePrefix << kUsers[i];
            thePrefix = ",";
        }
        anOut << ";\n";
    }

    void insertFakeUsers(std::ostream &anOut,
                         size_t        aGroupSize,
                         size_t        aGroupCount) {
        for (size_t theCount = 0; theCount < aGroupCount; theCount++) {
            anOut << "INSERT INTO Users (first_name, last_name, zipcode) VALUES ";
            const char *thePrefix = "";
            for (size_t theSize = 0; theSize < aGroupSize; theSize++) {
                anOut << thePrefix << '(' << '"' << Fake::People::first_name() << "\"," << '"' << Fake::People::last_name() << "\"," << Fake::Places::zipcode() << ')';
                thePrefix = ",";
            }
            anOut << ";\n";
        }
    }

    void insertBooks(std::ostream &anOut,
                     size_t anOffset, size_t aLimit) {
        static const char *kBooks[] = {
            " (\"The Green Mile\",4)",
            " (\"The Stand\",4)",
            " (\"The Misery\",4)",
            " (\"11/22/63\",4)",
            " (\"The Institute\",4)",
            " (\"The Thief of Time\",1)",
            " (\"The Wintersmith\",1)",
            " (\"The Monster Regiment\",1)",
            " (\"Thud\",1)",
            " (\"The Time Police\",3)",
            " (\"The Mechanical\",2)",
            " (\"The Liberation\",2)",
            " (\"The Rising\",2)",
            " (\"Exhalation\",5)",
        };

        anOut << "INSERT INTO Books (title, user_id)";

        size_t      theSize = sizeof(kBooks) / sizeof(char *);
        size_t      theLimit = std::min(theSize, anOffset + aLimit);
        const char *thePrefix = " VALUES";
        for (size_t i = anOffset; i < theLimit; i++) {
            anOut << thePrefix << kBooks[i];
            thePrefix = ",";
        }
        anOut << ";\n";
    }
    //----------------------------------------------

    bool doTablesTest() {
        std::string       theDBName(getRandomDBName('B'));

        std::stringstream theStream1;
        theStream1 << "create database " << theDBName << ";\n";
        theStream1 << "use " << theDBName << ";\n";

        addUsersTable(theStream1);
        addAccountsTable(theStream1);
        addPaymentsTable(theStream1);

        theStream1 << "show tables;\n";
        theStream1 << "describe Accounts;\n";
        theStream1 << "drop table Accounts;\n";
        theStream1 << "show tables;\n";
        theStream1 << "drop database " << theDBName << ";\n";

        std::string       temp(theStream1.str());
        std::stringstream theInput(temp);
        std::stringstream theOutput1;
        bool              theResult = doScriptTest(theInput, theOutput1);
        if (theResult) {
            std::string       tempStr = theOutput1.str();
            std::stringstream theOutput2(tempStr);
            output << tempStr << "\n";  // DEBUG!

            Responses theResponses;
            auto      theCount = analyzeOutput(theOutput1, theResponses);

            Expected  theExpected({
                {Commands::createDB, 1},
                {Commands::useDB, 1},
                {Commands::createTable, 1},
                {Commands::createTable, 1},
                {Commands::createTable, 1},
                {Commands::showTables, 3},
                {Commands::describe, 3},
                {Commands::dropTable, 1},
                {Commands::showTables, 3},
                {Commands::dropDB, 1},
            });

            if (!theCount || !(theExpected == theResponses)) {
                theResult = false;
            }
        }
        return theResult;
    }

    bool doInsertTest() {
        std::string       theDBName(getRandomDBName('C'));

        std::stringstream theStream1;
        theStream1 << "create database " << theDBName << ";\n";
        theStream1 << "use " << theDBName << ";\n";

        addUsersTable(theStream1);
        insertUsers(theStream1, 0, 2);

        theStream1 << "show tables;\n";
        theStream1 << "dump database " << theDBName << ";\n";
        theStream1 << "drop database " << theDBName << ";\n";

        std::string       temp(theStream1.str());
        std::stringstream theInput(temp);
        std::stringstream theOutput;
        bool              theResult = doScriptTest(theInput, theOutput);
        if (theResult) {
            std::string tempStr = theOutput.str();
            output << "output \n"
                   << tempStr << "\n";
            // std::cout << tempStr << "\n";

            Responses theResponses;
            auto      theCount = analyzeOutput(theOutput, theResponses);

            Expected  theExpected({
                {Commands::createDB, 1},
                {Commands::useDB, 1},
                {Commands::createTable, 1},
                {Commands::insert, 2},
                {Commands::showTables, 1},
                {Commands::dumpDB, 3, '>'},
                {Commands::dropDB, 0},
            });

            if (!theCount || !(theExpected == theResponses)) {
                theResult = false;
            }
        }
        return theResult;
    }
};

}  // namespace ECE141

#endif /* TestAutomatic_h */