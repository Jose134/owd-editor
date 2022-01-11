#include "regionPanel.hpp"

enum {
    ID_XCTRL = wxID_HIGHEST + 1,
    ID_YCTRL = wxID_HIGHEST + 2,
    ID_WCTRL = wxID_HIGHEST + 3,
    ID_HCTRL = wxID_HIGHEST + 4,
    ID_APPLY = wxID_HIGHEST + 5
};

BEGIN_EVENT_TABLE(RegionPanel, wxCollapsiblePane)

EVT_TEXT_ENTER(ID_XCTRL, RegionPanel::OnXCtrl)
EVT_TEXT_ENTER(ID_YCTRL, RegionPanel::OnYCtrl)
EVT_TEXT_ENTER(ID_WCTRL, RegionPanel::OnWCtrl)
EVT_TEXT_ENTER(ID_HCTRL, RegionPanel::OnHCtrl)

EVT_BUTTON(ID_APPLY, RegionPanel::OnApplyPress)

END_EVENT_TABLE()

RegionPanel::RegionPanel(wxWindow *parent,
                wxWindowID winid,
                const wxString& label,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxValidator& val,
                const wxString& name)
    : wxCollapsiblePane(parent, winid, label, pos, size, style, val, name)
{

    wxSize gaps(editor::DEFAULT_SPACER_SIZE, editor::DEFAULT_SPACER_SIZE);
    wxGridSizer* mainSizer = new wxGridSizer(2, 2, gaps);

    m_xCtrl = new wxNumberCtrl(this, ID_XCTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_xCtrl);
    
    m_yCtrl = new wxNumberCtrl(this, ID_YCTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_yCtrl);
    
    m_wCtrl = new wxNumberCtrl(this, ID_WCTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_wCtrl);
    
    m_hCtrl = new wxNumberCtrl(this, ID_HCTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_hCtrl);

    SetSizer(mainSizer);
}

void RegionPanel::OnXCtrl(wxCommandEvent& event) {

}

void RegionPanel::OnYCtrl(wxCommandEvent& event) {

}

void RegionPanel::OnWCtrl(wxCommandEvent& event) {

}

void RegionPanel::OnHCtrl(wxCommandEvent& event) {

}

void RegionPanel::OnApplyPress(wxCommandEvent& event) {

}
