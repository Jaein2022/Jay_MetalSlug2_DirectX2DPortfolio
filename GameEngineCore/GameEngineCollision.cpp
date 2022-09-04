#include "PreCompile.h"
#include "GameEngineCollision.h"
#include "GameEngineCoreDebug.h"
#include "GameEngineCore.h"

bool (*GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionType::CT_Max)][static_cast<int>(CollisionType::CT_Max)])
(const GameEngineTransform& _transformA, const GameEngineTransform& _transformB);

class GameEngineCollisionFunctionInit
{
	//이 클래스의 존재 이유: 
public:
	GameEngineCollisionFunctionInit()
	{
		memset(
			GameEngineCollision::collisionFunctions_,
			0,
			sizeof(GameEngineCollision::collisionFunctions_));

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionType::CT_Sphere)][static_cast<int>(CollisionType::CT_Sphere)]
			= &GameEngineTransform::SphereToSphere;

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionType::CT_AABB)][static_cast<int>(CollisionType::CT_AABB)]
			= &GameEngineTransform::AABBToAABB;

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionType::CT_OBB)][static_cast<int>(CollisionType::CT_OBB)]
			= &GameEngineTransform::OBBToOBB;



		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionType::CT_Sphere2D)][static_cast<int>(CollisionType::CT_Sphere2D)]
			= &GameEngineTransform::Sphere2DToSphere2D;

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionType::CT_AABB2D)][static_cast<int>(CollisionType::CT_AABB2D)]
			= &GameEngineTransform::AABB2DToAABB2D;

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionType::CT_OBB2D)][static_cast<int>(CollisionType::CT_OBB2D)]
			= &GameEngineTransform::OBB2DToOBB2D;

		//추가 함수가 필요하다면 그때그때 만들어서 추가할 것.
	}

	~GameEngineCollisionFunctionInit()
	{
	}
};

GameEngineCollisionFunctionInit inst_;

GameEngineCollision::GameEngineCollision() 
	: debugType_(CollisionType::CT_Max),
	color_(1.f, 0.f, 0.f, 0.5f),
	debugCameraOrder_(CameraOrder::MainCamera)	//기본 디버그카메라 세팅: 메인카메라.
	//메인카메라는 엔진 기본제공 카메라이므로 엔진 수준에서 이런 편의기능을 제공할 수 있다.
{
}

GameEngineCollision::~GameEngineCollision()
{
}

void GameEngineCollision::ChangeOrder(int _collisionOrder)
{
	this->GetActor()->GetLevel()->PushCollision(this, _collisionOrder);
}

bool GameEngineCollision::IsCollision(
	CollisionType _thisType,
	int _collisionOrder,
	CollisionType _otherType,
	std::function<bool(GameEngineCollision* _this, GameEngineCollision* _other)> _function /*= nullptr*/
)
{
	if (false == this->IsUpdate())
	{
		return false;
	}

	int thisType = static_cast<int>(_thisType);
	int otherType = static_cast<int>(_otherType);

	if (nullptr == GameEngineCollision::collisionFunctions_[thisType][otherType])
	{
		MsgBoxAssert("아직 준비되지 않은 충돌 함수입니다.");
		return false;
	}

	std::map<int, std::list<GameEngineCollision*>>& allCollisions
		= this->GetActor()->GetLevel()->allCollisions_;

	std::list<GameEngineCollision*>& collisionGroup = allCollisions[_collisionOrder];

	for (GameEngineCollision* collision : collisionGroup)
	{
		if (false == collision->IsUpdate())
		{
			continue;
		}

		if (true == GameEngineCollision::collisionFunctions_[thisType][otherType](this->GetTransform(), collision->GetTransform()))
		{
			if (nullptr != _function)
			{
				if (true == _function(this, collision))	//연결된 충돌 함수가 true를 반환하면 충돌체크를 한번만 한다.
														//연결된 충돌 함수가 false를 반환하면 충돌체크를 계속 한다.
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

void GameEngineCollision::DebugRender()
{
	GameEngineCamera* debugCamera
		= this->GetActor()->GetLevel()->cameras_[static_cast<UINT>(debugCameraOrder_)];

	switch (this->debugType_)
	{
		//case CollisionType::CT_Point2D:
		//	break;

	case CollisionType::CT_Sphere2D:
		GameEngineDebug::DrawSphere(GetTransform(), debugCamera, color_);
		break;

	case CollisionType::CT_AABB2D:
		GameEngineDebug::DrawBox(GetTransform(), debugCamera, color_);
		break;

	case CollisionType::CT_OBB2D:
		GameEngineDebug::DrawBox(GetTransform(), debugCamera, color_);
		break;



		//case CollisionType::CT_Point:
		//	break;

	case CollisionType::CT_Sphere:
		GameEngineDebug::DrawSphere(GetTransform(), debugCamera, color_);
		break;

	case CollisionType::CT_AABB:
		GameEngineDebug::DrawBox(GetTransform(), debugCamera, color_);
		break;

	case CollisionType::CT_OBB:
		GameEngineDebug::DrawBox(GetTransform(), debugCamera, color_);
		break;


	default:
		MsgBoxAssert("지원되지 않는 콜리전 타입입니다. 콜리전타입이 Max가 아닌지 확인하십시오.");
		break;
	}
}

void GameEngineCollision::SetUIDebugCamera()
{
	debugCameraOrder_ = CameraOrder::UICamera;
}

void GameEngineCollision::Start()
{
	this->GetActor()->GetLevel()->PushCollision(this, this->GetOrder());
}
