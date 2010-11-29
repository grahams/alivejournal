#include <List.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <ctype.h>

#include "LJApplication.h"
#include "ResponseBlockManager.h"

typedef struct
{
	const char *name;
	const char *value;
}hash_table;

ResponseBlockManager::ResponseBlockManager(char *http_response)
{
	char *block = NULL; 
    char *name = NULL; 
    char *value = NULL;
	char *separator = new char [4];
	
	sprintf( separator, "\n%c\n", 13 );
	
	char *string = strstr( http_response, separator );
    
	if( string != NULL ) {
        string += 3;
        block = new char[strlen(string)+1];
		block = strcpy( block,string );

		hash_values = new BList();
		string = strtok(block, "\n");
		do {
			if( string ) {
				name = new char [strlen(string)+1];
				strcpy(name, string);
				string = strtok(NULL, "\n");

				if( string ) {
					value = new char [strlen(string)+1];
					strcpy(value, string);
				}
                else {
					value = NULL;
				}
				
				hash_table *table = new hash_table;
				table->name = name;
				table->value = value;
				hash_values->AddItem(table);
				string = strtok(NULL, "\n");
			}
		} while (string);

		delete[] block;
        block = NULL;
	}
}

ResponseBlockManager::~ResponseBlockManager(void)
{
    if( hash_values != NULL ) {
        while( hash_values->CountItems() > 0 ) {
            hash_table *table = (hash_table *)hash_values->FirstItem();
            hash_values->RemoveItem( table );

#if 0
            if( table != NULL ) {
                if( table->name != NULL ) {
                    delete table->name;
                    table->name = NULL;
                }
                if( table->value != NULL ) {
                    delete table->value;
                    table->value = NULL;
                }
                delete table;
                table = NULL;
            }
#endif
        }

        delete hash_values;
    }
}

const char *
ResponseBlockManager::GetValue(const char *name, bool decode_url=false)
{

	for (int32 i=0; i<hash_values->CountItems(); i++)
	{
		hash_table *table = (hash_table *)hash_values->ItemAt(i);
		if (!strcmp(table->name, name)){

			if (decode_url) {
                return ((LJApplication *)be_app)->DecodeString(const_cast < char *> (table->value));
            }
			else {
                return table->value;
            }

			break;
		}
	}
	return NULL;
}
