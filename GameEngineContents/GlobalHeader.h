#pragma once

const float gravity_ = 9.80665f;
const float playSpeed_ = 100.f;

const PixelColor groundColor_(128, 255, 255, 255);
const PixelColor steppablePixelColor_(0, 255, 255, 255);

enum class ObjectOrder	//충돌체오더/오브젝트오더 겸함.
{
	Soldier,				//플레이어
	Soldier_MeleeAttack,	//플레이어 근접공격.
	Soldier_Projectile,		//플레이어 원거리공격.



	Rebel = 9,							//반란군.
	RebelMachine,						//반란군 기계/차량.
	RebelAttack_MeleeAttack,			//반란군 근접공격.
	RebelAttack_FlyingSword,			//아라비안 투척검.
	RebelAttack_SolidBullet,			//반란군 총알.
	RebelAttack_Explosive,				//반란군 로켓/바주카/수류탄 공격.
	RebelAttack_Flame,					//반란군 화염공격.


	SteppableObject,					//트럭 잔해 등의 밟고 올라설 수 있는 오브젝트.


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
