
#include <Application.h>
#include <TextControl.h>

#include "message_labels.h"
#include "LJLoginWindow.h"
#include "LJApplication.h"

LJLoginWindow::LJLoginWindow()
                         : BWindow(BRect(100, 0, 296, 329), "AliveJournal", 
                                   B_TITLED_WINDOW, 0, B_CURRENT_WORKSPACE)
{

    MainView = new BView(BRect(0, 0, 196, 329), "View", B_FOLLOW_TOP_BOTTOM 
                        | B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    MainView->SetViewColor((rgb_color) {216,216,216,255});

    // Set up the menubar
    menubar = new BMenuBar(BRect(0, 0, 16494, 16), "MenuBar", 
                    B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, B_ITEMS_IN_ROW, true);
    menuFile = new BMenu("File", B_ITEMS_IN_COLUMN);
    menuFileItemSettings = new BMenuItem("Settings", 
                                            new BMessage(SETTINGS_MSG), 0, 0);
    menuFileItemAbout = new BMenuItem("About AliveJournal", 
                                        new BMessage(B_ABOUT_REQUESTED), 0, 0);
    separator1 = new BSeparatorItem();
    menuFileItemQuit = new BMenuItem("Quit", new BMessage(MENU_QUIT), 'Q', 0);

    menubar->AddItem(menuFile);
    menuFile->AddItem(menuFileItemSettings);
    menuFile->AddItem(menuFileItemAbout);
    menuFile->AddItem(separator1);
    menuFile->AddItem(menuFileItemQuit);

    // Add the LiveJournal Logo
    LJLogo = new LJLogoView();

    // The text box for the username
    tcUsername = new BTextControl(BRect(10, 220, 188, 239), "tc", "Username", 
                        NULL, new BMessage(DEFAULT_MSG), B_FOLLOW_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    tcUsername->SetDivider(50);

    // The text box for the password 
    tcPassword = new BTextControl(BRect(10, 243, 188, 262), "tc", "Password", 
                        NULL, new BMessage(DEFAULT_MSG), B_FOLLOW_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    tcPassword->SetDivider(50);
    tcPassword->TextView()->HideTyping(true);

    // The button to log the user into LJ
    LogInButton = new BButton(BRect(13, 296, 184, 319), "Button", "Log On", 
                        new BMessage(LOGON_MSG), B_FOLLOW_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    LogInButton->MakeDefault(true);

    // Whether or not the user wants us to store their password between
    // sessions
    cbPassword = new BCheckBox(BRect(65, 267, 188, 285), "checkbox", 
                        "Remember Password", new BMessage(DEFAULT_MSG), 
                        B_FOLLOW_BOTTOM | B_FOLLOW_H_CENTER, B_WILL_DRAW | 
                        B_NAVIGABLE);

    AddChild(MainView);
    MainView->AddChild(menubar);

    MainView->AddChild(LJLogo);
    MainView->AddChild(tcUsername);
    MainView->AddChild(tcPassword);
    MainView->AddChild(LogInButton);
    MainView->AddChild(cbPassword);

    MainView->SetViewColor((rgb_color) {216,216,216,255});

    logged_on = false;
    accept_closing = false;

    char *title = new char[32];
    sprintf( title, "AliveJournal v%s", VERSION_NUMBER );
    SetTitle( title );
}

LJLoginWindow::~LJLoginWindow(void)
{

}

bool
LJLoginWindow::QuitRequested(void)
{

    be_app->PostMessage(B_QUIT_REQUESTED);
    return false;
}

void
LJLoginWindow::MessageReceived(BMessage *message)
{

    switch (message->what) {
        case MENU_QUIT:
            PostMessage(B_QUIT_REQUESTED);
            break;
        case SETTINGS_MSG:
            be_app->PostMessage(message);
            break;
        case LOGON_MSG:
            be_app->PostMessage(LOGON_MSG);
            break;
        case B_ABOUT_REQUESTED:
            be_app->PostMessage(B_ABOUT_REQUESTED);
            break;
    }
}

void
LJLoginWindow::FrameMoved(BPoint origin)
{

}

void
LJLoginWindow::FrameResized(float width, float height)
{

}


