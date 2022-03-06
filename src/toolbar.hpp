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

    DECLARE_EVENT_TABLE()

private:
    MainFrame* m_mainFrame;

    wxBitmapToggleButton* m_toggleCenterBtn;

    wxStaticText* m_zoomLabel;

    wxSimplebook* m_toolsBook;
    std::unordered_map<int, wxWindow*> m_toolsPages;

    //COLOR BACKGROUND OPTIONS
    wxColourPickerCtrl* m_colorPickerCtrl;
    wxButton* m_colorMeanBtn;

    wxField* m_colorOffsetXField;
    int m_colorOffsetX;

    wxField* m_colorOffsetYField;
    int m_colorOffsetY;

    wxField* m_colorMarginField;
    int m_colorMargin;

    //CROP OPTIONS
    wxField* m_cropOffsetField;
    int m_cropOffset;

    //BLUR OPTIONS
    wxField* m_blurRadiusField;
    int m_blurRadius;
};