#include "PreCompile.h"
#include "GameEngineTransform.h"

bool GameEngineTransform::SphereToSpherer(const GameEngineTransform& _sphereA, const GameEngineTransform& _sphereB)
{
	return _sphereA.collisionDataObject_.SPHERE_.Intersects(_sphereB.collisionDataObject_.SPHERE_);
}

bool GameEngineTransform::AABBToAABB(const GameEngineTransform& _boxA, const GameEngineTransform& _boxB)
{
	return _boxA.collisionDataObject_.AABB_.Intersects(_boxB.collisionDataObject_.AABB_);
}

bool GameEngineTransform::OBBToOBB(const GameEngineTransform& _boxA, const GameEngineTransform& _boxB)
{
	return _boxA.collisionDataObject_.OBB_.Intersects(_boxB.collisionDataObject_.OBB_);
}

void GameEngineTransform::CollisionScaleSetting()
{
	collisionDataObject_.OBB_.Extents = (data_.worldScaleVector_ * 0.5f);
}

void GameEngineTransform::CollisionRotationSetting()
{
	collisionDataObject_.OBB_.Orientation = data_.worldRotationVector_.DegreeRotationToQuarternionReturn();
}

void GameEngineTransform::CollisionPositionSetting()
{
	collisionDataObject_.OBB_.Center = data_.worldPositionVector_;
}

void GameEngineTransform::CollisionDataSetting()
{
	CollisionScaleSetting();
	CollisionRotationSetting();
	CollisionPositionSetting();
}
