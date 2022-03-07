#pragma once

#include "includes.hpp"

#include "mainFrame.hpp"
#include "wxNumberCtrl.hpp"
#include "wxField.hpp"

#include <wx/simplebook.h>
#include <wx/tglbtn.h>
#include <wx/clrpicker.h>

#include <unordered_map>
#include <tuple>

class Toolbar : public wxPanel {
public:
    Toolbar(wxFrame* parent, wxFrame* mainFrame);
    void DisplayToolControls(int toolID);
    void PanelImageUpdated();

private:

    void UpdateZoomText();

    //EVENTS
    void OnZoomUpdated(wxCommandEvent& event);

    void OnColorPress(wxCommandEvent &event);
    void OnCropPress(wxCommandEvent &event);
    void OnBlurPress(wxCommandEvent &event);
    void OnExportPress(wxCommandEvent &event);

    void OnZoomOutPress(wxCommandEvent &event);
    void OnZoomFitPress(wxCommandEvent &event);
    void OnZoomInPress(wxCommandEvent &event);

    void OnCenterToggle(wxCommandEvent &event);

    void OnColorPickerChange(wxColourPickerEvent &event);
    void OnColorWhitePress(wxCommandEvent &event);
    void OnColorBlackPress(wxCommandEvent &event);
    void OnColorMeanPress(wxCommandEvent &event);
    void OnColorOffsetChange(wxCommandEvent &event);
    void OnColorMarginChange(wxCommandEvent &event);
    void OnCropOffsetChange(wxCommandEvent &event);
    void OnBlurRadiusChange(wxCommandEvent &event);

    void OnCropXChanged(wxCommandEvent &event);
    void OnCropYChanged(wxCommandEvent &event);
    void OnCropWChanged(wxCommandEvent &event);
    void OnCropHChanged(wxCommandEvent &event);

    void OnStack(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

private:
    MainFrame* m_mainFrame;

    wxBitmapToggleButton* m_toggleCenterBtn;

    wxStaticText* m_zoomLabel;

    wxSimplebook* m_toolsBook;
    std::unordered_map<int, wxWindow*> m_toolsPages;

    //LAST EDIT
    wxString m_lastEditName;

    //COLOR BACKGROUND OPTIONS
    wxColourPickerCtrl* m_colorPickerCtrl;
    wxButton* m_colorMeanBtn;

    wxField* m_colorOffsetXField;
    wxField* m_colorOffsetYField;
    wxField* m_colorMarginField;

    int m_colorOffsetX, m_colorOffsetY, m_colorMargin;

    //CROP OPTIONS
    wxField* m_cropXField;
    wxField* m_cropYField;
    wxField* m_cropWField;
    wxField* m_cropHField;

    int m_cropX, m_cropY, m_cropW, m_cropH;

    //BLUR OPTIONS
    wxField* m_blurRadiusField;
    int m_blurRadius;
};