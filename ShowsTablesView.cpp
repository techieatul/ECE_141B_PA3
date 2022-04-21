#include "ShowsTablesView.hpp"
#include <algorithm>

namespace ECE141{
    ShowsTablesView::ShowsTablesView(bool aPrintRow){
        this->theCurrCol = 0;
        this->printrow=aPrintRow;

    }
    ShowsTablesView::~ShowsTablesView(){}
    // Fucntion to add each element to a Row <Name Contact Email>

    bool ShowsTablesView::addToRow(std::string aMember){
        this->theRow.push_back(aMember);
        if(theRows.size()==0){
            this->theColWidth.push_back(aMember.length()); 
        }else{
            this->theColWidth[this->theCurrCol] = std::max(aMember.length(),this->theColWidth[this->theCurrCol]);
        }
        this->theCurrCol++;
        
        return true;
    }
    // Function to add an entire Row (vector of string) to another vector (vector of Rows)
    bool ShowsTablesView::addRow(std::vector<std::string>& aRow){
        for(size_t l = 0; l<aRow.size();l++){
            if(theRows.size()==0){
                this->theColWidth.push_back(aRow[l].length()); 
            }else{
                this->theColWidth[l] = std::max(aRow[l].length(),this->theColWidth[l]);
            }
        }
        this->theRows.push_back(aRow);
        return true;
    }
    // Function to mark end of a single Row
    bool ShowsTablesView::endOfRow(){
        this->theRows.push_back(theRow);
        this->theRow = {};
        this->theCurrCol=0;
        return true;
    }
    // Function to print out the table or vector<vector<aRow>>
    bool ShowsTablesView::show(std::ostream &aStream){
        printLine(aStream);
        aStream<<"\n";
        for(size_t k=0; k<this->theRows.size();k++){
            printRow(aStream,this->theRows[k]);
            aStream<<"\n";
            if(k==0 || k==this->theRows.size()-1){
                printLine(aStream);
                aStream<<"\n";
            }else{
                if(printrow){
                    printLine(aStream);
                    aStream<<"\n";
                }
            }
            
            
        } 

    }
    // function to print +---------+--------+--------+ 
    bool ShowsTablesView::printLine(std::ostream &aStream){
        size_t theColSize = (this->theColWidth).size();
        for(size_t i = 0; i<theColSize;i++){
            aStream<<"+";
            for(size_t j = 0; j<this->theColWidth[i]+5;j++){
                aStream<<"-";
            }
            

        }
        aStream<<"+";

    }
    // Function to print | <Element>   | <Element>   |

    bool ShowsTablesView::printRow(std::ostream &aStream,std::vector<std::string> aRow){
        size_t theColSize = (this->theColWidth).size();
        for(size_t i = 0; i<theColSize;i++){
            aStream<<"| " <<aRow[i];
            
            for(size_t j = 0; j<(this->theColWidth[i]+5)-aRow[i].length()-1;j++){
                aStream<<" ";
            }
            

        } 
        aStream<<"|";
    
    }
}