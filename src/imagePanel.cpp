#include "imagePanel.hpp"

wxDEFINE_EVENT(EVT_IMGPANEL_ZOOM, wxCommandEvent);

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)

EVT_PAINT(ImagePanel::OnPaintEvent)

EVT_MOTION(ImagePanel::OnMouseMoved)
EVT_LEFT_DOWN(ImagePanel::OnMouseDown)
EVT_LEFT_UP(ImagePanel::OnMouseReleased)
EVT_RIGHT_DOWN(ImagePanel::OnRightClick)
EVT_LEAVE_WINDOW(ImagePanel::OnMouseLeftWindow)
EVT_KEY_DOWN(ImagePanel::OnKeyPressed)
EVT_KEY_UP(ImagePanel::OnKeyReleased)
EVT_MOUSEWHEEL(ImagePanel::OnMouseWheelMoved)

END_EVENT_TABLE()


ImagePanel::ImagePanel (wxFrame* parent, wxWindowID winid, wxString file, wxBitmapType format)
    : wxPanel(parent, winid) {

    m_mousedown = false;
    m_initialMousePos = wxPoint(0,0);
    m_dx = 0;
    m_dy = 0;
    m_prevDx = 0;
    m_prevDy = 0;
    m_zoom = 1.0;

    m_renderGrid = true;
    m_gridThreshold = 10.0;

    m_displayCenter = false;
    m_meanColor = wxColour(0,0,0);

    if (file != wxEmptyString) {
        m_image.LoadFile(file, format);
        paintNow();
    }
}

void ImagePanel::SetImage (wxImage image) {
    this->m_image = image;
    this->m_bitmap = wxBitmap(image);
    paintNow();
}

void ImagePanel::SetImage (wxString filename) {
    m_image.LoadFile(filename);
    m_bitmap = wxBitmap(filename);
    CalculateMeanColor();
    Autozoom();
}

void ImagePanel::SetDisplayCenter(bool displayCenter) {
    m_displayCenter = displayCenter;
    paintNow();    
}

double ImagePanel::GetZoomLevel () {
    return m_zoom;
}

void ImagePanel::ZoomOut () {
    m_zoom -= editor::ZOOM_FACTOR / m_image.GetWidth();
    if (m_zoom < editor::MIN_ZOOM_LEVEL) { m_zoom = editor::MIN_ZOOM_LEVEL; }    

    paintNow();

    wxCommandEvent event(EVT_IMGPANEL_ZOOM);
    event.SetString(std::to_string(m_zoom));
    wxPostEvent(this, event);
}

void ImagePanel::ZoomIn () {
    m_zoom += editor::ZOOM_FACTOR / m_image.GetHeight();
    paintNow();

    wxCommandEvent event(EVT_IMGPANEL_ZOOM);
    event.SetString(std::to_string(m_zoom));
    wxPostEvent(this, event);
}

void ImagePanel::Autozoom () {
    if (!m_image.IsOk()) return;

    wxSize panelSize = this->GetSize();

    double imageRatio = (double)m_image.GetWidth() / m_image.GetHeight();
    double panelRatio = (double)panelSize.GetWidth() / panelSize.GetHeight();

    if (imageRatio > panelRatio) {
        m_zoom = (double)panelSize.GetWidth() / m_image.GetWidth();
    }
    else {
        m_zoom = (double)panelSize.GetHeight() / m_image.GetHeight();
    }

    paintNow();

    wxCommandEvent event(EVT_IMGPANEL_ZOOM);
    event.SetString(std::to_string(m_zoom));
    wxPostEvent(this, event);
}

void ImagePanel::CalculateMeanColor () {
    if (!m_image.IsOk()) return;

    unsigned char *data = m_image.GetData();
    long r_sum = 0, g_sum = 0, b_sum = 0;

    size_t pixelCount = m_image.GetWidth() * m_image.GetHeight();
    size_t dataSize = pixelCount * 3;
    for (size_t i = 0; i < dataSize; i += 3) {
        r_sum += data[i];
        g_sum += data[i+1];
        b_sum += data[i+2];
    }

    m_meanColor = wxColour(r_sum / pixelCount, g_sum / pixelCount, b_sum / pixelCount);
}

void ImagePanel::paintNow () {
    wxClientDC dc(this);
    render(dc);
}

