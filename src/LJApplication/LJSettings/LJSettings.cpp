
#include <Application.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <FindDirectory.h>
#include <Directory.h>
#include <Entry.h>
#include <Path.h>
#include <File.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <Alert.h>

#include "message_labels.h"
#include "LJSettings.h"


LJSettings::LJSettings()
             : BWindow(BRect(100, 100, 460, 405), "Settings", B_TITLED_WINDOW, 
                B_NOT_H_RESIZABLE | B_NOT_V_RESIZABLE, B_CURRENT_WORKSPACE)
{
    MainView = new BView(BRect(0, 0, 360, 305), "View", B_FOLLOW_TOP_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    MainView->SetViewColor((rgb_color) {216,216,216,255});
    AddChild(MainView);

    tabview1 = new BTabView(BRect(3, 5, 357, 301), "View", B_WIDTH_FROM_WIDEST,
                            B_FOLLOW_TOP_BOTTOM | B_FOLLOW_LEFT_RIGHT, 
                            B_FULL_UPDATE_ON_RESIZE | B_WILL_DRAW |
                            B_NAVIGABLE_JUMP | B_FRAME_EVENTS | B_NAVIGABLE);

    viewnetwork = new LJSettingsViewNetwork();

    MainView->AddChild(tabview1);

    tabview1->AddTab(viewnetwork, new BTab());
    tabview1->Select(0);
    
    settings = new SettingsFile();

    if( settings->InitCheck() == B_OK ) {
        if ((settings->Load() != B_OK) || 
                                (settings->CountNames(B_ANY_TYPE) == 0)){
            settings->Save();
        }
    }
    
    //Get char variables
    const char *char_setting;
    if( settings->FindString("proxy_address", &char_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        settings->AddString( "proxy_address", "" );
        settings->FindString( "proxy_address", &char_setting );
    }
    viewnetwork->prAddress->SetText(char_setting);
    
    if( settings->FindString("proxy_port", &char_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        settings->AddString( "proxy_port", "" );
        settings->FindString( "proxy_port", &char_setting );
    }
    viewnetwork->prPort->SetText(char_setting);
    
    bool bool_setting = false;
    if( settings->FindBool("use_proxy", &bool_setting) == B_NAME_NOT_FOUND ) {
        settings->AddBool( "use_proxy", false );
    }

    if( bool_setting ) {
        viewnetwork->use_proxy->SetValue(B_CONTROL_ON);
    }
    
    //Get Int16 variable
    int16 int_setting;
    char *port_number = new char(16);

    // Get the LJ Server/Port data
    if( settings->FindString("server_address", &char_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        settings->AddString( "server_address", "www.livejournal.com" );
        settings->FindString( "server_address", &char_setting );
    }
    viewnetwork->serverAddress->SetText(char_setting);
    
    if( settings->FindString("server_port", &char_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        settings->AddString( "server_port", "80" );
        settings->FindString( "server_port", &char_setting );
    }
    viewnetwork->serverPort->SetText(char_setting);

    if( settings->FindInt16("network_timeout", &int_setting) ==
                                                        B_NAME_NOT_FOUND ) {
        settings->AddInt16("network_timeout", 10 );
        int_setting = 10;
    }

    sprintf(port_number, "%i", int_setting);
    viewnetwork->tcTimeout->SetText(port_number);
    
    accept_closing = false;
}

LJSettings::~LJSettings(void)
{

}

bool
LJSettings::QuitRequested(void)
{

    Hide();
    return false;

}

void
LJSettings::MessageReceived(BMessage *message)
{

    switch (message->what){
        case PROXY_ADDR_MSG:
            settings->ReplaceString("proxy_address", viewnetwork->prAddress->Text());
            break;
        case PROXY_PORT_MSG:
            settings->ReplaceString("proxy_port", viewnetwork->prPort->Text());
            break;
        case SERVER_ADDR_MSG:
            settings->ReplaceString("server_address", viewnetwork->serverAddress->Text());
            break;
        case SERVER_PORT_MSG:
            settings->ReplaceString("server_port", viewnetwork->serverPort->Text());
            break;
        case USE_PROXY_MSG:
            if (viewnetwork->use_proxy->Value() == B_CONTROL_ON){
                settings->ReplaceBool("use_proxy", true);
            }else{
                settings->ReplaceBool("use_proxy", false);
            }
        case TC_TIMEOUT_MSG:
            settings->ReplaceInt16("network_timeout", atoi(viewnetwork->tcTimeout->Text()));
            break;
    }
}


