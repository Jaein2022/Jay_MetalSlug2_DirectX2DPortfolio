#include "PreCompile.h"
#include "TestPistolBullet.h"
#include "TestPixelIndicator.h"
#include "TestLevel.h"

TestPistolBullet::TestPistolBullet()
	: bulletSpeed_(12.f),
	pistolBulletCollision_(nullptr),
	pistolBulletRenderer_(nullptr),
	effectiveHitSparkRenderer_(nullptr),
	glancingHitSparkRenderer_(nullptr),
	groundChecker_(nullptr)
{
}

TestPistolBullet::~TestPistolBullet()
{
}

void TestPistolBullet::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);


	pistolBulletCollision_ = CreateComponent<GameEngineCollision>("PistolBulletCollision");
	pistolBulletCollision_->GetTransform().SetLocalScale(40, 24, 10);
	pistolBulletCollision_->GetTransform().SetLocalPosition(0, 0, 0);
	pistolBulletCollision_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));
	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.


	pistolBulletRenderer_ = CreateComponent<GameEngineTextureRenderer>("PistolBulletRenderer");
	pistolBulletRenderer_->SetTexture("PistolBullet.png");
	pistolBulletRenderer_->GetTransform().SetLocalScale(40, 24, 1);
	pistolBulletRenderer_->SetPivot(PivotMode::Center);
	pistolBulletRenderer_->GetTransform().SetLocalPosition(0, 0, 0);


	effectiveHitSparkRenderer_ = CreateComponent<GameEngineTextureRenderer>("EffectiveHitSparkRenderer");
	if (0 == GameEngineTexture::Find("Spark_EffectiveHit.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Spark_EffectiveHit.png", 10, 1);
	}
	effectiveHitSparkRenderer_->SetTexture("Spark_EffectiveHit.png");
	effectiveHitSparkRenderer_->CreateFrameAnimation_CutTexture("EffectiveHitSpark", 
		FrameAnimation_Desc("Spark_EffectiveHit.png", 0, 9, 0.02f, false));
	effectiveHitSparkRenderer_->AnimationBindEnd("EffectiveHitSpark",
		[this](const FrameAnimation_Desc& _desc)->void {
			this->Death();
		}
	);
	effectiveHitSparkRenderer_->GetTransform().SetLocalScale(68, 64, 1);
	effectiveHitSparkRenderer_->SetPivot(PivotMode::Center);
	effectiveHitSparkRenderer_->ChangeFrameAnimation("EffectiveHitSpark");
	effectiveHitSparkRenderer_->Off();



	glancingHitSparkRenderer_ = CreateComponent<GameEngineTextureRenderer>("GlancingHitSparkRenderer");
	if (0 == GameEngineTexture::Find("Spark_GlancingHit.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Spark_GlancingHit.png", 10, 1);
	}
	glancingHitSparkRenderer_->SetTexture("Spark_GlancingHit.png");
	glancingHitSparkRenderer_->CreateFrameAnimation_CutTexture("GlancingHitSpark", 
		FrameAnimation_Desc("Spark_GlancingHit.png", 0, 9, 0.02f, false));
	glancingHitSparkRenderer_->AnimationBindEnd("GlancingHitSpark",
		[this](const FrameAnimation_Desc& _desc)->void {
			this->Death();
		}
	);
	glancingHitSparkRenderer_->GetTransform().SetLocalScale(128, 64, 1);
	glancingHitSparkRenderer_->SetPivot(PivotMode::Right);
	glancingHitSparkRenderer_->ChangeFrameAnimation("GlancingHitSpark");
	glancingHitSparkRenderer_->Off();


	groundChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"GroundChecker",
		this,
		float4::Black,
		float4(0, 0, -15),
		float4(5, 5, 1)
	);

	this->Death(1.f);

}

void TestPistolBullet::Update(float _deltaTime)
{
	this->GetTransform().SetWorldMove(firingDirection_ * bulletSpeed_ * _deltaTime * TestLevel::playSpeed_);

	//GameEngineDebug::DrawBox(pistolBulletCollision_->GetTransform(), float4::Red);

	if (true == CheckGroundHit() && false == glancingHitSparkRenderer_->IsUpdate())
	{
		pistolBulletRenderer_->Off();
		pistolBulletCollision_->Off();
		glancingHitSparkRenderer_->On();
		glancingHitSparkRenderer_->GetTransform().SetWorldRotation(0, 0, -90);
		firingDirection_ = float4::Zero;
	}
}

void TestPistolBullet::End()
{
	int i = 0;
}

bool TestPistolBullet::CheckGroundHit()
{
	if (TestLevel::groundColor_.color_ <=  groundChecker_->GetColorValue_UINT())
	{
		return true;
	}

	return false;
}
