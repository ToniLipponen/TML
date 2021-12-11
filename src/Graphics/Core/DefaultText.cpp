#include "Headers/DefaultText.h"
#include <TML/Graphics/Drawable/Text.h>

#ifndef _WIN32
    #include <incbin/incbin.h>
    INCBIN(DEFAULT_FONT, "../res/SF-UI-Text-Regular.otf");
#else
    #include <Font.h>
#endif


extern tml::Text* DEFAULT_TEXT;
extern tml::Font* DEFAULT_FONT;

void LoadDefaultText()
{
    DEFAULT_FONT = new tml::Font;
#ifndef _WIN32
    DEFAULT_FONT->LoadFromMemory(gDEFAULT_FONTData, gDEFAULT_FONTSize);
#else
    DEFAULT_FONT->LoadFromMemory(DEFAULT_FONT.data(), DEFAULT_FONT.size());
#endif
    DEFAULT_TEXT = new tml::Text;
}