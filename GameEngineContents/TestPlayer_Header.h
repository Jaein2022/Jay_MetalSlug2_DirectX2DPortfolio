#pragma once

enum class PlayerState
{
	Dead = 1000,

	Pistol_Standing_Aiming_Forward								= 1111,
	Pistol_Standing_Aiming_Upward								= 1112,
	Pistol_Standing_Aiming_ForwardToUpward						= 1114,
	Pistol_Standing_Aiming_UpwardToForward						= 1115,
	Pistol_Standing_Firing_Forward								= 1121,
	Pistol_Standing_Firing_Upward								= 1122,
	Pistol_Standing_FiringToAiming_Forward						= 1131,
	Pistol_Standing_FiringToAiming_Upward						= 1132,
	Pistol_Standing_ThrowingGrenade								= 1141,
	Pistol_Standing_ThrowingGrenadeToAiming						= 1151,
	Pistol_Standing_MeleeAttack									= 1171,

	Pistol_Running_Aiming_Forward								= 1211,
	Pistol_Running_Aiming_Upward								= 1212,
	Pistol_Running_Aiming_ForwardToUpward						= 1214,
	Pistol_Running_Aiming_UpwardToForward						= 1215,
	Pistol_Running_Firing_Forward								= 1221,
	Pistol_Running_Firing_Upward								= 1222,
	Pistol_Running_FiringToAiming_Forward						= 1231,
	Pistol_Running_FiringToAiming_Upward						= 1232,
	Pistol_Running_ThrowingGrenade								= 1241,
	Pistol_Running_ThrowingGrenadeToAiming						= 1251,
	Pistol_Running_MeleeAttack									= 1271,

	Pistol_Ducking_Aiming										= 1311,
	Pistol_Ducking_Firing										= 1321,
	Pistol_Ducking_FiringToAiming								= 1331,
	Pistol_Ducking_ThrowingGrenade								= 1341,
	Pistol_Ducking_ThrowingGrenadeToAiming						= 1351,
	Pistol_Ducking_DuckStepping									= 1361,
	Pistol_Ducking_MeleeAttack									= 1371,

	Pistol_VerticalJumping_Aiming_Forward						= 1411,
	Pistol_VerticalJumping_Aiming_Downward						= 1413,
	Pistol_VerticalJumping_Aiming_ForwardToDownward				= 1416,
	Pistol_VerticalJumping_Aiming_DownwardToForward				= 1417,
	Pistol_VerticalJumping_Firing_Forward						= 1421,
	Pistol_VerticalJumping_Firing_Upward						= 1422,
	Pistol_VerticalJumping_Firing_Downward						= 1423,
	Pistol_VerticalJumping_FiringToAiming_Forward				= 1431,
	Pistol_VerticalJumping_FiringToAiming_Upward				= 1432,
	Pistol_VerticalJumping_FiringToAiming_Downward				= 1433,
	Pistol_VerticalJumping_ThrowingGrenade						= 1441,
	Pistol_VerticalJumping_ThrowingGrenadeToAiming				= 1451,
	Pistol_VerticalJumping_MeleeAttack							= 1471,

	Pistol_ForwardJumping_Aiming_Forward						= 1511,
	Pistol_ForwardJumping_Aiming_Downward						= 1513,
	Pistol_ForwardJumping_Aiming_ForwardToDownward				= 1516,
	Pistol_ForwardJumping_Aiming_DownwardToForward				= 1517,
	Pistol_ForwardJumping_Firing_Forward						= 1521,
	Pistol_ForwardJumping_Firing_Upward							= 1522,
	Pistol_ForwardJumping_Firing_Downward						= 1523,
	Pistol_ForwardJumping_FiringToAiming_Forward				= 1531,
	Pistol_ForwardJumping_FiringToAiming_Upward					= 1532,
	Pistol_ForwardJumping_FiringToAiming_Downward				= 1533,
	Pistol_ForwardJumping_ThrowingGrenade						= 1541,
	Pistol_ForwardJumping_ThrowingGrenadeToAiming				= 1551,
	Pistol_ForwardJumping_MeleeAttack							= 1571,

	Pistol_StandingToDucking									= 1611,
	Pistol_RunningToStanding									= 1711,
	Pistol_JumpingToStanding									= 1811,






	HeavyMachineGun_Standing_Aiming_Forward						= 2111,
	HeavyMachineGun_Standing_Aiming_Upward						= 2112,
	HeavyMachineGun_Standing_Aiming_ForwardToUpward				= 2114,
	HeavyMachineGun_Standing_Aiming_UpwardToForward				= 2115,
	HeavyMachineGun_Standing_Firing_Forward						= 2121,
	HeavyMachineGun_Standing_Firing_Upward						= 2122,
	HeavyMachineGun_Standing_Firing_ForwardToUpward				= 2124,
	HeavyMachineGun_Standing_Firing_UpwardToForward				= 2125,
	HeavyMachineGun_Standing_ThrowingGrenade					= 2141,
	HeavyMachineGun_Standing_ThrowingGrenadeToAiming			= 2151,
	HeavyMachineGun_Standing_MeleeAttack						= 2171,

