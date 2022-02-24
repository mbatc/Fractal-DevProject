#include "ConfigurationManagerModule.h"
#include "ConfigurationManagerPanel.h"

namespace Fractal
{
  flREGISTER_MODULE_STATIC(ConfigurationManagerModule);

  bool ConfigurationManagerModule::OnInit()
  {
    flREGISTER_PANEL(BuildConfigurationPanel);

    UIManager *pManager = GetModule<UIManager>();

    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view

    pEditorView->AddMenuItem("Window/Build Configuration", MakeTask([=]() {
      OpenBuildConfigurationPanel();
      return 0;
      }
    ));

    return true;
  }

  void ConfigurationManagerModule::OnShutdown()
  {
    UIManager *pManager = GetModule<UIManager>();

    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view

    pEditorView->RemoveMenuItem("Window/Build Configuration");
  }

  void ConfigurationManagerModule::OpenBuildConfigurationPanel()
  {
    UIManager *pManager = GetModule<UIManager>();
    
    UI::IView *pEditorView = pManager->GetView("Editor"); // Get the editor view

    pEditorView->OpenOrAdd<BuildConfigurationPanel>();
  }
}
