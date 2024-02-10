#pragma once
#define M_PI       3.14159265358979323846
#include"includes.hpp"
#include <algorithm>
#include "GlobalVar.hpp"
#include "entity.hpp"
#include<complex>
int RoundHalfEven(float x) {
	return (x >= 0.0)
		? static_cast<int>(std::round(x))
		: static_cast<int>(std::round(-x)) * -1;
}

float AL1AF0(float num) {
	if (num > 0)
		return max(num, 1.0f);
	return min(num, -1.0f);
}
float CalculatePitchIncrement(QAngle AimbotDesiredAngles, QAngle ViewAngles) {
	float wayA = AimbotDesiredAngles.x - ViewAngles.x;
	float wayB = 180 - abs(wayA);
	if (wayA > 0 && wayB > 0)
		wayB *= -1;
	if (fabs(wayA) < fabs(wayB))
		return wayA;
	return wayB;
}

float CalculateYawIncrement(QAngle AimbotDesiredAngles, QAngle ViewAngles) {
	float wayA = AimbotDesiredAngles.y - ViewAngles.y;
	float wayB = 360 - abs(wayA);
	if (wayA > 0 && wayB > 0)
		wayB *= -1;
	if (fabs(wayA) < fabs(wayB))
		return wayA;
	return wayB;
}
inline QAngle CalculateAngle(Vec3 from, Vector3 to) {
	Vec3 newDirection = { GameUnitsToMeter(to.x - from.x),GameUnitsToMeter(to.y - from.y),GameUnitsToMeter(to.z - from.z) };
	float lens = sqrtf(newDirection.x * newDirection.x + newDirection.y * newDirection.y + newDirection.z * newDirection.z);
	/*if (lens > 0) {
		newDirection.x /= lens;
		newDirection.y /= lens;
		newDirection.z /= lens;
	}*/
	float pitch = atan2f(newDirection.z * (-1.f), lens) * (180.f / M_PI);//-asinf(newDirection.z) * (180 / M_PI);
	float yaw = atan2f(newDirection.y, newDirection.x) * (180.f / M_PI);
	QAngle returnangle{ pitch, yaw };
	return returnangle;
}
inline QAngle SmoothLinear(QAngle LocalAngle, QAngle TargetAngle, float speed)
{
	QAngle Result = LocalAngle;

	float deltax, deltay;
	deltax = (TargetAngle.x - LocalAngle.x) * speed;
	deltay = (TargetAngle.y - LocalAngle.y) * speed;

	Result.x = LocalAngle.x + deltax;
	Result.y = LocalAngle.y + deltay;

	return Result;
}
inline QAngle SmoothAccelerate(QAngle LocalAngle, QAngle TargetAngle, float Speed, float Acc)
{
	QAngle Result = LocalAngle;
	__try
	{
		QAngle delta = TargetAngle - LocalAngle;

		float tmp = Acc / sqrt(delta.x* delta.x+ delta.y * delta.y);
		tmp = tmp * tmp * 0.005f;

		float c = Speed;
		float chu = tmp * Speed;
		c += chu;

		if (c >= 1) c = 1;

		Result.x += delta.x * c;
		Result.y += delta.y * c;
		return Result;
	}
	__except (1)
	{
		return Result;
	}
}

bool WorldToScreen(Matrix matrix,Vector3 pos, OW::Vector2* OutPos, OW::Vector2 WindowSize, bool ignoreret = false)
{
	struct Vector3 out;
	float _x = matrix.matrix[0] * pos.x + matrix.matrix[1] * pos.y + matrix.matrix[2] * pos.z + matrix.matrix[3];
	float _y = matrix.matrix[4] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[6] * pos.z + matrix.matrix[7];
	out.z = matrix.matrix[12] * pos.x + matrix.matrix[13] * pos.y + matrix.matrix[14] * pos.z + matrix.matrix[15];

	_x *= 1.f / out.z;
	_y *= 1.f / out.z;

	int width = screenWeight;
	int height = screenHeight;

	out.x = width * .5f;
	out.y = height * .5f;

	out.x += 0.5f * _x * width + 0.5f;
	out.y -= 0.5f * _y * height + 0.5f;

	if (out.x < 0 || out.y < 0 || out.y >= screenWeight || out.y >= screenHeight) { return false; }
	OutPos->X = out.x;
	OutPos->Y = out.y;
	return out.z > 0.001f;
}

