#pragma once

#include "includes.h"

wxDECLARE_EVENT(EVT_IMGPANEL_ZOOM, wxCommandEvent);

class ImagePanel : public wxPanel {
public:
    //Constructors
    ImagePanel(wxFrame* parent, wxWindowID winid = wxID_ANY, wxString file=wxEmptyString, wxBitmapType format=wxBITMAP_TYPE_ANY);

    //Image setters and getters
    void SetImage(wxImage image);
    void SetImage(wxString filename);

    wxImage& GetImage() { return this->m_image; }
    const wxImage& GetImage() const { return this->m_image; }

    bool GetDisplayCenter() const { return m_displayCenter; }
    void SetDisplayCenter(bool displayCenter);

    wxColour GetMeanColor() const { return m_meanColor; }

    //Features
    double GetZoomLevel();
    void Autozoom();
    void ZoomOut();
    void ZoomIn();

private:
    void clamp(int &val, int min, int max);

    //Painting
    void paintNow();
    void render(wxDC& dc);

    void CalculateMeanColor();

    //Events
    void OnPaintEvent(wxPaintEvent& event);
    void OnMouseMoved(wxMouseEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseWheelMoved(wxMouseEvent& event);
    void OnMouseReleased(wxMouseEvent& event);
    void OnRightClick(wxMouseEvent& event);
    void OnMouseLeftWindow(wxMouseEvent& event);
    void OnKeyPressed(wxKeyEvent& event);
    void OnKeyReleased(wxKeyEvent& event);

    DECLARE_EVENT_TABLE()

private:
    wxImage m_image;
    wxBitmap m_bitmap;
    wxColour m_meanColor;

    bool m_mousedown;
    wxPoint m_initialMousePos;
    int m_dx, m_dy;
    int m_prevDx, m_prevDy;
    
    double m_zoom;

    bool m_renderGrid;
    double m_gridThreshold;

    bool m_displayCenter;

};