#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>


using namespace std;
//Helper functions to check if grouped characters are an accepting state
bool acceptingState(char);
bool isFloatLiteral(string);
bool isIntLiteral(string);

int main()
{
    map<string,string> mymap;
    //array of reserved words
    string reservedWordsString[] = {"break", "bool", "case", "char", "const", "continue", "default", "do", "double", 
        "else", "extern", "float", "long", "register", "for", "if", "int", "goto", "long", "short", "sizeof", "static", 
        "signed", "struct", "return", "_Packed", "switch", "typedef", "union", "void", "unsigned", "volatile", "while"};
    //array of accepting state regular expressions
    string regexRegex[13][8] = {{"addOp", "+", "-"}, {"assignOp", "=", "+=", "-=", "*=", "/="}, {"comma", ","}, {"char", "\\", "\"", "\'"}, 
        {"comparator", "<", ">", ">=", "<=", "!="}, {"incrOp", "++", "--"}, {"leftBrace", "{"}, {"rightBrace", "}"}, 
        {"logicalNot", "!"}, {"leftParen", "("}, {"rightParen", ")"}, {"multOp", "*", "/"}, {"semicolon", ";"}};
    //regular expression array is looped through and inserted into a map for easier access
    for(int i = 0; i < 13; i++){
        for(int j = 1; j < 8; j++){
            if(regexRegex[i][j] != ""){
                mymap[regexRegex[i][j]] = regexRegex[i][0];
            }
            else{
                break;
            }
        }
    }
    //reserved words array is inserted into the map for easier access
    for(int i = 0; i < 33; i++){
        mymap[reservedWordsString[i]] = reservedWordsString[i];
    }
    //vector that will hold every token and its lexeme
    vector<pair<string,string>> tokenLexeme;
    
    //txt file will be read to convert into token and lexeme
    ifstream myFile;
    myFile.open("test.txt");
    string myFileContent = "";
    string groupedchar = "";
    
    //csv file will be created and tokens and lexemes will be inserted
    ofstream fileCSV;
    fileCSV.open("outputcsv.csv");
    
    //helper variables for comments and strings
    bool isComment = false;
    int intIsString = 0;
    bool stringExist = false;
    
    //loop through the file
    while(getline(myFile,myFileContent)){
        intIsString = 0;
        stringExist = false;
        groupedchar = "";
        //loop through a line of text of the txt file
        for(int i = 0; i < myFileContent.size(); i++){
            //if statement to recognize a string
            if(myFileContent[i] == '"'){
                intIsString++;
            }
            //if statement to recognize a comment
            if(groupedchar == "//"){
                break;
            }
            //if statement to recongize a multiline comment
            if(groupedchar == "/*"){
                isComment = true;
            }
            //if statement to recognize end of multiline comment
            if(groupedchar == "*/"){
                isComment = false;
            }
            //if statement to recognize end of string
            if(intIsString == 2){
                stringExist = true;
                intIsString = 0;
            }
            //if statement to recognize an accepting state
            if(acceptingState(myFileContent[i]) && intIsString == 0 && isComment == false){
                //if a token is found its lexeme will be put into a vector
                if(mymap[groupedchar] == groupedchar && groupedchar != "" && groupedchar != " "){
                    tokenLexeme.push_back(make_pair(mymap[groupedchar], groupedchar));
                }
                else if(isIntLiteral(groupedchar) && !groupedchar.empty() && groupedchar != " "){
                    tokenLexeme.push_back(make_pair("intLiteral", groupedchar));
                }
                else if(isFloatLiteral(groupedchar) && !groupedchar.empty() && groupedchar != " "){
                    tokenLexeme.push_back(make_pair("floatLiteral", groupedchar));
                }
                else if(stringExist == true && !groupedchar.empty() && groupedchar != " "){
                    tokenLexeme.push_back(make_pair("string", groupedchar));
                }
                else{
                    //if there are no other options and grouped string is accepted it will be put into vector
                    if(groupedchar != " " && groupedchar != ""){
                        tokenLexeme.push_back(make_pair("identifier", groupedchar));
                    }
                }
                groupedchar = "";
            }
            else{
                groupedchar += myFileContent[i];
            }
            //if loop iterator is on an accepting state it will insert it into vecotr
            if(acceptingState(myFileContent[i]) && isComment == false){
                string temp = "";
                temp += myFileContent[i];
                if(myFileContent[i] == '('){
                    tokenLexeme.push_back(make_pair(mymap[temp], temp));
                }
                else if(myFileContent[i] == ')'){
                    tokenLexeme.push_back(make_pair(mymap[temp], temp));
                }
                else if(myFileContent[i] == '{'){
                    tokenLexeme.push_back(make_pair(mymap[temp], temp));
                }
                else if(myFileContent[i] == '}'){
                    tokenLexeme.push_back(make_pair(mymap[temp], temp));
                }
                else if(myFileContent[i] == ';'){
                    tokenLexeme.push_back(make_pair(mymap[temp], temp));
                }
                else if(myFileContent[i] == '!'){
                    tokenLexeme.push_back(make_pair(mymap[temp], temp));
                }
            }
            
        }
    }
    for(int i = 0; i < tokenLexeme.size(); i++){
        fileCSV << "Token:" << "," << tokenLexeme[i].first << ",," << "Lexeme:"<< "," << tokenLexeme[i].second << endl;
    }
    fileCSV.close();
    myFile.close();
    return 0;
}

