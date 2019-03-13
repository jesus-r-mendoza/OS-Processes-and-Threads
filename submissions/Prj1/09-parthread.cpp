#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <pthread.h>
#include <thread>
using namespace std;

int NUM_THREADS = 10; // This is only a default value; can be overriden
string outName;

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
    int segmntLen = count / NUM_THREADS;
    int x = 0;
    for ( int i = 0; i < NUM_THREADS; i++ ) {
        if ( i == NUM_THREADS - 1 )
            segments.push_back(result.substr(x));
        else
            segments.push_back(result.substr(x, segmntLen));
        x += segmntLen;
    }
    in.clear();
    in.close();
    return segments;
}

void* compress(void* name) {
    char *sudoFile = static_cast<char *>(name);
    cout << "called compress\n";
    ofstream out;

    out.open(outName, ios_base::out);
    if (out.fail()) {
        cerr << "\n ** The file: >> " << outName << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        exit(1);
    }
    
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
    out << resultant;
    out.clear();
    out.close();
    cout << resultant;
    pthread_exit(NULL);
}

int main(int argc, char* args[]) {
    cout << "executing main\n";
    bool hasOutputFile;

    if ( argc ==  2 ) 
        hasOutputFile = false;
    else if ( argc == 3 )
        hasOutputFile = true;
    else if ( argc == 4 ) {
        hasOutputFile = true;
        NUM_THREADS = stoi(args[3]);
    }
    else {
        printf("\n ** Incorrect number of arguments **");
        printf("\n ** %d argument(s) ; Not acceptable **", argc-1);
        printf("\n ** Use only 1, 2 or 3 arguments ** \n\n");
        return 1;
    }

    if (hasOutputFile) {
        outName = args[2];
    } else {
        outName = args[1];
        outName.erase(outName.length() - 4) += "-compressed.txt";
    }

    int rc;
    vector<string> sections = splitFile(args[1]);
    pthread_t tid[NUM_THREADS];
    for ( int t = 0; t < NUM_THREADS; t++ ) {
        void* ptr = &sections[t];
        rc = pthread_create(&tid[t], NULL, compress, ptr);
        if (rc) {
            cout << "\n\n** There was an error when creating a new Thread **\n\n";
            return -1;
        }
    }
    
    for ( int i = 0; i < NUM_THREADS; i++ ) {
        pthread_join(tid[i], NULL);
    }

    // for ( int f = 0; f < NUM_THREADS; f++ ) {
    //     cout << "Process: " << f << endl;
    //     if ( fork() == 0 ) {
    //         out << compress(sections[f]);
    //         out.clear();
    //         out.close();
    //         exit(0);
    //     }
    //     wait(NULL);
    // }

    // for ( int w = 0; w < NUM_THREADS; w++ )
    //     wait(NULL);
    
    return 0;
}
