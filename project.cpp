/*

Blake Cantrell
CS301-002
Final Project

*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <cctype>
#include <algorithm>
using namespace std;

struct Condition {
    string column;
    string op;
    string value;
};

Condition parseCondition(const string& conditionStr) {
    Condition condition;
    regex conditionPattern(R"(\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*(<=|>=|<>|=|<|>)\s*(\S+)\s*)");
    smatch match;
    
    if (regex_match(conditionStr, match, conditionPattern)) {
        condition.column = match[1].str();
        condition.op = match[2].str();
        condition.value = match[3].str();
        
        // Debug
        // cout << "Condition parsed: column=" << condition.column << ", op=" << condition.op << ", value=" << condition.value << endl;
    
    } else {
        cout << "Invalid condition: " << conditionStr << endl;
        throw invalid_argument("Invalid condition: " + conditionStr);
    }

    return condition;
}

// Function created to retrieve the index of a string in a vector
int getIndex(vector<string> x, string a){
    auto elm = find(x.begin(), x.end(), a);
    if (elm != x.end()){
        int index = elm - x.begin();
        return index;
    }
    else return -1;
}

bool isNum(const std::string& str){
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

bool evaluateCondition(const vector<vector<string> >& table, const Condition& condition, const vector<string>& row) { 
    int columnIndex = getIndex(table[0], condition.column);
    if (columnIndex == -1) {
        cout << "Column not found: " << condition.column << endl;
        return false;
    }

    string colVal = row[columnIndex];
    
    // Debug
    // cout << "Comparing column value: " << colVal << " with condition value: " << condition.value << endl;

    if (isNum(condition.value)){
        try {
            int columnValue = stoi(colVal);
            int numericValue = stoi(condition.value);

            if (condition.op == "=") {
                return columnValue == numericValue;
            }
            else if (condition.op == ">"){
                return columnValue > numericValue;
            }
            else if (condition.op == "<") {
                return columnValue < numericValue;
            }
            else if (condition.op == ">=") {
                return columnValue >= numericValue;
            }
            else if (condition.op == "<=") {
                return columnValue <= numericValue;
            }
            else if (condition.op == "<>") {
                return columnValue != numericValue;
            }
        } catch (invalid_argument&) {
            cout << "Invalid number format in condition: " << condition.column << " = " << colVal << ", " << condition.value << endl;
            return false;
        } catch (out_of_range) {
            cout << "Number out of range in condition: " << condition.column << " = " << colVal << ", " << condition.value << endl;
            return false;
        }
    }
    else {
        if (condition.op == "=") {
            return colVal == condition.value;
        }
        else if (condition.op == ">"){
            return colVal > condition.value;
        }
        else if (condition.op == "<") {
            return colVal < condition.value;
        }
        else if (condition.op == ">=") {
            return colVal >= condition.value;
        }
        else if (condition.op == "<=") {
            return colVal <= condition.value;
        }
        else if (condition.op == "<>") {
            return colVal != condition.value;
        }
        else {
            cout << "Invalid operation for string comparison: " << condition.op << endl;
            return false;
        }  
    }
    return false;
}

void orderBy(vector<vector<string> >& table, const vector<pair<string, int> >& sortingCriteria) {
    sort(table.begin() + 1, table.end(), [&](const vector<string>& row1, const vector<string>& row2) {
        for (const auto& criteria : sortingCriteria) {
            string columnName = criteria.first;
            int columnIndex = getIndex(table[0], columnName);
            int sortOrder = criteria.second;
            
            // Debug
            // cout << "first: " << criteria.first << " Second: " << criteria.second << endl;

            if (row1[columnIndex].empty() || row2[columnIndex].empty()) {
                if (row1[columnIndex].empty() && !row2[columnIndex].empty()) {
                    return false;
                }
                else if (!row1[columnIndex].empty() && row2[columnIndex].empty()) {
                    return true;
                }
            }
            
            if (sortOrder == 1){
                if (row1[columnIndex] != row2[columnIndex]) {
                    return row1[columnIndex] < row2[columnIndex];
                }
            }
            else {
                if (row1[columnIndex] != row2[columnIndex]) {
                    return row1[columnIndex] > row2[columnIndex];
                }
            }
        }
        return false;
    });
}

int main(int argc, char *argv[]) {
    
    int tc = atoi(argv[1]);
    ifstream tcfile ("TAB_COLUMNS.csv");
    string line; 
    regex delim(",");
    unordered_map<string, vector<string> >tcMap;
    vector<vector<string> > table;
    
    // Mapping TAB_COLUMNS.csv
    if (!tcfile.is_open())
        cout << "Can't open file" << endl;
    else{
        while (getline(tcfile, line)){
            auto begin = std::sregex_token_iterator(line.begin(), line.end(), delim, -1);
		    auto end = std::sregex_token_iterator();
            tcMap[*begin];
            
            for (std::sregex_token_iterator word = std::next(begin); word != end; ++word){
                std::string str = *word;
                tcMap[*begin].push_back(str);
            }
        }
    }
    
    string input;
    while (input != "EXIT"){
        cout << "MLS> ";
        getline(cin, input);
        if (input == "EXIT") break;
        //input = "SELECT * FROM EMPLOYEE;";

        stringstream ss(input);
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> tokens(begin, end);

        if (!tokens.empty() && tokens.back().back() == ';') {
            tokens.back().pop_back();
        }

        vector<string> select;
        bool sel = false;
        bool fro = false;
        bool wher = false;
        bool ordby = false;
        string from;
        vector<Condition> conditions;
        vector<string> order;
        
        // Checking user input
        string whereClause;
        for(int i = 0; i < tokens.size(); i++){
            if (tokens[i] == "SELECT"){
                sel = true;
                continue;
            } 
            if (tokens[i] == "FROM"){
                sel = false;
                fro = true;
                continue;
            }
            if (tokens[i] == "WHERE"){
                sel = false;
                fro = false;
                wher = true;
                continue;
            }
            if (tokens[i] == "ORDERBY"){
                sel = false;
                fro = false;
                wher = false;
                ordby = true;
                continue;
            }
            if (sel){
                select.push_back(tokens[i]);
            }
            if (fro){
                from = tokens[i];
            }
            if (wher){
                whereClause += tokens[i] + " ";
            }
            if (ordby){
                order.push_back(tokens[i]);
            }
        }

        if (!whereClause.empty() && whereClause.back() == ' '){
            whereClause.pop_back();
        }

        istringstream iss(whereClause);
        string conditionStr;
        while(getline(iss, conditionStr, ',')) {
            if (!conditionStr.empty()) {
                try {
                    conditions.push_back(parseCondition(conditionStr));
                } catch (const invalid_argument& e) {
                    cout << "Error parsing condition: " <<  e.what() << endl;
                }
            }
        }

        string file = from + ".csv";

        string row;
        ifstream fp (file);
        if (!fp.is_open()){
            cout << "Can't open file" << endl;
        }
        else{
            int count = 0;
            vector<string> v;
            for (auto const &str: tcMap[from]){
                if (count % 3 == 0){
                    v.push_back(str);
                }
                count++;
            }
            reverse(v.begin(), v.end());
            table.push_back(v);
            v.clear();

            while (getline(fp, row)){
                string t;
                stringstream sstr(row);

                while(getline(sstr, t, ',')){
                    if (t[0] == ' '){
                        t.erase(0, 1);
                    }
                    v.push_back(t);
                }
                table.push_back(v);
                v.clear();
            }
        }

        int ind = getIndex(table[0], "TC");
        for (int i = 1; i < table.size(); i++){
            int tblTc = atoi(table[i][ind].c_str());
            if (tblTc > tc){
                table[i].clear();
            }
        }

        vector<vector<string> > filteredTable;

        if (!conditions.empty()){
            filteredTable.push_back(table[0]); 
            for (size_t i = 1; i < table.size(); ++i) {
                const auto &row = table[i];
                bool satisfiesConditions = true;
                for (const auto &condition : conditions) {
                    if (!evaluateCondition(table, condition, row)) {
                        satisfiesConditions = false;
                        break;
                    }
                }
                if (satisfiesConditions) {
                    filteredTable.push_back(row);
                }
            }
        }
        else {
            filteredTable = table;
        }

        if (!order.empty()) {
            vector<pair<string, int> > sortingCriteria;
            int sortOrder;

            for (const string &orderByClause : order) {
                
                string trimmedClause = orderByClause;
                if (trimmedClause.back() == ',') {
                    trimmedClause.pop_back();
                }

                size_t colonPos = trimmedClause.find(':');
                if (colonPos != string::npos) {
                    string columnName = trimmedClause.substr(0, colonPos);
                    if (trimmedClause.substr(colonPos + 1) == "-1"){
                        sortOrder = -1;
                    }
                    else {
                        sortOrder = 1;
                    }
                    sortingCriteria.push_back({columnName, sortOrder});
                }
            }

            // Debug
            /*cout << "Sorting criteria:" << endl;
            for (const auto &criteria : sortingCriteria) {
                cout << "Column: " << criteria.first << ", Order: " << criteria.second << endl;
            }*/

            orderBy(filteredTable, sortingCriteria);
        }

        // Print all if SELECT is '*'
        if (select[0] == "*"){
            for (int i = 0; i < filteredTable.size(); i++){
                if (filteredTable[i].empty()){
                    continue;
                }
                for (int j = 0; j < filteredTable[i].size(); j++){
                    if (j == (filteredTable[i].size())-1){
                        cout << filteredTable[i][j];
                    }
                    else cout << filteredTable[i][j] << ",";
                }    
                cout << endl;
            }
        }

        // Taking the SELECT input and outputing the desired output
        else {
            vector<string> s;
            for (int i = 0; i < select.size(); i++){            
                string sele = select[i];
                string t;
                stringstream sstr(sele);
                while(getline(sstr, t, ',')){
                    string r;
                    stringstream ssstr(t);
                    while(getline(ssstr, r, ':')){
                        s.push_back(r);
                    }
                }
            }
            
            vector<int> arr1;
            vector<int> arr0;
            for (int i = 0; i < s.size(); i=i+2){
                if (stoi(s[i+1]) == 1){

                    int dex = getIndex(table[0], s[i]);
                    arr1.push_back(dex);
                }
                else{
                    int dex = getIndex(table[0], s[i]);
                    arr0.push_back(dex);
                }
            }
            
            // Debug
            /*for (int i = 0; i < arr0.size(); i++){
                //cout << "Arr1: " << arr1[i] << endl;
                cout << "Arr0: " << arr0[i] << endl;
            }*/
            
            if (!arr0.empty()){
                for (int i = 0; i < filteredTable.size(); i++){
                    if (filteredTable[i].empty()) {
                        continue;
                    }
                    bool first = true;
                    for (int j = 0; j < filteredTable[i].size(); j++){
                        if (find(arr0.begin(), arr0.end(), j) != arr0.end()){
                            continue;
                        }
                        if (!first) {
                            cout << ",";
                        }
                        cout << filteredTable[i][j];
                        first = false; 
                    }                  
                    cout << endl;
                }
            }

            else if (!arr1.empty()){
                vector<vector<string> > one;
                vector<string> o;
                for (int i = 0; i < filteredTable.size(); i++){
                    for (int k = 0; k < arr1.size(); k++){
                        for (int j = 0; j < filteredTable[i].size(); j++){  
                            if (arr1[k] == j){
                                o.push_back(filteredTable[i][j]);
                            }
                        }
                        
                    }
                    one.push_back(o);
                    o.clear();                         
                }
                    
                for (const auto &row : one) {
                    bool first = true;
                    for (const auto &col : row) {
                        if (!first) {
                            cout << ",";
                        }
                        cout << col;
                        first = false;
                    }
                    cout << endl;
                }
            }
        }
        
        for (int i = 0; i < table.size(); i++){
            table[i].clear();
        }
        table.clear();
        cout << endl;
    }

    return 0;
}