#include "PreCompile.h"
#include "TestPistolBullet.h"
#include "TestPixelIndicator.h"
#include "TestLevel.h"

TestPistolBullet::TestPistolBullet()
	: bulletSpeed_(10.f),
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
	pistolBulletCollision_->GetTransform().SetLocalScale(40, 24, 1);
	pistolBulletCollision_->GetTransform().SetLocalPosition(0, 0, 0);
	pistolBulletCollision_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));


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
		FrameAnimation_Desc("Spark_EffectiveHit.png", 0, 9, 0.2f, false));
	effectiveHitSparkRenderer_->GetTransform().SetLocalScale(68, 64, 1);
	effectiveHitSparkRenderer_->SetPivot(PivotMode::Center);
	effectiveHitSparkRenderer_->ChangeFrameAnimation("EffectiveHitSpark");
	effectiveHitSparkRenderer_->Off();



	glancingHitSparkRenderer_ = CreateComponent<GameEngineTextureRenderer>("GlancingHitSparkRenderer");
	if (0 == GameEngineTexture::Find("Spark_GlancingHit.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Spark_GlancingHit.png", 12, 1);
	}
	glancingHitSparkRenderer_->SetTexture("Spark_GlancingHit.png");
	glancingHitSparkRenderer_->CreateFrameAnimation_CutTexture("GlancingHitSpark", 
		FrameAnimation_Desc("Spark_GlancingHit.png", 0, 11, 0.2f, false));
	glancingHitSparkRenderer_->GetTransform().SetLocalScale(150, 250, 1);
	glancingHitSparkRenderer_->GetTransform().SetLocalRotation(0, 0, 90);
	glancingHitSparkRenderer_->SetPivot(PivotMode::Bot);
	glancingHitSparkRenderer_->ChangeFrameAnimation("GlancingHitSpark");
	glancingHitSparkRenderer_->Off();


	groundChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"GroundChecker",
		this,
		float4::Black,
		float4(0, 0, -5),
		float4(5, 5, 1)
	);

	this->Death(1.f);

}

void TestPistolBullet::Update(float _deltaTime)
{
	this->GetTransform().SetWorldMove(firingDirection_ * bulletSpeed_ * _deltaTime * TestLevel::playSpeed_);

	GameEngineDebug::DrawBox(pistolBulletCollision_->GetTransform(), float4(1.f, 0.f, 0.f, 0.5f));

	if (true == CheckGroundHit())
	{
		this->Death();
	}
}

void TestPistolBullet::End()
{
	int i = 0;
}

bool TestPistolBullet::CheckGroundHit()
{
	if (TestPixelIndicator::magenta_.color_ ==  groundChecker_->GetColorValue_UINT())
	{
		return true;
	}

	return false;
}
