#pragma once
#include "InsertTableStatement.hpp"

namespace ECE141 {

bool InsertTableStatement::insertTableStatement(Tokenizer &aTokenizer) {
    if (2 < aTokenizer.remaining()) {
        Token &theToken = aTokenizer.peek(1);
        if (Keywords::into_kw == theToken.keyword) {
            theToken = aTokenizer.peek(2);
            if (TokenType::identifier == theToken.type) {
                aTokenizer.next(3);
                StringList theFields;
                if (aTokenizer.skipIf('(')) {
                    parseIdentifierList(theFields, aTokenizer);
                    if (0 == theFields.size()) return false;
                }

                if (aTokenizer.skipIf(Keywords::values_kw)) {
                    if (!makeRowsFromValueLists(aTokenizer, theFields)) {
                        return false;
                    }
                    return true;
                } else
                    return false;
            }
        }
    }
}

bool InsertTableStatement::createRow(InsertTableStatement               &aStatement,
                                     std::map<std::string, std::string> &aKVList) {
    KeyValues            theData;

    const AttributeList &theAttributes = entity->getAttributes();
    for (auto &theAttr : theAttributes) {
        const std::string &theName = theAttr.getName();
        Value              theValue("NULL");
        if (aKVList.count(theName)) {
            theValue = aKVList[theName];
        }

        theData[theName] = theValue;
        // std::cout << "size " << theData.size() << "\n";
    }

    Row theRow(theData);
    rows->push_back(theRow);

    return true;
}
bool InsertTableStatement::makeRowsFromValueLists(Tokenizer  &aTokenizer,
                                                  StringList &aFields) {
    while (aTokenizer.current().data == "(") {
        aTokenizer.next();  // into value
        StringList theValues;
        parseValueList(theValues, aTokenizer);

        aTokenizer.skipIf(',');  // ignore comma...

        // let's combine the keys+values, so we can build a row...
        size_t theSize = aFields.size();
        if (theValues.size() == theSize) {
            std::map<std::string, std::string> theKVs;
            for (size_t i = 0; i < theSize; i++) {
                theKVs[aFields[i]] = theValues[i];
            }
            if (!createRow(*this, theKVs)) {
                return false;
            }
        } else
            return false;
    }

    return true;
}

}  // namespace ECE141