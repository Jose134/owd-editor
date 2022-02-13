#pragma once

#include "includes.hpp"
#include "wxNumberCtrl.hpp"

class RegionPanel : public wxPanel {

public:
    RegionPanel(wxWindow *parent,
                wxWindowID winid,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCP_DEFAULT_STYLE,
                const wxString& name = wxASCII_STR(wxPanelNameStr));

    wxRect GetRegion() const { return m_region; }

    wxSize GetMaxRegionSizer() const { return m_maxRegionSize; }
    void SetMaxRegionSize(wxSize size) { m_maxRegionSize = size; }

private:

    void OnXCtrl(wxCommandEvent& event);
    void OnYCtrl(wxCommandEvent& event);
    void OnWCtrl(wxCommandEvent& event);
    void OnHCtrl(wxCommandEvent& event);
    void OnApplyPress(wxCommandEvent& event);
    
    DECLARE_EVENT_TABLE();

private:
    wxRect m_region;
    wxSize m_maxRegionSize;

    wxNumberCtrl* m_xCtrl;
    wxNumberCtrl* m_yCtrl;
    wxNumberCtrl* m_wCtrl;
    wxNumberCtrl* m_hCtrl;

};