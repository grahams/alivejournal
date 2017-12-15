#include <List.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "LJApplication.h"
#include "CommandBlockManager.h"

typedef struct
{
	const char *name;
	const char *value;
} hash_table;

CommandBlockManager::CommandBlockManager(void)
{
    hash_values = new BList();
}

CommandBlockManager::~CommandBlockManager(void)
{
}

const char *
CommandBlockManager::GetValue(const char *name, bool decode_url)
{
	for(int32 i = 0; i < hash_values->CountItems(); i++) {
		hash_table *table = (hash_table *)hash_values->ItemAt(i);
		if(!strcmp(table->name, name)) {

			if(decode_url) {
                return ((LJApplication *)be_app)->DecodeString(
                                        const_cast <char *> (table->value));
            }
			else {
                return table->value;
            }

			break;
		}
	}

	return NULL;
}

void
CommandBlockManager::SetValue(const char *name, const char *value, 
                                                    bool encode_url)
{
    char *encodedvalue;

    if( HasValue( name ) ) {
        RemoveValue( name );
    }

    if(encode_url) {
        encodedvalue = (char *)((LJApplication *)be_app)->EncodeString(
                                                                (char *)value);
    }
    else {
        encodedvalue = (char *)value;
    }

    hash_table *table = new hash_table;
    char *mName = new char[strlen(name)+1];
    char *mValue = new char[strlen(encodedvalue)+1];

    mName = strcpy(mName,name);
    mValue = strcpy(mValue,encodedvalue);

    table->name = mName;
    table->value = mValue;

    hash_values->AddItem(table);
}

bool
CommandBlockManager::RemoveValue(const char *name)
{
	for(int32 i=0; i < hash_values->CountItems(); i++) {
		hash_table *table = (hash_table *)hash_values->ItemAt(i);

		if(!strcmp(table->name, name)) {
            hash_values->RemoveItem(table);

            delete table->name;
            table->name = NULL;
            delete table->value;
            table->value = NULL;
            delete table;
            table = NULL;
            
            // Bust out of this loop
            return true;
		}
	}
    return false;
}

bool
CommandBlockManager::HasValue(const char *name)
{
	for(int32 i=0; i < hash_values->CountItems(); i++) {
		hash_table *table = (hash_table *)hash_values->ItemAt(i);

		if(!strcmp(table->name, name)) {
            return true;
		}
	}
    return false;
}

char *
CommandBlockManager::GetCommandBlock()
{
    std::string  block;
    int hashsize = hash_values->CountItems();

    block = "";
    
	for(int32 i=0; i < hashsize; i++) {
		hash_table *table = (hash_table *)hash_values->ItemAt(i);

        block += table->name;
        block += "=";
        block += table->value;

        if( i < (hashsize - 1) ) {
            block += "&";
        }
	}
    
    int blocksize = strlen(block.c_str()) + 1;
    char *command_block = new char[blocksize];
    command_block = strcpy(command_block,block.c_str());

    return command_block;
}
