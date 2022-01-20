#include "app.hpp"
#include "mainFrame.hpp"

bool App::OnInit()
{
    wxInitAllImageHandlers();

    MainFrame *frame = new MainFrame();
    frame->DragAcceptFiles(true);
    frame->Show(true);
    return true;
}