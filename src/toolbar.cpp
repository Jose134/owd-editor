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
    ID_COLOR_OFFSETX_SLIDER    = wxID_HIGHEST + 10,
    ID_COLOR_OFFSETX_CTRL      = wxID_HIGHEST + 11,
    ID_COLOR_OFFSETY_SLIDER    = wxID_HIGHEST + 12,
    ID_COLOR_OFFSETY_CTRL      = wxID_HIGHEST + 13,
    ID_COLOR_MARGIN_SLIDER    = wxID_HIGHEST + 14,
    ID_COLOR_MARGIN_CTRL      = wxID_HIGHEST + 15,
    ID_CROP_OFFSET_SLIDER     = wxID_HIGHEST + 16,
    ID_CROP_OFFSET_CTRL       = wxID_HIGHEST + 17,
    ID_BLUR_RADIUS_SLIDER     = wxID_HIGHEST + 18,
    ID_BLUR_RADIUS_CTRL       = wxID_HIGHEST + 19,

    ID_TOGGLE_CENTER          = wxID_HIGHEST + 20
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
EVT_SLIDER(ID_COLOR_OFFSETX_SLIDER, Toolbar::OnColorOffsetSlide)
EVT_TEXT_ENTER(ID_COLOR_OFFSETX_CTRL, Toolbar::OnColorOffsetText)
EVT_SLIDER(ID_COLOR_OFFSETY_SLIDER, Toolbar::OnColorOffsetSlide)
EVT_TEXT_ENTER(ID_COLOR_OFFSETY_CTRL, Toolbar::OnColorOffsetText)
EVT_SLIDER(ID_COLOR_MARGIN_SLIDER, Toolbar::OnColorMarginSlide)
EVT_TEXT_ENTER(ID_COLOR_MARGIN_CTRL, Toolbar::OnColorMarginText)

EVT_SLIDER(ID_CROP_OFFSET_SLIDER, Toolbar::OnCropOffsetSlide)
EVT_TEXT_ENTER(ID_CROP_OFFSET_CTRL, Toolbar::OnCropOffsetText)

EVT_SLIDER(ID_BLUR_RADIUS_SLIDER, Toolbar::OnBlurRadiusSlide)
EVT_TEXT_ENTER(ID_BLUR_RADIUS_CTRL, Toolbar::OnBlurRadiusText)


END_EVENT_TABLE()

