
#include "stdafx.h"

#ifndef Stage1_H
#define Stage1_H

class Stage1 {
private:

    int anonFD;
    unsigned char* key;
    bool decodeSections();

public:
    Stage1(int fd, unsigned char* key);

    static void buildElf(Stage1 b);
};

#endif //!~ Stage1_H
 

Stage1::Stage1(int fd, unsigned char* key) 
{    
    this->anonFD = fd;
    this->key = key;
}

void Stage1::buildElf(Stage1 b) 
{

}