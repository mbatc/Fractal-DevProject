#pragma once

#include "flEngine.h"

namespace Fractal
{
  class BuildConfigurationPanel : public UI::Panel
  {
  public:
    bool OnGUI() override;

    std::string GetTitle() override;

  private:
    std::string SelectedModuleTitle(BuildTool *pTool) const;

    int64_t m_selectedModule = -1;
    std::string m_selectConfig = "";
  };
}
