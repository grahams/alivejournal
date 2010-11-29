
#include <Application.h>
#include <NetworkKit.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <Alert.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>
#include <OS.h>
#include <queue>
#include <ListView.h>

#include "message_labels.h"
#include "LJApplication.h"
#include "LJSettingsViewNetwork.h"
#include "ResponseBlockManager.h"
#include "CommandBlockManager.h"
#include "LJViewFriends.h"
#include "HistoryListItem.h"


LJApplication::LJApplication()
                     : BApplication("application/x-CSH.AliveJournal-Client")
{
    _bxl_attached = false;

    help = new BubbleHelper();
    help->EnableHelp(true);

    mood_id = NULL;
    mood_name = NULL;
    music = NULL;
    security = NULL;
    usejournal = NULL;
    picture = NULL;
    autoformat = true;
}

LJApplication::~LJApplication(void)
{
    if( help != NULL ) {
        delete help;
        help = NULL;
    }
}

void
LJApplication::ReadyToRun(void)
{
    LoginWindow = new LJLoginWindow();
    AccountWindow = new LJAccount();
    SettingsWindow = new LJSettings();
    WriterWindow = new LJWriter();
    EventEditor = new LJEventEditor();
    PostOptionsWindow = new LJPostOptionsWindow();
    AboutWindow = new LJAboutWindow();
    SettingsFile *set = SettingsWindow->settings;

    if (!_bxl_attached){
        _bxl_attached = true;
    }

    const char *string_setting;
    bool bool_setting = false;
    
    LoginWindow->Lock();
    if( set->FindString("username", &string_setting) == B_NAME_NOT_FOUND ) {
        set->AddString( "username", "" );
        string_setting = new char[sizeof("")];
        string_setting = strcpy((char *)string_setting,"");
    }

    LoginWindow->tcUsername->SetText(string_setting);
    current_username = const_cast <char *> (LoginWindow->tcUsername->Text());
    
    if( set->FindBool("save_password", &bool_setting) == B_NAME_NOT_FOUND ) {
        set->AddBool( "save_password", false );
        set->AddString( "password", "" );
        bool_setting = false;
    }

    BMessage *saved_moods = new BMessage( SAVED_MOODS_MSG ); 
    if( set->FindMessage("saved_moods", saved_moods) == B_NAME_NOT_FOUND ) {
        saved_moods = new BMessage( SAVED_MOODS_MSG );
        saved_moods->AddInt16( "mood_count", 0 );
        saved_moods->AddInt16( "high_mood_id", 0 );
        set->AddMessage( "saved_moods", saved_moods );
    }

    if (bool_setting) {
        LoginWindow->cbPassword->SetValue(B_CONTROL_ON);
        set->FindString("password", &string_setting);
        LoginWindow->tcPassword->SetText(string_setting);
    }

    current_password = const_cast <char *> (LoginWindow->tcPassword->Text());
    LoginWindow->Unlock();

    if( set->FindBool( "autoformat", &autoformat ) == B_NAME_NOT_FOUND ) {
        set->AddBool( "autoformat", true ); 
        autoformat = true;
    }
    PostOptionsWindow->MainView->autoFormat->SetValue(autoformat); 

    BPoint point_setting;
    BPoint new_point_setting(200,200);

    if( set->FindPoint("login_window_origin", &point_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        set->AddPoint("login_window_origin", new_point_setting );
        point_setting = new_point_setting;
    }
    LoginWindow->MoveTo(point_setting);
    
    if( set->FindPoint("settings_window_origin", &point_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        set->AddPoint("settings_window_origin", new_point_setting );
        point_setting = new_point_setting;
    }
    SettingsWindow->MoveTo(point_setting);
    
    if( set->FindPoint("writer_window_origin", &point_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        set->AddPoint("writer_window_origin", new_point_setting );
        point_setting = new_point_setting;
    }
    WriterWindow->MoveTo(point_setting);

    if( set->FindPoint("writer_window_size", &point_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        set->AddPoint("writer_window_size", 
                                  WriterWindow->Bounds().RightBottom()  );
        set->FindPoint("writer_window_size", &point_setting );
    }
    WriterWindow->ResizeTo(point_setting.x, point_setting.y );

    if( set->FindPoint("account_window_origin", &point_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        set->AddPoint("account_window_origin", new_point_setting );
        point_setting = new_point_setting;
    }
    AccountWindow->MoveTo(point_setting);
    
    if( set->FindPoint("eventeditor_window_origin", &point_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        set->AddPoint("eventeditor_window_origin", new_point_setting );
        point_setting = new_point_setting;
    }
    EventEditor->MoveTo(point_setting);

    if( set->FindPoint("eventeditor_window_size", &point_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        set->AddPoint("eventeditor_window_size", 
                                  EventEditor->Bounds().RightBottom()  );
        set->FindPoint("eventeditor_window_size", &point_setting );
    }
    EventEditor->ResizeTo(point_setting.x, point_setting.y );
    
    LoginWindow->Show();
}

bool
LJApplication::QuitRequested(void)
{
    SettingsFile *set = SettingsWindow->settings;

    SettingsWindow->accept_closing = true;
    
    set->ReplacePoint("login_window_origin", 
                                            LoginWindow->Frame().LeftTop() );

    set->ReplacePoint("settings_window_origin", 
                                            SettingsWindow->Frame().LeftTop() );

    set->ReplacePoint("writer_window_origin", 
                                            WriterWindow->Frame().LeftTop() );
    set->ReplacePoint("writer_window_size", 
                                        WriterWindow->Bounds().RightBottom() );

    set->ReplacePoint("account_window_origin", 
                                            AccountWindow->Frame().LeftTop() );

    set->ReplacePoint("eventeditor_window_origin", 
                                            EventEditor->Frame().LeftTop() );
    set->ReplacePoint("eventeditor_window_size", 
                                        EventEditor->Bounds().RightBottom() );

    set->ReplaceString("username", current_username);
    bool save_password;
    
    if( LoginWindow->cbPassword->Value() == B_CONTROL_ON ) {
        set->ReplaceString( "password", current_password );
        save_password = true;
    }
    else {
        set->ReplaceString( "password", "" );
        save_password = false;
    }
    
    set->ReplaceBool( "autoformat", autoformat );
    set->ReplaceBool("save_password", save_password);

    set->Save();
    
    while( CountWindows()>0 ) {
        WindowAt(0)->Lock();
        WindowAt(0)->Quit();
    }

    return true;

}

void
LJApplication::MessageReceived(BMessage *message)
{
    const char *username;
    FriendListItem *item;

    switch (message->what) {
        case SETTINGS_MSG:
            SettingsWindow->Show();
            break;
        case SHOW_WRITER_WINDOW_MSG:
            WriterWindow->Show();
            break;
        case LOGON_MSG:
            DoLogin();
            break;
        case SEND_MSG:
            SendEvent();
            break;
        case ACCOUNT_MSG:
            AccountWindow->Show();
            AccountWindow->FillList();
            break;
        case LOGOFF_MSG:    
            WriterWindow->Hide();
            AccountWindow->CleanUp();
            LoginWindow->Show();
            break;
        case POSTOPTIONS_MSG:
            PostOptionsWindow->Show();
            break;
        case CLEARWRITER_MSG:
            WriterWindow->Lock();
            WriterWindow->tvEvent->SetText(NULL, (int32) 0);
            WriterWindow->tcSubject->SetText("");
            WriterWindow->Unlock();
        case CURRENT_MSG:
            // Did the user specify a mood?
            if( message->FindString( "mood_id" ) ){
                if( mood_id == NULL ) {
                    mood_id= new char[16];
                }
                mood_id = strcpy( mood_id, message->FindString( "mood_id" ) );
            }
            else if( message->FindString( "mood_name" ) ){
                if( mood_name == NULL ) {
                    mood_name = new char[128];
                }
                mood_name = strcpy( mood_name, 
                                    message->FindString( "mood_name" ) ); 
            }

            // Did the user specify a current song?
            if( message->FindString( "music" ) ) {
                if( music == NULL ) {
                    music = new char[128];
                }
                music = strcpy( music,  message->FindString( "music" ) );
            }

            // Did the user specify an alternate journal?
            if( message->FindString("usejournal") ) {
                if( usejournal == NULL ) {
                    usejournal = new char[128];
                }
                usejournal = 
                    strcpy( usejournal, message->FindString("usejournal") );
            }
        
            // Did the user specify an alternate picture?
            if( message->FindString("picture") ) {
                if( picture == NULL ) {
                    picture = new char[128];
                }
                picture = strcpy( picture, message->FindString("picture") );
            }

            // Did the user specify an alternate picture?
            if( message->FindString("security") ) {
                if( security == NULL ) {
                    security = new char[128];
                }
                security = strcpy( security, message->FindString("security") );
            }

            // Should we autoformat the entered text?
            if( message->FindBool( "autoformat" ) ) {
                autoformat = true;
            }
            else {
                autoformat = false;
            }

            break;
        case ADDFRIEND_MSG:
            if (message->FindString("username", &username) == B_OK)
                AddFriend(username);
            break;
        case DELFRIEND_MSG:
            if (message->FindPointer("friendItem", (void **)&item) == B_OK)
                RemoveFriend(item);
            break;
        case CLEAR_EVENT_EDITOR:
            if (EventEditor->IsHidden()) EventEditor->Show();
            EventEditor->ClearEditor();
            break;
        case EDIT_HISTORY_EVENT:
            const char *uj;
            if(message->FindString("itemid", &EventEditor->itemid) != B_OK) {
                break;     
            }
            if(message->FindString("usejournal", &uj) == B_OK) {
                if( uj != NULL ) {
                    EventEditor->SetJournal(uj);
                }
            }
            else {
                EventEditor->SetJournal("");
            }

            EventEditor->GetItem();
            break;
        default:
            BApplication::MessageReceived(message);
            break;
    }
}

void
LJApplication::AboutRequested(void)
{
    AboutWindow->Show();
}

int
sortMoods( const void *one, const void *two )
{
    return( strcmp( (*(BMessage**)one)->FindString("mood_name"), 
                  (*(BMessage**)two)->FindString("mood_name") ) );
}

void
LJApplication::DoLogin(void)
{
    BMessage *saved_moods = new BMessage( SAVED_MOODS_MSG );
    SettingsFile *set = SettingsWindow->settings;
    const char *old_username;
    const char *old_password;
    int16 mood_count = 0;
    int16 saved_mood_count = 0;
    int16 high_mood_id = 0;
    int16 access_count = 0;
    int16 pickw_count = 0;
    
    old_username = current_username;
    old_password = current_password;
    current_username = const_cast <char *> (LoginWindow->tcUsername->Text());
    current_password = const_cast <char *> (LoginWindow->tcPassword->Text());
    
    LoginWindow->Lock();
    LoginWindow->tcUsername->SetEnabled(false);
    LoginWindow->tcPassword->SetEnabled(false);
    LoginWindow->Unlock();

    set->FindMessage("saved_moods", saved_moods);

    saved_moods->FindInt16( "mood_count", &saved_mood_count );
    saved_moods->FindInt16( "high_mood_id", &high_mood_id );

    CommandBlockManager *man = new CommandBlockManager();
    char moodnum[16];
    sprintf( (char *)&moodnum, "%d", high_mood_id );

    man->SetValue("mode", "login");
    man->SetValue("getmoods", moodnum);
    man->SetValue("getpickws", "1");
    man->SetValue("clientversion", CLIENT_VERSION, true);

    char *response = SendCommand(man);

    delete man;
    man = NULL;

    if (response){
        ResponseBlockManager *block = new ResponseBlockManager(response);

        if( block->GetValue("success") ) {
            if( (!strcmp(block->GetValue("success"), "OK"))
                                            && (block->GetValue("name")) ) {
                int namelen = strlen(block->GetValue("name"));
                char *title = new char[namelen+16];

                // This is for My Roommate, Jon Parise.. :P
                if( block->GetValue("name")[namelen-1] == 's' ) {
                    sprintf(title, "%s' LiveJournal", block->GetValue("name"));
                }
                else {
                    sprintf(title, "%s's LiveJournal", block->GetValue("name"));
                }

                WriterWindow->SetTitle(title);
                delete title;

                LoginWindow->Hide();
                WriterWindow->Show();
                SettingsWindow->settings->ReplaceString("username", 
                                                        current_username);
                SettingsWindow->settings->ReplaceString("password", 
                                                        current_password);
            }
            else if (block->GetValue("errmsg")){
                (new BAlert("", block->GetValue("errmsg"), "OK", NULL, NULL, 
                    B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
            }

            LoginWindow->Lock();
            LoginWindow->tcUsername->SetEnabled(true);
            LoginWindow->tcPassword->SetEnabled(true);
            LoginWindow->Unlock();

            if(block->GetValue("mood_count")) {
                mood_count = atoi( block->GetValue("mood_count") );
            }
            else {
                mood_count = 0;
            }
                    
            // Get the list of moods to populate the mood menu in Post Options
            if ( (saved_mood_count > 0) || (mood_count > 0) ) {
                BList list(100);
                char *request_temp;
                int i;
                
                request_temp = new char[16];

                // add the stored moods to the list
                for( i = 1; i <= saved_mood_count; i++ ) {
                    BMessage *newMoodMsg = new BMessage( NEW_MOOD_MSG );

                    // Get the id and name for the current entry.
                    sprintf( request_temp, "mood_%i_id", i );
                    newMoodMsg->AddString( "mood_id", 
                                    saved_moods->FindString(request_temp) );

                    sprintf( request_temp, "mood_%i_name", i );
                    newMoodMsg->AddString( "mood_name", 
                                    saved_moods->FindString(request_temp) );

                    list.AddItem( newMoodMsg );
                }

                // add the new moods to the list
                for( i = 1; i <= mood_count; i++ ) {
                    BMessage *newMoodMsg = new BMessage( NEW_MOOD_MSG );

                    // Get the id and name for the current entry.
                    sprintf( request_temp, "mood_%i_id", i );
                    const char *tempId = block->GetValue(request_temp);

                    newMoodMsg->AddString( "mood_id", tempId );
                    sprintf( request_temp, "mood_%i_id", i+saved_mood_count );
                    saved_moods->AddString( request_temp, tempId );

                    if( atoi(tempId) > high_mood_id ) {
                        high_mood_id = atoi( tempId );
                    }

                    sprintf( request_temp, "mood_%i_name", i );
                    const char *tempName = block->GetValue(request_temp);

                    newMoodMsg->AddString( "mood_name", tempName );
                    sprintf( request_temp, "mood_%i_name", i+saved_mood_count );
                    saved_moods->AddString( request_temp, tempName );

                    list.AddItem( newMoodMsg );
                }

                // sort the list alphabetically
                list.SortItems( sortMoods );

                // add each item of the list to the menu
                while( list.CountItems() > 0 ) {
                    BMessage *newMoodMsg = (BMessage *) list.FirstItem();
                    list.RemoveItem(newMoodMsg);
                    PostOptionsWindow->PostMessage( newMoodMsg );
                    delete newMoodMsg;
                }

                // store any new moods that were received this time around
                if( mood_count > 0 ) {
                    saved_moods->ReplaceInt16( "mood_count", 
                                                saved_mood_count + mood_count );
                    saved_moods->ReplaceInt16( "high_mood_id", high_mood_id );
                                                             
                    set->ReplaceMessage( "saved_moods", saved_moods );
                    set->Save();
                }
                delete request_temp;
            }

            if(block->GetValue("access_count")) {
                access_count = atoi( block->GetValue("access_count") );
            }
                    
            // Get the list of journals this user has access to
            if ( access_count >= 0 ) {
                BMessage *newJournalMsg = new BMessage( NEW_JOURNAL_MSG );
                char *request_temp = new char[16];

                newJournalMsg->AddString( "usejournal", "default" );
                PostOptionsWindow->PostMessage( newJournalMsg );
                delete newJournalMsg;

                for( int i = 1; i <= access_count; i++ ) {
                    newJournalMsg = new BMessage( NEW_JOURNAL_MSG );

                    // Get the id for the current entry.
                    sprintf( request_temp, "access_%i", i );
                    newJournalMsg->AddString("usejournal", 
                                            block->GetValue(request_temp) );

                    PostOptionsWindow->PostMessage( newJournalMsg );
                    AccountWindow->viewHistory->MessageReceived(newJournalMsg);

                    delete newJournalMsg;
                }
                delete request_temp;
            }

            if(block->GetValue("pickw_count")) {
                pickw_count = atoi( block->GetValue("pickw_count") );
            }
                    
            // Get the list of pictures the user has set
            if( pickw_count > 0 ) {
                char *request_temp = new char[16];
                int i;
                
                for( i = 1; i <= pickw_count; i++ ) {
                    BMessage *newPictureMsg = new BMessage( NEW_PICTURE_MSG );

                    // Get the id for this picture
                    sprintf( request_temp, "pickw_%i", i );
                    newPictureMsg->AddString("picture", 
                                            block->GetValue(request_temp) );

                    PostOptionsWindow->PostMessage( newPictureMsg );

                    delete newPictureMsg;
                }
                delete request_temp;
            }
        }

        delete block;
    }
    else {
        printf( "response was null\n" );
    }
    
    delete saved_moods;
}

void
LJApplication::SendEvent(void)
{
    char *text = const_cast <char *> (WriterWindow->tvEvent->Text());
    char *subject = const_cast <char *> (WriterWindow->tcSubject->Text());
    
    time_t      now;
    char t_year[16];
    char t_mon[16];
    char t_day[16];
    char t_hour[16];
    char t_min[16]; 

    //Formating Time    
    now = time(NULL);
    strftime( (char *)&t_year, 16, "%Y", localtime(&now) ); 
    strftime( (char *)&t_mon, 16, "%m", localtime(&now) ); 
    strftime( (char *)&t_day, 16, "%d", localtime(&now) ); 
    strftime( (char *)&t_hour, 16, "%H", localtime(&now) ); 
    strftime( (char *)&t_min, 16, "%M", localtime(&now) ); 
    
    CommandBlockManager *man = new CommandBlockManager();

    man->SetValue("mode", "postevent");
    man->SetValue("event", text, true);
    man->SetValue("subject", subject, true);
    man->SetValue("year", (char *)&t_year);
    man->SetValue("mon", (char *)&t_mon);
    man->SetValue("day", (char *)&t_day);
    man->SetValue("hour", (char *)&t_hour);
    man->SetValue("min", (char *)&t_min);
    
    // If the user chose a "current music", send it to the server
    if( music != NULL ) {
        man->SetValue("prop_current_music", music, true);
    }   
    
    // If the user chose a "current mood", send it (and its id) to the server
    if( mood_id != NULL ) {
        man->SetValue("prop_current_moodid", mood_id, true);
    }
    if( mood_name != NULL ) {
        man->SetValue("prop_current_mood", mood_name, true);
    }

    // Send the security settings for the user to the server
    if( security != NULL ) {
        if( !strcmp(security, "friends") ) {
            man->SetValue("security", "usemask", true);
            man->SetValue("allowmask", "1", true);
        }
        else {
            man->SetValue("security", security, true);
        }
    }

    if( usejournal != NULL ) {
        if( strcmp( usejournal, "default" ) ) {
            man->SetValue("usejournal", usejournal, true);
        }
    }

    if( picture != NULL ) {
        man->SetValue("prop_picture_keyword", picture, true);
    }

    if( autoformat ) {
        man->SetValue("prop_opt_preformatted", "0", true);
    }
    else {
        man->SetValue("prop_opt_preformatted", "1", true);
    }

    char * response = SendCommand(man);

    delete man;
    man = NULL;
    
    if(response) {
        ResponseBlockManager *block = new ResponseBlockManager(response);
        
        if( block->GetValue("success") ) {
            if( !strcmp(block->GetValue("success"), "OK") ) {
                WriterWindow->Lock();
                WriterWindow->tvEvent->SetText(NULL, (int32) 0);
                WriterWindow->tcSubject->SetText("");
                WriterWindow->Unlock();

                PostOptionsWindow->Lock();
                PostOptionsWindow->clearCurrents();
                PostOptionsWindow->Unlock();
                
                if( music != NULL ) {
                    delete music;
                    music = NULL;
                }   
                
                if( mood_id != NULL ) {
                    delete mood_id;
                    mood_id = NULL;
                }
                
                if( mood_name != NULL ) {
                    delete mood_name;
                    mood_name = NULL;
                }

                if( usejournal != NULL ) {
                    delete usejournal;
                    usejournal = NULL;
                }

                if( picture != NULL ) {
                    delete picture;
                    picture = NULL;
                }

                if( security != NULL ) {
                    delete security;
                    security = NULL;
                }
            }
            else if( block->GetValue("errmsg") ) {
                (new BAlert("", block->GetValue("errmsg"), "OK", NULL, NULL, 
                    B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
            }
        }
        delete block;
    }
}

void
LJApplication::AddFriend(const char *username)
{
    FriendListItem  *item;
    
    if( strlen(username)>0 ) {
        CommandBlockManager *man = new CommandBlockManager();

        man->SetValue("mode", "editfriends");
        man->SetValue("editfriend_add_1_user", username );

        char *response = SendCommand(man);

        delete man;
        man = NULL;

        if( response ) {
            ResponseBlockManager *block = new ResponseBlockManager(response);
            
            if( block->GetValue("success") ) {
                if( !strcmp(block->GetValue("success"), "OK") ) {
                    AccountWindow->Lock();
                    AccountWindow->viewfriends->friends_list->
                    AddItem(item = new FriendListItem(username, false));
                    
                    int32 index =
                       AccountWindow->viewfriends->friends_list->IndexOf(item);
                    AccountWindow->viewfriends->friends_list->Select(index);
                    AccountWindow->viewfriends->friends_list->ScrollToSelection();
                    AccountWindow->CompareLists();
                    AccountWindow->Unlock();                            
                }
                else if( block->GetValue("errmsg") ) {
                        (new BAlert("", block->GetValue("errmsg"), "OK",
                            NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
                }
            }
            delete block;
        }
    }
}

void
LJApplication::RemoveFriend(FriendListItem *item)
{
    const char *username = item->name;

    CommandBlockManager *man = new CommandBlockManager();

    man->SetValue("mode", "editfriends");
    char deletename[strlen("editfriend_delete_") + strlen(username) + 1];
    sprintf( deletename, "editfriend_delete_%s", username );
    man->SetValue(deletename, "1");

    char *response = SendCommand(man);

    delete man;
    man = NULL;

    if( response ) {
        ResponseBlockManager *block = new ResponseBlockManager(response);
        
        if( block->GetValue("success") ) {
            if( !strcmp(block->GetValue("success"), "OK") ) { 
                AccountWindow->Lock();
                AccountWindow->viewfriends->friends_list->RemoveItem(item);
                AccountWindow->CompareLists(); AccountWindow->Unlock();
            }
            else if( block->GetValue("errmsg") ) { 
                (new BAlert("", block->GetValue("errmsg"), "OK", NULL, 
                        NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go(); 
            } 
        } 

        delete block;
    }
}

char *
LJApplication::SendCommand(CommandBlockManager *man)
{
    const char      *command = FormatCommand(man);
    int             error_code = 0;
    char            *response;
    const char      *message = NULL;
    
    BNetAddress     addr;
    BNetEndpoint    net_endpoint(SOCK_STREAM);
    BNetBuffer      buffer(4096);
    
    bigtime_t       net_timeout = (bigtime_t)
                atof(SettingsWindow->viewnetwork->tcTimeout->Text())*1000000;
        
    
    bool use_proxy = false;
    const char *proxy_addr=NULL, *proxy_port=NULL;
    
    SettingsWindow->settings->FindBool("use_proxy", &use_proxy);
    net_endpoint.SetTimeout(net_timeout);  

    if( use_proxy ) {
        proxy_addr = SettingsWindow->viewnetwork->prAddress->Text();
        proxy_port = SettingsWindow->viewnetwork->prPort->Text();
        addr.SetTo(proxy_addr, atoi(proxy_port));
    }
    else {
        const char *server_addr=NULL, *server_port=NULL;

        server_addr = SettingsWindow->viewnetwork->serverAddress->Text();
        server_port = SettingsWindow->viewnetwork->serverPort->Text();

        addr.SetTo(server_addr, atoi(server_port));
    }
    
        char foobie[65536];
    if( net_endpoint.InitCheck() != B_OK ) {
        error_code = 2;
    }
    else if( net_endpoint.Connect(addr) != B_OK ) {
        error_code = 3;
    }
    else { 
        cout << "Command:" << command << endl;
        net_endpoint.Send( command, strlen(command) );
        //delete command;

        memset( (char *) &foobie, 0, 65536);

        //while( net_endpoint.Receive(buffer, 1024) > 1 ) {
        while( net_endpoint.Receive(foobie, 1024) > 1 ) {
            error_code = 1; //indicating data are received
        }
    }

    switch (error_code) {
        case 0:
            //No Response at all -- loop timed out.
            message = "Network Error : no answer  -- connection timed out.";
            break;
        case 1:
            //Response received correctly
            //response = new char [buffer.Size()+1];
            //buffer.RemoveString(response, buffer.Size());
            response = new char [strlen(foobie)+1];
            response = strcpy(response, foobie);
            return response;
            break;
        case 2:
            //Network Error : Can't open Internet Connection
            message = "Network Error : Can't open Internet Connection.";
            break;
        case 3:
            //Network Error : Can't connect to site
            message = "Network Error : Can't connect to site.";
            break;
    }
    
    if( message ) {
        BAlert *alert = new BAlert("neterror_alert", message, "OK", 
            NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
        alert->Go();
    }

    return NULL;
}

const char *
LJApplication::FormatCommand(CommandBlockManager *man)
{
    char *str_len = new char[16];
    memset( (void *)str_len, 0, 16);


    man->SetValue("user", current_username, true);
    man->SetValue("password", current_password, true);

    bool use_proxy = false;
    SettingsWindow->settings->FindBool("use_proxy", &use_proxy);
    
    string header;

    if( use_proxy ) {
        header = "POST http://";
        header += SettingsWindow->viewnetwork->serverAddress->Text();
        header += "/cgi-bin/log.cgi HTTP/1.1\r\n";
    }
    else {
        header = "POST /cgi-bin/log.cgi HTTP/1.1\r\n";
    }
    
    header += "Host: ";
    header += SettingsWindow->viewnetwork->serverAddress->Text();
    header += "\r\n";

    char *content;
    content = man->GetCommandBlock();
    sprintf( str_len, "%ld", strlen(content) );
    
    header += "Content-length: ";
    header += str_len;
    header += "\r\n";

    delete str_len;

    header += "Content-type: application/x-www-form-urlencoded\r\n\r\n";
    header += content;
    
    char *command = new char[ strlen(header.c_str()) + 1 ];
    command = strcpy( command, header.c_str() );
    return command;
}


const char *
LJApplication::EncodeString(char *string)
{
    int escapecount = 0;
    char *src, *dest;
    char *newstr;
    
    char hextable[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    
    if( string == 0 ) {
         return 0;
    }

    for( src = string; *src != 0; src++ ) {
        if( !isalnum(*src) ) { 
            escapecount++;
        }
    }
    
    newstr = new char[strlen(string) - escapecount + (escapecount * 3) + 1];
    
    src = string;
    dest = newstr;
    while( *src != 0 ) {
        if( !isalnum(*src) ) {
            *dest++ = '%';
            *dest++ = hextable[*src / 16];
            *dest++ = hextable[*src % 16];
            src++;
        } 
        else {
            *dest++ = *src++;
        }
    }

    *dest = 0;
    return newstr;
}

const char *
LJApplication::DecodeString(char *string)
{
    int destlen = 0;
    char *src, *dest;
    char *newstr;
    
    if( string == 0 ) {
        return 0;
    }

    for( src = string; *src != 0; src++ ) {
        // TODO: this isn't robust. should check the next two chars for 0 
        if( *src == '%' ) { 
            src+=2; 
        } 
        destlen++;
    }
    
    newstr = new char [destlen + 1];
    src = string;
    dest = newstr;

    while( *src != 0 ) {
        if( *src == '%' ) {
            char h = toupper(src[1]);
            char l = toupper(src[2]);
            int vh, vl;

            // SMG: Clean this garbage up into readable non-hack code
            vh = isalpha(h) ? (10+(h-'A')) : (h-'0');
            vl = isalpha(l) ? (10+(l-'A')) : (l-'0');

            *dest++ = (char) (vh*16+vl);
            src += 3;
        } 
        else if( *src == '+' ) {
            *dest++ = ' ';
            src++;
        } 
        else {
            *dest++ = *src++;
        }
    }

    *dest = 0;
    return newstr;  
}


const char *
LJApplication::GetCurrentJournal()
{
    return (const char *) usejournal;
}
