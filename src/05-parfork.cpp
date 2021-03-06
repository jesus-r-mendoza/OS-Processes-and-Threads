#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
using namespace std;

int NUM_PROCESSES = 10; // This is only a default value; can be overriden

string countStr(int count, char binary) {
    if ( binary == '1' ) {
        return '+' + to_string(count) + '+';
    }
    return '-' + to_string(count) + '-';
}

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

vector<string> splitFile(string inFileName) {
    
    ifstream in;
    in.open(inFileName);

    if (in.fail()) {
        cerr << "\n ** The file: >> " << inFileName << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        exit(1);
    }

    vector<string> segments;
    string result = "";
    
    string line;

    vector<string> parts;
    int count = 0;

    while ( getline( in, line ) ) {
        parts = split(line, " ");
        for ( string str : parts ) {
            result += str;
            count += str.length();
        }
        
    }
    int segmntLen = count / NUM_PROCESSES;
    int x = 0;
    for ( int i = 0; i < NUM_PROCESSES; i++ ) {
        if ( i == NUM_PROCESSES - 1 )
            segments.push_back(result.substr(x));
        else
            segments.push_back(result.substr(x, segmntLen));
        x += segmntLen;
    }
    in.clear();
    in.close();
    return segments;
}

string compress(string sudoFile) {
    
    string resultant = "";

    vector<string> lines;
    vector<string> parts;

    string result;
    int count;
    int snippet;
    char curr;
    char next;
    bool isLast;
    int len;
    string temp;

    lines = split(sudoFile, "\n");
    for ( string line : lines ) {

        parts = split(line, " ");
        for ( string str : parts ) {
            
            len = str.length()-1;
            count = 1;
            snippet = -1;
            result = "";
            if ( len < 2 ) {
                result = str;
                resultant += (result);
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
            if ( str[len] == ' ' )
                result += str[len];
            resultant += result;
        }
    }
    return resultant;
}

int main(int argc, char* args[]) {

    bool hasOutputFile;

    if ( argc ==  2 ) 
        hasOutputFile = false;
    else if ( argc == 3 )
        hasOutputFile = true;
    else if ( argc == 4 ) {
        hasOutputFile = true;
        NUM_PROCESSES = stoi(args[3]);
    }
    else {
        printf("\n ** Incorrect number of arguments **");
        printf("\n ** %d argument(s) ; Not acceptable **", argc-1);
        printf("\n ** Use only 1, 2 or 3 arguments ** \n\n");
        return 1;
    }

    string outName;

    if (hasOutputFile) {
        outName = args[2];
    } else {
        outName = args[1];
        outName.erase(outName.length() - 4) += "-compressed.txt";
    }

    ofstream out;

    out.open(outName, ios_base::out);
    if (out.fail()) {
        cerr << "\n ** The file: >> " << outName << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        exit(1);
    }

    vector<string> sections = splitFile(args[1]);
    //string total = "";
    for ( int f = 0; f < NUM_PROCESSES; f++ ) {
        cout << "Process: " << f << endl;
        if ( fork() == 0 ) {
            out << compress(sections[f]);
            out.clear();
            out.close();
            exit(0);
        }
        wait(NULL);
    }

    for ( int w = 0; w < NUM_PROCESSES; w++ )
        wait(NULL);
    
    out.clear();
    out.close();
    return 0;
}
