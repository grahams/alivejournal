#include <ListItem.h>
#include <View.h>
#include <Bitmap.h>
#include <Font.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "HistoryListItem.h"


HistoryListItem::HistoryListItem(const char *sLabel, const char *sItem_id)
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
	
    label = new char[strlen(sLabel)+1];
	label = strcpy( label, sLabel );
    item_id = new char[strlen(sItem_id)+1];
	item_id = strcpy( item_id, sItem_id );
}


void
HistoryListItem::DrawItem(BView *owner, BRect frame, bool complete)
{
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
	
	if (IsSelected()) {
		owner->SetLowColor(high_color);
		owner->SetHighColor(high_font);	
	}else{
		owner->SetHighColor(low_font);
		owner->SetLowColor(low_color);
	}	

	owner->MovePenTo(frame.left+2, frame.bottom-3);
	owner->DrawString(label);
	
}
