
#ifndef _LJViewFriends_
#define _LJViewFriends_

#include <View.h>
#include <Bitmap.h>
#include <ScrollView.h>
#include <ListView.h>
#include <TextControl.h>
#include <TextView.h>
#include <Application.h>
#include <Button.h>
#include <StringView.h>
#include <stdio.h>

class LJViewFriends : public BView
{
    public: // Constructors & Destructor
        LJViewFriends ();
        ~LJViewFriends (void);

    public: // Member Functions
        virtual void AttachedToWindow(void);
        virtual void AllAttached(void);
        virtual void MessageReceived(BMessage *message);

    public: // Data Members
        BScrollView     *scrollview1;
        BListView       *friends_list;
        BTextControl    *tcAddUser;
        BButton         *buttonAdd;
        BButton         *buttonDel;
        BStringView     *stringview1;
        BStringView     *stringview2;
        BStringView     *stringview3;
        void            FillList(void);
        thread_id       fill_thread;

        bool            _bxl_attached;
};

#endif // _LJViewFriends_
