#include <ListItem.h>
class BBitmap;

#ifndef _FriendListItem_

class FriendListItem : public BListItem
{
public:
                    FriendListItem (const char *label, bool fstatus);
    virtual void    DrawItem(BView *owner, BRect frame, bool complete = false);
    bool            status;
    char *          name;
    
    rgb_color       high_color;
    rgb_color       low_color;
    rgb_color       high_font;
    rgb_color       low_font;
    BBitmap         *true_icon;
    BBitmap         *false_icon;
};

#define _FriendListItem_
#endif
