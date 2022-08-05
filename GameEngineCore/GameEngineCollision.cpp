#include "PreCompile.h"
#include "GameEngineCollision.h"
#include "GameEngineLevel.h"

bool (*GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionBodyType::CT_Max)][static_cast<int>(CollisionBodyType::CT_Max)])
(const GameEngineTransform& _transformA, const GameEngineTransform& _transformB);

class GameEngineCollisionFunctionInit
{
	//�� Ŭ������ ���� ����: 
public:
	GameEngineCollisionFunctionInit()
	{
		memset(
			GameEngineCollision::collisionFunctions_,
			0,
			sizeof(GameEngineCollision::collisionFunctions_));

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionBodyType::CT_Sphere)][static_cast<int>(CollisionBodyType::CT_Sphere)]
			= &GameEngineTransform::SphereToSphere;

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionBodyType::CT_AABB)][static_cast<int>(CollisionBodyType::CT_AABB)]
			= &GameEngineTransform::AABBToAABB;

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionBodyType::CT_OBB)][static_cast<int>(CollisionBodyType::CT_OBB)]
			= &GameEngineTransform::OBBToOBB;



		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionBodyType::CT_Sphere2D)][static_cast<int>(CollisionBodyType::CT_Sphere2D)]
			= &GameEngineTransform::Sphere2DToSphere2D;

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionBodyType::CT_AABB2D)][static_cast<int>(CollisionBodyType::CT_AABB2D)]
			= &GameEngineTransform::AABB2DToAABB2D;

		GameEngineCollision::collisionFunctions_[static_cast<int>(CollisionBodyType::CT_OBB2D)][static_cast<int>(CollisionBodyType::CT_OBB2D)]
			= &GameEngineTransform::OBB2DToOBB2D;

		//�߰� �Լ��� �ʿ��ϴٸ� �׶��׶� ���� �߰��� ��.
	}

	~GameEngineCollisionFunctionInit()
	{
	}
};

GameEngineCollisionFunctionInit inst_;

GameEngineCollision::GameEngineCollision()
{
}

GameEngineCollision::~GameEngineCollision()
{
}

void GameEngineCollision::ChangeOrder(int _order)
{
	this->GetActor()->GetLevel()->PushCollision(this, _order);
}

bool GameEngineCollision::IsCollision(
	CollisionBodyType _thisType,
	int _groupOrder,
	CollisionBodyType _otherType,
	std::function<bool(GameEngineCollision* _this, GameEngineCollision* _other)> _function /*= nullptr*/
)
{
	int thisType = static_cast<int>(_thisType);
	int otherType = static_cast<int>(_otherType);

	if (nullptr == GameEngineCollision::collisionFunctions_[thisType][otherType])
	{
		MsgBoxAssert("���� �غ���� ���� �浹 �Լ��Դϴ�.");
		return false;
	}

	std::map<int, std::list<GameEngineCollision*>>& allCollisions
		= this->GetActor()->GetLevel()->allCollisions_;

	std::list<GameEngineCollision*>& collisionGroup = allCollisions[_groupOrder];

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
				if (true == _function(this, collision))	//����� �浹 �Լ��� true�� ��ȯ�ϸ� �浹üũ�� �ѹ��� �Ѵ�.
														//����� �浹 �Լ��� false�� ��ȯ�ϸ� �浹üũ�� ��� �Ѵ�.
				{
					return true;
				}
			}
		}
	}

	return false;
}

void GameEngineCollision::Start()
{
	this->GetActor()->GetLevel()->PushCollision(this, this->GetOrder());
}
