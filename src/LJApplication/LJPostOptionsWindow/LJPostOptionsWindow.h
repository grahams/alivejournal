
#ifndef _LJPostOptionsWindow_
#define _LJPostOptionsWindow_

#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <queue>

#include "LJPostOptionsWindowMainView/LJPostOptionsWindowMainView.h"

class LJPostOptionsWindow : public BWindow
{
    public: // Constructors & Destructor
        LJPostOptionsWindow ();
        ~LJPostOptionsWindow (void);

    public: // Member Functions

    virtual bool    QuitRequested(void);
    virtual void    MessageReceived(BMessage *message);
    void            clearCurrents();       // Clears the elements of the window

    public:     // Data Members
        LJPostOptionsWindowMainView *MainView;

    private:    // Data Members
        BMessage     *current_msg;      // The message to be sent when the
                                        // saves these currents
};

#endif // _LJPostOptionsWindow_
