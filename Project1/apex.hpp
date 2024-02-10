#pragma once
#include "simhei.h"
#include"imgui_toggle.h"
#include"skStr.h"
#include"includes.hpp"
#include "Driver.h"
#include "structures.hpp"
#include "target.hpp"
#include "GlobalVar.hpp"
#include "offsets.h"
#include"entity.hpp"
#include"GlowEsp.hpp"
#include"Renderer.hpp"
#include "candy.h"
#include"string.h"
#include <stdio.h>
inline std::vector<CEntity> entities = {};
CEntity AimEntity;
CEntity LocalEntity;
Matrix m;
static auto NewAngle = QAngle{ 0,0};
static auto PreviousAngle = QAngle{ 0,0};
static auto AngleBackup = QAngle{ 0,0 };
int lastfoundentity = 0;
int keydown = 0;
DWORD keydowntime = 0;
std::string random_string_window()
{
	srand((unsigned)time(NULL));
	std::string str = skCrypt("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890").decrypt();
	std::string newstr;
	int pos;
	while (newstr.size() != 5)
	{
		pos = ((rand() % (str.size() + 1)));
		newstr += str.substr(pos, 1);
	}
	return newstr;
}
void bigmapthread() {
	int localPlayerTeamID = LocalEntity.Team;
	if (localPlayerTeamID != 1) {
		float curTime = read<float>(LocalEntity.Address + timeBase);
		double continueTime = 0.2;
		float endTime = curTime + continueTime;
		while (curTime < endTime)
		{
			write<int>(LocalEntity.Address + OFFSET_TEAM, 1);
			curTime = read<float>(LocalEntity.Address + timeBase);
		}
		curTime = read<float>(LocalEntity.Address + timeBase);
		endTime = curTime + continueTime;
		while (curTime < endTime)
		{
			write<int>(LocalEntity.Address + OFFSET_TEAM, localPlayerTeamID);
			curTime = read<float>(LocalEntity.Address + timeBase);
		}
	}
	/*for (int i = 1; i <= 32; i++)
	{
		int nowtime = GetTickCount();
		while (GetTickCount() - nowtime < 200) {
			write<int>(LocalEntity.Address + OFFSET_TEAM, i);
		}
	}
	Sleep(50);
	write<int>(LocalEntity.Address + OFFSET_TEAM, localPlayerTeamID);*/
}
void skinchange() {
	std::map<int, std::vector<int>> weaponSkinMap;
	weaponSkinMap[103] = { 9,11 };   //R99  
	weaponSkinMap[0] = { 7 };     //R301   
	weaponSkinMap[79] = { 9 };    //转换者  
	weaponSkinMap[89] = { 16 };   //赫姆洛克
	weaponSkinMap[87] = { 17 };    //平行 
	weaponSkinMap[110] = { 11 };      //3030  
	weaponSkinMap[111] = { 11,10 };   //car   
	weaponSkinMap[20] = { 10 };       //暴走
	weaponSkinMap[89] = { 12 }; //哈沃克
	weaponSkinMap[83] = { 10 };     //专注
	weaponSkinMap[109] = { 13 };     // 电冲 
	weaponSkinMap[112] = { 8 };     //复仇女神

	weaponSkinMap[86] = { 11 };     //ev8  
	//weaponSkinMap[102] = { 15 };     //heping  
	weaponSkinMap[94] = { 11 };     //aoquan  
	float curTime = read<float>(LocalEntity.Address + timeBase);
	float endTime = curTime + 0.5;
	int waponIndex = LocalEntity.WeaponIndex;
	if (waponIndex) {
		if (weaponSkinMap.count(waponIndex) != 0) {
			int index = weaponSkinMap[waponIndex][0];
			if (LocalEntity.WeaponEntity) {
				if (index > 0 && read<int>(LocalEntity.WeaponEntity + OFFSET_SKIN) != index) {
					while (curTime < endTime)
					{
						write<int>(LocalEntity.WeaponEntity + OFFSET_SKIN, index);
						curTime = read<float>(LocalEntity.Address + timeBase);
					}
				}
			}
		}
	}
}
void skinthread() {
	while (1) {
		if (GetAsyncKeyState(VK_F5)) {
			bigmapthread();
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F6)) {
			skinchange();
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F7)) {
			Config::freecam = !Config::freecam;
			Sleep(500);
		}
		if (!LocalEntity.Address||entities.empty()) continue;
		//nospread
		if (Config::nospread&& LocalEntity.WeaponIndex != 5 && LocalEntity.WeaponIndex != 17) write<float>(LocalEntity.WeaponEntity + OFF_PROJECTILESPEED, 0.f);
		//tpp
		if (Config::thirdpp) {
			if (LocalEntity.zoomin)
			{
				if (read<int>(oBaseAddress + thirdperson_override + 141) != -1)
				{
					write<int>(oBaseAddress + thirdperson_override + 141, -1);
					write<int>(LocalEntity.Address + m_thirdPersonShoulderView, 0);
				}
			}
			else
			{
				if (read<int>(oBaseAddress + thirdperson_override + 141) != 1)
				{
					write<int>(oBaseAddress + thirdperson_override + 141, 1);
					write<int>(LocalEntity.Address + m_thirdPersonShoulderView , 1);
					write<int>(LocalEntity.Address + m_thirdPerson, 1);
					write<int>(LocalEntity.Address + m_hThirdPersonEnt, 1);
				}
			}
		}
		else if (read<int>(oBaseAddress + thirdperson_override + 141 ) != -1)
		{
			write<int>(oBaseAddress + thirdperson_override + 141, -1);
			write<int>(LocalEntity.Address + m_thirdPersonShoulderView, 0);
		}
		//free cam
		if (Config::freecam == true) {
			write<int>(LocalEntity.Address + OFFSET_SAVERMODE, 7);
		}
		else if (Config::freecam2 == true) {
			write<int>(LocalEntity.Address + OFFSET_SAVERMODE, 22);
		}
		else if (!Config::freecam2 && !Config::freecam &&read<int>(LocalEntity.Address + OFFSET_SAVERMODE)!= 0) {
			write<int>(LocalEntity.Address + OFFSET_SAVERMODE, 0);
		}
		/*if (Config::autograb) {
			write<int>(oBaseAddress + in_jump + 0x8, 4);
			auto Gn = read<int>(LocalEntity.Address + m_grapple + m_grappleAttached);
			if (Gn == 1) {
				write<int>(oBaseAddress + in_jump + 0x8, 5);
				write<int>(oBaseAddress + m_grappleActivateTime, 1); //m_grappleActivateTime
			}
		}*/

		/*if (Config::Heirloom && LocalEntity.WeaponIndex == 113) {
			uintptr_t ViewModelHandle = read<uintptr_t>(LocalEntity.Address+ OFFSET_VIEWMODEL) & 0xFFFF;
			uintptr_t ViewModel = read<uintptr_t>(LocalEntity.Address + (ViewModelHandle <<5));
			//write<int>(LocalEntity.WeaponEntity + OFFSET_SKIN, 65);
			write<int>(ViewModel + OFFSET_CURFRAME, 912);
			write<std::string>(ViewModel + OFFSET_MODELNAME, "mdl/weapons/kunai/ptpov_kunai_wraith.rmdl");
		}*/
		Sleep(10);
	}
}
void aimbotthread() {
	float crosshairX = screenWeight / 2.f;
	float crosshairY = screenHeight / 2.f;
	QAngle aimingangle = { 0,0 };
	Vector3 HeadPosition1 = { 0,0,0 };
	while (1) {
		int dotrigger = 0;
		if (!LocalEntity.Address || entities.empty()) continue;
		//std::cout << "1\n";
		uint64_t viewRenderer = read<uint64_t>(oBaseAddress + OFFSET_RENDER);
		uint64_t viewMatrix = read<uint64_t>(viewRenderer + OFFSET_MATRIX);
		m = read<Matrix>(viewMatrix);
		Vector3 HeadPosition = { 0,0,0 };
		float closestX = 9999;
		float closestY = 9999;
		int decideaimorno = 0;
		for (CEntity ce : entities) {
			if (ce.Address == LocalEntity.Address) continue;

			float WorldTime = read<float>(LocalEntity.Address + timeBase);
			ce.IsAimedAt = ce.IsTarget(WorldTime);
			CURSORINFO ci = { sizeof(CURSORINFO) };
			if (GetCursorInfo(&ci))
			{
				if (ci.flags == 0) {
					if (!GetAsyncKeyState(1) && Config::trigger && LocalEntity.WeaponIndex != 5 && LocalEntity.WeaponIndex != 113) {
						if (ce.IsAimedAt && ce.visible && ce.Team && !ce.DyingState && !ce.BleedOutState && ce.Health > 0 && LocalEntity.Health > 0) {
							float dis = pow(pow(LocalEntity.HeadPos.x - ce.HeadPos.x, 2) + pow(LocalEntity.HeadPos.y - ce.HeadPos.y, 2) + pow(LocalEntity.HeadPos.z - ce.HeadPos.z, 2), 0.5);
							if ((GameUnitsToMeter(dis) < Config::triggerdiszoom && LocalEntity.zoomin) || (GameUnitsToMeter(dis) < Config::triggerdisnotzoom && !LocalEntity.zoomin)) {
								if (!keydown)
									mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
								keydown = 1;
								keydowntime = GetTickCount();
								dotrigger = 1;
							}
						}
					}
				}
			}
			if (Config::bone == 0||Config::autobone) HeadPosition = ce.GetBonePositionVector3(HitboxType::Head);
			else if(Config::bone==1) HeadPosition = ce.GetBonePositionVector3(HitboxType::Neck);
			else if (Config::bone == 2) HeadPosition = ce.GetBonePositionVector3(HitboxType::Stomach);
			else if (Config::bone == 3) HeadPosition = ce.GetBonePositionVector3(HitboxType::Hip);
			//printf("%f,%f,%f\n", HeadPosition.x, HeadPosition.y, HeadPosition.z);
			// Convert to screen position
			Vector3 PreditPos = HeadPosition;
			Vector3 w2sHeadAimPos;
			if (Config::Prediction) {
				ce.AbsVelocity= read<Vec3>(ce.Address + OFF_ABSVELOCITY);
				AimCorrection(LocalEntity.HeadPos, LocalEntity.WeaponProjectileScale, &PreditPos, ce.AbsVelocity, LocalEntity.WeaponProjectileSpeed);
				//printf("%f,%f\n\n", aimingangle.x, aimingangle.y);
				w2sHeadAimPos = _WorldToScreen(PreditPos, m);
			}
			else {
				w2sHeadAimPos = _WorldToScreen(HeadPosition, m);
			}
			float entX = w2sHeadAimPos.x;
			float entY = w2sHeadAimPos.y;
			if (ce.visible && ce.Team && !ce.DyingState && !ce.BleedOutState)//可见
			{
				// Aimbot fov
				if (abs(crosshairX - entX) < abs(crosshairX - closestX) && sqrt(pow((crosshairX - entX), 2) + pow((crosshairY - entY), 2)) < FOV)
				{
					// Aimbot find closest target
					//auto viewAngles = read <QAngle>(LocalEntity.Address + OFF_VIEW_ANGLES).fixAngle();
					closestX = entX;
					closestY = entY;
					AimEntity = ce;
					AimEntity.aimx = entX;
					AimEntity.aimy = entY;
					decideaimorno = 1;
					HeadPosition1 = PreditPos;
					
				}
			}
		}

		auto viewAngles = read <QAngle>(LocalEntity.Address + OFF_VIEW_ANGLES);
		if(!decideaimorno) aimingangle= viewAngles;
		aimingangle = CalculateAngle(LocalEntity.HeadPos, HeadPosition1);
		aimingangle = SmoothLinear(viewAngles, aimingangle, aSmoothAmount / 10.f);
		//printf("%f,%f\n", AimEntity.aimx, AimEntity.aimy);
		if ((GetAsyncKeyState(Config::aim_key) || GetAsyncKeyState(Config::aim_key2)) && decideaimorno&&(!keydown||!Config::trigger) && LocalEntity.WeaponIndex != 5 && LocalEntity.WeaponIndex != 17) {
			if (AimEntity.aimx != 9999.f && AimEntity.aimy != 9999.f && AimEntity.aimx && AimEntity.aimy && !LocalEntity.DyingState&&!LocalEntity.BleedOutState)
			{
				//Aim(&LocalEntity, &AimEntity);
				// If mouse cursor shown
				CURSORINFO ci = { sizeof(CURSORINFO) };
				if (GetCursorInfo(&ci))
				{
					if (ci.flags == 0) {
						if (LocalEntity.zoomin) {
							aX = (closestX - crosshairX) / smoothzoomin;
							aY = (closestY - crosshairY) / smoothzoomin;
						}
						else {
							aX = (closestX - crosshairX) / aSmoothAmount;
							aY = (closestY - crosshairY) / aSmoothAmount;
						}
						if (Config::trackcompensate&& fabs(lastclosestX - closestX)<30.f) {
							if (fabs(lastclosestX - closestX) > fabs(aX))
								aX = (lastclosestX - closestX);
							if (fabs(lastclosestY - closestY) > fabs(aY))
								aY = (lastclosestY - closestY);
						}
						lastclosestX = closestX;
						lastclosestY = closestY;
						mouse_event(MOUSEEVENTF_MOVE, aX, aY, 0, 0);
						//write<QAngle>(LocalEntity.Address + OFF_VIEW_ANGLES, aimingangle);
					}
				}
			}
		}

		auto PunchWeapon = read<QAngle>(LocalEntity.Address + m_vecPunchWeapon_Angle);
		if (Config::NoRecoil && LocalEntity.Health > 0) {
			auto viewAngles1= read <QAngle>(LocalEntity.Address + OFF_VIEW_ANGLES);
			NewAngle.x = viewAngles1.x + ((PreviousAngle.x - PunchWeapon.x) * (Config::Recoilnumx / 100.f));
			NewAngle.y = viewAngles1.y + ((PreviousAngle.y - PunchWeapon.y) * (Config::Recoilnumy / 100.f));
			if (NewAngle.x != viewAngles1.x || NewAngle.y != viewAngles1.y)
				write<QAngle>(LocalEntity.Address + OFF_VIEW_ANGLES, NewAngle);
		}
		if (keydown && GetTickCount() - keydowntime > Config::triggertime && Config::dianshe) {
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			Sleep(Config::intervaltime);
			keydown = 0;
		}
		else if (!Config::dianshe&& keydown&&!dotrigger) {
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			keydown = 0;
		}
		PreviousAngle = PunchWeapon;
		Sleep(1);
	}
}
void mainthread() {
	while (true)
	{
		// Matrix set up
		// Local player set up
		uintptr_t locPlayer = read<uintptr_t>(oBaseAddress + OFFSET_LOCAL_ENT);

		LocalEntity.Address = locPlayer;

		if (LocalEntity.Address) {
			Vector3 tmp = GetEntityBasePosition(LocalEntity.Address);
			LocalEntity.Pos = { tmp.x,tmp.y,tmp.z };		// 坐标
			//Vector3 HeadPosition = GetEntityBonePosition(LocalEntity.Address, 8, tmp);
			
			Vec3 HeadPosition = LocalEntity.GetBonePosition(HitboxType::Head);
			
			// Convert to screen position
			int DyingState = read<int>(LocalEntity.Address + m_lifeState);
			int Bleedout = read<int>(LocalEntity.Address + OFFSET_BLEED_OUT_STATE);
			// Get screen position
			// Get entity total visible time
			entNewVisTime = read<float>(LocalEntity.Address + OFFSET_VISIBLE_TIME);
			// Get entity knocked state
			int entKnockedState = read<int>(LocalEntity.Address + OFFSET_BLEED_OUT_STATE);
			// Get player team ID
			// Get entity team ID
			int entTeamID = read<int>(LocalEntity.Address + OFFSET_TEAM);
			// Is it an enemy
			LocalEntity.Health = read<int>(LocalEntity.Address + OFF_HEALTH);			// 血量
			LocalEntity.MaxHealth = read<int>(LocalEntity.Address + OFF_MAXHEALTH);
			LocalEntity.Shield = read<int>(LocalEntity.Address + OFF_SHIELD);
			LocalEntity.MaxShield = read<int>(LocalEntity.Address + OFF_MAXSHIELD);
			LocalEntity.AbsVelocity = read<Vec3>(LocalEntity.Address + OFF_ABSVELOCITY);
			LocalEntity.zoomin= read<short>(LocalEntity.Address + OFF_ZOOMING);
			//printf("%f,%f,%f\n", LocalEntity.AbsVelocity.x, LocalEntity.AbsVelocity.y, LocalEntity.AbsVelocity.z);
			//printf("%d\n", LocalEntity.Health);
			LocalEntity.Team = entTeamID;			// 阵营
			LocalEntity.HeadPos = HeadPosition;// { HeadPosition.x, HeadPosition.y, HeadPosition.z };	// 头部坐标
			LocalEntity.DyingState = DyingState;
			LocalEntity.BleedOutState = Bleedout;
			if (!LocalEntity.DyingState && !LocalEntity.BleedOutState) {
				uintptr_t WeaponHandle = read<uintptr_t>(LocalEntity.Address + OFF_WEAPON_HANDLE);
				uintptr_t WeaponHandleMasked = WeaponHandle & 0xffff;
				uintptr_t WeaponEntity = read<uintptr_t>(oBaseAddress + OFFSET_ENTITYLIST + (WeaponHandleMasked << 5));
				LocalEntity.WeaponEntity = WeaponEntity;
				LocalEntity.WeaponIndex = read<int>(WeaponEntity + OFF_WEAPON_INDEX);
				LocalEntity.WeaponProjectileScale = read<float>(WeaponEntity + OFF_PROJECTILESCALE);
				float bulletspeed= read<float>(WeaponEntity + OFF_PROJECTILESPEED);
				if (bulletspeed != 0) {
					lastbulletspeed = bulletspeed;
				}
				LocalEntity.WeaponProjectileSpeed = lastbulletspeed;
				//printf("%d,%f,%f\n", LocalEntity.WeaponIndex, LocalEntity.WeaponProjectileSpeed, LocalEntity.WeaponProjectileScale);
			}
		}
		else {
			entities = {};
			LocalEntity = {};
			continue;
		}

		// Before entity loop starts
		int closestX = 9999;
		int closestY = 9999;
		CEntity TmpEntity{};
		CEntity tmpaim{};
		std::vector<CEntity> tmpentities = {};
		// Entity loop starts here

		for (int i = 0; i < 64; i++)
		{
			DWORD64 Entity = GetEntityById(i, oBaseAddress);
			if (Entity == 0)
				continue;
			DWORD64 EntityHandle = read<DWORD64>(Entity + OFFSET_NAME);
			std::string Identifier = read<std::string>(EntityHandle);
			LPCSTR IdentifierC = Identifier.c_str();
			if (strcmp(IdentifierC, "player"))
			{
				Vector3 tmp = GetEntityBasePosition(Entity);
				TmpEntity.Pos = { tmp.x,tmp.y,tmp.z };		// 坐标
				//Vector3 HeadPosition = GetEntityBonePosition(Entity, 8, tmp);

				Vec3 HeadPosition = TmpEntity.GetBonePosition(HitboxType::Head);
				
				// Convert to screen position
				int DyingState = read<int>(Entity + m_lifeState);
				int Bleedout = read<int>(Entity + OFFSET_BLEED_OUT_STATE);
				// Get screen position
				// Get entity total visible time
				entNewVisTime = read<float>(Entity + OFFSET_VISIBLE_TIME);
				// Get entity knocked state
				int entKnockedState = read<int>(Entity + OFFSET_BLEED_OUT_STATE);
				// Get player team ID
				int playerTeamID = read<int>(locPlayer + OFFSET_TEAM);
				// Get entity team ID
				int entTeamID = read<int>(Entity + OFFSET_TEAM);
				// Is it an enemy

				TmpEntity.Address = Entity;
				TmpEntity.Health = read<int>(Entity + OFF_HEALTH);			// 血量
				TmpEntity.MaxHealth = read<int>(Entity + OFF_MAXHEALTH);
				TmpEntity.Shield = read<int>(Entity + OFF_SHIELD);
				TmpEntity.MaxShield = read<int>(Entity + OFF_MAXSHIELD);
				TmpEntity.Team = entTeamID!=playerTeamID;			// 阵营
				TmpEntity.HeadPos = HeadPosition;// { HeadPosition.x, HeadPosition.y, HeadPosition.z };	// 头部坐标
				TmpEntity.AbsVelocity= read<Vec3>(TmpEntity.Address + OFF_ABSVELOCITY);
				TmpEntity.Name = IdentifierC;		// 类名
				TmpEntity.DyingState = DyingState;
				float WorldTime = read<float>(LocalEntity.Address + timeBase);
				TmpEntity.visible = TmpEntity.IsVisible(WorldTime);

				TmpEntity.DyingState = entKnockedState;
				TmpEntity.BleedOutState = Bleedout;
				//printf("%f,%f,%f\n", HeadPosition.x, HeadPosition.y, HeadPosition.z);
				//modifyHighlights();
				modifyHighlights();
				//TmpEntity.glow();
			}
			tmpentities.push_back(TmpEntity);
		}
		entities = tmpentities;
		// After entity loop ends
		Sleep(10);
	}
}
inline void esp() {
	ImDrawList* Draw = ImGui::GetBackgroundDrawList();
	ImVec2 CrossHair = ImVec2(screenWeight / 2.0f, screenHeight / 2.0f);
	if (Config::draw_fov)
	{
		Draw->AddCircle(CrossHair, FOV, ImGui::GetColorU32(Config::fovcol), 3000, 2);
	}
	spectatorcount = 0;
	if (!entities.empty()) {
		int in500enemies = 0;
		float closestemenydis = 999999;
		for (CEntity drawentity : entities) {
			if (drawentity.Address == LocalEntity.Address) continue;
			if (Config::spectcount) {
				float targetangle = read<float>(drawentity.Address + LocalYaw);
				float targetyaw = -targetangle; // yaw is inverted
				if (targetyaw < 0)
					targetyaw += 360;
				targetyaw += 90; // yaw is off by 90
				if (targetyaw > 360)
					targetyaw -= 360;
				float localangle = read<float>(LocalEntity.Address + LocalYaw);
				float localyaw = -localangle; // yaw is inverted
				if (localyaw < 0)
					localyaw += 360;
				localyaw += 90; // yaw is off by 90
				if (localyaw > 360)
					localyaw -= 360;

				if (targetyaw == localyaw && drawentity.Health == 0)
					spectatorcount++;
			}
			if (!drawentity.Team && !Config::espteammates) continue;
			if (drawentity.DyingState || drawentity.BleedOutState) continue;
			if (drawentity.Health == 0) continue;
			Vector3 drawpos = { drawentity.HeadPos.x,drawentity.HeadPos.y,drawentity.HeadPos.z };
			Vector3 downpos = drawentity.GetBonePositionVector3(HitboxType::Hip);//GetEntityBonePosition(drawentity.Address, id::hips, GetEntityBasePosition(drawentity.Address));
			float dis = pow(pow(LocalEntity.HeadPos.x - drawentity.HeadPos.x, 2) + pow(LocalEntity.HeadPos.y - drawentity.HeadPos.y, 2) + pow(LocalEntity.HeadPos.z - drawentity.HeadPos.z, 2), 0.5);
			if (GameUnitsToMeter(dis) < 500.f) in500enemies++;
			if (GameUnitsToMeter(dis) < closestemenydis) closestemenydis = GameUnitsToMeter(dis);
			if (GameUnitsToMeter(dis) < Config::espdist && !GameUnitsToMeter(dis) < 1) {
				if (Config::healthbar) {
					OW::Vector2 Vec2_A{}, Vec2_B{};
					if (!WorldToScreen(m, drawpos, &Vec2_A, OW::Vector2(screenWeight, screenHeight)))
						continue;
					if (!WorldToScreen(m, downpos, &Vec2_B, OW::Vector2(screenWeight, screenHeight)))
						continue;
					float height = abs(Vec2_A.Y - Vec2_B.Y);
					float width = height * 0.85;
					float Height2 = (abs(Vec2_A.Y - Vec2_B.Y) + 8) * 2;
					//OW::Render::DrawHealthBar(OW::Vector2(Vec2_B.X - width / 2, Vec2_B.Y + Height2/2), -Height2, drawentity.Health + drawentity.Shield, drawentity.MaxHealth + drawentity.MaxShield, drawentity.MaxShield, drawentity.Shield);
					OW::Render::DrawSeerLikeHealth(Vec2_A.X, Vec2_A.Y - 30.f, drawentity.Shield, drawentity.MaxShield, drawentity.Health);
				}
				if (Config::draw2dbox) {
					OW::Vector2 Vec2_A{}, Vec2_B{};
					if (!WorldToScreen(m, drawpos, &Vec2_A, OW::Vector2(screenWeight, screenHeight)))
						continue;
					if (!WorldToScreen(m, downpos, &Vec2_B, OW::Vector2(screenWeight, screenHeight)))
						continue;

					float height = abs(Vec2_A.Y - Vec2_B.Y) * 2;
					float width = height * 0.85;
					ImU32 colorvisbox;
					if (drawentity.visible) colorvisbox = ImGui::GetColorU32(Config::EnemyCol);
					else ImU32 colorvisbox = ImGui::GetColorU32(Config::invisEnemyCol);
					OW::Render::DrawCorneredBox(Vec2_A.X - (width / 2) - 1, Vec2_A.Y - (height / 5) - 1, width + 2, height + 12, colorvisbox);
					OW::Render::DrawFilledRect(Vec2_A.X - (width / 2) - 1, Vec2_A.Y - (height / 5) - 1, width + 2, height + 12, ImColor(50, 0, 0, 60));
				}
				if (Config::draw_dis) {
					OW::Vector2 Vec2_A{};
					if (!WorldToScreen(m, drawpos, &Vec2_A, OW::Vector2(screenWeight, screenHeight)))
						continue;
					Vec2_A.Y -= 80;
					std::string disstring = std::to_string((int)GameUnitsToMeter(dis));
					disstring += 'm';
					OW::Render::DrawStrokeText(ImVec2(Vec2_A.X, Vec2_A.Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1)),(char*) & disstring, 25.f);
				}
				if (Config::draw_bone) {
					OW::Vector2 Vec2_1{}, Vec2_2{}, Vec2_3{}, Vec2_4{}, Vec2_5{}, Vec2_6{}, Vec2_7{}, Vec2_8{}, Vec2_9{}, Vec2_10{}, Vec2_11{}, Vec2_12{}, Vec2_13{}, Vec2_14{}, Vec2_15{}, Vec2_16{}, Vec2_17{};
					if (!WorldToScreen(m, drawpos, &Vec2_1, OW::Vector2(screenWeight, screenHeight)))
						continue;
					if (!WorldToScreen(m, drawentity.GetBonePositionVector3(HitboxType::Neck), &Vec2_2, OW::Vector2(screenWeight, screenHeight)))
						continue;
					if (!WorldToScreen(m, drawentity.GetBonePositionVector3(HitboxType::Stomach), &Vec2_3, OW::Vector2(screenWeight, screenHeight)))
						continue;
					if (!WorldToScreen(m, drawentity.GetBonePositionVector3(HitboxType::Hip), &Vec2_4, OW::Vector2(screenWeight, screenHeight)))
						continue;

					if (drawentity.visible) {
						OW::Render::DrawLine(Vec2_1, Vec2_2, OW::Color(255, 0, 51, 255), 2);
						OW::Render::DrawLine(Vec2_2, Vec2_3, OW::Color(255, 0, 51, 255), 2);
						OW::Render::DrawLine(Vec2_3, Vec2_4, OW::Color(255, 0, 51, 255), 2);
					}
					else {
						OW::Render::DrawLine(Vec2_1, Vec2_2, OW::Color(153, 153, 255, 255), 2);
						OW::Render::DrawLine(Vec2_2, Vec2_3, OW::Color(153, 153, 255, 255), 2);
						OW::Render::DrawLine(Vec2_3, Vec2_4, OW::Color(153, 153, 255, 255), 2);
					}
				}
				if (Config::draw_line) {
					OW::Vector2 Vec2_A{}, Vec2_B{};
					if (!WorldToScreen(m, drawpos, &Vec2_A, OW::Vector2(screenWeight, screenHeight)))
						continue;
					Vec2_B = OW::Vector2(screenWeight/2, screenHeight/2);
					if (drawentity.visible) {
						OW::Render::DrawLine(Vec2_A, Vec2_B, OW::Color(204, 51, 51, 255), 2);
					}
					else {
						OW::Render::DrawLine(Vec2_A, Vec2_B, OW::Color(153, 102, 255, 255), 2);
					}
				}
			}
		}
		if (spectatorcount != 0) {
			timecountnow = GetTickCount();
			realspeccount = spectatorcount;
		}
		else
		{
			if (GetTickCount() - timecountnow > timetocountspeczero) {
				realspeccount = spectatorcount;
			}
		}
		if (Config::spectcount) {
			std::string Spec = "Spectator:" + std::to_string(realspeccount);
			if (read<int>(LocalEntity.Address + OFF_HEALTH) > 0)
				OW::Render::DrawStrokeText(ImVec2(10, 30), ImColor(255, 0, 0, 255), (const char*)&Spec, 24);
		}
		if (Config::combatalert) {
			char comalertsrt[100];
			sprintf(comalertsrt, "Combat Alert: %d enemies within 500m", (int)in500enemies);

			char closestr[100];
			sprintf(closestr, "Closest: %.1f m", closestemenydis);

			ImVec2 comalertsrtSize = ImGui::CalcTextSize(comalertsrt);
			OW::Render::DrawStrokeText(ImVec2((screenWeight - comalertsrtSize.x) / 2, 120), ImColor(255, 0, 0, 255), comalertsrt, 30);

			ImVec2 closestrSize = ImGui::CalcTextSize(closestr);
			OW::Render::DrawStrokeText(ImVec2((screenWeight - closestrSize.x) / 2, 150), ImColor(255, 0, 0, 255), closestr, 30);
		}
	}
}
void overlaythread() {
	//__try {
		int doingone = 0;
		int tab_index = 0;
		std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point b = std::chrono::system_clock::now();
		std::string themenuname = random_string_window().c_str();
		HWND tWnd = FindWindowA(skCrypt("Respawn001"), NULL);

		WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, random_string_window().c_str(), NULL };
		RegisterClassEx(&wc);
		hwnd = CreateWindow(wc.lpszClassName, random_string_window().c_str(), WS_POPUP, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);

		if (!CreateDeviceD3D(hwnd))
		{
			CleanupDeviceD3D();
			UnregisterClass(wc.lpszClassName, wc.hInstance);
		}

		MARGINS margins = { -1 };
		DwmExtendFrameIntoClientArea(hwnd, &margins);

		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);

		ShowWindow(hwnd, SW_SHOWDEFAULT);
		UpdateWindow(hwnd);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromMemoryTTF((void*)SimHei_data, sizeof(SimHei_data), 18.f, NULL, io.Fonts->GetGlyphRangesChineseFull());
		//ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.Fonts->AddFontFromFileTTF(".\\SimHei.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

		//ImGui::StyleColorsDark();
		ImCandy::Theme_Blender();
		//ImGui::StyleColorsLight();


		ImGui_ImplWin32_Init(tWnd);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		int FPS;
		DEVMODE dm;
		while (FindWindowA(skCrypt("Respawn001"), NULL))
		{

			dm.dmSize = sizeof(DEVMODE);

			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

			//FPS = dm.dmDisplayFrequency;
			//FPS = 60;
			FPS = Config::MenuFPS;
			a = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> work_time = a - b;
			if (work_time.count() < 1000 / FPS)
			{
				std::chrono::duration<double, std::milli> delta_ms(1000 / FPS - work_time.count());
				auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
				std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
			}

			b = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> sleep_time = b - a;
			//viewMatrix = SDK->RPM<Matrix>(viewMatrixPtr);
			//viewMatrix_xor = SDK->RPM<Matrix>(viewMatrix_xor_ptr);
			DWORD Style = GetWindowLong(tWnd, GWL_STYLE);
			RECT rect;
			GetWindowRect(tWnd, &rect);

			SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_NOZORDER);

			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				//continue;
			}

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				ImGui::GetIO().MouseDown[0] = true;
			}
			else
			{
				ImGui::GetIO().MouseDown[0] = false;
			}

			if (GetAsyncKeyState(VK_INSERT)) {
				Config::Menu = !Config::Menu;
				//ImGui::GetIO().MouseDrawCursor = Config::Menu;
				Sleep(250);
			}
			if (!Config::Menu && doingone == 0) {
				doingone = 1;
				SetFocus(tWnd);
				SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);
			}
			if (Config::Menu && doingone == 1)
			{
				SetFocus(hwnd);
				ShowCursor(TRUE);
				SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW);
				//UpdateWindow(hwnd);
				//Config::doingentity = 0;
				//Sleep(100);
				//Config::doingentity = 1;
				//_beginthread((_beginthread_proc_type)entity_thread, 0, 0);
				doingone = 0;
			}
			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			if (!Config::Menu) themenuname = random_string_window().c_str();
			if (Config::Menu)
			{
				ImGui::SetNextWindowPos(ImVec2{ 50, 30 }, ImGuiCond_Once);
				ImGui::SetNextWindowSize(ImVec2{ 500, 600 }, ImGuiCond_Once);
				bool _visible = true;
				//if (ImGui::Begin(skCrypt(u8"Imperial Beta"), &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {
				if (ImGui::Begin((char*) & themenuname, &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar)) {
					//ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.8f, 1.0f));
					ImGui::Text(skCrypt("Rigel External"));
					ImGui::PopStyleColor(1);
					if (ImGui::Button(skCrypt(u8"AIM"), ImVec2(150, 50)))
						tab_index = 1;

					ImGui::SameLine();

					if (ImGui::Button(skCrypt(u8"ESP"), ImVec2(150, 50)))
						tab_index = 2;
					ImGui::SameLine();

					if (ImGui::Button(skCrypt(u8"MISC"), ImVec2(150, 50)))
						tab_index = 3;

					ImGui::Separator();

					if (tab_index == 1) {
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.0f));
						ImGui::Toggle(skCrypt(u8"TriggerBot"), &Config::trigger, ImGuiToggleFlags_Animated);
						if (Config::trigger) {
							ImGui::Toggle(skCrypt(u8"Click"), &Config::dianshe, ImGuiToggleFlags_Animated);
							if (Config::dianshe) {
								ImGui::BulletText(u8"Hold on Fire for:");
								ImGui::SliderInt(u8"Delay Time", &Config::triggertime, 10, 1000, "%d");
								ImGui::BulletText(u8"After one shot, wait for:");
								ImGui::SliderInt(u8"Interval", &Config::intervaltime, 10, 1000, "%d");
								ImGui::Spacing();
							}
							ImGui::SliderFloat(u8"Trigger Distance", &Config::triggerdisnotzoom, 0.1f, 50.f, "%.2f");
							ImGui::SliderFloat(u8"Trigger Distance (Zoom)", &Config::triggerdiszoom, 0.1f, 200.f, "%.2f");
							ImGui::Spacing();
						}
						ImGui::Toggle(skCrypt(u8"Prediction"), &Config::Prediction, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Tracking Compensate"), &Config::trackcompensate, ImGuiToggleFlags_Animated);
						ImGui::PopStyleColor(1);
						ImGui::SliderFloat(u8"Smooth", &aSmoothAmount, 0.1f, 20.f, "%.2f");
						ImGui::SliderFloat(u8"Smooth(Zoom)", &smoothzoomin, 0.1f, 20.f, "%.2f");
						ImGui::SliderFloat(u8"Fov", &FOV, 0.1f, 500.f, "%.2f");
						ImGui::BulletText(skCrypt(u8"KeyBind"));
						if (ImGui::BeginCombo(skCrypt(u8"##Key"), keys))
						{
							for (int i = 0; i < 5; i++)
							{
								const bool type = keys == key_type[i];
								if (ImGui::Selectable(key_type[i], type))
								{
									keys = key_type[i];
									switch (i)
									{
									case 0:
										Config::aim_key = VK_LBUTTON;
										break;
									case 1:
										Config::aim_key = VK_RBUTTON;
										break;
									case 2:
										Config::aim_key = VK_MBUTTON;
										break;
									case 3:
										Config::aim_key = VK_XBUTTON1;
										break;
									case 4:
										Config::aim_key = VK_XBUTTON2;
										break;
									}
								}
							}
							ImGui::EndCombo();
						}
						ImGui::BulletText(skCrypt(u8"KeyBind2"));
						if (ImGui::BeginCombo(skCrypt(u8"##Key2"), keys2))
						{
							for (int i = 0; i < 5; i++)
							{
								const bool type = keys2 == key_type2[i];
								if (ImGui::Selectable(key_type2[i], type))
								{
									keys2 = key_type2[i];
									switch (i)
									{
									case 0:
										Config::aim_key2 = VK_LBUTTON;
										break;
									case 1:
										Config::aim_key2 = VK_RBUTTON;
										break;
									case 2:
										Config::aim_key2 = VK_MBUTTON;
										break;
									case 3:
										Config::aim_key2 = VK_XBUTTON1;
										break;
									case 4:
										Config::aim_key2 = VK_XBUTTON2;
										break;
									}
								}
							}
							ImGui::EndCombo();
						}
						ImGui::BulletText(skCrypt(u8"BoneSelect"));
						if (ImGui::BeginCombo(skCrypt(u8"##Bone"), bones))
						{
							for (int i = 0; i < 4; i++)
							{
								const bool type = bones == bones_type[i];
								if (ImGui::Selectable(bones_type[i], type))
								{
									bones = bones_type[i];
									switch (i)
									{
									case 0:
										Config::bone = 0;
										break;
									case 1:
										Config::bone = 1;
										break;
									case 2:
										Config::bone = 2;
										break;
									case 3:
										Config::bone = 3;
										break;
									}
								}
							}
							ImGui::EndCombo();
						}
					}

					if (tab_index == 2) {
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.0f));
						//ImGui::Toggle(skCrypt(u8"GlowESP"), &Config::BodyEsp, ImGuiToggleFlags_Animated);
						//if (Config::BodyEsp) {
						//	ImGui::Toggle(skCrypt(u8"Enable Shield Color"), &Config::BodyEspShield, ImGuiToggleFlags_Animated);
						//}
						//else Config::BodyEspShield = false;
						ImGui::PopStyleColor(1);
						if (!Config::BodyEspShield) {
							ImGui::ColorEdit3(skCrypt(u8"Visible Enemy Color"), (float*)&Config::enargb);
							ImGui::ColorEdit3(skCrypt(u8"Invisible Enemy Color"), (float*)&Config::invisenargb);
						}
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.0f));
						ImGui::Toggle(u8"ItemESP", &Config::ItemEsp, ImGuiToggleFlags_Animated);
						ImGui::Spacing();
						ImGui::PopStyleColor(1);
						ImGui::BulletText(skCrypt(u8"Drawing Options"));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.0f));
						ImGui::Toggle(skCrypt(u8"Combat Alert"), &Config::combatalert, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Draw Distance"), &Config::draw_dis, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Draw Line"), &Config::draw_line, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Draw Bone"), &Config::draw_bone, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Draw Spec"), &Config::spectcount, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Draw Fov"), &Config::draw_fov, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Draw Teammates"), &Config::espteammates, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Draw HealthBar"), &Config::healthbar, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Draw 2D Box"), &Config::draw2dbox, ImGuiToggleFlags_Animated);
						ImGui::PopStyleColor(1);
						ImGui::SliderFloat(u8"Max Drawing Distance", &Config::espdist, 10.f, 1000.f, "%.1f");

						ImGui::ColorEdit3(skCrypt(u8"Invisible Enemy Box"), (float*)&Config::EnemyCol);
						ImGui::ColorEdit3(skCrypt(u8"Visible Enemy Box"), (float*)&Config::invisEnemyCol);
					}
					if (tab_index == 3) {
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.0f));
						//ImGui::Toggle(skCrypt(u8"Heirloom", &Config::Heirloom, ImGuiToggleFlags_Animated);
						ImGui::SliderInt(u8"Menu Frame Rate", &Config::MenuFPS, 60, 200, "%d");
						ImGui::Toggle(skCrypt(u8"Free Cam"), &Config::freecam, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"Fix Camera"), &Config::freecam2, ImGuiToggleFlags_Animated);
						//ImGui::Toggle(u8"Third Person", &Config::thirdpp, ImGuiToggleFlags_Animated);

						if (Config::freecam) Config::freecam2 = false;
						if (Config::freecam2) Config::freecam = false;
						ImGui::Spacing();
						ImGui::Toggle(skCrypt(u8"No Spread"), &Config::nospread, ImGuiToggleFlags_Animated);
						ImGui::Toggle(skCrypt(u8"No Recoil"), &Config::NoRecoil, ImGuiToggleFlags_Animated);
						
						ImGui::SliderFloat(skCrypt(u8"Recoil Restrain X"), &Config::Recoilnumx, 1.f, 100.f, "%.1f");
						ImGui::SliderFloat(skCrypt(u8"Recoil Restrain Y"), &Config::Recoilnumy, 1.f, 100.f, "%.1f");
						ImGui::Spacing();
						ImGui::BulletText(skCrypt("HotKeys:"));
						ImGui::Text("F5 ----- MapRadar");
						ImGui::Text("F6 ----- SkinChanger");
						ImGui::Text("F7 ----- Free Cam Switch");
						ImGui::PopStyleColor(1);
					}
				}
				ImGui::End();
			}

			esp();
			ImGui::EndFrame();
			ImGui::Render();
			g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
			auto ss = ImVec4(0, 0, 0, 0);
			g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&ss);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			g_pSwapChain->Present(1, 0);
		}

		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
		::DestroyWindow(hwnd);
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
	//}
	//__except (1) {

	//}
}