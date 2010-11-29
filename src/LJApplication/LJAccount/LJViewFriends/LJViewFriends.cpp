
#include <Application.h>
#include <Window.h>
#include <Alert.h>
#include <Font.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <OS.h>

#include "message_labels.h"

#include "LJViewFriends.h"
#include "FriendListItem.h"
#include "ResponseBlockManager.h"
#include "CommandBlockManager.h"
#include "LJApplication.h"

LJViewFriends::LJViewFriends()
                         : BView(BRect(10, 10, 353, 222), "Friends", 
                                B_FOLLOW_TOP_BOTTOM | B_FOLLOW_LEFT_RIGHT, 
                                B_WILL_DRAW | B_NAVIGABLE)
{
    _bxl_attached = false;

    friends_list = new BListView(BRect(7, 27, 172, 204), "listview", 
                            B_SINGLE_SELECTION_LIST, B_FOLLOW_TOP_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | 
                            B_FULL_UPDATE_ON_RESIZE);

    scrollview1 = new BScrollView("View", friends_list, B_FOLLOW_TOP | 
                            B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE, false, 
                            true, B_FANCY_BORDER);

    tcAddUser = new BTextControl(BRect(196, 28, 336, 47), "tc", "Name", NULL, 
                            new BMessage(DEFAULT_MSG), B_FOLLOW_TOP | 
                            B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE);
    tcAddUser->SetDivider(30);

    buttonAdd = new BButton(BRect(228, 56, 337, 79), "ButtonAdd", 
                            "Add To List", new BMessage(ADDFRIEND_MSG), 
                            B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW | 
                            B_NAVIGABLE);
    buttonDel = new BButton(BRect(229, 97, 337, 121), "ButtonDel", 
                            "Delete Selected", new BMessage(DELFRIEND_MSG), 
                            B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW | 
                            B_NAVIGABLE);

    stringview1 = new BStringView(BRect(197, 60, 222, 73), "StringView", 
                            "<-----", B_FOLLOW_TOP | B_FOLLOW_LEFT, 
                            B_WILL_DRAW | B_NAVIGABLE);
    stringview2 = new BStringView(BRect(198, 103, 223, 114), "StringView", 
                            "----->", B_FOLLOW_TOP | B_FOLLOW_LEFT, 
                            B_WILL_DRAW | B_NAVIGABLE);

    stringview3 = new BStringView(BRect(8, 3, 331, 15), "StringView", 
       "This is your Friends List, you can add a username or delete one.", 
        B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE);

    SetViewColor( (rgb_color) {216,216,216,255} );

    BFont font(be_bold_font);
    font.SetSize(12);
    friends_list->SetFont(&font);
    friends_list->SetDrawingMode(B_OP_OVER);
}

LJViewFriends::~LJViewFriends(void)
{

}

void
LJViewFriends::AttachedToWindow(void)
{
    if ( !_bxl_attached ){
        AddChild(scrollview1);
        AddChild(tcAddUser);
        AddChild(buttonAdd);
        AddChild(buttonDel);
        AddChild(stringview1);
        AddChild(stringview2);
        AddChild(stringview3);
        _bxl_attached = true;
    }

    buttonAdd->SetTarget(this);
    buttonDel->SetTarget(this);
}

void
LJViewFriends::AllAttached(void)
{

}

void
LJViewFriends::MessageReceived(BMessage *message)
{

    const char *username = NULL;
    string cmd;

    if( (message->what == ADDFRIEND_MSG) ) {
        username = tcAddUser->Text();
        if ( strlen(username)>0 ) {
            message->AddString("username", username);
            be_app->PostMessage(message);
        }
        
    }
    else if ( (message->what == DELFRIEND_MSG) ) {
        if (friends_list->CurrentSelection()>0) {
            FriendListItem *item = (FriendListItem *) 
            friends_list->ItemAt(friends_list->CurrentSelection());
            message->AddPointer("friendItem", item);
            be_app->PostMessage(message);   
        }
    }
    else{
        BView::MessageReceived(message);
    }
}

int32 FillFriendsList_Thread(void *data)
{
    LJViewFriends *view = (LJViewFriends *)data;
    
    CommandBlockManager *man = new CommandBlockManager();
    man->SetValue("mode", "getfriends");
    char *response = ((LJApplication *)be_app)->SendCommand(man);
    
    delete man;
    man = NULL;
    
    if( response ) {
        char key[30];
        
        ResponseBlockManager *block = new ResponseBlockManager(response);

        view->Window()->Lock();

        if ( !view->friends_list->IsEmpty() ) {
            view->friends_list->MakeEmpty();
        }

        if ( view->Window()->IsLocked() ) {
            view->Window()->Unlock();
        }

        int friend_count = atoi( block->GetValue("friend_count") );
        
        if ( block->GetValue("friend_count") ) {
            view->Window()->Lock();

            for (int i=1; i <= friend_count; i++) {
                sprintf(key, "friend_%d_user", i);
                view->friends_list->AddItem(
                            new FriendListItem(block->GetValue(key), false) );
            }

            if (view->Window()->IsLocked()) {
                view->Window()->Unlock();
            }
        }
        delete block;
    }
    return B_OK;
}


void
LJViewFriends::FillList(void)
{
    fill_thread = spawn_thread( FillFriendsList_Thread, 
                        "LJViewFriends::FillList", B_NORMAL_PRIORITY, this );
    resume_thread( fill_thread );
}




