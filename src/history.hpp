#pragma once

#include "includes.hpp"

#include <vector>

struct HistoryItem {
    wxString editname;
    wxString filename;
};

class History {

    public:
        History() {}
        
        void Push(wxString edit, const wxImage& img);
        wxImage GetPreview(int index);
        wxImage Restore(int index);

        std::vector<HistoryItem>& GetVector();

    private:
        std::vector<HistoryItem> m_hist;
};