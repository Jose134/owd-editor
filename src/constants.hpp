#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

namespace editor {
    const wxString WINDOW_TITLE = wxT("onewaifuaday editor");
    const wxSize MIN_BUTTON_SIZE = wxSize(250, 35);
    const wxSize MIN_SQUARE_BUTTON_SIZE = wxSize(35, 35);
    const int DEFAULT_SPACER_SIZE = 5;
    const double MIN_ZOOM_LEVEL = 0.01;
    const double ZOOM_FACTOR = 7.5;
    const wxColour CANVAS_BACKGROUND = wxColour(75, 75, 75);
    const int DEFAULT_BLUR_RADIUS = 40;
}