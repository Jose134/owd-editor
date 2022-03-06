#pragma once

#include "includes.hpp"
#include "wxNumberCtrl.hpp"

#include "constants.hpp"

wxDECLARE_EVENT(EVT_VALUE_CHANGED, wxCommandEvent);

class wxField : public wxPanel {
    DECLARE_CLASS( wxField )
    public:
        wxField() {}
        
        wxField(wxWindow *parent, wxWindowID id = wxID_ANY, const int value = 0, const wxString& label = wxEmptyString,
            const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL, const wxString & name = wxPanelNameStr);

        int GetInt();
        double GetDouble();
        wxString GetLabel();
        bool GetIntOnly();

        void SetValue(int i);
        void SetValue(double d);
        void SetLabel(const wxString& label);
        void SetRange(int minimum, int maximum);
        void AcceptIntOnly(bool b);


    private:
        DECLARE_EVENT_TABLE()

        void OnSliderMove(wxCommandEvent &event);
        void OnCtrlChanged(wxCommandEvent &event);
        void SendValueChangedEvent();

    private:
        wxSlider* m_slider;
        wxNumberCtrl* m_ctrl;
        wxStaticText* m_label;

        int m_valInt;
        double m_valDouble;

        int m_minVal, m_maxVal;

        bool m_intOnly;
};