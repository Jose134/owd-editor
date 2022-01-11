#include "toolbar.hpp"

#include <wx/simplebook.h>
#include "imagePanel.hpp"
#include "regionPanel.hpp"

#include "constants.hpp"
#include "edits.hpp"


enum {
    ID_REGION_PANEL           = wxID_HIGHEST + 1,
    ID_REGION_X_CTRL          = wxID_HIGHEST + 2,
    ID_REGION_Y_CTRL          = wxID_HIGHEST + 3,
    ID_REGION_W_CTRL          = wxID_HIGHEST + 4,
    ID_REGION_H_CTRL          = wxID_HIGHEST + 5,
    ID_REGION_SELECT          = wxID_HIGHEST + 6,

    ID_COLOR                  = wxID_HIGHEST + 7,
    ID_CROP                   = wxID_HIGHEST + 8,
    ID_BLUR                   = wxID_HIGHEST + 9,
    ID_EXPORT                 = wxID_HIGHEST + 10,

    ID_COLOR_COLOR_PICKER     = wxID_HIGHEST + 11,
    ID_COLOR_WHITE_BTN        = wxID_HIGHEST + 12,
    ID_COLOR_BLACK_BTN        = wxID_HIGHEST + 13,
    ID_COLOR_MEAN_BTN         = wxID_HIGHEST + 14,
    ID_COLOR_OFFSET_SLIDER    = wxID_HIGHEST + 15,
    ID_COLOR_OFFSET_CTRL      = wxID_HIGHEST + 16,
    ID_COLOR_MARGIN_SLIDER    = wxID_HIGHEST + 17,
    ID_COLOR_MARGIN_CTRL      = wxID_HIGHEST + 18,
    ID_CROP_OFFSET_SLIDER     = wxID_HIGHEST + 19,
    ID_CROP_OFFSET_CTRL       = wxID_HIGHEST + 20,
    ID_BLUR_RADIUS_SLIDER     = wxID_HIGHEST + 21,
    ID_BLUR_RADIUS_CTRL       = wxID_HIGHEST + 22,

    ID_TOGGLE_CENTER          = wxID_HIGHEST + 23
};

BEGIN_EVENT_TABLE (Toolbar, wxPanel)

EVT_BUTTON(ID_COLOR, Toolbar::OnColorPress)
EVT_BUTTON(ID_CROP, Toolbar::OnCropPress)
EVT_BUTTON(ID_BLUR, Toolbar::OnBlurPress)
EVT_BUTTON(ID_EXPORT, Toolbar::OnExportPress)

EVT_BUTTON(wxID_ZOOM_OUT, Toolbar::OnZoomOutPress)
EVT_BUTTON(wxID_ZOOM_FIT, Toolbar::OnZoomFitPress)
EVT_BUTTON(wxID_ZOOM_IN, Toolbar::OnZoomInPress)

EVT_TOGGLEBUTTON(ID_TOGGLE_CENTER, Toolbar::OnCenterToggle)

EVT_COLOURPICKER_CHANGED(ID_COLOR_COLOR_PICKER, Toolbar::OnColorPickerChange)
EVT_BUTTON(ID_COLOR_WHITE_BTN, Toolbar::OnColorWhitePress)
EVT_BUTTON(ID_COLOR_BLACK_BTN, Toolbar::OnColorBlackPress)
EVT_BUTTON(ID_COLOR_MEAN_BTN, Toolbar::OnColorMeanPress)
EVT_SLIDER(ID_COLOR_OFFSET_SLIDER, Toolbar::OnColorOffsetSlide)
EVT_TEXT_ENTER(ID_COLOR_OFFSET_CTRL, Toolbar::OnColorOffsetText)
EVT_SLIDER(ID_COLOR_MARGIN_SLIDER, Toolbar::OnColorMarginSlide)
EVT_TEXT_ENTER(ID_COLOR_MARGIN_CTRL, Toolbar::OnColorMarginText)

