
#ifndef _LJEventEditor_
#define _LJEventEditor_

#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <ScrollView.h>
#include <TextView.h>
#include <Button.h>
#include <Application.h>
#include <TextControl.h>
#include <StringView.h>
#include <string>

class LJEventEditor : public BWindow
{
    public: // Constructors & Destructor
        LJEventEditor ();
        ~LJEventEditor (void);

    public: // Member Functions
        virtual bool QuitRequested(void);
        virtual void MessageReceived(BMessage *message);
        virtual void FrameResized(float width, float height);
        void SetJournal(const char *journalname);

    public: // Data Members
        BView           *mainview;
        BScrollView     *scrollview;
        BTextView       *textview;
        BButton         *saveButton;
        BButton         *cancelButton;
        BTextControl    *tcSubject;
        BTextControl    *tcMonth;
        BTextControl    *tcDay;
        BTextControl    *tcYear;
        BStringView     *dateSlash1;
        BStringView     *dateSlash2;
        BTextControl    *tcHour;
        BTextControl    *tcMinute;
        BTextControl    *tcSeconde;
        BStringView     *timeColon1;
        BStringView     *timeColon2;
        void            ClearEditor();
        void            GetItem();
        const char      *itemid;
        std::string          usejournal;

};

#define _LJEventEditor_
#endif
