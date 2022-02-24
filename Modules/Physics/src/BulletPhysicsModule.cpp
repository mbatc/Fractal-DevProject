#include "BulletPhysicsModule.h"

namespace Fractal
{
  flREGISTER_MODULE_STATIC(BulletPhysicsModule);

  static btVector3 ToBullet(Vec3D vec)
  {
    return { (btScalar)vec.x, (btScalar)vec.y, (btScalar)vec.z };
  }

  static btVector4 ToBullet(Vec4D vec)
  {
    return { (btScalar)vec.x, (btScalar)vec.y, (btScalar)vec.z, (btScalar)vec.w };
  }

  static btQuaternion ToBullet(QuatD ori)
  {
    return { (btScalar)ori.x, (btScalar)ori.y, (btScalar)ori.z, (btScalar)ori.w };
  }

  static Vec3D FromBullet(btVector3 vec)
  {
    return { (double)vec.x(), (double)vec.y(), (double)vec.z() };
  }

  static Vec4D FromBullet(btVector4 vec)
  {
    return { (double)vec.x(), (double)vec.y(), (double)vec.z(), (double)vec.w() };
  }

  static QuatD FromBullet(btQuaternion ori)
  {
    return { (double)ori.x(), (double)ori.y(), (double)ori.z(), (double)ori.w() };
  }

  BulletPhysicsSceneExtension::~BulletPhysicsSceneExtension()
  {
    flDelete(m_pWorld);
  }

  UpdatePolicy BulletPhysicsSceneExtension::GetUpdatePolicy() const
  {
    return UpdatePolicy_Game;
  }

  void BulletPhysicsSceneExtension::OnCreate()
  {
    m_pCollisionConfiguration = flNew btDefaultCollisionConfiguration();
    m_pDispatcher = flNew btCollisionDispatcher(m_pCollisionConfiguration);
    m_pPairCache = flNew btDbvtBroadphase();
    m_pSolver = flNew btSequentialImpulseConstraintSolver;
    m_pWorld = flNew btDiscreteDynamicsWorld(m_pDispatcher, m_pPairCache, m_pSolver, m_pCollisionConfiguration);
  }

  void BulletPhysicsSceneExtension::OnPreUpdate()
  {
    GetScene()->ForEach<TransformComponent, RigidbodyComponent>(
      [=](SceneEntity entity)
      {
        RigidbodyComponent &rigidbody = entity.GetComponent<RigidbodyComponent>();
        if (rigidbody.pBody == nullptr)
          CreateCollisionShape(entity, &rigidbody);

        btVector3    pos = rigidbody.pBody->getWorldTransform().getOrigin();
        btQuaternion ori = rigidbody.pBody->getWorldTransform().getRotation();

        TransformComponent &transform = entity.GetTransform();
        transform.worldTransform.position    = FromBullet(pos);
        transform.worldTransform.orientation = FromBullet(ori);
        transform.RecalculateLocalTransform();
      }
    );

    m_pWorld->stepSimulation(1.0 / 60);
    m_pWorld->applyGravity();
  }

  void BulletPhysicsSceneExtension::OnUpdate()
  {
  }

  void BulletPhysicsSceneExtension::CreateCollisionShape(SceneEntity entity, RigidbodyComponent *pRigidbody)
  {
    if (BoxCollider *pBox = entity.TryGetComponent<BoxCollider>())
    {
      if (pBox->pShape == nullptr)
        pRigidbody->pShape = pBox->pShape = std::make_shared<btBoxShape>(ToBullet(pBox->dimensions / 2.0));
    }

    if (SphereCollider *pSphere = entity.TryGetComponent<SphereCollider>())
    {
      if (pSphere->pShape == nullptr)
        pRigidbody->pShape = pSphere->pShape = std::make_shared<btSphereShape>((btScalar)pSphere->radius);
    }

    pRigidbody->pBody = std::make_shared<btRigidBody>((btScalar)pRigidbody->mass, nullptr, pRigidbody->pShape.get());

    TransformComponent *pTransform = entity.TryGetTransform();
    pTransform->RecalculateWorldTransform();

    btTransform transform;
    transform.setRotation(ToBullet(pTransform->worldTransform.orientation));
    transform.setOrigin(ToBullet(pTransform->worldTransform.position));

    pRigidbody->pShape->setLocalScaling(ToBullet(pTransform->worldTransform.scale));
    pRigidbody->pBody->setWorldTransform(transform);
    pRigidbody->pBody->setGravity({ 0, -10, 0 });

    m_pWorld->addRigidBody(pRigidbody->pBody.get());
  }

  bool BulletPhysicsModule::OnInit()
  {
    flREGISTER_COMPONENT(BoxCollider, "Box Collider");
    flREGISTER_COMPONENT(SphereCollider, "Sphere Collider");
    flREGISTER_COMPONENT(RigidbodyComponent, "Rigidbody");

    flREGISTER_SCENE_EXTENSION(BulletPhysicsSceneExtension);

    return true;
  }

  void BulletPhysicsModule::OnShutdown()
  {
  }

  void Serialize(flOUT SerializedObject &dom, BoxCollider const &o)
  {
    dom["size"] << o.dimensions;
  }

  void Deserialize(flOUT SerializedObject const &dom, BoxCollider &o)
  {
    dom["size"] >> o.dimensions;
  }

  void Serialize(flOUT SerializedObject &dom, SphereCollider const &o)
  {
    dom["radius"] << o.radius;
  }

  void Deserialize(flOUT SerializedObject const &dom, SphereCollider &o)
  {
    dom["radius"] >> o.radius;
  }

  void Serialize(flOUT SerializedObject &dom, RigidbodyComponent const &o)
  {
    dom["mass"] << o.mass;
  }

  void Deserialize(flOUT SerializedObject const &dom, RigidbodyComponent &o)
  {
    dom["mass"] >> o.mass;
  }
}
