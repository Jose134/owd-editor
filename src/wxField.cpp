#include "wxField.hpp"

IMPLEMENT_CLASS( wxField, wxPanel )

wxDEFINE_EVENT(EVT_VALUE_CHANGED, wxCommandEvent);

enum {
    ID_SLIDER = wxID_HIGHEST + 1,
    ID_CTRL   = wxID_HIGHEST + 2
};

BEGIN_EVENT_TABLE( wxField, wxPanel )

EVT_SLIDER(ID_SLIDER, wxField::OnSliderMove)
EVT_TEXT_ENTER(ID_CTRL, wxField::OnCtrlChanged)

END_EVENT_TABLE()

wxField::wxField(wxWindow *parent, wxWindowID id, const int value, const wxString& label,
            const wxPoint &pos, const wxSize &size, long style, const wxString & name)
    :wxPanel(parent, id, pos, size, style, name)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    m_valInt = value;
    m_valDouble = value;

    m_slider = new wxSlider(this, ID_SLIDER, editor::DEFAULT_BLUR_RADIUS, 0, 1000);
    m_slider->SetValue(editor::DEFAULT_BLUR_RADIUS);
    sizer->Add(m_slider);
    sizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_ctrl = new wxNumberCtrl(this, ID_CTRL, editor::DEFAULT_BLUR_RADIUS, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_ctrl->AcceptIntOnly(true);
    m_ctrl->SetInt(value);
    sizer->Add(m_ctrl, 0, wxALIGN_CENTER_VERTICAL);
    sizer->AddStretchSpacer();

    m_label = new wxStaticText(this, wxID_ANY, label);
    sizer->Add(m_label, 0, wxALIGN_CENTER_VERTICAL);
    sizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    SetSizer(sizer);
}


int wxField::GetInt() {
    return m_valInt;
}

double wxField::GetDouble() {
    return m_valDouble;
}

wxString wxField::GetLabel() {
    return m_label->GetLabel();
}

void wxField::SetValue(int i) {
    m_valInt = i;
    m_valDouble = i;
    
    m_slider->SetValue(m_valInt);
    m_ctrl->SetInt(m_valInt);
}

void wxField::SetValue(double d) {
    m_valDouble = d;
    m_valInt = d;

    m_slider->SetValue(m_valDouble);
    m_ctrl->SetDouble(m_valDouble);
}

void wxField::SetLabel(const wxString& label) {
    m_label->SetLabel(label);
}

void wxField::SetRange(int minimum, int maximum) {
    if (m_valInt < minimum || m_valInt > maximum) {
        SetValue(minimum);
    } 

    m_slider->SetMin(minimum);
    m_slider->SetMax(maximum);

    m_ctrl->SetRange(minimum, maximum);
}

void wxField::AcceptIntOnly(bool b) {
    if (m_intOnly == b) return;

}

void wxField::OnSliderMove(wxCommandEvent &event) {
    SetValue(m_slider->GetValue());
    SendValueChangedEvent();
}

void wxField::OnCtrlChanged(wxCommandEvent &event) {
    if (m_intOnly) {
        SetValue(m_ctrl->GetInt());
    }
    else {
        SetValue(m_ctrl->GetDouble());        
    }
    SendValueChangedEvent();
}

void wxField::SendValueChangedEvent() {
    wxCommandEvent _event(EVT_VALUE_CHANGED);
    _event.SetInt(GetInt());
    // wxPostEvent(this, _event);
    ProcessWindowEvent(_event);
}