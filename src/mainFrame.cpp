#include "mainFrame.hpp"

#include <wx/filename.h>
#include <wx/splitter.h>

#include "compositionDialog.hpp"
#include "imagePanel.hpp"
#include "toolbar.hpp"
#include "edits.hpp"

enum {
    ID_NEW_COMPOSITION = wxID_HIGHEST + 1,
    ID_IMGPANEL = wxID_HIGHEST + 2
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)

EVT_MENU(ID_NEW_COMPOSITION, MainFrame::OnNewComposition)
EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
EVT_MENU(wxID_SAVEAS, MainFrame::OnSaveAs)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)

EVT_DROP_FILES(MainFrame::OnDropFiles)

END_EVENT_TABLE()


MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, editor::WINDOW_TITLE)
{
    //MENU BAR
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_NEW_COMPOSITION, "New Composition");
    menuFile->Append(wxID_OPEN);
    menuFile->Append(wxID_SAVE);
    menuFile->Append(wxID_SAVEAS);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    //STATUS BAR
    CreateStatusBar();

    //LAYOUT
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    /*
    
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
    

    wxFrame* win1 = new wxFrame(splitter, wxID_ANY, "Win1");
    wxBoxSizer* win1Sizer = new wxBoxSizer(wxVERTICAL);
    
    m_historyListView = new wxListView(win1, wxID_ANY);
    m_historyListView->AppendColumn("Edit Name");
    m_historyListView->AppendColumn("Filename");
    win1Sizer->Add(m_historyListView, 1, wxEXPAND);

    win1->SetSizer(win1Sizer);

    wxFrame* win2 = new wxFrame(splitter, wxID_ANY, "Win2");
    wxBoxSizer* win2Sizer = new wxBoxSizer(wxHORIZONTAL);

    m_imgPanel = new ImagePanel(win2, ID_IMGPANEL);
    win2Sizer->Add(m_imgPanel, 1, wxEXPAND);

    win2Sizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_toolbar = new Toolbar(win2, this);
    win2Sizer->Add(m_toolbar, 0, wxEXPAND | wxALL);

    win2Sizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    win2->SetSizer(win2Sizer);

    splitter->Initialize(win1);
    splitter->SplitVertically(win1, win2);
    mainSizer->Add(splitter);

    */


    //mainSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);


    m_imgPanel = new ImagePanel(this, ID_IMGPANEL);
    mainSizer->Add(m_imgPanel, 1, wxEXPAND);

    mainSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);

    m_toolbar = new Toolbar(this, this);
    mainSizer->Add(m_toolbar, 0, wxEXPAND | wxALL);

    mainSizer->AddSpacer(editor::DEFAULT_SPACER_SIZE);
    

    SetSizer(mainSizer);
}

void MainFrame::UpdateStatus () {
    if (!m_imgPanel->GetImage().IsOk()) return;

    SetStatusText(wxString::Format("%dx%d", m_imgPanel->GetImage().GetWidth(), m_imgPanel->GetImage().GetHeight()));
}

void MainFrame::SetOriginalImage(wxImage img) {
    m_imgFilename = "";
    m_imgDir = "";

    m_originalBitmap = wxBitmap(img);
    m_originalImg = img;
    m_imgPanel->SetImage(img);
    m_imgPanel->Autozoom();

    UpdateStatus();
    m_toolbar->PanelImageUpdated();
}

void MainFrame::SetOriginalImage(const wxString& filename, const wxString& dirname) {
    m_imgFilename = filename;
    m_imgDir = dirname;

    m_originalBitmap = wxBitmap(filename, wxBITMAP_TYPE_ANY);
    m_originalImg = wxImage(filename, wxBITMAP_TYPE_ANY);
    m_imgPanel->SetImage(filename);

    UpdateStatus();
    m_toolbar->PanelImageUpdated();
}

void MainFrame::ExportCurrent () {
    if (!m_imgPanel->GetImage().IsOk()) return;

    wxFileName fn(m_imgFilename);
    wxFileDialog saveFileDialog(this, ("Save as"), m_imgDir, fn.GetFullName(), "", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() != wxID_CANCEL) {
        wxString filename = saveFileDialog.GetPath();
        m_imgPanel->GetImage().SaveFile(filename);
    }
}

void MainFrame::OnExit (wxCommandEvent& event) {
    Close(true);
}

void MainFrame::OnNewComposition (wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, ("Open file"), m_imgDir, "", "Image files|*.jpg;*.png;*.bmp;*.gif|All Files|*", wxFD_OPEN|wxFD_MULTIPLE|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() != wxID_CANCEL) {
        wxArrayString filenames;
        openFileDialog.GetPaths(filenames);

        CompositionDialog* compDialog = new CompositionDialog(this, filenames);
        if (compDialog->ShowModal() == wxID_OK) {
            SetOriginalImage(compDialog->GetCompositedImage());

            wxFileName fname(filenames[0]);
            m_imgFilename = fname.GetPathWithSep() + "Composition.png";
            m_imgDir = fname.GetPathWithSep();
        }
    }
}

void MainFrame::OnOpen (wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, ("Open file"), m_imgDir, "", "Image files|*.jpg;*.png;*.bmp;*.gif|All Files|*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() != wxID_CANCEL) {
        wxString filenameFromRoot = wxString::Format("%s/%s", openFileDialog.GetDirectory(), openFileDialog.GetFilename());
        SetOriginalImage(openFileDialog.GetPath(), openFileDialog.GetDirectory());
    }
}

void MainFrame::OnSaveAs (wxCommandEvent& event) {
    ExportCurrent();
}

void MainFrame::OnAbout (wxCommandEvent& event) {
    wxMessageBox("This is a program made by @onewaifuaday for editing the pictures of his instagram posts in a fast way.",
                 "About onewaifuaday image editor", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnDropFiles (wxDropFilesEvent& event) {
    if (event.GetNumberOfFiles() > 0) {
        if (event.GetNumberOfFiles() > 1) {
            wxMessageDialog* makeComposition = new wxMessageDialog(this, "Do you want to make a composition out of these images?", "Confirm", wxYES_NO);
            if (makeComposition->ShowModal() == wxID_YES) {
                wxArrayString filenames(event.GetNumberOfFiles(), event.GetFiles());
                
                CompositionDialog* compDialog = new CompositionDialog(this, filenames);
                if (compDialog->ShowModal() == wxID_OK) {
                    SetOriginalImage(compDialog->GetCompositedImage());

                    wxFileName fname(event.GetFiles()[0]);
                    m_imgFilename = fname.GetPathWithSep() + "Composition.png";
                    m_imgDir = fname.GetPathWithSep();
                }
            }
        }
        else {
            wxFileName fname(event.GetFiles()[0]);
            SetOriginalImage(fname.GetFullPath(), fname.GetPathWithSep());
        }
    }
}