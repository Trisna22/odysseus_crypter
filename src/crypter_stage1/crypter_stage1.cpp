
#include "stdafx.h"

#include "Builder.h"

void usage(char* progname) {

    printf("Usage: %s [malware.exe] \n", progname);
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }
    
    Builder builer(argv[1]);


    void* stub = builer.locateStub();


    // Make file bigger.
    // Write zeroes
    printf("Succesfully encrypted and loaded in-memory buffer!\n");
    return 0;
}