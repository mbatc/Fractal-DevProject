#pragma once

#include "flEngine.h"

using namespace Fractal;

namespace TestProject
{
  class FollowTransform : public BehaviourComponent<FollowTransform>
  {
  public:
    SceneEntity targetTransform;

    void OnUpdate()
    {

    }
  };

  class MyCustomPanel : public UI::Panel
  {
  public:
    std::string GetTitle() override
    {
      return "My Custom Panel";
    }

  protected:
    bool OnGUI() override
    {
      clicked |= Widgets::Selectable("This is a whole new button!", false);

      if (clicked)
        Widgets::Label("You clicked me 2!");

      return true;
    }

    bool clicked = false;
  };
}