//checks for an accepting state
bool acceptingState(char acceptState){
    if(acceptState == '(' || acceptState == ')'){
        return true;
    }
    if(acceptState == '{' || acceptState == '}'){
        return true;
    }
    if(acceptState == ' ' || acceptState == '!' || acceptState == ';'){
        return true;
    }
    return false;
    
}

//checks if grouped characters are a int and acceptable
bool isIntLiteral(string intLit){
    bool hasLong = false;
    bool hasUnsigned = false;
    string regexint = "";
    if(intLit == "" || intLit.empty()){
        return false;
    }
    for(int i = 0; i < intLit.length(); i++){
        if(intLit[i] >= 48 && intLit[i] <= 57){
            regexint += "d";
        }
        else if(intLit[i] == 76 || intLit[i] == 108 && hasLong == false){
            regexint += intLit[i];
            //can only have l once
            hasLong = true;
        }
        else if(intLit[i] == 85 || intLit[i] == 117 && hasUnsigned == false){
            regexint += intLit[i];
            //can only have u once
            hasUnsigned = true;
        }
        else{
            return false;
        }
    }
    return true;
}

//checks if grouped characters are a float
bool isFloatLiteral(string floatLit){
    bool hasDot = false;
    bool hasDigit = false;
    bool hasPlus = false;
    bool hasMinus = false;
    bool hasLowE = false;
    bool hasUpE = false;
    string regexfloat = "";
    if(floatLit == "" || floatLit.empty()){
        return false;
    }
    for(int i = 0; i < floatLit.length(); i++){
        if(floatLit[i] >= 48 && floatLit[i] <= 57){
            regexfloat += "d";
        }
        else if(floatLit[i] == 46 && hasDot == false){
            regexfloat += ".";
            //can only have one decimal point
            hasDot = true;
        }
        else if(floatLit[i] == 43 && hasPlus == false){
            regexfloat += "+";
            //can only have one plus
            hasPlus = true;
        }
        else if(floatLit[i] == 45 && hasMinus == false){
            regexfloat += "-";
            //can only have one minus
            hasMinus = true;
        }
        else if(floatLit[i] == 69 && hasUpE == false || floatLit[i] == 101 && hasLowE == false){
            regexfloat += floatLit[i];
            //can only have one e of upper or lower case
            hasUpE = true;
            hasLowE = true;
        }
        else{
            return false;
        }
    }
    return true;
}
