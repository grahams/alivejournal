
#include <Application.h>
#include <Alert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <OS.h>

#include "message_labels.h"
#include "LJEventEditor.h"
#include "HistoryListItem.h"
#include "ResponseBlockManager.h"
#include "CommandBlockManager.h"
#include "LJApplication.h"

static int32 get_event(void *data);
static int32 send_event(void *data);

LJEventEditor::LJEventEditor()
                    : BWindow(BRect(100, 100, 495, 318), "Event Editor",                                B_TITLED_WINDOW, 0, B_CURRENT_WORKSPACE)
{
    mainview = new BView(BRect(0, 0, 395, 218), "View", B_FOLLOW_TOP_BOTTOM | 
                        B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    mainview->SetViewColor((rgb_color) {219,219,219,255});

    textview = new BTextView(BRect(11, 81, 370, 172), "View", 
                        BRect(0,0,90,90), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | 
                        B_FULL_UPDATE_ON_RESIZE);

    scrollview = new BScrollView("View", textview, B_FOLLOW_TOP_BOTTOM | 
                        B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE, 
                        false, true, B_FANCY_BORDER);

    saveButton = new BButton(BRect(318, 186, 382, 208), "Button", "Save", 
                        new BMessage(SAVE_MSG), B_FOLLOW_BOTTOM | 
                        B_FOLLOW_RIGHT, B_WILL_DRAW | B_NAVIGABLE);

    cancelButton = new BButton(BRect(240, 185, 304, 208), "Button", "Cancel", 
                        new BMessage(Cancel_MSG), B_FOLLOW_BOTTOM | 
                        B_FOLLOW_RIGHT, B_WILL_DRAW | B_NAVIGABLE);

    tcSubject = new BTextControl(BRect(8, 46, 386, 65), "tc", "Subject", 
                        NULL, new BMessage(DEFAULT_MSG), B_FOLLOW_TOP | 
                        B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    tcSubject->SetDivider(40);

    tcMonth = new BTextControl(BRect(8, 12, 86, 31), "tc", "Month", NULL, 
                        new BMessage(DEFAULT_MSG), B_FOLLOW_TOP | 
                        B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE);
    tcMonth->SetDivider(30);

    tcDay = new BTextControl(BRect(82, 12, 151, 31), "tc", "Day", NULL, 
                        new BMessage(DEFAULT_MSG), B_FOLLOW_LEFT | 
                        B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    tcDay->SetDivider(20);

    tcYear = new BTextControl(BRect(145, 12, 217, 31), "tc", "Year", NULL, 
                        new BMessage(DEFAULT_MSG), B_FOLLOW_LEFT | 
                        B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    tcYear->SetDivider(25);

    dateSlash1 = new BStringView(BRect(72, 15, 82, 28), "StringView", "/", 
                        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | 
                        B_NAVIGABLE);
    dateSlash1->SetAlignment(B_ALIGN_RIGHT);

    dateSlash2 = new BStringView(BRect(134, 15, 145, 28), "StringView", "/", 
                        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | 
                        B_NAVIGABLE);
    dateSlash2->SetAlignment(B_ALIGN_RIGHT);

    tcHour = new BTextControl(BRect(222, 12, 279, 31), "tc", "H", NULL, 
                        new BMessage(DEFAULT_MSG), B_FOLLOW_TOP | 
                        B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE);
    tcHour->SetDivider(10);

    tcMinute = new BTextControl(BRect(278, 12, 336, 31), "tc", "M", NULL, 
                        new BMessage(DEFAULT_MSG), B_FOLLOW_TOP | 
                        B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE);
    tcMinute->SetDivider(10);

    tcSeconde = new BTextControl(BRect(331, 12, 387, 31), "tc", "S", NULL, 
                        new BMessage(DEFAULT_MSG), B_FOLLOW_TOP | 
                        B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE);
    tcSeconde->SetDivider(10);

    timeColon1 = new BStringView(BRect(266, 15, 277, 28), "StringView", ":", 
                        B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW | 
                        B_NAVIGABLE);
    timeColon1->SetAlignment(B_ALIGN_RIGHT);

    timeColon2 = new BStringView(BRect(318, 15, 330, 28), "StringView", ":", 
                        B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW | 
                        B_NAVIGABLE);
    timeColon2->SetAlignment(B_ALIGN_RIGHT);

    AddChild(mainview);
    mainview->AddChild(scrollview);
    mainview->AddChild(saveButton);
    mainview->AddChild(cancelButton);
    mainview->AddChild(tcSubject);
    mainview->AddChild(tcMonth);
    mainview->AddChild(tcDay);
    mainview->AddChild(tcYear);
    mainview->AddChild(dateSlash1);
    mainview->AddChild(dateSlash2);
    mainview->AddChild(tcHour);
    mainview->AddChild(tcMinute);
    mainview->AddChild(tcSeconde);
    mainview->AddChild(timeColon1);
    mainview->AddChild(timeColon2);

    mainview->SetViewColor((rgb_color) {219,219,219,255});

    SetSizeLimits(395, 16384, 218, 16384);
    textview->SetTextRect( textview->Bounds() );
    BFont font(be_plain_font);
    font.SetSize(13);
    textview->SetFontAndColor(&font);
    
    usejournal = "";
}

LJEventEditor::~LJEventEditor(void)
{

}

bool
LJEventEditor::QuitRequested(void)
{
    Hide();
    return false;
}

void
LJEventEditor::MessageReceived(BMessage *message)
{
    switch (message->what) {
        case SAVE_MSG: 
            thread_id get_event_thread = spawn_thread(send_event, 
                            "EventEditor::GetEvent", B_NORMAL_PRIORITY, this);
            resume_thread(get_event_thread);    
            break;
    }
}

void
LJEventEditor::FrameResized(float width, float height)
{
    textview->SetTextRect(textview->Bounds());
}

void
LJEventEditor::ClearEditor()
{
    Lock();
    textview->SetText(NULL, (int32) 0);
    
    tcSubject->SetText("");
    tcYear->SetText("");
    tcMonth->SetText("");
    tcDay->SetText("");
    tcHour->SetText("");
    tcMinute->SetText("");
    tcSeconde->SetText("");
    Unlock();
}


void
LJEventEditor::GetItem()
{
    thread_id get_event_thread = spawn_thread(get_event, 
                            "EventEditor::GetEvent", B_NORMAL_PRIORITY, this);
    resume_thread(get_event_thread);    
}


static int32 get_event(void *data)
{
    LJEventEditor *w = (LJEventEditor *)data;
    static ResponseBlockManager *block = NULL;

    CommandBlockManager *man = new CommandBlockManager();

    man->SetValue("mode", "getevents");
    man->SetValue("selecttype", "one");
    man->SetValue("itemid", w->itemid);
    man->SetValue("lineendings", "unix");
    if( w->usejournal.length() > 0 ) {
        man->SetValue("usejournal", w->usejournal.c_str() );
    }
    
    char *response = ((LJApplication *)be_app)->SendCommand(man);

    delete man;
    man = NULL;
    
    if (response){
        if( block != NULL ) {
            delete block;
            block = NULL;
        }
        block = new ResponseBlockManager(response);
        
        if ( block->GetValue("success") ) {
            if ( strcmp(block->GetValue("success"), "OK") ) {
                if ( block->GetValue("errmsg") ) {
                    (new BAlert("", block->GetValue("errmsg"), "OK", NULL, 
                                NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
                }
                delete block;
                block = NULL;
                return 0;
            }
            
            w->Lock();
            const char *event = block->GetValue("events_1_event", true);
            const char *time = block->GetValue("events_1_eventtime");
            
            if ( event ) {
                w->textview->SetText( event, strlen(event));
            }

            if ( time ) {
                char *eventtime = new char [strlen(time)+1];
                strcpy(eventtime, time);
                
                if ( block->GetValue("events_1_subject") ) {
                    w->tcSubject->SetText( 
                                        block->GetValue("events_1_subject") );
                }
                    
                if (block->GetValue("events_1_eventtime")){
                    w->tcYear->TextView()->SetText(eventtime, 4);
                    w->tcMonth->TextView()->SetText(&eventtime[5], 2);
                    w->tcDay->TextView()->SetText(&eventtime[8], 2);
                    w->tcHour->TextView()->SetText(&eventtime[11], 2);
                    w->tcMinute->TextView()->SetText(&eventtime[14], 2);
                    w->tcSeconde->TextView()->SetText(&eventtime[17], 2);
                }
                delete eventtime;
            }
            if (w->IsLocked()) w->Unlock();
            
        }       
        delete block;
        block = NULL;
    }
    
    return B_OK;
}

static int32 send_event(void *data)
{

    LJEventEditor *w = (LJEventEditor *)data;

    CommandBlockManager *man = new CommandBlockManager();
    
    man->SetValue("mode", "editevent");
    man->SetValue("itemid", w->itemid );
    man->SetValue("lineendings", "unix");
    man->SetValue("subject", w->tcSubject->Text(), true);
    man->SetValue("event", w->textview->Text(), true);
    if( w->usejournal.length() > 0 ) {
        man->SetValue("usejournal", w->usejournal.c_str() );
    }

    man->SetValue("year", w->tcYear->Text());
    man->SetValue("mon", w->tcMonth->Text());
    man->SetValue("day", w->tcDay->Text());
    man->SetValue("hour", w->tcHour->Text());
    man->SetValue("min", w->tcMinute->Text());
    
    char *response = ((LJApplication *)be_app)->SendCommand(man);

    delete man;
    man = NULL;
    
    if ( response ){
        ResponseBlockManager *block = new ResponseBlockManager(response);
        
        if( block->GetValue("success") ) {
            if( strcmp(block->GetValue("success"), "OK") ) {
                if( block->GetValue("errmsg") ) {
                    (new BAlert("", block->GetValue("errmsg"), "OK", NULL, 
                        NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
                }
                return 0;
            }
            else {
                (new BAlert("", "Event Updated", "OK"))->Go();
                w->PostMessage(B_QUIT_REQUESTED);
            }
        }
        
        delete block;
    }
    
    return B_OK;
}

void
LJEventEditor::SetJournal(const char *journalname)
{
    usejournal = journalname;
}