	HeavyMachineGun_Running_Aiming_Forward						= 2211,
	HeavyMachineGun_Running_Aiming_Upward						= 2212,
	HeavyMachineGun_Running_Aiming_ForwardToUpward				= 2214,
	HeavyMachineGun_Running_Aiming_UpwardToForward				= 2215,
	HeavyMachineGun_Running_Firing_Forward						= 2221,
	HeavyMachineGun_Running_Firing_Upward						= 2222,
	HeavyMachineGun_Running_Firing_ForwardToUpward				= 2224,
	HeavyMachineGun_Running_Firing_UpwardToForward				= 2225,
	HeavyMachineGun_Running_ThrowingGrenade						= 2241,
	HeavyMachineGun_Running_ThrowingGrenadeToAiming				= 2251,
	HeavyMachineGun_Running_MeleeAttack							= 2271,

	HeavyMachineGun_Ducking_Aiming								= 2311,
	HeavyMachineGun_Ducking_Firing								= 2321,
	HeavyMachineGun_Ducking_FiringToAiming						= 2331,
	HeavyMachineGun_Ducking_ThrowingGrenade						= 2341,
	HeavyMachineGun_Ducking_ThrowingGrenadeToAiming				= 2351,
	HeavyMachineGun_Ducking_DuckStepping						= 2361,
	HeavyMachineGun_Ducking_MeleeAttack							= 2371,

	HeavyMachineGun_VerticalJumping_Aiming_Forward				= 2411,
	HeavyMachineGun_VerticalJumping_Aiming_Downward				= 2413,
	HeavyMachineGun_VerticalJumping_Aiming_ForwardToDownward	= 2416,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	HeavyMachineGun_VerticalJumping_Aiming_DownwardToForward	= 2417,
	HeavyMachineGun_VerticalJumping_Firing_Forward				= 2421,
	HeavyMachineGun_VerticalJumping_Firing_Upward				= 2422,
	HeavyMachineGun_VerticalJumping_Firing_Downward				= 2423,
	HeavyMachineGun_VerticalJumping_Firing_ForwardToUpward		= 2424,
	HeavyMachineGun_VerticalJumping_Firing_UpwardToForward		= 2425,
	HeavyMachineGun_VerticalJumping_Firing_ForwardToDownward	= 2426,
	HeavyMachineGun_VerticalJumping_Firing_DownwardToForward	= 2427,
	HeavyMachineGun_VerticalJumping_ThrowingGrenade				= 2441,
	HeavyMachineGun_VerticalJumping_ThrowingGrenadeToAiming		= 2451,
	HeavyMachineGun_VerticalJumping_MeleeAttack					= 2471,

	HeavyMachineGun_ForwardJumping_Aiming_Forward				= 2511,
	HeavyMachineGun_ForwardJumping_Aiming_Downward				= 2513,
	HeavyMachineGun_ForwardJumping_Aiming_ForwardToDownward		= 2516,
	HeavyMachineGun_ForwardJumping_Aiming_DownwardToForward		= 2517,
	HeavyMachineGun_ForwardJumping_Firing_Forward				= 2521,
	HeavyMachineGun_ForwardJumping_Firing_Upward				= 2522,
	HeavyMachineGun_ForwardJumping_Firing_Downward				= 2523,
	HeavyMachineGun_ForwardJumping_Firing_ForwardToUpward		= 2524,
	HeavyMachineGun_ForwardJumping_Firing_UpwardToForward		= 2525,
	HeavyMachineGun_ForwardJumping_Firing_ForwardToDownward		= 2526,
	HeavyMachineGun_ForwardJumping_Firing_DownwardToForward		= 2527,
	HeavyMachineGun_ForwardJumping_ThrowingGrenade				= 2541,
	HeavyMachineGun_ForwardJumping_ThrowingGrenadeToAiming		= 2551,
	HeavyMachineGun_ForwardJumping_MeleeAttack					= 2571,

	HeavyMachineGun_StandingToDucking							= 2611,
	HeavyMachineGun_RunningToStanding							= 2711,
	HeavyMachineGun_JumpingToStanding							= 2811,


};

template <>
struct magic_enum::customize::enum_range<PlayerState> {
	static constexpr int min = 1000;
	static constexpr int max = 3000;
	// 1000 ~ 6000 넘기지 말 것.
};

enum class PlayerWeaponType
{
	Pistol			= 1000,
	HeavyMachineGun = 2000,
	//RockeLauncher		= 3000,
	//Shotgun				= 4000,
	//Flameshot			= 5000
};

enum class PlayerLegState
{
	Standing			= 100,
	Running				= 200,
	Ducking				= 300,
	VerticalJumping		= 400,
	ForwardJumping		= 500,
	StandingToDucking	= 600,
	RunningToStanding	= 700,
	JumpingToStanding	= 800,
};

enum class PlayerTopState
{
	Aiming						= 10,
	Firing						= 20,
	FiringToAiming				= 30,
	ThrowingGrenade				= 40,	//이 동작은 전방으로 방향 고정.
	ThrowingGrenadeToAiming		= 50,	//이 동작은 전방으로 방향 고정.
	DuckStepping				= 60,	//이 동작은 전방으로 방향 고정.
	MeleeAttack					= 70,	//이 동작은 전방으로 방향 고정.
};

enum class AimingDirection
{
	Forward				= 1,
	Upward				= 2,
	Downward			= 3,
	ForwardToUpward		= 4,
	UpwardToForward		= 5,
	ForwardToDownward	= 6,
	DownwardToForward	= 7,
};
