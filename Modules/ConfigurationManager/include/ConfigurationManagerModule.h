#pragma once

#include "flEngine.h"

namespace Fractal
{
  class ConfigurationManagerModule : public Module
  {
    FRACTAL_DEFAULT_INTERFACE(ConfigurationManagerModule, Module)

  public:
    bool OnInit() override;
    void OnShutdown() override;

  private:
    void OpenBuildConfigurationPanel();
  };
}