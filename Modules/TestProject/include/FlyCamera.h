#include "flEngine.h"

using namespace Fractal;

namespace TestProject
{
  class FlyCamera : public Fractal::BehaviourComponent<FlyCamera>
  {
  public:
    float speed = 10;

    virtual void OnCreate()
    {
      if (!HasComponent<InputComponent>())
        AddComponent<InputComponent>();
    }

    virtual void OnUpdate()
    {
      if (auto pInput = TryGetComponent<Fractal::InputComponent>())
      {
        Vec3D move(0);
        move.z -= speed * pInput->GetInput("forward");
        move.z += speed * pInput->GetInput("back");
        move.x -= speed * pInput->GetInput("left");
        move.x += speed * pInput->GetInput("right");
        move.y += speed * pInput->GetInput("up");
        move.y -= speed * pInput->GetInput("down");

        Vec3D pitchYawRoll(0);
        pitchYawRoll.y += pInput->GetInput("yaw");
        pitchYawRoll.x += pInput->GetInput("pitch");

        float mult = 1 + pInput->GetInput("fast") - pInput->GetInput("slow") * 0.5f;

        if (auto pTransform = TryGetTransform())
        {
          pTransform->worldTransform.position += pTransform->worldTransform.orientation * move;
          pTransform->worldTransform.orientation *= QuatD(pitchYawRoll);

          pTransform->RecalculateLocalTransform();
        }
      }
    }
  };
}

namespace Fractal
{
  void Serialize(flOUT SerializedObject & dom, TestProject::FlyCamera const &o)
  {
    dom["speed"] << o.speed;
  }

  void Deserialize(flOUT SerializedObject const & dom, TestProject::FlyCamera &o)
  {
    dom["speed"] >> o.speed;
  }
}
