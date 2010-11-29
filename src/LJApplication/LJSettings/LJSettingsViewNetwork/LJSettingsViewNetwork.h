
#ifndef _LJSettingsViewNetwork_
#define _LJSettingsViewNetwork_

#include <View.h>
#include <Bitmap.h>
#include <Box.h>
#include <TextControl.h>
#include <TextView.h>
#include <Application.h>
#include <Button.h>
#include <CheckBox.h>
#include <StringView.h>

class LJSettingsViewNetwork : public BView
{
    public: // Constructors & Destructor
        LJSettingsViewNetwork ();
        ~LJSettingsViewNetwork (void);

    public: // Member Functions
        virtual void AttachedToWindow(void);

    public: // Data Members
        BBox            *prBox;
        BTextControl    *prAddress;
        BTextControl    *prPort;
        BButton         *prImportButton;
        BCheckBox       *use_proxy;

        BBox            *serverBox;
        BTextControl    *serverAddress;
        BTextControl    *serverPort;

        BBox            *timeoutBox;
        BTextControl    *tcTimeout;
        BStringView     *timeoutDesc;

        bool            _bxl_attached;
};

#endif // _LJSettingsViewNetwork_
