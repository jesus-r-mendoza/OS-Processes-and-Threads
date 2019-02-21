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

    if ( argc <= 1 || argc > 3 ) {
        cerr << "\n** You did not type in 2 command line arguments **\n" << endl;
        return 1;
    }

    ifstream in;
    in.open(arg[1]);

    if (in.fail()) {
        cerr << "\n ** The file: >> " << arg[1] << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        return 1;
    }

    bool hasOutputFile = argc == 3;
    ofstream out;
    string outName;

    if (hasOutputFile) {
        outName = arg[2];
    } else {
        outName = arg[1];
        outName.erase(outName.length() - 4) += "-compressed.txt";
        cout << outName << endl;
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
                cout << result << endl;
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
    return 0;
}