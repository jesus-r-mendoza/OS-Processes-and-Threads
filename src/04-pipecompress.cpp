#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
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

string compress(char* args[]) {

    ifstream in;
    in.open(args[1]);

    if (in.fail()) {
        cerr << "\n ** The file: >> " << args[1] << " << could not be opened, or does not exit. Please try again ** \n" << endl;
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
}

int main() {

    ofstream out;
    string outName;

    if (hasOutputFile) {
        outName = args[2];
    } else {
        outName = args[1];
        outName.erase(outName.length() - 4) += "-compressed.txt";
    }

    out.open(outName);
    if (out.fail()) {
        cerr << "\n ** The file: >> " << outName << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        return 1;
    }

    // Use above code in another function
    
    int pipe[2];

    if ( pipe( pipe ) == -1 ) {
        cerr << "\n ** Pipe Failed ** \n" << endl;
        return 1;
    }

    pid_t pid = fork();

    if ( pid < 0 ) {
        cerr << "\n ** Fork Failed ** \n" << endl;
        return 1;
    } else if ( pid == 0 ) {
        // childProcess();
    } else {
        // parentProcess();
    }

    return 0;
}