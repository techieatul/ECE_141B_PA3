#pragma once
#include "InsertTableStatement.hpp"

#include "Helpers.hpp"

namespace ECE141 {

bool InsertTableStatement::insertTableStatement(Tokenizer &aTokenizer) {
    if (2 < aTokenizer.remaining()) {  // now token: table name
        aTokenizer.next();
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

bool InsertTableStatement::createRow(InsertTableStatement               &aStatement,
                                     std::map<std::string, std::string> &aKVList) {
    KeyValues            theData;

    const AttributeList &theAttributes = entity->getAttributes();
    for (auto &theAttr : theAttributes) {
        const std::string &theName = theAttr.getName();

        Value              theValue("NULL");
        if (aKVList.count(theName)) {
            if (theAttr.getType() == DataTypes::int_type) {
                theValue = Helpers::convertStrToInt(aKVList[theName]);
            } else if (theAttr.getType() == DataTypes::bool_type) {
                theValue = Helpers::convertStrToBool(aKVList[theName]);
            } else if (theAttr.getType() == DataTypes::float_type) {
                theValue = Helpers::convertStrToDouble(aKVList[theName]);
            } else {
                theValue = aKVList[theName];
            }
        }

        theData[theName] = theValue;
        // std::cout << "size " << theData.size() << "\n";
    }

    Row theRow(theData);
    aStatement.rows->push_back(theRow);
    // std::cout << "size " << aStatement.rows[0].getData().size() << "\n";

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