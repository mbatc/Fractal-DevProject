#pragma once

#include "flEngine.h"

#include "btBulletDynamicsCommon.h"

namespace Fractal
{
  struct BoxCollider
  {
    Vec3D dimensions = { 1, 1, 1 };

    std::shared_ptr<btBoxShape> pShape = nullptr;

  };

  void Serialize(flOUT SerializedObject &dom, BoxCollider const &o);
  void Deserialize(flOUT SerializedObject const & dom, BoxCollider &o);

  struct SphereCollider
  {
    double radius = 1;

    std::shared_ptr<btSphereShape> pShape = nullptr;
  };

  void Serialize(flOUT SerializedObject &dom, SphereCollider const &o);
  void Deserialize(flOUT SerializedObject const &dom, SphereCollider &o);

  struct RigidbodyComponent
  {
    double mass = 0;

    std::shared_ptr<btCollisionShape> pShape = nullptr;
    std::shared_ptr<btRigidBody>      pBody  = nullptr;
  };

  void Serialize(flOUT SerializedObject &dom, RigidbodyComponent const &o);
  void Deserialize(flOUT SerializedObject const &dom, RigidbodyComponent &o);

  class BulletPhysicsSceneExtension : public SceneExtension
  {
  public:
    ~BulletPhysicsSceneExtension();

    virtual UpdatePolicy GetUpdatePolicy() const override;

    virtual void OnCreate();
    
    virtual void OnPreUpdate();
    virtual void OnUpdate();

  private:
    void CreateCollisionShape(SceneEntity entity, RigidbodyComponent *pRigidbody);

    btDefaultCollisionConfiguration *m_pCollisionConfiguration = nullptr;
    btCollisionDispatcher *m_pDispatcher = nullptr;
    btBroadphaseInterface *m_pPairCache = nullptr;
    btSequentialImpulseConstraintSolver *m_pSolver = nullptr;
    btDiscreteDynamicsWorld *m_pWorld = nullptr;
  };

  class BulletPhysicsModule : public Module
  {
    FRACTAL_DEFAULT_INTERFACE(BulletPhysicsModule, Module);
  public:
    virtual bool OnInit() override;

    virtual void OnShutdown() override;
  };
}
