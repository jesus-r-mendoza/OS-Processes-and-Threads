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
        outName.erase(outName.length() - 4) += "-decompressed.txt";
    }

    out.open(outName);
    if (out.fail()) {
        cerr << "\n ** The file: >> " << outName << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        return 1;
    }

    string line;
    vector<string> parts;

    string result;
    int start;
    int end;
    char curr;
    char bin;
    int count;

    while ( getline(in, line) ) {
        parts = split(line, " ");
        for ( string str : parts ) {
            result = "";
            for ( int i = 0; i < str.length()-1; i++ ) {
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
            out << result << str[str.length()-1];
        }
    }
    in.close();
    in.clear();

    out.close();
    out.clear();

    cout << "\nYour file: \n\n  --> [ " << arg[1] << " ] \n\nwas successfully decompressed to: \n\n  --> [ " << outName << " ]\n" << endl;

    return 0;
}