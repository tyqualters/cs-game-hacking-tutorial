#pragma once
#include <cmath>
#include <cstdint>

const float PI = 3.14159265359f;

// Glow Object
struct glow_t
{
	float r, g, b, a;
	char padding_0000[16];
	bool RenderWhenOccluded;
	bool RenderWhenUnoccluded;
	bool FullBloom;
	char padding_0001;
	int FullBloomStencilTestValue;
	int GlowStyle;
	char padding_0002[10];
};

// Matrix 3x4
struct Matrix_3x4_t
{
	float c[3][4];
};

// Vector
struct Vector
{
	float x, y, z;
	inline bool isEmpty()
	{
		return x == 0 && y == 0 && z == 0;
	}
};

// CBaseEntity
class CBaseEntity
{
public:
	char padding_0000[236];
	bool IsDormant;
	char padding_0002[6];
	int32_t TeamNum;
	char padding_0003[8];
	int32_t Health;
	int32_t Flags;
	char padding_0004[48];
	Vector VecOrigin;
	char padding_0005[2041];
	bool Spotted;
	char padding_0006[66];
	bool SpottedByMask;
	char padding_0007[4355];
	int32_t CompetitiveRanking;
	char padding_0008[256];
	int32_t CompetitiveWins;
	char padding_0009[2844];
	unsigned BoneMatrix;
	char padding_000A[236];
	Matrix_3x4_t Abdomen;
	Matrix_3x4_t Stomach;
	Matrix_3x4_t Neck;
	Matrix_3x4_t Head;
	char padding_000E[1904];
	int32_t AccountId;
	char padding_000F[84];
	Vector ViewPunchAngle;
	Vector AimPunchAngle;
	char padding_0010[524];
	int32_t Clip1;
	char padding_0012[200];
	float AccuracyPenalty;
	char padding_0013[1532];
	bool IsScoped;
	char padding_0015[7];
	bool IsDefusing;
	char padding_0016[19];
	bool GunGameImmunity;
	char padding_0017[27359];
	int32_t GlowIndex;
	char padding_0018[3876];
	bool HasHelmet;
	char padding_0019[11];
	int32_t ArmorValue;
	char padding_001A[12];
	bool HasDefuser;
};

// LocalPlayer
class C_BasePlayer
{
public:
	char padding_0000[243];
	int32_t TeamNum;
	char padding_0001[8];
	int32_t Health;
	int32_t Flags;
	char padding_0002[48];
	Vector VecOrigin;
	char padding_0003[6464];
	int32_t CompetitiveRanking;
	char padding_0004[256];
	int32_t CompetitiveWins;
	char padding_0005[2844];
	unsigned BoneMatrix;
	char padding_0006[236];
	Matrix_3x4_t Abdomen;
	Matrix_3x4_t Stomach;
	Matrix_3x4_t Neck;
	Matrix_3x4_t Head;
	char padding_0007[1904];
	int32_t AccountId;
	char padding_0008[84];
	Vector ViewPunchAngle;
	Vector AimPunchAngle;
	char padding_0009[428];
	int FOV;
	int FOVStart;
	char padding_000A[88];
	int32_t Clip1;
	char padding_000B[200];
	float AccuracyPenalty;
	char padding_000C[280];
	int TickBase;
	char padding_000D[1248];
	bool IsScoped;
	char padding_000E[7];
	bool IsDefusing;
	char padding_000F[19];
	bool GunGameImmunity;
	char padding_0010[27219];
	int ShotsFired;
	char padding_0011[108];
	int FlashMaxAlpha;
	int FlashDuration;
	char padding_0012[20];
	int32_t GlowIndex;
	char padding_0013[3876];
	bool HasHelmet;
	char padding_0014[11];
	int32_t ArmorValue;
	char padding_0015[12];
	bool HasDefuser;
	char padding_0016[91];
	int CrosshairId;
};

void NormalizeAngles(Vector& TargetVector)
{
	if (TargetVector.x > 180.f) TargetVector.x -= 360.f;
	if (TargetVector.x < -180.f) TargetVector.x += 360.f;
	if (TargetVector.y > 180.f) TargetVector.y -= 360.f;
	if (TargetVector.y < -180.f) TargetVector.y += 360.f;
}

void ClampAngles(Vector& TargetVector)
{
	if (TargetVector.x >= 89.f)
	{
		TargetVector.x = 88.9f;
	}
	else if (TargetVector.x <= -89.f)
	{
		TargetVector.x = -88.9f;
	}

	if (TargetVector.y >= 180.f)
	{
		TargetVector.x = 179.9f;
	}
	else if (TargetVector.y <= -180.f)
	{
		TargetVector.y = -179.9f;
	}

	TargetVector.z = 0.f;
}

void SetVectorAsEntityPosition(Vector& TargetVector, CBaseEntity* pEntity, int boneID = 8)
{
	uintptr_t entity_bone = pEntity->BoneMatrix;
	if (entity_bone == NULL) return;
	TargetVector.x = dereference_pointer<float>(entity_bone + 0x30 * boneID + 0x0C);
	TargetVector.y = dereference_pointer<float>(entity_bone + 0x30 * boneID + 0x1C);
	TargetVector.z = dereference_pointer<float>(entity_bone + 0x30 * boneID + 0x2C);
}

float Calc3DDistance(Vector& Src, Vector& Dst)
{
	return sqrt(pow(Dst.x - Src.x, 2) + pow(Dst.y - Src.y, 2) + pow(Dst.z - Src.z, 2));
}

float Calc2DDistance(Vector& Src, Vector& Dst)
{
	return sqrt(pow(Dst.x - Src.x, 2) + pow(Dst.y - Src.y, 2));
}

Vector CalcAngles(Vector& Src, Vector& Dst)
{
	Vector Delta = { Dst.x - Src.x, Dst.y - Src.y, Dst.z - Src.z };
	float DeltaLength = sqrtf(Delta.x * Delta.x + Delta.y * Delta.y + Delta.z * Delta.z);
	return Vector{ -asin(Delta.z / DeltaLength) * (180 / PI), atan2(Delta.y, Delta.x), 0.f };
}