EVT_SLIDER(ID_CROP_OFFSET_SLIDER, Toolbar::OnCropOffsetSlide)
EVT_TEXT_ENTER(ID_CROP_OFFSET_CTRL, Toolbar::OnCropOffsetText)

EVT_SLIDER(ID_BLUR_RADIUS_SLIDER, Toolbar::OnBlurRadiusSlide)
EVT_TEXT_ENTER(ID_BLUR_RADIUS_CTRL, Toolbar::OnBlurRadiusText)


END_EVENT_TABLE()

Toolbar::Toolbar (wxFrame *parent)
    : wxPanel(parent) {

    this->mainFrame = (MainFrame*)parent;

    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    //REGION

    /*
    RegionPanel* regionPanel = new RegionPanel(this, ID_REGION_PANEL, "Input region");
    box->Add(regionPanel, 0, wxEXPAND | wxALL, editor::DEFAULT_SPACER_SIZE);
    */

    /*
    wxCollapsiblePane* regionPanel = new wxCollapsiblePane(this, ID_REGION_PANEL, "Input region");
    wxSize gaps(editor::DEFAULT_SPACER_SIZE, editor::DEFAULT_SPACER_SIZE);
    wxGridSizer* mainSizer = new wxGridSizer(2, 2, gaps);

    wxNumberCtrl* m_xCtrl = new wxNumberCtrl(regionPanel, ID_REGION_X_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_xCtrl);
    
    wxNumberCtrl* m_yCtrl = new wxNumberCtrl(regionPanel, ID_REGION_Y_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_yCtrl);
    
    wxNumberCtrl* m_wCtrl = new wxNumberCtrl(regionPanel, ID_REGION_W_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_wCtrl);
    
    wxNumberCtrl* m_hCtrl = new wxNumberCtrl(regionPanel, ID_REGION_H_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer->Add(m_hCtrl);

    regionPanel->SetSizer(mainSizer);
    box->Add(regionPanel, 0, wxEXPAND | wxALL, 5);
    */

    //TOOLS    
    wxButton *colorBtn = new wxButton(this, ID_COLOR, "COLOR BACKGROUND");
    colorBtn->SetMinSize(editor::MIN_BUTTON_SIZE);
    box->Add(colorBtn);
    
    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton *cropBtn = new wxButton(this, ID_CROP, "CROP SQUARED");
    cropBtn->SetMinSize(editor::MIN_BUTTON_SIZE);
    box->Add(cropBtn);

    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton *blurBtn = new wxButton(this, ID_BLUR, "BLUR BACKGROUND");
    blurBtn->SetMinSize(editor::MIN_BUTTON_SIZE);
    box->Add(blurBtn);

    box->AddStretchSpacer();

    //ZOOM CONTROLS
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    
    wxBitmapButton *zoomoutBtn = new wxBitmapButton(this, wxID_ZOOM_OUT, wxBitmap("res/magnify-minus-outline-16.png"));
    zoomoutBtn->GetBitmap().CreateScaled(32, 32, 1, 1);
    zoomoutBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    hbox->Add(zoomoutBtn);

    hbox->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    wxBitmapButton *zoomfitBtn = new wxBitmapButton(this, wxID_ZOOM_FIT, wxBitmap("res/magnify-reset-outline-16.png"));
    zoomfitBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    hbox->Add(zoomfitBtn);

    hbox->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    wxBitmapButton *zoominBtn = new wxBitmapButton(this, wxID_ZOOM_IN, wxBitmap("res/magnify-plus-outline-16.png"));
    zoominBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    hbox->Add(zoominBtn);

    hbox->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    toggleCenterBtn = new wxBitmapToggleButton(this, ID_TOGGLE_CENTER, wxBitmap("res/cross-16.png"));
    toggleCenterBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    hbox->Add(toggleCenterBtn);

    hbox->AddStretchSpacer();

    zoomLabel = new wxStaticText(this, wxID_ANY, "");
    UpdateZoomText();
    hbox->Add(zoomLabel, 0, wxALIGN_CENTER_VERTICAL);

    hbox->AddStretchSpacer();

    box->Add(hbox, 0, wxEXPAND | wxALL);

    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    //TOOLS' CONTROLS
    toolsBook = new wxSimplebook(this);

    wxPanel *emptyPage = new wxPanel(toolsBook);
    toolsPages[wxID_NONE] = emptyPage;
    toolsBook->AddPage(emptyPage, "Emtpy");
    
    wxPanel *colorPage = new wxPanel(toolsBook);
    wxBoxSizer *colorPageBox = new wxBoxSizer(wxVERTICAL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer *colorColorControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    colorPickerCtrl = new wxColourPickerCtrl(colorPage, ID_COLOR_COLOR_PICKER);
    colorColorControlSizer->Add(colorPickerCtrl);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton *colorWhiteBtn = new wxButton(colorPage, ID_COLOR_WHITE_BTN, "");
    colorWhiteBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    colorWhiteBtn->SetBackgroundColour(wxColour(255, 255, 255));
    colorColorControlSizer->Add(colorWhiteBtn);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton *colorBlackBtn = new wxButton(colorPage, ID_COLOR_BLACK_BTN, "");
    colorBlackBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    colorBlackBtn->SetBackgroundColour(wxColour(0, 0, 0));
    colorColorControlSizer->Add(colorBlackBtn);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorMeanBtn = new wxButton(colorPage, ID_COLOR_MEAN_BTN, "");
    colorMeanBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    colorMeanBtn->SetBackgroundColour(mainFrame->GetImagePanel()->GetMeanColor());
    colorColorControlSizer->Add(colorMeanBtn);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorColorControlSizer->AddStretchSpacer();

    wxStaticText *colorColorLabel = new wxStaticText(colorPage, wxID_ANY, "Color");
    colorColorControlSizer->Add(colorColorLabel, 0, wxALIGN_CENTER_VERTICAL);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPageBox->Add(colorColorControlSizer, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer *colorOffsetControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    colorOffsetSlider = new wxSlider(colorPage, ID_COLOR_OFFSET_SLIDER, 0, 0, 1000);
    colorOffsetControlSizer->Add(colorOffsetSlider);
    colorOffsetControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorOffsetCtrl = new wxNumberCtrl(colorPage, ID_COLOR_OFFSET_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    colorOffsetCtrl->AcceptIntOnly(true);
    colorOffsetCtrl->SetInt(0);
    colorOffsetControlSizer->Add(colorOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL);
    colorOffsetControlSizer->AddStretchSpacer();

    wxStaticText *colorOffsetLabel = new wxStaticText(colorPage, wxID_ANY, "Offset");
    colorOffsetControlSizer->Add(colorOffsetLabel, 0, wxALIGN_CENTER_VERTICAL);
    colorOffsetControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPageBox->Add(colorOffsetControlSizer, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer *colorMarginControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    colorMarginSlider = new wxSlider(colorPage, ID_COLOR_MARGIN_SLIDER, 0, 0, 1000);
    colorMarginSlider->SetValue(0);
    colorMarginControlSizer->Add(colorMarginSlider);
    colorMarginControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorMarginCtrl = new wxNumberCtrl(colorPage, ID_COLOR_MARGIN_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    colorMarginCtrl->AcceptIntOnly(true);
    colorMarginCtrl->SetInt(0);
    colorMarginControlSizer->Add(colorMarginCtrl, 0, wxALIGN_CENTER_VERTICAL);
    colorMarginControlSizer->AddStretchSpacer();

    wxStaticText *colorMarginLabel = new wxStaticText(colorPage, wxID_ANY, "Margin");
    colorMarginControlSizer->Add(colorMarginLabel, 0, wxALIGN_CENTER_VERTICAL);
    colorMarginControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPageBox->Add(colorMarginControlSizer, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPage->SetSizer(colorPageBox);
    toolsPages[ID_COLOR] = colorPage;
    toolsBook->AddPage(colorPage, "Color tool options");


    wxPanel *cropPage = new wxPanel(toolsBook);
    wxBoxSizer *cropPageBox = new wxBoxSizer(wxVERTICAL);
    cropPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer *cropOffsetControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    cropOffsetSlider = new wxSlider(cropPage, ID_CROP_OFFSET_SLIDER, 0, 0, 1000);
    cropOffsetControlSizer->Add(cropOffsetSlider);
    cropOffsetControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    cropOffsetCtrl = new wxNumberCtrl(cropPage, ID_CROP_OFFSET_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    cropOffsetCtrl->AcceptIntOnly(true);
    cropOffsetControlSizer->Add(cropOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL);
    cropOffsetControlSizer->AddStretchSpacer();

    wxStaticText *cropOffsetLabel = new wxStaticText(cropPage, wxID_ANY, "Offset");
    cropOffsetControlSizer->Add(cropOffsetLabel, 0, wxALIGN_CENTER_VERTICAL);
    cropOffsetControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    cropPageBox->Add(cropOffsetControlSizer, 0, wxEXPAND | wxALL);
    cropPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    cropPage->SetSizer(cropPageBox);
    toolsPages[ID_CROP] = cropPage;
    toolsBook->AddPage(cropPage, "Crop tool options");


    wxPanel *blurPage = new wxPanel(toolsBook);
    wxBoxSizer *blurPageBox = new wxBoxSizer(wxVERTICAL);
    blurPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer *blurRadiusControlSizer = new wxBoxSizer(wxHORIZONTAL);

    m_blurRadius = editor::DEFAULT_BLUR_RADIUS;
    blurRadiusSlider = new wxSlider(blurPage, ID_BLUR_RADIUS_SLIDER, editor::DEFAULT_BLUR_RADIUS, 0, 1000);
    blurRadiusSlider->SetValue(editor::DEFAULT_BLUR_RADIUS);
    blurRadiusControlSizer->Add(blurRadiusSlider);
    blurRadiusControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    blurRadiusCtrl = new wxNumberCtrl(blurPage, ID_BLUR_RADIUS_CTRL, editor::DEFAULT_BLUR_RADIUS, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    blurRadiusCtrl->AcceptIntOnly(true);
    blurRadiusCtrl->SetInt(editor::DEFAULT_BLUR_RADIUS);
    blurRadiusControlSizer->Add(blurRadiusCtrl, 0, wxALIGN_CENTER_VERTICAL);
    blurRadiusControlSizer->AddStretchSpacer();

    wxStaticText *blurRadiusLabel = new wxStaticText(blurPage, wxID_ANY, "Radius");
    blurRadiusControlSizer->Add(blurRadiusLabel, 0, wxALIGN_CENTER_VERTICAL);
    blurRadiusControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    blurPageBox->Add(blurRadiusControlSizer, 0, wxEXPAND | wxALL);
    blurPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    blurPage->SetSizer(blurPageBox);
    toolsPages[ID_BLUR] = blurPage;
    toolsBook->AddPage(blurPage, "Blur tool options");

    box->Add(toolsBook, 0, wxEXPAND);

    //EXPORT BUTTON
    wxButton *exportBtn = new wxButton(this, ID_EXPORT, "EXPORT");
    exportBtn->SetMinSize(editor::MIN_BUTTON_SIZE);
    exportBtn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    box->Add(exportBtn);

    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    SetSizer(box);
    DisplayToolControls(wxID_NONE);
}

void Toolbar::UpdateZoomText () {
    double zoom = mainFrame->GetImagePanel()->GetZoomLevel();
    wxString text = wxString::Format("%d", (int)(zoom * 100));
    text += '%';
    zoomLabel->SetLabelText(text);
}

void Toolbar::DisplayToolControls (int toolID) {
    wxWindow *page;
    if (!mainFrame->GetOriginalImage().IsOk()) {
        page = toolsPages[wxID_NONE];
    }
    else {
        page = toolsPages.find(toolID) == toolsPages.end()
                     ? toolsPages[wxID_NONE]
                     : toolsPages[toolID];
    }

    toolsBook->SetSelection(toolsBook->FindPage(page));

    wxSize oldSize = toolsBook->GetSize();
    wxPoint oldPos = toolsBook->GetPosition();

    page->Fit();
    wxSize newSize = wxSize(this->GetSize().GetWidth(), page->GetSize().GetHeight());
    toolsBook->SetSize(newSize);
    toolsBook->SetMaxSize(newSize);

    toolsBook->SetPosition(wxPoint(oldPos.x + (newSize.GetWidth() - oldSize.GetWidth()),
                                   oldPos.y + (newSize.GetHeight() - oldSize.GetHeight())));
    
    mainFrame->Layout();
}

void Toolbar::PanelImageUpdated () {
    const wxImage &img = mainFrame->GetImagePanel()->GetImage();
    if (!img.IsOk()) return;

    int maxOffset = std::abs(img.GetWidth() - img.GetHeight()) / 2;

    cropOffset = 0;

    cropOffsetSlider->SetMin(-maxOffset);
    cropOffsetSlider->SetMax(maxOffset);
    cropOffsetSlider->SetValue(0);

    cropOffsetCtrl->SetRange(-maxOffset, maxOffset);
    cropOffsetCtrl->SetInt(0);

    colorOffset = 0;

    colorOffsetSlider->SetMin(-maxOffset);
    colorOffsetSlider->SetMax(maxOffset);
    colorOffsetSlider->SetValue(0);

    colorOffsetCtrl->SetRange(-maxOffset, maxOffset);
    colorOffsetCtrl->SetInt(0);

    colorMargin = 0;

    colorMarginSlider->SetValue(0);
    colorMarginSlider->SetMax(wxMax(img.GetWidth(), img.GetHeight()));
    colorMarginCtrl->SetRange(0, wxMax(img.GetWidth(), img.GetHeight()));

    colorMarginCtrl->SetInt(0);

    colorMeanBtn->SetBackgroundColour(mainFrame->GetImagePanel()->GetMeanColor());

    DisplayToolControls(wxID_NONE);
}

void Toolbar::OnZoomUpdated (wxCommandEvent& event) {
    UpdateZoomText();
}

void Toolbar::OnColorPress (wxCommandEvent &event) {
    if (mainFrame->GetImagePanel()->GetImage().IsOk()) {
        wxSize size = mainFrame->GetOriginalImage().GetSize();
        wxColour color = colorPickerCtrl->GetColour();
        wxImage img = editor::makeSquared(mainFrame->GetOriginalBitmap(), wxRect(0,0,size.GetWidth(), size.GetHeight()), &color, colorOffset, colorMargin);
        mainFrame->GetImagePanel()->SetImage(img);
        mainFrame->UpdateStatus();
    }

    DisplayToolControls(ID_COLOR);
}

void Toolbar::OnCropPress (wxCommandEvent &event) {
    if (mainFrame->GetImagePanel()->GetImage().IsOk()) {
        wxImage img = editor::makeSquaredCrop(mainFrame->GetOriginalImage(), cropOffset);
        mainFrame->GetImagePanel()->SetImage(img);
        mainFrame->UpdateStatus();
    }

    DisplayToolControls(ID_CROP);
}

void Toolbar::OnBlurPress (wxCommandEvent &event) {
    if (mainFrame->GetImagePanel()->GetImage().IsOk()) {
        wxImage img = editor::makeSquaredBlur(mainFrame->GetOriginalImage(), mainFrame->GetOriginalBitmap(), m_blurRadius);
        mainFrame->GetImagePanel()->SetImage(img);
        mainFrame->UpdateStatus();
    }

    DisplayToolControls(ID_BLUR);
}

void Toolbar::OnExportPress (wxCommandEvent &event) {
    mainFrame->ExportCurrent();
}

void Toolbar::OnZoomOutPress (wxCommandEvent &event) {
    if (!mainFrame->GetOriginalImage().IsOk()) return;

    mainFrame->GetImagePanel()->ZoomOut();
    UpdateZoomText();
}

void Toolbar::OnZoomFitPress (wxCommandEvent &event) {
    if (!mainFrame->GetOriginalImage().IsOk()) return;

    mainFrame->GetImagePanel()->Autozoom();
    UpdateZoomText();
}

void Toolbar::OnZoomInPress (wxCommandEvent &event) {
    if (!mainFrame->GetOriginalImage().IsOk()) return;

    mainFrame->GetImagePanel()->ZoomIn();
    UpdateZoomText();
}

void Toolbar::OnCenterToggle (wxCommandEvent &event) {
    mainFrame->GetImagePanel()->SetDisplayCenter(toggleCenterBtn->GetValue());
}

void Toolbar::OnColorPickerChange (wxColourPickerEvent &event) {
    OnColorPress(event);
}

void Toolbar::OnColorWhitePress (wxCommandEvent &event) {
    colorPickerCtrl->SetColour(wxColour(255, 255, 255));
    OnColorPress(event);
}

void Toolbar::OnColorBlackPress (wxCommandEvent &event) {
    colorPickerCtrl->SetColour(wxColour(0, 0, 0));
    OnColorPress(event);
}

void Toolbar::OnColorMeanPress (wxCommandEvent &event) {
    colorPickerCtrl->SetColour(mainFrame->GetImagePanel()->GetMeanColor());
    OnColorPress(event);
}

void Toolbar::OnColorOffsetSlide (wxCommandEvent &event) {
    colorOffset = event.GetInt();
    colorOffsetCtrl->SetInt(colorOffset);
    OnColorPress(event);
}

void Toolbar::OnColorOffsetText (wxCommandEvent &event) {
    colorOffset = colorOffsetCtrl->GetInt();
    colorOffsetSlider->SetValue(colorOffset);
    OnColorPress(event);
}

void Toolbar::OnColorMarginSlide (wxCommandEvent &event) {
    colorMargin = event.GetInt();
    colorMarginCtrl->SetInt(colorMargin);
    OnColorPress(event);

    wxSize imgSize = mainFrame->GetImagePanel()->GetImage().GetSize();
    wxSize panelSize = mainFrame->GetImagePanel()->GetSize();
    if (imgSize.GetWidth() > panelSize.GetWidth() || imgSize.GetHeight() > panelSize.GetHeight()) {
        mainFrame->GetImagePanel()->Autozoom();
    }
}

void Toolbar::OnColorMarginText (wxCommandEvent &event) {
    colorMargin = colorMarginCtrl->GetInt();
    colorMarginSlider->SetValue(colorMargin);
    OnColorPress(event);

    wxSize imgSize = mainFrame->GetImagePanel()->GetImage().GetSize();
    wxSize panelSize = mainFrame->GetImagePanel()->GetSize();
    if (imgSize.GetWidth() > panelSize.GetWidth() || imgSize.GetHeight() > panelSize.GetHeight()) {
        mainFrame->GetImagePanel()->Autozoom();
    }
}

void Toolbar::OnCropOffsetSlide (wxCommandEvent &event) {
    cropOffset = event.GetInt();
    cropOffsetCtrl->SetInt(cropOffset);
    OnCropPress(event);
}

void Toolbar::OnCropOffsetText (wxCommandEvent &event) {
    cropOffset = cropOffsetCtrl->GetInt();
    cropOffsetSlider->SetValue(cropOffset);
    OnCropPress(event);
}

void Toolbar::OnBlurRadiusSlide (wxCommandEvent &event) {
    m_blurRadius = event.GetInt();
    blurRadiusCtrl->SetInt(m_blurRadius);
    OnBlurPress(event);
}

void Toolbar::OnBlurRadiusText (wxCommandEvent &event) {
    m_blurRadius = blurRadiusCtrl->GetInt();
    blurRadiusSlider->SetValue(cropOffset);
    OnBlurPress(event);
}