Toolbar::Toolbar (wxFrame* parent, wxFrame* mainFrame)
    : wxPanel(parent) {

    this->m_mainFrame = (MainFrame*)mainFrame;

    wxBoxSizer* box = new wxBoxSizer(wxVERTICAL);
    box->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    //TOOLS    
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

    //ZOOM CONTROLS
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
    // wxBitmap("res/cross-16.png")
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

    //TOOLS' CONTROLS
    m_toolsBook = new wxSimplebook(this);

    wxPanel* emptyPage = new wxPanel(m_toolsBook);
    m_toolsPages[wxID_NONE] = emptyPage;
    m_toolsBook->AddPage(emptyPage, "Emtpy");
    
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

    wxBoxSizer* colorOffsetXControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_colorOffsetXSlider = new wxSlider(colorPage, ID_COLOR_OFFSETX_SLIDER, 0, 0, 1000);
    colorOffsetXControlSizer->Add(m_colorOffsetXSlider);
    colorOffsetXControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_colorOffsetXCtrl = new wxNumberCtrl(colorPage, ID_COLOR_OFFSETX_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_colorOffsetXCtrl->AcceptIntOnly(true);
    m_colorOffsetXCtrl->SetInt(0);
    colorOffsetXControlSizer->Add(m_colorOffsetXCtrl, 0, wxALIGN_CENTER_VERTICAL);
    colorOffsetXControlSizer->AddStretchSpacer();

    wxStaticText* colorOffsetXLabel = new wxStaticText(colorPage, wxID_ANY, "Offset X");
    colorOffsetXControlSizer->Add(colorOffsetXLabel, 0, wxALIGN_CENTER_VERTICAL);
    colorOffsetXControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPageBox->Add(colorOffsetXControlSizer, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer* colorOffsetYControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_colorOffsetYSlider = new wxSlider(colorPage, ID_COLOR_OFFSETY_SLIDER, 0, 0, 1000);
    colorOffsetYControlSizer->Add(m_colorOffsetYSlider);
    colorOffsetYControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_colorOffsetYCtrl = new wxNumberCtrl(colorPage, ID_COLOR_OFFSETY_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_colorOffsetYCtrl->AcceptIntOnly(true);
    m_colorOffsetYCtrl->SetInt(0);
    colorOffsetYControlSizer->Add(m_colorOffsetYCtrl, 0, wxALIGN_CENTER_VERTICAL);
    colorOffsetYControlSizer->AddStretchSpacer();

    wxStaticText* colorOffsetYLabel = new wxStaticText(colorPage, wxID_ANY, "Offset Y");
    colorOffsetYControlSizer->Add(colorOffsetYLabel, 0, wxALIGN_CENTER_VERTICAL);
    colorOffsetYControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPageBox->Add(colorOffsetYControlSizer, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer* colorMarginControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_colorMarginSlider = new wxSlider(colorPage, ID_COLOR_MARGIN_SLIDER, 0, 0, 1000);
    m_colorMarginSlider->SetValue(0);
    colorMarginControlSizer->Add(m_colorMarginSlider);
    colorMarginControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_colorMarginCtrl = new wxNumberCtrl(colorPage, ID_COLOR_MARGIN_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_colorMarginCtrl->AcceptIntOnly(true);
    m_colorMarginCtrl->SetInt(0);
    colorMarginControlSizer->Add(m_colorMarginCtrl, 0, wxALIGN_CENTER_VERTICAL);
    colorMarginControlSizer->AddStretchSpacer();

    wxStaticText* colorMarginLabel = new wxStaticText(colorPage, wxID_ANY, "Margin");
    colorMarginControlSizer->Add(colorMarginLabel, 0, wxALIGN_CENTER_VERTICAL);
    colorMarginControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPageBox->Add(colorMarginControlSizer, 0, wxEXPAND | wxALL);
    colorPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    colorPage->SetSizer(colorPageBox);
    m_toolsPages[ID_COLOR] = colorPage;
    m_toolsBook->AddPage(colorPage, "Color tool options");


    wxPanel* cropPage = new wxPanel(m_toolsBook);
    wxBoxSizer* cropPageBox = new wxBoxSizer(wxVERTICAL);
    cropPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer* cropOffsetControlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_cropOffsetSlider = new wxSlider(cropPage, ID_CROP_OFFSET_SLIDER, 0, 0, 1000);
    cropOffsetControlSizer->Add(m_cropOffsetSlider);
    cropOffsetControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_cropOffsetCtrl = new wxNumberCtrl(cropPage, ID_CROP_OFFSET_CTRL, 0, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_cropOffsetCtrl->AcceptIntOnly(true);
    cropOffsetControlSizer->Add(m_cropOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL);
    cropOffsetControlSizer->AddStretchSpacer();

    wxStaticText* cropOffsetLabel = new wxStaticText(cropPage, wxID_ANY, "Offset");
    cropOffsetControlSizer->Add(cropOffsetLabel, 0, wxALIGN_CENTER_VERTICAL);
    cropOffsetControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    cropPageBox->Add(cropOffsetControlSizer, 0, wxEXPAND | wxALL);
    cropPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    cropPage->SetSizer(cropPageBox);
    m_toolsPages[ID_CROP] = cropPage;
    m_toolsBook->AddPage(cropPage, "Crop tool options");


    wxPanel* blurPage = new wxPanel(m_toolsBook);
    wxBoxSizer* blurPageBox = new wxBoxSizer(wxVERTICAL);
    blurPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer* blurRadiusControlSizer = new wxBoxSizer(wxHORIZONTAL);

    m_blurRadius = editor::DEFAULT_BLUR_RADIUS;
    m_blurRadiusSlider = new wxSlider(blurPage, ID_BLUR_RADIUS_SLIDER, editor::DEFAULT_BLUR_RADIUS, 0, 1000);
    m_blurRadiusSlider->SetValue(editor::DEFAULT_BLUR_RADIUS);
    blurRadiusControlSizer->Add(m_blurRadiusSlider);
    blurRadiusControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_blurRadiusCtrl = new wxNumberCtrl(blurPage, ID_BLUR_RADIUS_CTRL, editor::DEFAULT_BLUR_RADIUS, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_blurRadiusCtrl->AcceptIntOnly(true);
    m_blurRadiusCtrl->SetInt(editor::DEFAULT_BLUR_RADIUS);
    blurRadiusControlSizer->Add(m_blurRadiusCtrl, 0, wxALIGN_CENTER_VERTICAL);
    blurRadiusControlSizer->AddStretchSpacer();

    wxStaticText* blurRadiusLabel = new wxStaticText(blurPage, wxID_ANY, "Radius");
    blurRadiusControlSizer->Add(blurRadiusLabel, 0, wxALIGN_CENTER_VERTICAL);
    blurRadiusControlSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    blurPageBox->Add(blurRadiusControlSizer, 0, wxEXPAND | wxALL);
    blurPageBox->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    blurPage->SetSizer(blurPageBox);
    m_toolsPages[ID_BLUR] = blurPage;
    m_toolsBook->AddPage(blurPage, "Blur tool options");

    box->Add(m_toolsBook, 0, wxEXPAND);

    //EXPORT BUTTON
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

    m_cropOffsetSlider->SetMin(-maxOffset);
    m_cropOffsetSlider->SetMax(maxOffset);
    m_cropOffsetSlider->SetValue(0);

    m_cropOffsetCtrl->SetRange(-maxOffset, maxOffset);
    m_cropOffsetCtrl->SetInt(0);

    m_colorOffsetX = 0;
    m_colorOffsetY = 0;

    m_colorOffsetXSlider->SetMin(-maxOffsetX);
    m_colorOffsetXSlider->SetMax(maxOffsetX);
    m_colorOffsetXSlider->SetValue(0);

    m_colorOffsetXCtrl->SetRange(-maxOffsetX, maxOffsetX);
    m_colorOffsetXCtrl->SetInt(0);

    m_colorOffsetYSlider->SetMin(-maxOffsetY);
    m_colorOffsetYSlider->SetMax(maxOffsetY);
    m_colorOffsetYSlider->SetValue(0);

    m_colorOffsetYCtrl->SetRange(-maxOffsetY, maxOffsetY);
    m_colorOffsetYCtrl->SetInt(0);

    m_colorMargin = 0;

    m_colorMarginSlider->SetValue(0);
    m_colorMarginSlider->SetMax(wxMax(img.GetWidth(), img.GetHeight()));
    m_colorMarginCtrl->SetRange(0, wxMax(img.GetWidth(), img.GetHeight()));

    m_colorMarginCtrl->SetInt(0);

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

void Toolbar::OnColorOffsetSlide (wxCommandEvent &event) {
    m_colorOffsetX = m_colorOffsetXSlider->GetValue();
    m_colorOffsetXCtrl->SetInt(m_colorOffsetX);

    m_colorOffsetY = m_colorOffsetYSlider->GetValue();
    m_colorOffsetYCtrl->SetInt(m_colorOffsetY);

    OnColorPress(event);
}

void Toolbar::OnColorOffsetText (wxCommandEvent &event) {
    m_colorOffsetX = m_colorOffsetXCtrl->GetInt();
    m_colorOffsetXSlider->SetValue(m_colorOffsetX);
    
    m_colorOffsetY = m_colorOffsetYCtrl->GetInt();
    m_colorOffsetYSlider->SetValue(m_colorOffsetY);
    
    OnColorPress(event);
}

void Toolbar::OnColorMarginSlide (wxCommandEvent &event) {
    m_colorMargin = event.GetInt();
    m_colorMarginCtrl->SetInt(m_colorMargin);
    OnColorPress(event);

    wxSize imgSize = m_mainFrame->GetImagePanel()->GetImage().GetSize();
    wxSize panelSize = m_mainFrame->GetImagePanel()->GetSize();
    if (imgSize.GetWidth() > panelSize.GetWidth() || imgSize.GetHeight() > panelSize.GetHeight()) {
        m_mainFrame->GetImagePanel()->Autozoom();
    }
}

void Toolbar::OnColorMarginText (wxCommandEvent &event) {
    m_colorMargin = m_colorMarginCtrl->GetInt();
    m_colorMarginSlider->SetValue(m_colorMargin);
    OnColorPress(event);

    wxSize imgSize = m_mainFrame->GetImagePanel()->GetImage().GetSize();
    wxSize panelSize = m_mainFrame->GetImagePanel()->GetSize();
    if (imgSize.GetWidth() > panelSize.GetWidth() || imgSize.GetHeight() > panelSize.GetHeight()) {
        m_mainFrame->GetImagePanel()->Autozoom();
    }
}

void Toolbar::OnCropOffsetSlide (wxCommandEvent &event) {
    m_cropOffset = event.GetInt();
    m_cropOffsetCtrl->SetInt(m_cropOffset);
    OnCropPress(event);
}

void Toolbar::OnCropOffsetText (wxCommandEvent &event) {
    m_cropOffset = m_cropOffsetCtrl->GetInt();
    m_cropOffsetSlider->SetValue(m_cropOffset);
    OnCropPress(event);
}

void Toolbar::OnBlurRadiusSlide (wxCommandEvent &event) {
    m_blurRadius = event.GetInt();
    m_blurRadiusCtrl->SetInt(m_blurRadius);
    OnBlurPress(event);
}

void Toolbar::OnBlurRadiusText (wxCommandEvent &event) {
    m_blurRadius = m_blurRadiusCtrl->GetInt();
    m_blurRadiusSlider->SetValue(m_cropOffset);
    OnBlurPress(event);
}