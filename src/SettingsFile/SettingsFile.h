// a settings file class.
// Copyright (c) 1999-2000, Be Incoporated. All Rights Reserved.

#ifndef _SETTINGSFILE_H_
#define _SETTINGSFILE_H_

#include <Message.h>
#include <FindDirectory.h>
#include <Path.h>

struct attr_info;

class SettingsFile : public BMessage {
public :
	SettingsFile(char const*leafname=NULL,char const*basename=NULL,directory_which dir=B_USER_SETTINGS_DIRECTORY);
	virtual ~SettingsFile();

	status_t InitCheck() const;

	status_t Load();
	status_t Save() const;

private:
	static status_t _StoreAttributes(BMessage const*m,BFile*f,const char*basename="");
	static status_t _ExtractAttribute(BMessage*m,BFile*f,const char*full_name,char*partial_name,attr_info*ai);

	status_t check;
	BPath path;
};

#endif

