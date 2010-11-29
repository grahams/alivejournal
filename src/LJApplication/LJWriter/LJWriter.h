
#ifndef _LJWriter_
#define _LJWriter_

#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Application.h>
#include <TextControl.h>
#include <TextView.h>
#include <ScrollView.h>
#include <Button.h>

class LJWriter : public BWindow
{
    public: // Constructors & Destructor
        LJWriter ();
        ~LJWriter (void);

    public: // Member Functions
        virtual bool QuitRequested(void);
        virtual void MessageReceived(BMessage *message);
        virtual void FrameResized(float width, float height);

    public:  // Data members
        BView           *MainView;

        BMenuBar        *menubar;
        BMenu           *fileMenu;

        BMenuItem       *fileLogOff;
        BMenuItem       *fileAcctMgr;
        BSeparatorItem  *fileSep;
        BMenuItem       *fileQuit;

        BTextControl    *tcSubject;
        BScrollView     *eventScroll;
        BTextView       *tvEvent;

        BButton         *sendButton;
        BButton         *clearButton;
        BButton         *postOptionsButton;

        bool            accept_closing;
};

#endif // _LJWriter_
