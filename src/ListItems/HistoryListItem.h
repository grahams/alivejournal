#include <ListItem.h>

#ifndef _HistoryListItem_

class HistoryListItem : public BListItem
{
public:
                    HistoryListItem (const char *sLabel, const char *sItem_id);
    virtual void    DrawItem(BView *owner, BRect frame, bool complete = false);
    char            *label;
    char            *item_id;
    
    rgb_color       high_color;
    rgb_color       low_color;
    rgb_color       high_font;
    rgb_color       low_font;
};

#define _HistoryListItem_
#endif
