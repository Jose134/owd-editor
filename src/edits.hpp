#pragma once

#include "includes.h"


namespace editor {
    wxImage makeSquared(const wxBitmap& bitmap, const wxRect region, const wxColour* bgColor = wxWHITE, int offset = 0, int margin = 0);
    wxImage makeSquaredCrop(const wxImage& img, int offset = 0);
    wxImage makeSquaredBlur(const wxImage& img, const int radius = 80);
    wxImage makeSquaredBlur(const wxImage& img, const wxBitmap& bitmap, const int radius = 80);
    void boxesForGauss(int v[], int sigma, int n);

    enum Direction {
        LR_TB, //Left to Right, Top to Bottom
        RL_TB, //Right to Left, Top to Bottom 
        TB_LR, //Top to Bottom, Left to Right
        TB_RL  //Top to Bottom, Right to Left
    };
    wxImage compositeImage(const std::vector<wxBitmap>& bitmaps, const wxColour bgColor, int cols=2, Direction dir=Direction::LR_TB);
}