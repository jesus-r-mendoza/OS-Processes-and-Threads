#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

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

    pid_t pid = fork();

    if ( pid < 0 ) {
        return 1;
    }
    else if ( pid == 0 ) {
        if ( hasOutputFile ) {
            // Here we assume that the 01-compress executable
            // is in the same directory as from where we are calling
            // the 03-forkcompress executable
            execl("./01-compress", arg[0], arg[1], arg[2], NULL);
        }
        else {
            // Here we assume that the 01-compress executable
            // is in the same directory as from where we are calling
            // the 03-forkcompress executable
            execl("./01-compress", arg[0], arg[1], NULL);
        }
    }
    else {
        wait(NULL);
    }
    return 0;
}