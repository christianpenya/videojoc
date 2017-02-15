
#include "PhysxManager.h"

#include <PxPhysicsAPI.h>

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;


#pragma comment(lib, "PhysX3DEBUG_x64.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x64.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysXProfileSDKDEBUG.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib")
#pragma comment(lib, "PhysX3CharacterKinematicDEBUG_x64.lib")
#pragma comment(lib, "PhysX3CookingDEBUG_x64.lib")

#if USE_PHYSX_DEBUG
#define					PVD_HOST			"127.0.0.1"
#endif

#ifdef CHECKED_RELEASE
#undef CHECKED_RELEASE
#endif
#define CHECKED_RELEASE(x) if(x!=nullptr) {x->release(); x=nullptr;}


inline physx::PxVec3 CastVec(const Vect3f& v)
{
    return physx::PxVec3(v.x, v.y, v.z);
}
inline Vect3f CastVec(const physx::PxVec3& v)
{
    return Vect3f(v.x, v.y, v.z);
}
inline Vect3f CastVec(const physx::PxExtendedVec3& v)
{
    return Vect3f((float)v.x, (float)v.y, (float)v.z);
}

inline physx::PxQuat CastQuat(const Quatf& q)
{
    return physx::PxQuat(q.x, q.y, q.z, q.w);
}
inline Quatf CastQuat(const physx::PxQuat& q)
{
    return Quatf(q.x, q.y, q.z, q.w);
}

