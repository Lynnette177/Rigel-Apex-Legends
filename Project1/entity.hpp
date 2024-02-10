#pragma once
#include"includes.hpp"
#include "structures.hpp"
#include "Driver.h"
#include "offsets.h"

DWORD64 GetEntityById(int Ent, DWORD64 Base)
{
	DWORD64 EntityList = Base + OFFSET_ENTITYLIST; //updated
	DWORD64 BaseEntity = read<DWORD64>(EntityList);
	if (!BaseEntity)
		return NULL;
	return  read<DWORD64>(EntityList + (Ent << 5));
}
uintptr_t GetEntityBoneArray(uintptr_t ent)
{
	return read<uintptr_t>(ent + OFFSET_BONES);
}

Vector3 GetEntityBonePosition(uintptr_t ent, uint32_t BoneId, Vector3 BasePosition)
{
	unsigned long long pBoneArray = GetEntityBoneArray(ent);

	Vector3 EntityHead = Vector3();

	EntityHead.x = read<float>(pBoneArray + 0xCC + (BoneId * 0x30)) + BasePosition.x;
	EntityHead.y = read<float>(pBoneArray + 0xDC + (BoneId * 0x30)) + BasePosition.y;
	EntityHead.z = read<float>(pBoneArray + 0xEC + (BoneId * 0x30)) + BasePosition.z;

	return EntityHead;
}
Vector3 GetEntityBasePosition(uintptr_t ent)
{
	return read<Vector3>(ent + OFFSET_ORIGIN);
}

class CEntity	// 游戏实体类
{
public:
	DWORD64 Address = 0;	// 实体地址
	int Health = 0;			// 血量
	int MaxHealth = 0;
	int Shield = 0;
	int MaxShield = 0;

