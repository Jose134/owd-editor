#pragma once

#include "includes.hpp"
#include <wx/mstream.h>

namespace icon {
    enum ID {
        ZOOM_OUT,
        ZOOM_FIT,
        ZOOM_IN,    
        CENTER
    };

    wxBitmap loadIcon(ID id);

}