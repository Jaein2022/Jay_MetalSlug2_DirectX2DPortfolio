#pragma once

const float gravity_ = 9.80665f;
const float playSpeed_ = 100.f;

const PixelColor groundColor_(128, 255, 255, 255);
const PixelColor steppableObjectColor_(0, 255, 255, 255);

enum class CollisionBodyOrder	//충돌체오더지만 오브젝트오더도 겸함.
{
	Soldier,				//플레이어
	Soldier_MeleeAttack,	//플레이어 근접공격.
	Soldier_Projectile,	//플레이어 원거리공격.



	Rebel = 10,							//반란군.
	RebelAttack_MeleeAttack,			//반란군 근접공격.
	RebelAttack_FlyingSword,			//아라비안 투척검.
	RebelAttack_SolidBullet,			//반란군 총알.
	RebelAttack_Explosive,				//반란군 로켓/바주카/수류탄 공격.
	RebelAttack_Flame,					//반란군 화염공격.
	//RebelAttack_Electricity,

	//Mummy,
	//MummyOrb,

	Prisoner,
	Hyakutaro,

	Weapon_HeavyMachineGun,
	Weapon_Shotgun,
	Weapon_FlameShot,
	Weapon_RocketLauncher,

	Item_Ammo,
	Item_Explosive,

	//그 외 잡다한 아이템들은 여기에.

	Background,

	UI,
};
