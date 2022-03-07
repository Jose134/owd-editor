#pragma once

#include "includes.hpp"
#include "imagePanel.hpp"
#include "history.hpp"

#include <wx/listctrl.h>

class Toolbar;

class MainFrame : public wxFrame
{
public:
    MainFrame();

    void UpdateStatus();
    const wxBitmap& GetOriginalBitmap() const { return m_originalBitmap; }
    const wxImage& GetOriginalImage() const { return m_originalImg; }
    void SetOriginalImage(wxImage img);
    void SetOriginalImage(const wxString& filename, const wxString& dirname="");
    ImagePanel* GetImagePanel() { return m_imgPanel; }
    void ExportCurrent();

    void PushToHistory(const wxString& editName);
    
private:

    void OnPanelZoom(wxCommandEvent& event);
    void OnNewComposition(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnDropFiles(wxDropFilesEvent& event);

    DECLARE_EVENT_TABLE()

private:
    History m_hist;
    wxListView* m_historyListView;

    wxBitmap m_originalBitmap;
    wxImage m_originalImg;

    wxString m_imgFilename;
    wxString m_imgDir;

    ImagePanel *m_imgPanel;
    Toolbar *m_toolbar;
};