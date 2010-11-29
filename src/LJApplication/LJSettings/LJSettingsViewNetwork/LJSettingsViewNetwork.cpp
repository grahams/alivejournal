
#include <Application.h>

#include "message_labels.h"
#include "LJSettingsViewNetwork.h"


LJSettingsViewNetwork::LJSettingsViewNetwork()
                     : BView(BRect(5, 5, 347, 327), "Network", 
                       B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE)
{
    _bxl_attached = false;

    prBox = new BBox(BRect(6, 4, 336, 110), "box", B_FOLLOW_TOP_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_FRAME_EVENTS |
                            B_NAVIGABLE_JUMP, B_FANCY_BORDER);
    prBox->SetLabel("Proxy");

    prAddress = new BTextControl(BRect(11, 50, 319, 69), "tc", "Address", 
                            NULL, new BMessage(PROXY_ADDR_MSG), 
                            B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT, 
                            B_WILL_DRAW | B_NAVIGABLE);
    prAddress->SetDivider(50);

    prPort = new BTextControl(BRect(11, 78, 319, 97), "tc", "Port #", NULL, 
                            new BMessage(PROXY_PORT_MSG), B_FOLLOW_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    prPort->SetDivider(50);

    prImportButton = new BButton(BRect(169, 16, 319, 40), "Button", 
                            "Import NetPositive Settings", 
                            new BMessage(GET_NETP_SETTINGS_MSG), 
                                    B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT, 
                                    B_WILL_DRAW | B_NAVIGABLE);
    prImportButton->SetEnabled(false);

    use_proxy = new BCheckBox(BRect(10, 20, 155, 38), "checkbox", 
                            "Use Proxy Server", new BMessage(USE_PROXY_MSG), 
                            B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | 
                            B_NAVIGABLE);


    serverBox = new BBox(BRect(6, 120, 336, 200), "box", B_FOLLOW_TOP_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_FRAME_EVENTS |
                            B_NAVIGABLE_JUMP, B_FANCY_BORDER);
    serverBox->SetLabel("LiveJournal Server");

    serverAddress = new BTextControl(BRect(11, 20, 319, 39), "tc", "Address", 
                            NULL, new BMessage(SERVER_ADDR_MSG), 
                            B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT, 
                            B_WILL_DRAW | B_NAVIGABLE);
    serverAddress->SetDivider(50);

    serverPort = new BTextControl(BRect(11, 48, 319, 67), "tc", "Port #", NULL, 
                            new BMessage(SERVER_PORT_MSG), B_FOLLOW_BOTTOM | 
                            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_NAVIGABLE);
    serverPort->SetDivider(50);


    timeoutBox = new BBox(BRect(6, 210, 336, 257), "box", B_FOLLOW_LEFT | 
                            B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS | 
                            B_NAVIGABLE_JUMP, B_FANCY_BORDER);
    timeoutBox->SetLabel("Network Time Out");

    tcTimeout = new BTextControl(BRect(18, 20, 93, 39), "tc", "Wait ", 
                            NULL, new BMessage(TC_TIMEOUT_MSG), B_FOLLOW_LEFT |
                            B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    tcTimeout->SetDivider(30);

    timeoutDesc = new BStringView(BRect(94, 25, 310, 37), "StringView", 
                            " seconds before cancelling a connection.", 
                            B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | 
                            B_NAVIGABLE);


    SetViewColor((rgb_color) {216,216,216,255});
}

LJSettingsViewNetwork::~LJSettingsViewNetwork(void)
{

}

void
LJSettingsViewNetwork::AttachedToWindow(void)
{
    if ( !_bxl_attached ) {
        AddChild(prBox);
        prBox->AddChild(prAddress);
        prBox->AddChild(prPort);
        prBox->AddChild(prImportButton);
        prBox->AddChild(use_proxy);

        AddChild(timeoutBox);
        timeoutBox->AddChild(tcTimeout);
        timeoutBox->AddChild(timeoutDesc);

        AddChild(serverBox);
        serverBox->AddChild(serverAddress);
        serverBox->AddChild(serverPort);

        _bxl_attached = true;
    }
}
