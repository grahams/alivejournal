#ifndef __COMMANDBLOCKMANAGER_H__
#define __COMMANDBLOCKMANAGER_H__

#include <List.h>

class CommandBlockManager
{
public:     // (Con|De)struction
						CommandBlockManager(void);
						~CommandBlockManager(void);

public:     // Accessors and Mutators
	const char *		GetValue(const char *name, bool decode=false);
	void                SetValue(const char *name, const char *value,
                                                   bool encode=false);
    bool                RemoveValue(const char *name);
    bool                HasValue(const char *name);

    char *              GetCommandBlock();

private:    // Data Members
	BList *				hash_values;
};

#endif // __COMMANDBLOCKMANAGER_H__
