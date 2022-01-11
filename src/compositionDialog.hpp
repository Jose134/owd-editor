#pragma once

#include "wxNumberCtrl.hpp"
#include <wx/clrpicker.h>

#include "includes.h"
#include "mainFrame.hpp"

#include <vector>

#include "edits.hpp"

class CompositionDialog: public wxDialog {

public:
    CompositionDialog(wxFrame* parent, const wxArrayString& files);

    wxImage GetCompositedImage() const { return m_compositedImage; };

private:
    void OnOk(wxCommandEvent& event);
    void OnColsCtrlEnter(wxCommandEvent& event);
    void OnDirCombo(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

private:
    int m_cols = 2;
    editor::Direction m_dir = editor::Direction::LR_TB;

    MainFrame* m_mainFrame;
    wxArrayString m_files;
    wxImage m_compositedImage;

    wxNumberCtrl* m_colsCtrl;
    wxStaticText* m_rowsLabel;
    wxColourPickerCtrl* m_colorPickerCtrl;
    wxComboBox* m_dirCombo;

};