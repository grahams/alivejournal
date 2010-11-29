
#ifndef _LJSettings_
#define _LJSettings_

#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <TabView.h>
#include <Point.h>

#include "LJSettingsViewNetwork/LJSettingsViewNetwork.h"
#include "SettingsFile.h"

class LJSettings : public BWindow
{
    public:
        /* Constructors & Destructor*/
        LJSettings ();
        ~LJSettings (void);

    public: // Member Functions 
        virtual bool QuitRequested(void);
        virtual void MessageReceived(BMessage *message);

    public: // Data Members
        BView                   *MainView;
        BTabView                *tabview1;
        LJSettingsViewNetwork   *viewnetwork;
        SettingsFile            *settings;
        bool                    accept_closing;
};

#endif // _LJSettings_
