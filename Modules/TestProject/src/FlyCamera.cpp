#include "include/FlyCamera.h"
#include "include/FollowTransform.h"

namespace TestProject
{
  flREGISTER_COMPONENT(FollowTransform, "Follow Transform");
  flREGISTER_COMPONENT(FlyCamera,       "Fly Camera");

  flREGISTER_PANEL(MyCustomPanel);

  void OpenMyPanel()
  {
    IUIManager *pManager = Fractal_GetApplication()->GetModule<IUIManager>();
    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view
    pEditorView->OpenOrAdd<TestProject::MyCustomPanel>();
  }
}

extern "C" {
  __declspec(dllexport) void Fractal_Plugin_Initialize()
  {
    flInfo("Initializing my plugin");

    IUIManager *pManager = Fractal_GetApplication()->GetModule<IUIManager>();
    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view
    pEditorView->AddMenuItem("Window/My Custom Panel", MakeTask([]() { TestProject::OpenMyPanel(); return 0; }));
  }
}
