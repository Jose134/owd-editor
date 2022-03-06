#include "toolbar.hpp"

#include <wx/simplebook.h>
#include "imagePanel.hpp"
#include "regionPanel.hpp"

#include "constants.hpp"
#include "edits.hpp"

#include "icons.hpp"

enum {
    ID_REGION_PANEL           = wxID_HIGHEST + 1,

    ID_COLOR                  = wxID_HIGHEST + 2,
    ID_CROP                   = wxID_HIGHEST + 3,
    ID_BLUR                   = wxID_HIGHEST + 4,
    ID_EXPORT                 = wxID_HIGHEST + 5,

    ID_COLOR_COLOR_PICKER     = wxID_HIGHEST + 6,
    ID_COLOR_WHITE_BTN        = wxID_HIGHEST + 7,
    ID_COLOR_BLACK_BTN        = wxID_HIGHEST + 8,
    ID_COLOR_MEAN_BTN         = wxID_HIGHEST + 9,

    ID_TOGGLE_CENTER          = wxID_HIGHEST + 10
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

END_EVENT_TABLE()

Toolbar::Toolbar (wxFrame* parent, wxFrame* mainFrame)
    : wxPanel(parent) {

    this->m_mainFrame = (MainFrame*)mainFrame;
    m_mainFrame->GetImagePanel()->Bind(EVT_IMGPANEL_ZOOM, &Toolbar::OnZoomUpdated, this);

    wxBoxSizer* box = new wxBoxSizer(wxVERTICAL);
    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    // TOOLS    
    wxButton* colorBtn = new wxButton(this, ID_COLOR, "COLOR BACKGROUND");
    colorBtn->SetMinSize(editor::MIN_BUTTON_SIZE);
    box->Add(colorBtn);
    
    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton* cropBtn = new wxButton(this, ID_CROP, "CROP SQUARED");
    cropBtn->SetMinSize(editor::MIN_BUTTON_SIZE);
    box->Add(cropBtn);

    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton* blurBtn = new wxButton(this, ID_BLUR, "BLUR BACKGROUND");
    blurBtn->SetMinSize(editor::MIN_BUTTON_SIZE);
    box->Add(blurBtn);

    box->AddStretchSpacer();

    // ZOOM CONTROLS
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    
    wxBitmapButton* zoomoutBtn = new wxBitmapButton(this, wxID_ZOOM_OUT, icon::loadIcon(icon::ID::ZOOM_OUT));
    zoomoutBtn->GetBitmap().CreateScaled(32, 32, 1, 1);
    zoomoutBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    hbox->Add(zoomoutBtn);

    hbox->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    wxBitmapButton* zoomfitBtn = new wxBitmapButton(this, wxID_ZOOM_FIT, icon::loadIcon(icon::ID::ZOOM_FIT));
    zoomfitBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    hbox->Add(zoomfitBtn);

    hbox->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    wxBitmapButton *zoominBtn = new wxBitmapButton(this, wxID_ZOOM_IN, icon::loadIcon(icon::ID::ZOOM_IN));
    zoominBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    hbox->Add(zoominBtn);

    hbox->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    m_toggleCenterBtn = new wxBitmapToggleButton(this, ID_TOGGLE_CENTER, icon::loadIcon(icon::ID::CENTER));
    m_toggleCenterBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    hbox->Add(m_toggleCenterBtn);

    hbox->AddStretchSpacer();

    m_zoomLabel = new wxStaticText(this, wxID_ANY, "");
    UpdateZoomText();
    hbox->Add(m_zoomLabel, 0, wxALIGN_CENTER_VERTICAL);

    hbox->AddStretchSpacer();

    box->Add(hbox, 0, wxEXPAND | wxALL);

    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    // TOOLS' CONTROLS
    m_toolsBook = new wxSimplebook(this);

    wxPanel* emptyPage = new wxPanel(m_toolsBook);
    m_toolsPages[wxID_NONE] = emptyPage;
    m_toolsBook->AddPage(emptyPage, "Emtpy");
    

    // COLOR TOOL CONTROLS
    wxPanel* colorPage = new wxPanel(m_toolsBook);
    wxBoxSizer* colorPageBox = new wxBoxSizer(wxVERTICAL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer* colorColorControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_colorPickerCtrl = new wxColourPickerCtrl(colorPage, ID_COLOR_COLOR_PICKER);
    colorColorControlSizer->Add(m_colorPickerCtrl);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton* colorWhiteBtn = new wxButton(colorPage, ID_COLOR_WHITE_BTN, "");
    colorWhiteBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    colorWhiteBtn->SetBackgroundColour(wxColour(255, 255, 255));
    colorColorControlSizer->Add(colorWhiteBtn);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton* colorBlackBtn = new wxButton(colorPage, ID_COLOR_BLACK_BTN, "");
    colorBlackBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    colorBlackBtn->SetBackgroundColour(wxColour(0, 0, 0));
    colorColorControlSizer->Add(colorBlackBtn);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_colorMeanBtn = new wxButton(colorPage, ID_COLOR_MEAN_BTN, "");
    m_colorMeanBtn->SetMinSize(editor::MIN_SQUARE_BUTTON_SIZE);
    m_colorMeanBtn->SetBackgroundColour(m_mainFrame->GetImagePanel()->GetMeanColor());
    colorColorControlSizer->Add(m_colorMeanBtn);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorColorControlSizer->AddStretchSpacer();

    wxStaticText* colorColorLabel = new wxStaticText(colorPage, wxID_ANY, "Color");
    colorColorControlSizer->Add(colorColorLabel, 0, wxALIGN_CENTER_VERTICAL);
    colorColorControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPageBox->Add(colorColorControlSizer, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_colorOffsetXField = new wxField(colorPage, wxID_ANY, 0, "Offset X");
    m_colorOffsetXField->AcceptIntOnly(true);
    m_colorOffsetXField->SetValue(0);
    m_colorOffsetXField->Bind(EVT_VALUE_CHANGED, &Toolbar::OnColorOffsetChange, this);

    colorPageBox->Add(m_colorOffsetXField, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    m_colorOffsetYField = new wxField(colorPage, wxID_ANY, 0, "Offset Y");
    m_colorOffsetYField->AcceptIntOnly(true);
    m_colorOffsetYField->SetValue(0);
    m_colorOffsetYField->Bind(EVT_VALUE_CHANGED, &Toolbar::OnColorOffsetChange, this);

    colorPageBox->Add(m_colorOffsetYField, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_colorMarginField = new wxField(colorPage, wxID_ANY, 0, "Margin");
    m_colorMarginField->AcceptIntOnly(true);
    m_colorMarginField->SetValue(0);
    m_colorMarginField->Bind(EVT_VALUE_CHANGED, &Toolbar::OnColorMarginChange, this);

    colorPageBox->Add(m_colorMarginField, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);


    colorPage->SetSizer(colorPageBox);
    m_toolsPages[ID_COLOR] = colorPage;
    m_toolsBook->AddPage(colorPage, "Color tool options");

    // CROP TOOL CONTROLS
    wxPanel* cropPage = new wxPanel(m_toolsBook);
    wxBoxSizer* cropPageBox = new wxBoxSizer(wxVERTICAL);
    cropPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer* cropOffsetControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_cropOffsetField = new wxField(cropPage, wxID_ANY, 0, "Offset");
    m_cropOffsetField->AcceptIntOnly(true);
    m_cropOffsetField->SetValue(0);
    m_cropOffsetField->Bind(EVT_VALUE_CHANGED, &Toolbar::OnCropOffsetChange, this);

    cropPageBox->Add(m_cropOffsetField, 0, wxEXPAND | wxALL);
    cropPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);


    cropPage->SetSizer(cropPageBox);
    m_toolsPages[ID_CROP] = cropPage;
    m_toolsBook->AddPage(cropPage, "Crop tool options");

    // BLUR TOOL CONTROLS
    wxPanel* blurPage = new wxPanel(m_toolsBook);
    wxBoxSizer* blurPageBox = new wxBoxSizer(wxVERTICAL);
    blurPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_blurRadiusField = new wxField(blurPage, wxID_ANY, 0, "Radius");
    m_blurRadiusField->AcceptIntOnly(true);
    m_blurRadiusField->SetValue(0);
    m_blurRadiusField->SetRange(0, INT_MAX);
    m_blurRadiusField->Bind(EVT_VALUE_CHANGED, &Toolbar::OnBlurRadiusChange, this);

    blurPageBox->Add(m_blurRadiusField, 0, wxEXPAND | wxALL);
    blurPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);


    blurPage->SetSizer(blurPageBox);
    m_toolsPages[ID_BLUR] = blurPage;
    m_toolsBook->AddPage(blurPage, "Blur tool options");

    box->Add(m_toolsBook, 0, wxEXPAND);

    // EXPORT BUTTON
    wxButton* exportBtn = new wxButton(this, ID_EXPORT, "EXPORT");
    exportBtn->SetMinSize(editor::MIN_BUTTON_SIZE);
    exportBtn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    box->Add(exportBtn);

    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    SetSizer(box);
    DisplayToolControls(wxID_NONE);
}

void Toolbar::UpdateZoomText () {
    double zoom = m_mainFrame->GetImagePanel()->GetZoomLevel();
    wxString text = wxString::Format("%d", (int)(zoom * 100));
    text += '%';
    m_zoomLabel->SetLabelText(text);
}

void Toolbar::DisplayToolControls (int toolID) {
    wxWindow *page;
    if (!m_mainFrame->GetOriginalImage().IsOk()) {
        page = m_toolsPages[wxID_NONE];
    }
    else {
        page = m_toolsPages.find(toolID) == m_toolsPages.end()
                     ? m_toolsPages[wxID_NONE]
                     : m_toolsPages[toolID];
    }

    m_toolsBook->SetSelection(m_toolsBook->FindPage(page));

    wxSize oldSize = m_toolsBook->GetSize();
    wxPoint oldPos = m_toolsBook->GetPosition();

    page->Fit();
    wxSize newSize = wxSize(this->GetSize().GetWidth(), page->GetSize().GetHeight());
    m_toolsBook->SetSize(newSize);
    m_toolsBook->SetMaxSize(newSize);

    m_toolsBook->SetPosition(wxPoint(oldPos.x + (newSize.GetWidth() - oldSize.GetWidth()),
                                   oldPos.y + (newSize.GetHeight() - oldSize.GetHeight())));
    
    m_mainFrame->Layout();
}

void Toolbar::PanelImageUpdated () {
    const wxImage &img = m_mainFrame->GetImagePanel()->GetImage();
    if (!img.IsOk()) return;

    int maxOffset = std::abs(img.GetWidth() - img.GetHeight()) / 2;

    int maxOffsetX = img.GetWidth() > img.GetHeight()
                        ? img.GetWidth()
                        : (3*img.GetHeight() - img.GetWidth()) / 2;

    int maxOffsetY = img.GetWidth() < img.GetHeight()
                        ? img.GetHeight()
                        : (3*img.GetWidth() - img.GetHeight()) / 2;

    m_cropOffset = 0;
    m_cropOffsetField->SetRange(-maxOffset, maxOffset);
    m_cropOffsetField->SetValue(0);

    m_colorOffsetX = 0;
    m_colorOffsetY = 0;
    m_colorOffsetXField->SetRange(-maxOffsetX, maxOffsetX);
    m_colorOffsetXField->SetValue(0);
    m_colorOffsetYField->SetRange(-maxOffsetY, maxOffsetY);
    m_colorOffsetYField->SetValue(0);

    m_colorMargin = 0;
    m_colorMarginField->SetValue(0);
    m_colorMarginField->SetRange(0, INT_MAX);

    m_colorMeanBtn->SetBackgroundColour(m_mainFrame->GetImagePanel()->GetMeanColor());

    DisplayToolControls(wxID_NONE);
}

void Toolbar::OnZoomUpdated (wxCommandEvent& event) {
    UpdateZoomText();
}

void Toolbar::OnColorPress (wxCommandEvent &event) {
    if (m_mainFrame->GetImagePanel()->GetImage().IsOk()) {
        wxBusyCursor wait;

        wxSize size = m_mainFrame->GetOriginalImage().GetSize();
        wxColour color = m_colorPickerCtrl->GetColour();
        wxImage img = editor::makeSquared(m_mainFrame->GetOriginalBitmap(), wxRect(0,0,size.GetWidth(), size.GetHeight()), &color, m_colorOffsetX, m_colorOffsetY, m_colorMargin);
        m_mainFrame->GetImagePanel()->SetImage(img);
        m_mainFrame->GetImagePanel()->Autozoom();
        m_mainFrame->UpdateStatus();
    }

    DisplayToolControls(ID_COLOR);
}

void Toolbar::OnCropPress (wxCommandEvent &event) {
    if (m_mainFrame->GetImagePanel()->GetImage().IsOk()) {
        wxBusyCursor wait;

        wxImage img = editor::makeSquaredCrop(m_mainFrame->GetOriginalImage(), m_cropOffset);
        m_mainFrame->GetImagePanel()->SetImage(img);
        m_mainFrame->UpdateStatus();
    }

    DisplayToolControls(ID_CROP);
}

void Toolbar::OnBlurPress (wxCommandEvent &event) {
    if (m_mainFrame->GetImagePanel()->GetImage().IsOk()) {
        wxBusyCursor wait;

        wxImage img = editor::makeSquaredBlur(m_mainFrame->GetOriginalImage(), m_mainFrame->GetOriginalBitmap(), m_blurRadius);
        m_mainFrame->GetImagePanel()->SetImage(img);
        m_mainFrame->UpdateStatus();
    }

    DisplayToolControls(ID_BLUR);
}

void Toolbar::OnExportPress (wxCommandEvent &event) {
    m_mainFrame->ExportCurrent();
}

void Toolbar::OnZoomOutPress (wxCommandEvent &event) {
    if (!m_mainFrame->GetOriginalImage().IsOk()) return;

    m_mainFrame->GetImagePanel()->ZoomOut();
    UpdateZoomText();
}

void Toolbar::OnZoomFitPress (wxCommandEvent &event) {
    if (!m_mainFrame->GetOriginalImage().IsOk()) return;

    m_mainFrame->GetImagePanel()->Autozoom();
    UpdateZoomText();
}

void Toolbar::OnZoomInPress (wxCommandEvent &event) {
    if (!m_mainFrame->GetOriginalImage().IsOk()) return;

    m_mainFrame->GetImagePanel()->ZoomIn();
    UpdateZoomText();
}

void Toolbar::OnCenterToggle (wxCommandEvent &event) {
    m_mainFrame->GetImagePanel()->SetDisplayCenter(m_toggleCenterBtn->GetValue());
}

void Toolbar::OnColorPickerChange (wxColourPickerEvent &event) {
    OnColorPress(event);
}

void Toolbar::OnColorWhitePress (wxCommandEvent &event) {
    m_colorPickerCtrl->SetColour(wxColour(255, 255, 255));
    OnColorPress(event);
}

void Toolbar::OnColorBlackPress (wxCommandEvent &event) {
    m_colorPickerCtrl->SetColour(wxColour(0, 0, 0));
    OnColorPress(event);
}

void Toolbar::OnColorMeanPress (wxCommandEvent &event) {
    m_colorPickerCtrl->SetColour(m_mainFrame->GetImagePanel()->GetMeanColor());
    OnColorPress(event);
}

void Toolbar::OnColorOffsetChange (wxCommandEvent &event) {
    m_colorOffsetX = m_colorOffsetXField->GetInt();
    m_colorOffsetY = m_colorOffsetYField->GetInt();

    OnColorPress(event);
}

void Toolbar::OnColorMarginChange (wxCommandEvent &event) {
    m_colorMargin = m_colorMarginField->GetInt();
    OnColorPress(event);

    wxSize imgSize = m_mainFrame->GetImagePanel()->GetImage().GetSize();
    wxSize panelSize = m_mainFrame->GetImagePanel()->GetSize();
    if (imgSize.GetWidth() > panelSize.GetWidth() || imgSize.GetHeight() > panelSize.GetHeight()) {
        m_mainFrame->GetImagePanel()->Autozoom();
    }
}

void Toolbar::OnCropOffsetChange (wxCommandEvent &event) {
    m_cropOffset = m_cropOffsetField->GetInt();

    OnCropPress(event);
}

void Toolbar::OnBlurRadiusChange (wxCommandEvent &event) {
    m_blurRadius = m_blurRadiusField->GetInt();

    OnBlurPress(event);
}