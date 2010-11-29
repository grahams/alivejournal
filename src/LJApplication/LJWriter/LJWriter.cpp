
#include <Application.h>
#include <Font.h>
#include <stdio.h>

#include "message_labels.h"
#include "LJWriter.h"


LJWriter::LJWriter()
            : BWindow(BRect(100, 100, 467, 287), "AliveJournal", 
              B_TITLED_WINDOW, 0, B_CURRENT_WORKSPACE)
{

    MainView = new BView(BRect(0, 0, 367, 187), "View", B_FOLLOW_TOP_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    MainView->SetViewColor((rgb_color) {216,216,216,255});

    // Set up the menu
    menubar = new BMenuBar(BRect(0, 0, 16509, 16), "MenuBar", 
                            B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, 
                            B_ITEMS_IN_ROW, true);
    fileMenu = new BMenu("File", B_ITEMS_IN_COLUMN);
    fileLogOff = new BMenuItem("Log Off", new BMessage(LOGOFF_MSG), 'L', 0);
    fileAcctMgr = new BMenuItem("Account Manager", 
                            new BMessage(ACCOUNT_MSG), 'A', 0);
    fileSep = new BSeparatorItem();
    fileQuit = new BMenuItem("Quit", new BMessage(MENU_QUIT), 'Q', 0);

    menubar->AddItem(fileMenu);
    fileMenu->AddItem(fileLogOff);
    fileMenu->AddItem(fileAcctMgr);
    fileMenu->AddItem(fileSep);
    fileMenu->AddItem(fileQuit);

    tcSubject = new BTextControl(BRect(6, 26, 361, 45), "tc", "Subject", 
                            NULL, new BMessage(DEFAULT_MSG), B_FOLLOW_TOP | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE | 
                            B_NAVIGABLE_JUMP);
    tcSubject->SetDivider(50);

    tvEvent = new BTextView(BRect(8, 55, 335, 132), "View", BRect(0,0,90,90), 
                            B_FOLLOW_ALL_SIDES, B_WILL_DRAW | 
                            B_FULL_UPDATE_ON_RESIZE | B_NAVIGABLE | 
                            B_NAVIGABLE_JUMP);

    eventScroll = new BScrollView("View", tvEvent, B_FOLLOW_TOP_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | 
                            B_FULL_UPDATE_ON_RESIZE, false, true, 
                            B_FANCY_BORDER);

    sendButton = new BButton(BRect(276, 155, 360, 180), "Button", "Send", 
                            new BMessage(SEND_MSG), B_FOLLOW_BOTTOM |                                       B_FOLLOW_RIGHT, B_WILL_DRAW | B_NAVIGABLE | 
                            B_NAVIGABLE_JUMP);
    clearButton = new BButton(BRect(183, 155, 267, 180), "Button", "Clear", 
                            new BMessage(CLEARWRITER_MSG), B_FOLLOW_BOTTOM | 
                            B_FOLLOW_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    postOptionsButton = new BButton(BRect(7, 155, 91, 180), "Button", 
                            "Post Options", new BMessage(POSTOPTIONS_MSG), 
                            B_FOLLOW_BOTTOM | B_FOLLOW_LEFT, B_WILL_DRAW | 
                            B_NAVIGABLE);

    AddChild(MainView);
    MainView->AddChild(menubar);
    MainView->AddChild(tcSubject);
    MainView->AddChild(eventScroll);
    MainView->AddChild(sendButton);
    MainView->AddChild(clearButton);
    MainView->AddChild(postOptionsButton);

    MainView->SetViewColor((rgb_color) {216,216,216,255});
    SetSizeLimits(367, 16384, 187, 16384);
    tvEvent->SetTextRect(tvEvent->Bounds());
    BFont font(be_plain_font);
    font.SetSize(11);
    tvEvent->SetFontAndColor(&font);
    
    accept_closing = false;
    sendButton->SetTarget(be_app);
    fileAcctMgr->SetTarget(be_app);
}

LJWriter::~LJWriter(void)
{

}

bool
LJWriter::QuitRequested(void)
{
    be_app->PostMessage(B_QUIT_REQUESTED);
    return false;
}

void
LJWriter::MessageReceived(BMessage *message)
{
    switch (message->what) {
        case SEND_MSG:
            be_app->PostMessage(SEND_MSG);
            break;
        case ACCOUNT_MSG:
            be_app->PostMessage(ACCOUNT_MSG);
            break;
        case LOGOFF_MSG:
            be_app->PostMessage(LOGOFF_MSG);
            break;
        case MENU_QUIT:
            be_app->PostMessage(B_QUIT_REQUESTED);
            break;
        default:
            be_app->PostMessage(message);
    }
}

void
LJWriter::FrameResized(float width, float height)
{
    tvEvent->SetTextRect(tvEvent->Bounds());
}


