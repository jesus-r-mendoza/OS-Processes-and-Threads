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
        vect.push_back(segment);
    }
    vect.push_back(str);
    return vect;
}

string countStr(int count, char binary) {
    if ( binary == '1' ) {
        return '+' + to_string(count) + '+';
    }
    return '-' + to_string(count) + '-';
}

int main() {

    ifstream file ("in.txt");
    string line;
    vector<string> parts;

    string result;
    int count;
    int snippet;
    char curr;
    char next;
    bool isLast;
    int len;

    if (file.is_open()) {
        while( getline(file, line) ){
            parts = split(line, " ");

            for ( string str : parts ) {
                
                len = str.length();
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

                        if ( snippet < 0 ) {
                            // result length needs to be chaged later
                            snippet = result.length();
                        }

                    }

                    if ( curr != next || isLast ) {

                        if ( count >= 16 ) {
                            result.erase(snippet-1); // this will erase everything after the snippet index
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
                cout << result << endl;
            }
        }
        return 0;
    }

    cout << "An error occurred while opening the file." << endl;
    return 1;
}