	int	Team = -1;			// 阵营
	Vec3 Pos{ 0,0,0 };		// 坐标
	Vector3 Camerapos{ 0,0,0 };
	Vec3 HeadPos{ 0,0,0 };	// 头部坐标
	Vec3 AbsVelocity{ 0,0,0 };
	Vec2 HeadScreenPos{ -1,-1 };// 头部屏幕坐标
	std::string Name;		// 类名
	int DyingState = 0;		// 濒死状态
	int BleedOutState = 0;
	bool visible = false;
	float aimx = 9999;
	float aimy = 9999;
	uintptr_t WeaponEntity=0;
	int WeaponIndex=0;
	float WeaponProjectileSpeed=0;
	float WeaponProjectileScale=0;
	bool IsAimedAt = false;
	bool zoomin = false;
	bool IsTarget(float WorldTime)
	{
		float Time1;
		Time1 = read<float>(this->Address + OFF_LAST_AIMEDAT_TIME);
		return (Time1 + 0.2) >= WorldTime;
	}
	bool IsVisible(float WorldTime)
	{
		float Time1;
		Time1 = read<float>(this->Address + lastVisibleTime);
		return (Time1 + 0.2) >= WorldTime;
	}
	void glow() {
		if (Config::BodyEsp) {
			if (Team) {
				int glowEnable = read<int>(Address + OFFSET_GLOW_ENABLE);
				int glowThroughWall = read<int>(Address + OFFSET_GLOW_THROUGH_WALLS);
				int highlightId = read<int>(Address + OFFSET_GLOW_HIGHLIGHT_ID + 1);
				if (!Config::BodyEspShield) {
					if (glowEnable != 1) write<int>(Address + OFFSET_GLOW_ENABLE, 1);
					if (glowThroughWall != 2) write<int>(Address + OFFSET_GLOW_THROUGH_WALLS, 2);
					if (glowThroughWall != 2) write<int>(Address + OFFSET_GLOW_FIX, 2);
					int id = (visible) ? 0 : 1;
					//if (aimbotLocked) id = 2;
					if (highlightId != id) write<int>(Address + OFFSET_GLOW_HIGHLIGHT_ID + 1, id);
				}
				else {
					if (glowEnable != 1) write<int>(Address + OFFSET_GLOW_ENABLE, 1);
					if (glowThroughWall != 2) write<int>(Address + OFFSET_GLOW_THROUGH_WALLS, 2);
					if (glowThroughWall != 2) write<int>(Address + OFFSET_GLOW_FIX, 2);
					int id;
					if (Shield <= 0) id = 90;//no shields
					else if (Shield <= 50) id = 91;//white shields 
					else if (Shield <= 70) id = 92;//blue shields
					else if (Shield <= 100) id = 93;//purple shields / gold
					else  id = 94;//red shields
					if (highlightId != id) write<int>(Address + OFFSET_GLOW_HIGHLIGHT_ID + 1, id);
				}

			}
			else {
				int glowEnable = read<int>(Address + OFFSET_GLOW_ENABLE);
				int glowThroughWall = read<int>(Address + OFFSET_GLOW_THROUGH_WALLS);
				int highlightId = read<int>(Address + OFFSET_GLOW_HIGHLIGHT_ID + 1);
				if (glowEnable != 1) write<int>(Address + OFFSET_GLOW_ENABLE, 1);
				if (glowThroughWall != 2) write<int>(Address + OFFSET_GLOW_THROUGH_WALLS, 2);
				if (glowThroughWall != 2) write<int>(Address + OFFSET_GLOW_FIX, 2);
				int id = 2;
				if (highlightId != id) write<int>(Address + OFFSET_GLOW_HIGHLIGHT_ID + 1, id);
			}
		}
		else {
			int glowEnable = read<int>(Address + OFFSET_GLOW_ENABLE);
			int glowThroughWall = read<int>(Address + OFFSET_GLOW_THROUGH_WALLS);
			int highlightId = read<int>(Address + OFFSET_GLOW_HIGHLIGHT_ID + 1);
			if (glowEnable != 1) write<int>(Address + OFFSET_GLOW_ENABLE, 1);
			if (glowThroughWall != 2) write<int>(Address + OFFSET_GLOW_THROUGH_WALLS, 2);
			if (glowThroughWall != 2) write<int>(Address + OFFSET_GLOW_FIX, 2);
			int id = 2;
			if (highlightId != id) write<int>(Address + OFFSET_GLOW_HIGHLIGHT_ID + 1, id);
		}
	}
	bool IsCombatReady() {
		if (!Address)return false;
		if (DyingState) return false;
		if (BleedOutState) return false;
		if (Health == 0) return false;
		return true;
	}
	int GetBoneFromHitbox(HitboxType HitBox) const {
		uintptr_t ModelPointer = read<uintptr_t>(this->Address + OFF_STUDIOHDR);
		if (!(ModelPointer))
			return -1;

		uintptr_t StudioHDR = read<uintptr_t>(ModelPointer + 0x8);
		if (!(StudioHDR))
			return -1;

		auto HitboxCache = read<uint16_t>(StudioHDR + 0x34);
		auto HitboxArray = StudioHDR + ((uint16_t)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1)));
		if (!(HitboxArray))
			return -1;

		auto IndexCache = read<uint16_t>(HitboxArray + 0x4);
		auto HitboxIndex = ((uint16_t)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));
		auto BonePointer = HitboxIndex + HitboxArray + (static_cast<int>(HitBox) * 0x20);
		if (!(BonePointer))
			return -1;

		return read<uint16_t>(BonePointer);
	}

	Vec3 GetBonePosition(HitboxType HitBox) const {
		Vec3 Offset = Vec3(0.0f, 0.0f, 0.0f);
		Vec3 OrPosition = this->Pos;
		int Bone = GetBoneFromHitbox(HitBox);
		if (Bone < 0 || Bone > 255)
			return Vec3{ OrPosition.x + Offset.x, OrPosition.y + Offset.y ,OrPosition.z + Offset.z };

		uintptr_t BonePtr = read<uintptr_t>(this->Address + OFF_BONES);
		BonePtr += (Bone * sizeof(Matrix3x4));
		if (!(BonePtr))
			return Vec3{ OrPosition.x + Offset.x, OrPosition.y + Offset.y ,OrPosition.z + Offset.z };

		Matrix3x4 BoneMatrix = read<Matrix3x4>(BonePtr);
		Vec3 BonePosition = BoneMatrix.GetPosition();

		//if (!BonePosition.IsValid())
		//	return LocalOrigin.Add(Vector3D(0, 0, 0));

		BonePosition = { BonePosition.x+OrPosition.x,BonePosition.y + OrPosition.y ,BonePosition.z + OrPosition.z };
		return BonePosition;
	}
	Vector3 GetBonePositionVector3(HitboxType HitBox) const {
		Vec3 Offset = Vec3(0.0f, 0.0f, 0.0f);
		Vec3 OrPosition = this->Pos;
		int Bone = GetBoneFromHitbox(HitBox);
		if (Bone < 0 || Bone > 255)
			return Vector3{ OrPosition.x + Offset.x, OrPosition.y + Offset.y ,OrPosition.z + Offset.z };

		uintptr_t BonePtr = read<uintptr_t>(this->Address + OFF_BONES);
		BonePtr += (Bone * sizeof(Matrix3x4));
		if (!(BonePtr))
			return Vector3{ OrPosition.x + Offset.x, OrPosition.y + Offset.y ,OrPosition.z + Offset.z };

		Matrix3x4 BoneMatrix = read<Matrix3x4>(BonePtr);
		Vec3 BonePosition = BoneMatrix.GetPosition();

		//if (!BonePosition.IsValid())
		//	return LocalOrigin.Add(Vector3D(0, 0, 0));

		Vector3 BonePositionreturn = { BonePosition.x + OrPosition.x,BonePosition.y + OrPosition.y ,BonePosition.z + OrPosition.z };
		return BonePositionreturn;
	}
};