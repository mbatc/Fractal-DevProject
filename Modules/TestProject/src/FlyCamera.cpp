#include "include/FlyCamera.h"
#include "include/FollowTransform.h"

namespace TestProject
{
  flREGISTER_COMPONENT_STATIC(FollowTransform, "Follow Transform");
  flREGISTER_COMPONENT_STATIC(FlyCamera,       "Fly Camera");

  flREGISTER_PANEL_STATIC(MyCustomPanel);

  void OpenMyPanel()
  {
    UIManager *pManager = Fractal_GetApplication()->GetModule<UIManager>();
    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view
    pEditorView->OpenOrAdd<TestProject::MyCustomPanel>();
  }
}

extern "C" {
  __declspec(dllexport) void Fractal_Plugin_Initialize()
  {
    flInfo("Initializing my plugin");

    UIManager *pManager = Fractal_GetApplication()->GetModule<UIManager>();
    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view
    pEditorView->AddMenuItem("Window/My Custom Panel", MakeTask([]() { TestProject::OpenMyPanel(); return 0; }));
  }
}
