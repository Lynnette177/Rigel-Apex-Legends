#pragma once
#include <windows.h>
//UPDATED 30/03/2022
#define	  LocalYaw  0x21fc-0x8		// 本地Yaw m_currentFramePlayer.m_ammoPoolCount - 0x8

#define  m_lifeState  0x0658			// 生命状态 y

#define OFF_HEALTH  0x02fc                          //[RecvTable.DT_Player]->m_iHealth
#define OFF_MAXHEALTH  0x0438                         //[RecvTable.DT_Player]->m_iMaxHealth
#define OFF_SHIELD  0x01a0                            //[RecvTable.DT_TitanSoul]->m_shieldHealth
#define OFF_MAXSHIELD  0x01a4                         //[RecvTable.DT_TitanSoul]->m_shieldHealthMax
#define m_vecPunchWeapon_Angle  0x23f8		// 后坐力Y [?]->?
#define OFF_VIEW_ANGLES 0x24f4-0x14               //[DataMap.C_Player]-> m_ammoPoolCapacity - 0x14 m_viewAngles
#define timeBase  0x2048				// 世界时间 m_currentFramePlayer.timeBase=0x20b8
#define lastVisibleTime  0x194d + 0x3		// 最后可视时间 +0x3
#define OFFSET_VISIBLE_TIME 0x194d + 0x3      //CPlayer!lastVisibleTime


#define OFFSET_ENTITYLIST 0x1db2ed8   //cl_entitylist
#define OFFSET_LOCAL_ENT 0x21615d8   //LocalPlayer
#define OFFSET_ORIGIN 0x017c          //m_vecAbsOrigin m_Pos//坐标

#define OFFSET_NAME	0x0449              //m_iName 不是m_iSignifierName

#define OFFSET_MATRIX 0x11a350
#define OFFSET_RENDER 0x73cc460

#define OFFSET_TEAM	0x030c                 //m_iTeamNum m_iTeamNum
#define OFFSET_BLEED_OUT_STATE 0x26a0     //m_bleedoutState m_bleedoutState倒地状态

//#define OFFSET_ITEM_ID	 0x1518


#define OFFSET_GLOW_ENABLE          0x28C //currentid
#define OFFSET_GLOW_THROUGH_WALLS   0x026c // vibility type2 = enabled, 5 = disabled
#define OFFSET_GLOW_FIX   0x268
#define OFFSET_GLOW_HIGHLIGHT_ID   0x28C              //[DT_HighlightSettings].m_highlightServerActiveStates     
#define OFFSET_GLOW_HIGHLIGHT_SETTINGS   0xB7A6050
//武器和预判
#define OFF_WEAPON_HANDLE   0x18f4                    //[RecvTable.DT_Player]->m_latestPrimaryWeapons
#define OFF_WEAPON_INDEX   0x1738                     //[RecvTable.DT_WeaponX]->m_weaponNameIndex
#define OFF_PROJECTILESCALE   0x04ec + 0x1978                ////CWeaponX!m_flProjectileScale 下面那个+8 CWeaponX!m_flProjectileScale maybe its WeaponSettings.projectile_gravity_scale now
#define OFF_PROJECTILESPEED   0x04e4 + 0x1978                 //CWeaponX!m_flProjectileSpeed m_flProjectileSpeed maybe its WeaponSettings.projectile_launch_speed now
#define OFF_ABSVELOCITY  0x0170                     //[DataMap.C_BaseEntity]->m_vecAbsVelocity
#define OFF_LAST_AIMEDAT_TIME 0x194d + 0x3 + 0x8   //m_hudInfo_visibilityTestAlwaysPasses++ or CWeaponX!lastCrosshairTargetTime visible+0x8 上面是0x3
#define OFF_ZOOMING  0x1b91           //m_bZooming

#define OFF_CAMERAORIGIN 0x1e90                    //[Miscellaneous]->CPlayer!camera_origin

#define OFFSET_SAVERMODE 0x3454                 //m_iObserverMode
#define OFFSET_SKIN 0x0d18          //m_nSkin

#define thirdperson_override 0x01d00750
#define m_thirdPersonShoulderView 0x36c0
#define m_thirdPerson 0x3724
#define m_hThirdPersonEnt 0x36bc

#define OFFSET_VIEWMODEL 0x2ce0 //m_hViewModels
#define OFFSET_CURFRAME 0x00d8 //m_currentFrame.modelIndex
#define OFFSET_MODELNAME 0x0030 //m_ModelName

#define OFF_STUDIOHDR  0xfb0                      //[Miscellaneous]->CBaseAnimating!m_pStudioHdr
#define OFF_BONES  0x0d60 + 0x48 //m_nForceBone + 0x48 //updated 11/21/2023
#define OFFSET_BONES 0x0d60 + 0x48            //m_bConstrainBetweenEndpoints m_nForceBone + 0x48