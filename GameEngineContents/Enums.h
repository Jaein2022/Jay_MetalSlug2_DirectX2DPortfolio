#pragma once

enum class PlayerRenderStatus
{

	Standing_Pistol_Aiming_Forward,
	Standing_Pistol_Aiming_ForwardToUpward,
	Standing_Pistol_Aiming_Upward,
	Standing_Pistol_Aiming_UpwardToForward,
	Standing_Pistol_Firing_Forward,
	Standing_Pistol_FiringToAiming_Forward,
	Standing_Pistol_Firing_Upward,
	Standing_Pistol_FiringToAiming_Upward,
	Standing_Pistol_ThrowingGrenade,
	Standing_Pistol_MeleeAttack1,
	Standing_Pistol_MeleeAttack2,
	//Standing_Pistol_Idling,

	StandingToDucking_Pistol,

	Running_Pistol_Aiming_Forward,
	Running_Pistol_Aiming_ForwardToUpward,
	Running_Pistol_Aiming_Upward,
	Running_Pistol_Aiming_UpwardToForward,
	Running_Pistol_Firing_Forward,
	Running_Pistol_Firing_Upward,
	Running_Pistol_ThrowingGrenade,
	Running_Pistol_MeleeAttack1,
	Running_Pistol_MeleeAttack2,

	RunningToStanding_Pistol,

	Ducking_Pistol_Aiming,
	Ducking_Pistol_Firing,
	Ducking_Pistol_FiringToAiming,
	Ducking_Pistol_Advancing,
	Ducking_Pistol_ThrowingGrenade,
	Ducking_Pistol_MeleeAttack1,
	Ducking_Pistol_MeleeAttack2,

	VerticalJumping_Pistol_Aiming_Forward,
	VerticalJumping_Pistol_Aiming_ForwardToDownward,
	VerticalJumping_Pistol_Aiming_Downward,
	VerticalJumping_Pistol_Firing_Forward,
	VerticalJumping_Pistol_Firing_Upward,
	VerticalJumping_Pistol_Firing_Downward,
	VerticalJumping_Pistol_ThrowingGrenade,
	VerticalJumping_Pistol_MeleeAttack1,
	VerticalJumping_Pistol_MeleeAttack2,

	VerticalJumpingToStanding_Pistol,

	ForwardJumping_Pistol_Aiming_Forward,
	ForwardJumping_Pistol_Aiming_ForwardToDownward,
	ForwardJumping_Pistol_Aiming_Downward,
	ForwardJumping_Pistol_Firing_Forward,
	ForwardJumping_Pistol_Firing_Upward,
	ForwardJumping_Pistol_Firing_Downward,
	ForwardJumping_Pistol_ThrowingGrenade,
	ForwardJumping_Pistol_MeleeAttack1,
	ForwardJumping_Pistol_MeleeAttack2,

	ForwardJumpingToStanding_Pistol,

	Dying,
	Max
};

enum class PlayerTopStatus
{
	Aiming,
	Firing,
	ThrowingGrenade,
	MeleeAttack,
	Max
};

enum class PlayerLegStaus
{
	Standing,
	Running,
	Ducking,
	DuckStepping,
	Jumping,
	Max
};

enum class PlayerWeaponType
{
	Pistol,
	HeavyMachineGun,
	Max
};