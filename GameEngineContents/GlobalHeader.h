#pragma once

const float gravity_ = 9.80665f;
const float playSpeed_ = 100.f;

const PixelColor groundColor_(128, 255, 255, 255);
const PixelColor steppableObjectColor_(0, 255, 255, 255);

enum class CollisionBodyOrder	//�浹ü�������� ������Ʈ������ ����.
{
	Soldier,				//�÷��̾�
	Soldier_MeleeAttack,	//�÷��̾� ��������.
	Soldier_Projectile,	//�÷��̾� ���Ÿ�����.



	Rebel = 10,							//�ݶ���.
	RebelAttack_MeleeAttack,			//�ݶ��� ��������.
	RebelAttack_FlyingSword,			//�ƶ��� ��ô��.
	RebelAttack_SolidBullet,			//�ݶ��� �Ѿ�.
	RebelAttack_Explosive,				//�ݶ��� ����/����ī/����ź ����.
	RebelAttack_Flame,					//�ݶ��� ȭ������.
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

	//�� �� ����� �����۵��� ���⿡.

	Background,

	UI,
};