struct Vector3 _WorldToScreen(const struct Vector3 pos, struct Matrix matrix) {
	struct Vector3 out;
	float _x = matrix.matrix[0] * pos.x + matrix.matrix[1] * pos.y + matrix.matrix[2] * pos.z + matrix.matrix[3];
	float _y = matrix.matrix[4] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[6] * pos.z + matrix.matrix[7];
	out.z = matrix.matrix[12] * pos.x + matrix.matrix[13] * pos.y + matrix.matrix[14] * pos.z + matrix.matrix[15];
	if (out.z > 0.001f) {
		_x *= 1.f / out.z;
		_y *= 1.f / out.z;

		int width = screenWeight;
		int height = screenHeight;

		out.x = width * .5f;
		out.y = height * .5f;

		out.x += 0.5f * _x * width + 0.5f;
		out.y -= 0.5f * _y * height + 0.5f;

		return out;
	}
	else return Vector3{9999, 9999};
}
	inline void SolveQuartic(const std::complex<float> coefficients[5], std::complex<float> roots[4]) {
		const std::complex<float> a = coefficients[4];
		const std::complex<float> b = coefficients[3] / a;
		const std::complex<float> c = coefficients[2] / a;
		const std::complex<float> d = coefficients[1] / a;
		const std::complex<float> e = coefficients[0] / a;

		const std::complex<float> Q1 = c * c - 3.f * b * d + 12.f * e;
		const std::complex<float> Q2 = 2.f * c * c * c - 9.f * b * c * d + 27.f * d * d + 27.f * b * b * e - 72.f * c * e;
		const std::complex<float> Q3 = 8.f * b * c - 16.f * d - 2.f * b * b * b;
		const std::complex<float> Q4 = 3.f * b * b - 8.f * c;

		const std::complex<float> Q5 = std::pow(Q2 / 2.f + std::sqrt(Q2 * Q2 / 4.f - Q1 * Q1 * Q1), 1.f / 3.f);
		const std::complex<float> Q6 = (Q1 / Q5 + Q5) / 3.f;
		const std::complex<float> Q7 = 2.f * std::sqrt(Q4 / 12.f + Q6);

		roots[0] = (-b - Q7 - std::sqrt(4.f * Q4 / 6.f - 4.f * Q6 - Q3 / Q7)) / 4.f;
		roots[1] = (-b - Q7 + std::sqrt(4.f * Q4 / 6.f - 4.f * Q6 - Q3 / Q7)) / 4.f;
		roots[2] = (-b + Q7 - std::sqrt(4.f * Q4 / 6.f - 4.f * Q6 + Q3 / Q7)) / 4.f;
		roots[3] = (-b + Q7 + std::sqrt(4.f * Q4 / 6.f - 4.f * Q6 + Q3 / Q7)) / 4.f;
	}
	inline void AimCorrection(Vec3 LocalPos, float scale1, Vector3* InVecArg, Vec3 currVelocity, float Bulletspeed)
	{
		double G = GameUnitsToMeter(750.0f / scale1);
		double A = GameUnitsToMeter(LocalPos.y);
		double B = GameUnitsToMeter(LocalPos.z);
		double C = GameUnitsToMeter(LocalPos.x);
		double M = GameUnitsToMeter(InVecArg->y);
		double N = GameUnitsToMeter(InVecArg->z);
		double O = GameUnitsToMeter(InVecArg->x);
		double P = GameUnitsToMeter(currVelocity.y);
		double Q = GameUnitsToMeter(currVelocity.z);
		double R = GameUnitsToMeter(currVelocity.x);
		double S = GameUnitsToMeter(Bulletspeed);
		double H = M - A;
		double J = O - C;
		double K = N - B;
		double L = -.5f * G;
		double c4 = L * L;
		double c3 = -2 * Q * L;
		double c2 = (Q * Q) - (2 * K * L) - (S * S) + (P * P) + (R * R);
		double c1 = (2 * K * Q) + (2 * H * P) + (2 * J * R);
		double c0 = (K * K) + (H * H) + (J * J);

		std::complex<float> pOutRoots[4];
		const std::complex<float> pInCoeffs[5] = { c0, c1, c2, c3, c4 };
		SolveQuartic(pInCoeffs, pOutRoots);
		float fBestRoot = FLT_MAX;
		for (int i = 0; i < 4; i++) {
			if (pOutRoots[i].real() > 0.f && std::abs(pOutRoots[i].imag()) < 0.0001f && pOutRoots[i].real() < fBestRoot) {
				fBestRoot = pOutRoots[i].real();
			}
		}
		InVecArg->y = MeterToGameUnits(A + (H + P * fBestRoot));
		InVecArg->z = MeterToGameUnits(B + (K + Q * fBestRoot - L * fBestRoot * fBestRoot));
		InVecArg->x = MeterToGameUnits(C + (J + R * fBestRoot));
	}
		/*inline void AimCorrection(Vector3 LocalPos, float scale1, Vector3* InVecArg, Vec3 currVelocity, float Bulletspeed)
	{
		double G = scale1 / 750.f;//(750.0f / scale1 / 2.f);
		double A = (LocalPos.x);
		double B = (LocalPos.y);
		double C = (LocalPos.z);
		double M = (InVecArg->x);
		double N = (InVecArg->y);
		double O = (InVecArg->z);
		double P = (currVelocity.x);
		double Q = (currVelocity.y);
		double R = (currVelocity.z);
		double S = (Bulletspeed);
		double H = M - A;
		double J = O - C;
		double K = N - B;
		double L = -.5f * G;
		double c4 = L * L;
		double c3 = -2 * Q * L;
		double c2 = (Q * Q) - (2 * K * L) - (S * S) + (P * P) + (R * R);
		double c1 = (2 * K * Q) + (2 * H * P) + (2 * J * R);
		double c0 = (K * K) + (H * H) + (J * J);

		std::complex<float> pOutRoots[4];
		const std::complex<float> pInCoeffs[5] = { c0, c1, c2, c3, c4 };
		SolveQuartic(pInCoeffs, pOutRoots);
		float fBestRoot = FLT_MAX;
		for (int i = 0; i < 4; i++) {
			if (pOutRoots[i].real() > 0.f && std::abs(pOutRoots[i].imag()) < 0.0001f && pOutRoots[i].real() < fBestRoot) {
				fBestRoot = pOutRoots[i].real();
			}
		}
		InVecArg->x = (A + (H + P * fBestRoot));
		InVecArg->y = (B + (K + Q * fBestRoot - L * fBestRoot * fBestRoot));
		InVecArg->z = (C + (J + R * fBestRoot));
	}*/