
#ifndef _LJApplication_
#define _LJApplication_

#include <Application.h>
#include <Window.h>

#include "LJLoginWindow/LJLoginWindow.h"
#include "LJAccount/LJAccount.h"
#include "LJSettings/LJSettings.h"
#include "LJWriter/LJWriter.h"
#include "LJEventEditor/LJEventEditor.h"
#include "LJPostOptionsWindow/LJPostOptionsWindow.h"
#include "LJAboutWindow/LJAboutWindow.h"
#include "FriendListItem.h"
#include "VersionInfo.h"
#include "BubbleHelper.h"
#include "CommandBlockManager.h"


class LJApplication : public BApplication
{
    public: // Constructors & Destructor
        LJApplication ();
        ~LJApplication (void);

    public: // Member Functions
        virtual void    ReadyToRun(void);
        virtual bool    QuitRequested(void);
        virtual void    MessageReceived(BMessage *message);
        virtual void    AboutRequested(void);
        void            DoLogin(void);
        void            SendEvent(void);
        void            AddFriend(const char *username);
        void            RemoveFriend(FriendListItem *item);
        char *          SendCommand(CommandBlockManager *man);
        const char *    FormatCommand(CommandBlockManager *man);
        const char *    EncodeString(char *string);
        const char *    DecodeString(char *string);
        const char *    GetCurrentJournal();

    public: // Data Members
        LJLoginWindow           *LoginWindow;
        LJAccount               *AccountWindow;
        LJSettings              *SettingsWindow;
        LJWriter                *WriterWindow;
        LJEventEditor           *EventEditor;
        LJPostOptionsWindow     *PostOptionsWindow;
        LJAboutWindow           *AboutWindow;
        BubbleHelper            *help;

    private: // Data Members
        char *current_username;
        char *current_password;

        char *mood_id;
        char *mood_name;
        char *music;
        char *security;
        char *usejournal;
        char *picture;
        bool autoformat;

        bool _bxl_attached;
};

#endif // _LJApplication_
