
#ifndef _LJPostOptionsWindowMainView_
#define _LJPostOptionsWindowMainView_

#include <View.h>
#include <Bitmap.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <TextControl.h>
#include <TextView.h>
#include <Application.h>
#include <Button.h>
#include <CheckBox.h>

class LJPostOptionsWindowMainView : public BView
{
    public: // Constructors & Destructor
        LJPostOptionsWindowMainView ();
        ~LJPostOptionsWindowMainView (void);

    public: // Member Functions
        virtual void AttachedToWindow(void);
        virtual void AllAttached(void);
        virtual void MessageReceived(BMessage *message);

    public: // Data Members
        BMenuField      *moodField;
        BPopUpMenu      *moodMenu;
        BTextControl    *otherMood;
        BTextControl    *music;
        BButton         *saveButton;
        BButton         *clearButton;
        BButton         *musicButton;
        BCheckBox       *autoFormat;
        BPopUpMenu      *journalMenu;
        BMenuField      *journalField;
        BPopUpMenu      *picMenu;
        BMenuField      *picField;
        BPopUpMenu      *securityMenu;
        BMenuField      *securityField;
        bool            _bxl_attached;
};

#endif // _LJPostOptionsWindowMainView_
