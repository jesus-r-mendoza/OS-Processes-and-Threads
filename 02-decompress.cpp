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

int main() {

    ifstream file("cmprsd.txt");
    string line;
    vector<string> parts;

    string result;
    int start;
    int end;
    char curr;
    char bin;
    int count;

    if (file.is_open()) {
        while ( getline(file, line) ) {
            parts = split(line, " ");

            for ( string str : parts ) {
                result = "";
                for ( int i = 0; i < str.length(); i++ ) {

                    curr = str[i];
                    if ( curr != '0' && curr != '1' ) {
                        start = i;
                        if ( curr == '+' ) {
                            bin = '1';
                            end = str.find('+', start+1);
                        } else {
                            bin = '0';
                            end = str.find('-', start+1);
                        }                        
                        count = stoi(str.substr(start+1, end-start-1));
                        result += string(count, bin);
                        i = end;
                    } else {
                        result += curr;
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