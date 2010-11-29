#include <ListItem.h>
#include <View.h>
#include <Bitmap.h>
#include <Font.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FriendListItem.h"
#include "FriendListIcon.h"

FriendListItem::FriendListItem(const char *label, bool fstatus)
: BListItem()
{
    rgb_color hcolor = {180, 210, 255, 0};
    rgb_color lcolor  = {255, 255, 255, 0};
    
    rgb_color hfont  = {0, 0, 0, 0};
    rgb_color lfont   = {0, 0, 0, 0};
    
    high_color = hcolor;
    low_color = lcolor;
    
    high_font = hfont;
    low_font = lfont;
    
    name = new char[strlen(label)+1];
    name = strcpy( name, label );
    status = fstatus;
    
    true_icon = new BBitmap(BRect(0, 0, 15, 15), B_COLOR_8_BIT);
    true_icon->SetBits(trueIconBits, true_icon->BitsLength(), 0, B_COLOR_8_BIT);

    false_icon = new BBitmap(BRect(0, 0, 15, 15), B_COLOR_8_BIT);
    false_icon->SetBits(falseIconBits, false_icon->BitsLength(), 0, B_COLOR_8_BIT);
    
}


void
FriendListItem::DrawItem(BView *owner, BRect frame, bool complete = false)
{
    BBitmap *icon;
    if (status) icon = true_icon;
    else icon = false_icon;

    if (IsSelected() || complete) {
        if (IsSelected()) {
            owner->SetHighColor(high_color);
        }else{
            owner->SetHighColor(low_color);
            
        }
        owner->FillRect(frame);
    }
    
    if (IsSelected()){
        owner->SetHighColor(0,0,0);
        owner->SetLowColor(255,255,255);
        owner->StrokeRect(frame, B_MIXED_COLORS);
        owner->SetHighColor(high_color);
    }
    

    BFont font(be_plain_font);
    font.SetSize(11);
    owner->SetFont(&font);
    
    if (status)
        owner->DrawBitmap(icon, BRect(frame.right-frame.Height(), frame.top, frame.right,frame.bottom));
    
    if (IsSelected()) {
        owner->SetLowColor(high_color);
        owner->SetHighColor(high_font); 
    }else{
        owner->SetHighColor(low_font);
        owner->SetLowColor(low_color);
    }   

    owner->MovePenTo(frame.left+2, frame.bottom-3);
    owner->DrawString(name);
    
}
