#include "PreCompile.h"
#include "RebelBullet.h"
#include "Soldier.h"

RebelBullet::RebelBullet()
	: bulletSpeed_(4.f),
	firingDirection_(0),
	bulletRenderer_(nullptr),
	bulletCollisionBody_(nullptr)
{
}

RebelBullet::~RebelBullet()
{
}

void RebelBullet::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);


	bulletRenderer_ = CreateComponent<GameEngineTextureRenderer>("BulletRenderer");
	if (0 == GameEngineTexture::Find("RebelBullet.png")->GetCutCount())
	{
		GameEngineTexture::Cut("RebelBullet.png", 2, 1);
	}
	bulletRenderer_->SetTexture("RebelBullet.png");
	bulletRenderer_->SetPivot(PivotMode::Center);
	bulletRenderer_->GetTransform().SetLocalScale(32, 32, 1);
	bulletRenderer_->GetTransform().SetLocalPosition(0, 0, 0);

	bulletRenderer_->CreateFrameAnimation_CutTexture("Flickering",
		FrameAnimation_Desc("RebelBullet.png", 0, 1, 0.2f, true)
	);
	bulletRenderer_->ChangeFrameAnimation("Flickering");


	bulletCollisionBody_ = CreateComponent<GameEngineCollision>("BulletCollisionBody");
	bulletCollisionBody_->ChangeOrder(this->GetOrder());
	bulletCollisionBody_->SetCollisionMode(CollisionMode::Single);
	bulletCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));
	bulletCollisionBody_->GetTransform().SetLocalScale(32, 32, 10);
	bulletCollisionBody_->GetTransform().SetLocalPosition(0, 0, 0);
	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.

}

void RebelBullet::Update(float _deltaTime)
{
	this->GetTransform().SetWorldMove(float4::Right * firingDirection_ * bulletSpeed_ * _deltaTime * playSpeed_);

	bulletCollisionBody_->IsCollision(
		CollisionType::CT_AABB,
		CollisionBodyOrder::Soldier,
		CollisionType::CT_AABB,
		std::bind(&RebelBullet::Hit, this, std::placeholders::_1, std::placeholders::_2)
	);

	if (this->GetTransform().GetWorldPosition().x + 32.f
		< (-GameEngineWindow::GetInst()->GetScale().HX()
			+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x))
	{
		this->Death();		//총알은 윈도우 좌측경계를 넘어가자마자 소멸.
	}

	if (this->GetTransform().GetWorldPosition().x - 32.f
		> (GameEngineWindow::GetInst()->GetScale().HX()
			+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x))
	{
		this->Death();		//총알은 윈도우 우측경계를 넘어가자마자 소멸.
	}
}

void RebelBullet::End()
{
}

CollisionReturn RebelBullet::Hit(GameEngineCollision* _thisCollision, GameEngineCollision* _soldierCollision)
{
	_soldierCollision->GetActor<Soldier>()->TakeDamage(this->GetOrder());

	this->Death();

	return CollisionReturn::Stop;
}
