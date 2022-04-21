#pragma once

#include <iostream>
#include "View.hpp"
#include <vector>




namespace ECE141{
    class ShowsTablesView: public View{
        protected:
            std::vector<std::string> theRow;
            std::vector<std::vector<std::string>> theRows;
            std::vector<size_t> theColWidth;
            size_t theCurrCol;

        public:
            ShowsTablesView(bool aPrintRow);
            ~ShowsTablesView();
            bool printrow;
            bool addToRow(std::string aMember);
            bool addRow(std::vector<std::string>& aRow);
            bool endOfRow();
            bool show(std::ostream &aStream);
            bool printLine(std::ostream &aStream);
            bool printRow(std::ostream &aStream,std::vector<std::string> aRow);
            

};

}


