#pragma once
#include "GlobalHeader.h"

class Rebel: public GameEngineActor
{
	//이 클래스의 존재 이유: TakeDamage();
public:
	Rebel();
	~Rebel();

protected:
	Rebel(const Rebel& _other) = delete;
	Rebel(Rebel&& _other) noexcept = delete;

private:
	Rebel& operator=(const Rebel& _other) = delete;
	Rebel& operator=(const Rebel&& _other) = delete;


public:	
	virtual void TakeDamage(
		int _damage, 
		GameEngineCollision* _soldierWeaponCollision,
		GameEngineCollision* _rebelCollision
	) = 0;


private:

};

