#include "history.hpp"
#include <cstdio>

void History::Push(wxString edit, const wxImage& image) {
    std::string filename = std::tmpnam(nullptr);
    filename += ".png";
    image.SaveFile(filename);
    m_hist.push_back({edit, filename});
}

wxImage History::Restore(int index) {
    wxImage img = wxImage(m_hist[index].filename, wxBITMAP_TYPE_PNG);
    m_hist.erase(m_hist.begin()+index+1, m_hist.end());
    return img;
}

std::vector<HistoryItem>& History::GetVector() {
    return m_hist;
}