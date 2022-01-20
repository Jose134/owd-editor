#include "compositionDialog.hpp"

enum {
    ID_ROWS_TXT  = wxID_HIGHEST + 1,
    ID_COLS_CTRL = wxID_HIGHEST + 2,
    ID_COLOR_CTRL = wxID_HIGHEST + 3,
    ID_DIR_COMBO = wxID_HIGHEST + 4
};

BEGIN_EVENT_TABLE (CompositionDialog, wxDialog)

EVT_BUTTON(wxID_OK, CompositionDialog::OnOk)
EVT_TEXT_ENTER(ID_COLS_CTRL, CompositionDialog::OnColsCtrlEnter)
EVT_COMBOBOX(ID_DIR_COMBO, CompositionDialog::OnDirCombo)

END_EVENT_TABLE()

CompositionDialog::CompositionDialog(wxFrame* parent, const wxArrayString& files)
    : wxDialog(parent, wxID_ANY, "Composition", wxDefaultPosition, wxSize(500, 350)) {

    m_mainFrame = (MainFrame*)parent;
    m_files = files;

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer* fileCountSizer = new wxBoxSizer(wxHORIZONTAL);
    fileCountSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxString fileCountTxt = wxString::Format("%lu files loaded.", files.GetCount());
    wxStaticText* fileCountLabel = new wxStaticText(this, wxID_ANY, fileCountTxt);
    fileCountSizer->Add(fileCountLabel, wxALIGN_LEFT);
    
    mainSizer->Add(fileCountSizer, 0, wxEXPAND | wxALL);
    mainSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxStaticBoxSizer* layoutSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Layout");
    layoutSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    wxBoxSizer* colorRow = new wxBoxSizer(wxHORIZONTAL);
    colorRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_colorPickerCtrl = new wxColourPickerCtrl(this, ID_COLOR_CTRL);
    colorRow->Add(m_colorPickerCtrl);
    colorRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxStaticText* colorTxt = new wxStaticText(this, wxID_ANY, "Color");
    colorRow->Add(colorTxt, 0, wxALIGN_CENTER_VERTICAL);
    colorRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    layoutSizer->Add(colorRow);
    layoutSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    wxBoxSizer* colsRow = new wxBoxSizer(wxHORIZONTAL);
    colsRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_cols = 2;
    m_colsCtrl = new wxNumberCtrl(this, ID_COLS_CTRL, 2, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_colsCtrl->AcceptIntOnly(true);
    m_colsCtrl->SetRange(1, INT_MAX);
    colsRow->Add(m_colsCtrl);
    colsRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxStaticText* colsTxt = new wxStaticText(this, wxID_ANY, "Columns");
    colsRow->Add(colsTxt, 0, wxALIGN_CENTER_VERTICAL);
    colsRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    layoutSizer->Add(colsRow);
    layoutSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    wxBoxSizer* rowsRow = new wxBoxSizer(wxHORIZONTAL);
    rowsRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    int defaultRows = ceil((double)files.GetCount() / 2);
    m_rowsLabel = new wxStaticText(this, wxID_ANY, wxString::Format("Rows: %d", defaultRows));
    rowsRow->Add(m_rowsLabel, 0, wxALIGN_CENTER_VERTICAL);
    rowsRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    layoutSizer->Add(rowsRow);
    layoutSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxBoxSizer* dirRow = new wxBoxSizer(wxHORIZONTAL);
    dirRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_dir = editor::Direction::LR_TB;
    wxString choices[] = {
        "Left to Right; Top to Bottom",
        "Right to Left; Top to Bottom",
        "Top to Bottom; Left to Right",
        "Top to Bottom; Right to Left"
    };
    m_dirCombo = new wxComboBox(this, ID_DIR_COMBO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 4, choices);
    m_dirCombo->SetSelection(0);
    dirRow->Add(m_dirCombo);
    dirRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxStaticText* dirTxt = new wxStaticText(this, wxID_ANY, "Direction");
    dirRow->Add(dirTxt, 0, wxALIGN_CENTER_VERTICAL);
    dirRow->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    layoutSizer->Add(dirRow);
    layoutSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    
    mainSizer->Add(layoutSizer, 0, wxEXPAND | wxALL, editor::DEFAULT_SPACER_SIZE);
    mainSizer->AddStretchSpacer();

    wxBoxSizer* finishSizer = new wxBoxSizer(wxHORIZONTAL);
    finishSizer->AddStretchSpacer();
    
    wxButton* cancelBtn = new wxButton(this, wxID_CANCEL, "CANCEL");
    finishSizer->Add(cancelBtn);
    finishSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    wxButton* acceptBtn = new wxButton(this, wxID_OK, "CREATE");
    acceptBtn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    finishSizer->Add(acceptBtn);
    finishSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    mainSizer->Add(finishSizer, 0, wxEXPAND | wxHORIZONTAL);
    mainSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    SetSizer(mainSizer);
}

void CompositionDialog::OnColsCtrlEnter(wxCommandEvent& event) {
    m_cols = m_colsCtrl->GetInt();

    int rows = ceil((double)m_files.GetCount() / m_cols);
    m_rowsLabel->SetLabelText(wxString::Format("Rows: %d", rows));
}

void CompositionDialog::OnDirCombo(wxCommandEvent& event) {
    m_dir = (editor::Direction)m_dirCombo->GetSelection();
}

void CompositionDialog::OnOk(wxCommandEvent& event) {
    std::vector<wxBitmap> bitmaps;
    bitmaps.reserve(m_files.GetCount());
    for (size_t i = 0; i < m_files.GetCount(); i++) {
        bitmaps.push_back(wxBitmap(m_files[i]));
    }

    m_compositedImage = editor::compositeImage(bitmaps, m_colorPickerCtrl->GetColour(), m_cols, m_dir);
    event.Skip();
}