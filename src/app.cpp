#include "app.hpp"
#include "mainFrame.hpp"

bool App::OnInit()
{
    MainFrame *frame = new MainFrame();
    frame->DragAcceptFiles(true);
    frame->Show(true);
    return true;
}