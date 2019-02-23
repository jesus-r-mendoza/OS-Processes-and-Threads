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
            execl("./compress", arg[0], arg[1], arg[2], NULL);
            printf(arg[1]);
            printf("  -  ");
            printf(arg[2]);
            printf("\n\n has 2 args\n\n");
        }
        else {
            execl("./compress", arg[0], arg[1], NULL);
            printf(arg[1]);
            printf("\n\nhas 1 arg\n\n");
        }
        printf("Something should have executed\n");
    }
    else {
        wait(NULL);
    }
    return 0;
}