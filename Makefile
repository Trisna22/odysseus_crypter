#       Project:		CrypterBuilder
#       Author:         ramb0
#       Version:        1.0
#		- CRYPTER 1 : In memory execute new (encrypted) executable.
#		- CRYPTER 2 : Sections encrypt functionality of CRYPTER1.
#		- EXAMPLE   : Example project to encrypt.
#       - STUB      : The full finished binary.

CC              = g++ --no-warnings
MAIN_FILE       = main.cpp
SOURCE_FOLDER   = ./src
BUILD_FOLDER    = ./bin
CRYPTER1        = crypter_stage1
CRYPTER2		= crypter_stage2
STUB            = stub
EXAMPLE         = example

default:
	mkdir -p ./bin/
	mkdir -p ./src/

	$(CC) $(SOURCE_FOLDER)/$(CRYPTER2)/$(CRYPTER2).cpp -o $(BUILD_FOLDER)/$(CRYPTER2)
	$(CC) $(SOURCE_FOLDER)/$(CRYPTER1)/$(CRYPTER1).cpp -o $(BUILD_FOLDER)/$(CRYPTER1)
	$(CC) $(SOURCE_FOLDER)/$(EXAMPLE)/$(EXAMPLE).cpp -o $(BUILD_FOLDER)/$(EXAMPLE)
	$(CC) $(SOURCE_FOLDER)/$(STUB)/$(STUB).cpp -o $(BUILD_FOLDER)/$(STUB)

run:
	@echo =============== STAGE 1 ===============
	$(BUILD_FOLDER)/$(CRYPTER1) $(BUILD_FOLDER)/$(EXAMPLE)


	@echo =============== STAGE 2 ===============
	@echo "-> BEFORE STAGE 2:"
	objdump -d -M intel -j .secure $(BUILD_FOLDER)/$(STUB)
	$(BUILD_FOLDER)/$(CRYPTER2) $(BUILD_FOLDER)/$(STUB)
	@echo "-> AFTER STAGE 2:"
	objdump -d -M intel -j .secure $(BUILD_FOLDER)/$(STUB)

	@echo "============= STUB ==============\n"
	
	$(BUILD_FOLDER)/$(STUB)
