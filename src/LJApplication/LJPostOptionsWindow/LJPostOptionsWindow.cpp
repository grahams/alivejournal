
#include <Application.h>
#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <Message.h>
#include <Messenger.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <List.h>

#include "message_labels.h"
#include "LJPostOptionsWindow.h"

LJPostOptionsWindow::LJPostOptionsWindow()
                         : BWindow(BRect(100, 100, 400, 243), "Post Options", 
                            B_TITLED_WINDOW, 0, B_CURRENT_WORKSPACE)
{
    MainView = new LJPostOptionsWindowMainView();
    AddChild(MainView);
    SetSizeLimits(300, 300, 143, 143);

    // setup the security menu
    BMenu *menu = MainView->securityField->Menu();
    BMenuItem *item;
    BMessage *message;

    message = new BMessage( SECURITY_MSG );
    message->AddString( "security", "public" );
    item = new BMenuItem( "Public", message );
    item->SetMarked( true );
    menu->AddItem( item );

    message = new BMessage( SECURITY_MSG );
    message->AddString( "security", "private" );
    item = new BMenuItem( "Private", message );
    menu->AddItem( item ); 

    message = new BMessage( SECURITY_MSG );
    message->AddString( "security", "friends" );
    item = new BMenuItem( "Friends Only", message );
    menu->AddItem( item );

    current_msg = new BMessage( CURRENT_MSG );
}

LJPostOptionsWindow::~LJPostOptionsWindow(void)
{

}

bool
LJPostOptionsWindow::QuitRequested(void)
{
    clearCurrents();    
    Hide();
    return false;
}

void
LJPostOptionsWindow::MessageReceived(BMessage *message)
{
    switch (message->what) {
        case DEFAULT_MSG: {
            //Nothing here :)
            break;
        }       

        case CURRENT_MSG: {
            if( strcmp( "", MainView->otherMood->Text() ) ) {
                current_msg->RemoveName( "mood_name" );
                current_msg->AddString( "mood_name", 
                                                MainView->otherMood->Text() );
            }

            if( strcmp( "", MainView->music->Text() ) ) {
                current_msg->RemoveName( "music" );
                current_msg->AddString( "music", MainView->music->Text() );
            }

            if( MainView->autoFormat->Value() == B_CONTROL_ON ) {
                current_msg->RemoveName( "autoformat" );
                current_msg->AddBool( "autoformat", true ); 
            }

            be_app->MessageReceived(current_msg); 
            
            Hide();
            break;
        }

        case CLEAR_MSG: {
            BMenuItem *tempitem = NULL;

            tempitem =  MainView->moodField->Menu()->FindMarked();

            if ( tempitem != NULL) {
                tempitem->SetMarked( false );    
            }

            tempitem =  MainView->journalField->Menu()->FindMarked();

            if ( tempitem != NULL) {
                tempitem->SetMarked( false );    
            }
            
            MainView->otherMood->SetText( "" );
            MainView->music->SetText( "" );

            break;
        }

        case MUSICBUTTON_MSG: {
            // We send a message to soundplay to get some information about
            // the currently playing track, and 'answer' contans soundplay's
            // response, if there was one.   Thanks to Marco Nelissen for
            // help with this code.
            BMessage question( B_GET_PROPERTY );
            BMessage answer;
            BMessenger messenger( "application/x-vnd.marcone-soundplay" );

            question.AddSpecifier( "info" );
            question.AddSpecifier( "track", int32(0) );
            messenger.SendMessage( &question, &answer, 2E6, 2E6 );

            if( answer.FindString("name") != NULL ) {
                BString nametemp( answer.FindString("name") );                

                // remove the .mp3 extension if it exists.  Might want to do
                // some more elegant handling of this later to strip off
                // other extensions (like .ogg, .aiff, etc)
                nametemp.RemoveLast( ".mp3" );
                MainView->music->SetText( nametemp.String() );
            }  

            break;
        }

        case MOOD_MSG: {
            current_msg->RemoveName( "mood_id" );
            current_msg->AddString( "mood_id", 
                                            message->FindString("mood_id") );

            MainView->otherMood->SetText( "" );
            break;
        }

        case PICTURE_MSG: {
            current_msg->RemoveName( "picture" );
            current_msg->AddString( "picture",
                                            message->FindString("picture") );
            break;
        }

        case JOURNAL_MSG: {
            current_msg->RemoveName( "usejournal" );
            current_msg->AddString( "usejournal",
                                            message->FindString("usejournal") );
            break;
        }

        case SECURITY_MSG: {
            current_msg->RemoveName( "security" );
            current_msg->AddString( "security",
                                            message->FindString("security") );
            break;
        }

        case NEW_MOOD_MSG: {
            BMessage *outMsg = new BMessage( MOOD_MSG );

            outMsg->AddString( "mood_id", message->FindString("mood_id") );
            outMsg->AddString( "mood_name", message->FindString("mood_name") );
        
            Lock();
            MainView->moodField->Menu()->AddItem( 
                    new BMenuItem(outMsg->FindString("mood_name"), outMsg) );
            Unlock();

            break;
        }

        case NEW_JOURNAL_MSG: {
            BMessage *outMsg = new BMessage( JOURNAL_MSG );

            outMsg->AddString( "usejournal",message->FindString("usejournal") );
            Lock();
            MainView->journalField->Menu()->AddItem( 
                    new BMenuItem(outMsg->FindString("usejournal"), outMsg) );
            Unlock();

            break;
        }

        case NEW_PICTURE_MSG: {
            BMessage *outMsg = new BMessage( PICTURE_MSG );

            outMsg->AddString( "picture", message->FindString("picture") );
        
            Lock();
            MainView->picField->Menu()->AddItem( 
                    new BMenuItem(outMsg->FindString("picture"), outMsg) );
            Unlock();

            break;
        }

        default: {
            BWindow::MessageReceived(message);
            break;
        }
    }

}

void
LJPostOptionsWindow::clearCurrents()
{
    BMenuItem *tempitem = NULL;

    MainView->otherMood->SetText( "" );
    MainView->music->SetText( "" );

    tempitem =  MainView->moodField->Menu()->FindMarked();
    if ( tempitem != NULL ) {
        tempitem->SetMarked( false );    
    }    
    
    tempitem =  MainView->journalField->Menu()->FindMarked();
    if ( tempitem != NULL ) {
        tempitem->SetMarked( false );    
    }    

    tempitem =  MainView->picField->Menu()->FindMarked();
    if ( tempitem != NULL ) {
        tempitem->SetMarked( false );    
    }    

    tempitem =  MainView->securityField->Menu()->FindMarked();
    if ( tempitem != NULL ) {
        tempitem->SetMarked( false );    
    }    
}

