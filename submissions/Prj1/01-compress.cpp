#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> split(string str, string regex) {
    int found;
    vector<string> vect;
    string segment;
    while ( ( found = str.find(regex) ) != -1 ) {
        segment = str.substr(0, found);
        str.erase(0, found + regex.length());
        vect.push_back(segment += ' ');
    }
    vect.push_back(str += '\n');
    return vect;
}

string countStr(int count, char binary) {
    if ( binary == '1' ) {
        return '+' + to_string(count) + '+';
    }
    return '-' + to_string(count) + '-';
}

int main(int argc, char* arg[]) {

    bool hasOutputFile;

    if ( argc ==  2 ) 
        hasOutputFile = false;
    else if ( argc == 3 )
        hasOutputFile = true;
    else {
        printf("\n ** Incorrect number of arguments **");
        printf("\n ** %d argument(s) ; Not acceptable **", argc-1);
        printf("\n ** Use only 1 or 2 arguments ** \n\n");
        return 1;
    }

    ifstream in;
    in.open(arg[1]);

    if (in.fail()) {
        cerr << "\n ** The file: >> " << arg[1] << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        return 1;
    }

    ofstream out;
    string outName;

    if (hasOutputFile) {
        outName = arg[2];
    } else {
        outName = arg[1];
        outName.erase(outName.length() - 4) += "-compressed.txt";
    }

    out.open(outName);
    if (out.fail()) {
        cerr << "\n ** The file: >> " << outName << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        return 1;
    }

    string line;
    vector<string> parts;

    string result;
    int count;
    int snippet;
    char curr;
    char next;
    bool isLast;
    int len;

    while( getline(in, line) ){
        parts = split(line, " ");
        for ( string str : parts ) {
            
            len = str.length()-1;
            count = 1;
            snippet = -1;
            result = "";
            if ( len < 2 ) {
                result = str;
                out << result << endl;
                continue;
            }
            
            for ( int c = 0; c < len-1; c++ ) {
                curr = str[c];
                next = str[c+1];
                isLast = c == len - 2;
                if ( curr == next ) {
                    count++;
                    result += curr;
                    if ( snippet < 0 )
                        snippet = result.length();
                }
                if ( curr != next || isLast ) {
                    if ( count >= 16 ) {
                        result.erase(snippet-1);
                        result += countStr(count, curr);
                    }
                    else {
                        result += curr;
                    }
                    if ( isLast && curr != next ) {
                        result += next;
                    }
                    count = 1;
                    snippet = -1;
                }
            }
            out << result << str[len];
        }
    }
    in.close();
    in.clear();

    out.close();
    out.clear();

    cout << "\nYour file: \n\n  --> [ " << arg[1] << " ] \n\nwas successfully compressed to: \n\n  --> [ " << outName << " ]\n" << endl;

    return 0;
}