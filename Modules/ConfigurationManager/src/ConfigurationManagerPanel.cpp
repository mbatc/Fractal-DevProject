#include "ConfigurationManagerPanel.h"

namespace Fractal
{
  std::string ConfigName(std::string const &name) { return name.empty() ? "[ ALL ]" : name; }

  void Input(std::string *pValue)
  {
    Widgets::Input("", pValue);
  }

  void Input(std::filesystem::path *pValue)
  {
    std::string path = pValue->string();
    Input(&path);
    *pValue = path;
  }

  template<typename T>
  bool ListInput(std::string name, std::vector<T> &list)
  {
    if (Widgets::CollapsingHeader(name.c_str()))
    {
      Widgets::PushID(name.c_str());
      Widgets::BeginChild("ListFrame", 0, 0.3f, true);
      for (int64_t i = 0; i < (int64_t)list.size(); ++i)
      {
        Widgets::PushID(i);
        Input(&list[i]);
        Widgets::PopID();
      }
      Widgets::EndChild();
      if (Widgets::Button("Add"))
        list.push_back("");
      if (list.size() > 0)
      {
        Widgets::SameLine();
        if (Widgets::Button("Remove"))
          list.pop_back();
      }
      Widgets::PopID();
    }
    return true;
  }

  bool BuildConfigurationPanel::OnGUI()
  {
    ScriptManager *pScripts = Fractal_GetApplication()->GetModule<ScriptManager>();
    BuildTool *pTool = pScripts->GetBuildTool();

    if (Widgets::BeginDropdownBox("Module", SelectedModuleTitle(pTool).c_str()))
    {
      for (int64_t i = 0; i < (int64_t)pTool->modules.size(); ++i)
        if (Widgets::Selectable(pTool->modules[i].name.c_str(), m_selectedModule == i))
          m_selectedModule = i;

      if (Widgets::Selectable(" + Add New Module", false))
      { // TODO: Open add module modal dialog
      }

      Widgets::EndDropdownBox();
    }

    Widgets::Separator();

    if (m_selectedModule >= 0 && m_selectedModule < (int64_t)pTool->modules.size())
    {
      BuildTool::Module &module = pTool->modules[m_selectedModule];

      Widgets::BeginChild("ModuleConfigFrame", 0, 0, false);

      ListInput("Files", module.files);

      Widgets::Separator();

      if (Widgets::BeginDropdownBox("Configuration", ConfigName(m_selectConfig).c_str()))
      {
        for (auto &[name, config] : module.configurations)
          if (Widgets::Selectable(ConfigName(m_selectConfig).c_str(), name == m_selectConfig))
            m_selectConfig = name;

        if (Widgets::Selectable(" + Add New Configuration", false))
        { // TODO: Open add configuration modal dialog

        }

        Widgets::EndDropdownBox();
      }

      Widgets::BeginChild("ConfigurationFrame", 0, 0, true);
      BuildConfiguration &config = module.configurations[m_selectConfig];
      ListInput("Include Directories", config.includeDirectories);
      ListInput("Library Directories", config.libraryDirectories);
      ListInput("Links", config.links);
      ListInput("Defines", config.defines);
      ListInput("Flags", config.flags);
      Widgets::EndChild();

      Widgets::EndChild();
    }

    return true;
  }

  std::string BuildConfigurationPanel::GetTitle()
  {
    return "Build Configuration Manager";
  }

  std::string BuildConfigurationPanel::SelectedModuleTitle(BuildTool *pTool) const
  {
    return m_selectedModule < 0 || m_selectedModule >= (int64_t)pTool->modules.size() ? "[ None ]" : pTool->modules[m_selectedModule].name;
  }
}
