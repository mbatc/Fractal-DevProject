#include "ConfigurationManagerPanel.h"

using namespace Fractal;

namespace Fractal
{
  namespace UI
  {
    flREGISTER_PANEL(BuildConfigurationPanel);
  }

  void OpenBuildConfigurationPanel()
  {
    IUIManager *pManager = Fractal_GetApplication()->GetModule<IUIManager>();
    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view
    pEditorView->OpenOrAdd<UI::BuildConfigurationPanel>();
  }
}

extern "C" {
  __declspec(dllexport) void Fractal_Plugin_Initialize()
  {
    flInfo("Initializing my plugin");

    IUIManager * pManager = Fractal_GetApplication()->GetModule<IUIManager>();
    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view
    pEditorView->AddMenuItem("Window/Build Configuration", MakeTask([]() { Fractal::OpenBuildConfigurationPanel(); return 0; }));
  }
}
