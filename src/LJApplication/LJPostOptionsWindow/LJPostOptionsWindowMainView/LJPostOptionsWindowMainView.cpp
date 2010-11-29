
#include <Application.h>

#include "message_labels.h"
#include "LJPostOptionsWindowMainView.h"
#include "LJApplication.h"

LJPostOptionsWindowMainView::LJPostOptionsWindowMainView()
                 : BView(BRect(0, 0, 300, 143), "View", B_FOLLOW_TOP_BOTTOM | 
                                B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE)
{
    _bxl_attached = false;

    moodMenu = new BPopUpMenu("Mood", true, false, B_ITEMS_IN_COLUMN);
    moodMenu->SetRadioMode(true);

    moodField = new BMenuField(BRect(10, 10, 95, 29), "menufield", NULL, 
                            moodMenu, new BMessage(DEFAULT_MSG), 
                            B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | 
                            B_NAVIGABLE);
    moodField->SetDivider(0);

    otherMood = new BTextControl(BRect(112, 10, 286, 30), "otherMood", 
                            "Other:", NULL, new BMessage(DEFAULT_MSG), 
                            B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | 
                            B_NAVIGABLE);
    otherMood->SetDivider(50);

    ((LJApplication *)be_app)->help->SetHelp(otherMood, 
        "Enter your mood here if it is not listed in the menu to the left" );

    music = new BTextControl(BRect(10, 40, 264, 60), "tc", "Music:", NULL, 
                            new BMessage(DEFAULT_MSG), B_FOLLOW_LEFT | 
                            B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    music->SetDivider(50);

    saveButton = new BButton(BRect(221, 109, 285, 133), "Save", "Save", 
                            new BMessage(CURRENT_MSG), B_FOLLOW_BOTTOM |    
                            B_FOLLOW_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    saveButton->MakeDefault(true);

    clearButton = new BButton(BRect(142, 109, 206, 133), "Clear", "Clear", 
                            new BMessage(CLEAR_MSG), B_FOLLOW_BOTTOM | 
                            B_FOLLOW_RIGHT, B_WILL_DRAW | B_NAVIGABLE);

    musicButton = new BButton(BRect(271, 41, 291, 60), "Button", "...", 
                            new BMessage(MUSICBUTTON_MSG), B_FOLLOW_LEFT | 
                            B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

    ((LJApplication *)be_app)->help->SetHelp(musicButton, 
        "Get the Current Trackname from Soundplay");

    autoFormat = new BCheckBox(BRect(10, 112, 107, 130), "checkbox", 
                            "Auto-Formatting", new BMessage(DEFAULT_MSG), 
                            B_FOLLOW_BOTTOM | B_FOLLOW_LEFT, B_WILL_DRAW | 
                            B_NAVIGABLE);
    autoFormat->SetValue(B_CONTROL_ON);

    ((LJApplication *)be_app)->help->SetHelp(autoFormat, 
                        "Do you want AliveJournal to Auto Format your Posts?");

    journalMenu = new BPopUpMenu("Journals", true, false, B_ITEMS_IN_COLUMN);
    journalMenu->SetRadioMode(true);

    journalField = new BMenuField(BRect(10, 70, 90, 89), "journalField", 
                            "Access", journalMenu, new BMessage(DEFAULT_MSG), 
                            B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | 
                            B_NAVIGABLE);
    journalField->SetDivider(0);
    ((LJApplication *)be_app)->help->SetHelp(journalField,
                "Journal that you wish to post to" );

    picMenu = new BPopUpMenu("Pictures", true, false, B_ITEMS_IN_COLUMN);
    picMenu->SetRadioMode(true);

    picField = new BMenuField(BRect(110, 70, 190, 89), "picField", "Picture", 
                            picMenu, new BMessage(DEFAULT_MSG), B_FOLLOW_LEFT |
                            B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    picField->SetDivider(0);
    ((LJApplication *)be_app)->help->SetHelp(picField,
                "Picture that you wish to use for this post" );

    securityMenu = new BPopUpMenu("Security", true, false, B_ITEMS_IN_COLUMN);
    securityMenu->SetRadioMode(true);

    securityField = new BMenuField(BRect(210, 70, 290, 89), "securityField", 
                            "Security", securityMenu, 
                            new BMessage(DEFAULT_MSG), B_FOLLOW_LEFT | 
                            B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    securityField->SetDivider(0);
    ((LJApplication *)be_app)->help->SetHelp(securityField, 
                "Who should be allowed to see this post?" );

    SetViewColor((rgb_color) {219,219,219,255});
}

LJPostOptionsWindowMainView::~LJPostOptionsWindowMainView(void)
{

}

void
LJPostOptionsWindowMainView::AttachedToWindow(void)
{

    if ( !_bxl_attached ) {
        AddChild(moodField);
        AddChild(otherMood);
        //otherMood->AddChild(othertip);
        AddChild(music);
        AddChild(saveButton);
        AddChild(clearButton);
        AddChild(musicButton);
        //musicButton->AddChild(bxltips2);
        AddChild(autoFormat);
        //autoFormat->AddChild(formattip);
        AddChild(journalField);
        AddChild(picField);
        AddChild(securityField);

        _bxl_attached = true;
    }
}

void
LJPostOptionsWindowMainView::AllAttached(void) 
{

}

void
LJPostOptionsWindowMainView::MessageReceived(BMessage *message)
{
    switch (message->what) {
        case DEFAULT_MSG: 
            break;

        default: 
            BView::MessageReceived(message);
            break;
    }
}


