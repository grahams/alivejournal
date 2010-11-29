
#ifndef _LJLoginWindow_
#define _LJLoginWindow_

#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Application.h>
#include <TextControl.h>
#include <TextView.h>
#include <Button.h>
#include <CheckBox.h>

#include "LJLogoView/LJLogoView.h"
#include "VersionInfo.h"

class LJLoginWindow : public BWindow
{
    public:     // Constructors & Destructor
        LJLoginWindow ();
        ~LJLoginWindow (void);

    public:     // Member Functions
        virtual bool QuitRequested(void);
        virtual void MessageReceived(BMessage *message);
        virtual void FrameMoved(BPoint origin);
        virtual void FrameResized(float width, float height);

    public:    // Data Members
        BView * MainView;
        BMenuBar * menubar;
        BMenu * menuFile;
        BMenuItem * menuFileItemSettings;
        BMenuItem * menuFileItemAbout;
        BSeparatorItem * separator1;
        BMenuItem * menuFileItemQuit;
        LJLogoView * LJLogo;
        BTextControl * tcUsername;
        BTextControl * tcPassword;
        BButton * LogInButton;
        BCheckBox * cbPassword;
        bool logged_on;
        bool accept_closing;
};

#endif // _LJLoginWindow_
