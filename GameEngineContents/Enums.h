#pragma once

enum class PlayerState
{

	Pistol_Standing_Aiming_Forward						= 1111,
	Pistol_Standing_Aiming_Upward						= 1112,
	Pistol_Standing_Aiming_ForwardToUpward				= 1114,
	Pistol_Standing_Aiming_UpwardToForward				= 1115,
	Pistol_Standing_Firing_Forward						= 1121,
	Pistol_Standing_Firing_Upward						= 1122,
	Pistol_Standing_FiringToAiming_Forward				= 1131,
	Pistol_Standing_FiringToAiming_Upward				= 1132,
	Pistol_Standing_ThrowingGrenade						= 1141,
	Pistol_Standing_MeleeAttack							= 1151,
	//Standing_Pistol_Idling,

	Pistol_Running_Aiming_Forward						= 1211,
	Pistol_Running_Aiming_Upward						= 1212,
	Pistol_Running_Aiming_ForwardToUpward				= 1214,
	Pistol_Running_Aiming_UpwardToForward				= 1215,
	Pistol_Running_Firing_Forward						= 1221,
	Pistol_Running_Firing_Upward						= 1222,
	Pistol_Running_ThrowingGrenade						= 1241,
	Pistol_Running_MeleeAttack							= 1251,

	Pistol_Ducking_Aiming								= 1311,
	Pistol_Ducking_Firing								= 1321,
	Pistol_Ducking_FiringToAiming						= 1331,
	Pistol_Ducking_ThrowingGrenade						= 1341,
	Pistol_Ducking_MeleeAttack							= 1351,
	Pistol_Ducking_DuckStepping							= 1361,

	Pistol_VerticalJumping_Aiming_Forward				= 1411,
	Pistol_VerticalJumping_Aiming_Downward				= 1413,
	Pistol_VerticalJumping_Aiming_ForwardToDownward		= 1416,
	Pistol_VerticalJumping_Firing_Forward				= 1421,
	Pistol_VerticalJumping_Firing_Upward				= 1422,
	Pistol_VerticalJumping_Firing_Downward				= 1423,
	Pistol_VerticalJumping_ThrowingGrenade				= 1441,
	Pistol_VerticalJumping_MeleeAttack					= 1451,

	Pistol_ForwardJumping_Aiming_Forward				= 1511,
	Pistol_ForwardJumping_Aiming_Downward				= 1513,
	Pistol_ForwardJumping_Aiming_ForwardToDownward		= 1516,
	Pistol_ForwardJumping_Firing_Forward				= 1521,
	Pistol_ForwardJumping_Firing_Upward					= 1522,
	Pistol_ForwardJumping_Firing_Downward				= 1523,
	Pistol_ForwardJumping_ThrowingGrenade				= 1541,
	Pistol_ForwardJumping_MeleeAttack					= 1551,

	Pistol_StandingToDucking							= 1611,
	Pistol_RunningToStanding							= 1711,
	Pistol_VerticalJumpingToStanding					= 1811,
	Pistol_ForwardJumpingToStanding						= 1911,






	Dead = 0
};

enum class AimingDirection
{
	Forward				= 1,
	Upward				= 2,
	Downward			= 3,
	ForwardToUpward		= 4,
	UpwardToForward		= 5,
	ForwardToDownward	= 6,
};

enum class PlayerTopStatus
{
	Aiming			= 10,
	Firing			= 20,
	FiringToAiming	= 30,
	ThrowingGrenade = 40,	//이 동작은 전방으로 방향 고정.
	MeleeAttack		= 50,	//이 동작은 전방으로 방향 고정.
	DuckStepping	= 60	//이 동작은 전방으로 방향 고정.
};

enum class PlayerLegStatus
{
	Standing					= 100,
	Running						= 200,
	Ducking						= 300,
	VerticalJumping				= 400,
	ForwardJumping				= 500,
	StandingToDucking			= 600,	//이 동작은 조준상태, 전방 방향으로만 있을 수 있음.
	RunningToStanding			= 700,	//이 동작은 조준상태, 전방 방향으로만 있을 수 있음.
	VerticalJumpingToStanding	= 800,	//이 동작은 조준상태, 전방 방향으로만 있을 수 있음.
	ForwardJumpingToStanding	= 900	//이 동작은 조준상태, 전방 방향으로만 있을 수 있음.
};

enum class PlayerWeaponType
{
	Pistol				= 1000,
	HeavyMachineGun		= 2000,
	RockeLauncher		= 3000,
	Shotgun				= 4000,
	Flameshot			= 5000
};