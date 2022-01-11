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
    MainFrame *mainFrame;
    wxBoxSizer *box;

    wxBitmapToggleButton *toggleCenterBtn;

    wxStaticText *zoomLabel;

    wxSimplebook *toolsBook;
    std::unordered_map<int, wxWindow*> toolsPages;

    //COLOR BACKGROUND OPTIONS
    wxColourPickerCtrl *colorPickerCtrl;
    wxButton *colorMeanBtn;

    wxSlider *colorOffsetSlider;
    wxNumberCtrl *colorOffsetCtrl;
    int colorOffset;

    wxSlider *colorMarginSlider;
    wxNumberCtrl *colorMarginCtrl;
    int colorMargin;

    //CROP OPTIONS
    wxSlider *cropOffsetSlider;
    wxNumberCtrl *cropOffsetCtrl;
    int cropOffset;

    //BLUR OPTIONS
    wxSlider *blurRadiusSlider;
    wxNumberCtrl *blurRadiusCtrl;
    int m_blurRadius;
};