void ImagePanel::render (wxDC& dc) {
    if (!m_image.IsOk()) return;

    dc.SetUserScale(m_zoom, m_zoom);

    if (m_image.GetWidth() * m_zoom <= this->GetSize().GetWidth()) {
        m_dx = this->GetSize().GetWidth() - m_image.GetWidth() * m_zoom;
        m_dx /= 2 * m_zoom;
        m_prevDx = m_dx;
    }

    if (m_image.GetHeight() * m_zoom <= this->GetSize().GetHeight()) {
        m_dy = this->GetSize().GetHeight() - m_image.GetHeight() * m_zoom;
        m_dy /= 2 * m_zoom;
        m_prevDy = m_dy;
    }

    wxBrush bgBrush(editor::CANVAS_BACKGROUND);
    dc.SetBackground(bgBrush);
    dc.Clear();


    dc.DrawBitmap(m_bitmap, m_dx, m_dy, false);

    if (m_renderGrid && m_zoom > m_gridThreshold) {
        wxClientDC cdc(this);
        wxPen pen(wxColour(255, 255, 255), 1, wxPENSTYLE_SOLID);
        cdc.SetPen(pen);

        for (int i = 0; i < m_image.GetWidth(); i++) {
            int x1 = (i + m_dx) * m_zoom;
            int y1 = (m_dy) * m_zoom;
            int x2 = (i + m_dx) * m_zoom;
            int y2 = (m_dy + m_image.GetHeight()) * m_zoom;

            cdc.DrawLine(x1, y1, x2, y2);
        }

        for (int i = 0; i < m_image.GetHeight(); i++) {
            int x1 = (m_dx) * m_zoom;
            int y1 = (i + m_dy) * m_zoom;
            int x2 = (m_dx + m_image.GetWidth()) * m_zoom;
            int y2 = (i + m_dy) * m_zoom;

            cdc.DrawLine(x1, y1, x2, y2);
        }
    }

    if (m_displayCenter) {
        wxClientDC cdc(this);
        wxPen pen(wxColour(0, 255, 255), 1, wxPENSTYLE_SOLID);
        cdc.SetPen(pen);

        int offsetX = m_dx * m_zoom + (m_image.GetWidth() * m_zoom) / 2;
        cdc.DrawLine(offsetX, 0, offsetX, GetSize().GetHeight());

        int offsetY = m_dy * m_zoom + (m_image.GetHeight() * m_zoom) / 2;
        cdc.DrawLine(0, offsetY, GetSize().GetWidth(), offsetY);
    }

}

void ImagePanel::OnPaintEvent (wxPaintEvent& event) {
    paintNow();
}

void ImagePanel::OnMouseMoved (wxMouseEvent& event) {
    if (!m_image.IsOk()) return;

    if (m_mousedown) {
        wxSize panelSize = this->GetSize();
        wxPoint curPosition = event.GetPosition();

        m_dx = (curPosition.x - m_initialMousePos.x) / m_zoom + m_prevDx;
        m_dy = (curPosition.y - m_initialMousePos.y) / m_zoom + m_prevDy;

        int minDx = -std::abs(m_image.GetWidth() - panelSize.GetWidth() / m_zoom);
        int minDy = -std::abs(m_image.GetHeight() - panelSize.GetHeight() / m_zoom);

        clamp(m_dx, minDx, 0);
        clamp(m_dy, minDy, 0);

        paintNow();
    }
}

void ImagePanel::OnMouseDown (wxMouseEvent& event) {
    m_mousedown = true;
    m_initialMousePos = event.GetPosition();
    this->SetFocus();
}

void ImagePanel::OnMouseReleased (wxMouseEvent& event) {
    m_mousedown = false;
    m_prevDx = m_dx;
    m_prevDy = m_dy;
}

void ImagePanel::OnMouseWheelMoved (wxMouseEvent& event) {
    if (!m_image.IsOk()) return;

    int delta = event.GetWheelRotation();
    if (delta > 0) ZoomIn();
    else ZoomOut();
}

void ImagePanel::OnRightClick (wxMouseEvent& event) {}
void ImagePanel::OnMouseLeftWindow (wxMouseEvent& event) {}
void ImagePanel::OnKeyPressed (wxKeyEvent& event) {}

void ImagePanel::OnKeyReleased (wxKeyEvent& event) {
    if (!m_image.IsOk()) return;

    if (event.GetKeyCode() == WXK_SPACE) {
        Autozoom();
    }
}

void ImagePanel::clamp (int &val, int min, int max) {
    if (val < min) {
        val = min;
    }
    else if (val > max) {
        val = max;
    }
}