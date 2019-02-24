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

#define BUFFER_SIZE 7
#define READ_END 0
#define WRITE_END 1

int pipe_fd[2];
string intputFileName;
string ouputFileName;

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

void parentProcess(string inFileName) {
    // Parent Process
    close(pipe_fd[READ_END]);

    ifstream in;
    in.open(inFileName);

    if (in.fail()) {
        cerr << "\n ** The file: >> " << inFileName << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        exit(1);
    }

    string line;

    while ( getline( in, line ) ) {
        int i = 0;
        line += '\n';
        while ( i < line.length() ) {
            string temp = line.substr(i, BUFFER_SIZE);
                           
            i += BUFFER_SIZE;
            if ( i >= line.length() ) {
                write(pipe_fd[WRITE_END], temp.c_str(), strlen(temp.c_str()));
            } else {
                write(pipe_fd[WRITE_END], temp.c_str(), BUFFER_SIZE);
            }
        }
    }
    close(pipe_fd[WRITE_END]);        
    in.clear();
    in.clear();
}

void childProcess(string outFileName) {
    // Child Process
    close(pipe_fd[WRITE_END]);

    char buffer[BUFFER_SIZE];

    ofstream out;

    out.open(outFileName);
    if (out.fail()) {
        cerr << "\n ** The file: >> " << outFileName << " << could not be opened, or does not exit. Please try again ** \n" << endl;
        exit(1);
    }
    int x;
    while ( (x = read(pipe_fd[READ_END], buffer, BUFFER_SIZE)) > 0 ) {
    
        for ( int j = 0; j < x; j++ ) {
            out << buffer[j];
        }
        
    }
    close(pipe_fd[READ_END]);
    out.clear();
    out.clear();
}

int main(int argc, char* args[]) {

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

    string outName;

    if (hasOutputFile) {
        outName = args[2];
    } else {
        outName = args[1];
        outName.erase(outName.length() - 4) += "-compressed.txt";
    }

    pid_t pid;

    if ( pipe(pipe_fd) == -1 ) {
        cerr << "\n ** Pipe Failed ** \n" << endl;
        return 1;
    }

    pid = fork();

    if ( pid < 0 ) {
        cerr << "\n ** Fork Failed ** \n" << endl;
        return 1;
    } else if ( pid > 0 ) {
        parentProcess(args[1]);
    } else { 
        /* pid == 0 */
        childProcess(outName);
    }
    
    return 0;
}