#pragma once

#include "includes.h"

#include "mainFrame.hpp"
#include "wxNumberCtrl.hpp"

#include <wx/simplebook.h>
#include <wx/tglbtn.h>
#include <wx/clrpicker.h>

#include <unordered_map>
#include <tuple>

class Toolbar : public wxPanel {
public:
    Toolbar(wxFrame *parent);
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
    void OnColorOffsetSlide(wxCommandEvent &event);
    void OnColorOffsetText(wxCommandEvent &event);
    void OnColorMarginSlide(wxCommandEvent &event);
    void OnColorMarginText(wxCommandEvent &event);

    void OnCropOffsetSlide(wxCommandEvent &event);
    void OnCropOffsetText(wxCommandEvent &event);

    void OnBlurRadiusSlide(wxCommandEvent &event);
    void OnBlurRadiusText(wxCommandEvent &event);

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

    wxSlider* m_colorOffsetXSlider;
    wxNumberCtrl* m_colorOffsetXCtrl;
    int m_colorOffsetX;

    wxSlider* m_colorOffsetYSlider;
    wxNumberCtrl* m_colorOffsetYCtrl;
    int m_colorOffsetY;

    wxSlider* m_colorMarginSlider;
    wxNumberCtrl* m_colorMarginCtrl;
    int m_colorMargin;

    //CROP OPTIONS
    wxSlider* m_cropOffsetSlider;
    wxNumberCtrl* m_cropOffsetCtrl;
    int m_cropOffset;

    //BLUR OPTIONS
    wxSlider* m_blurRadiusSlider;
    wxNumberCtrl* m_blurRadiusCtrl;
    int m_blurRadius;
};