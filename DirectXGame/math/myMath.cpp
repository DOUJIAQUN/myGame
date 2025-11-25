#include "myMath.h"
#include <assert.h>

using namespace KamataEngine;

// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {

	Matrix4x4 ScallMat, RotateMat, RotateMatX, RotateMatY, RotateMatZ, TranslateMat, returnMat;

	// スケール行列作成
	ScallMat = { scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1 };

	// XYZ回転行列作成
	RotateMatX = { 1, 0, 0, 0, 0, cosf(rot.x), sinf(rot.x), 0, 0, -sinf(rot.x), cosf(rot.x), 0, 0, 0, 0, 1 };

	RotateMatY = { cosf(rot.y), 0, -sinf(rot.y), 0, 0, 1, 0, 0, sinf(rot.y), 0, cosf(rot.y), 0, 0, 0, 0, 1 };

	RotateMatZ = { cosf(rot.z), sinf(rot.z), 0, 0, -sinf(rot.z), cosf(rot.z), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

	// XYZ回転行列の合成(Z*X*Y)
	RotateMat = MatrixMultiply(RotateMatZ, RotateMatX);
	// ↑の結果＊Y軸回転
	RotateMat = MatrixMultiply(RotateMat, RotateMatY);

	// 平行移動行列作成
	TranslateMat = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, translate.x, translate.y, translate.z, 1 };

	// スケール＊回転＊平行移動をワールド変換行列に
	returnMat = MatrixMultiply(ScallMat, RotateMat);
	returnMat = MatrixMultiply(returnMat, TranslateMat);

	return returnMat;
}

// 行列の掛け算
Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2) {

	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return result;
}

Matrix4x4 myMath::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

// 1. 平行移動行列
Matrix4x4 myMath::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1;
			}
			else {
				result.m[i][j] = 0;
			}
		}
	}
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}
// 2. 拡大縮小行列
Matrix4x4 myMath::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1;
			}
			else {
				result.m[i][j] = 0;
			}
		}
	}
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	return result;
}
// 1. X軸回転行列
Matrix4x4 myMath::MakeRotateXMatrix(float radius) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1;
			}
			else {
				result.m[i][j] = 0;
			}
		}
	}
	result.m[1][1] = float(std::cos(radius));
	result.m[1][2] = float(std::sin(radius));

	result.m[2][1] = float(-std::sin(radius));
	result.m[2][2] = float(std::cos(radius));
	return result;
}
// 2. Y軸回転行列
Matrix4x4 myMath::MakeRotateYMatrix(float radius) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1;
			}
			else {
				result.m[i][j] = 0;
			}
		}
	}
	result.m[0][0] = float(std::cos(radius));
	result.m[0][2] = float(-std::sin(radius));

	result.m[2][0] = float(std::sin(radius));
	result.m[2][2] = float(std::cos(radius));
	return result;
}
// 3. Z軸回転行列
Matrix4x4 myMath::MakeRotateZMatrix(float radius) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1;
			}
			else {
				result.m[i][j] = 0;
			}
		}
	}
	result.m[0][0] = float(std::cos(radius));
	result.m[0][1] = float(std::sin(radius));

	result.m[1][0] = float(-std::sin(radius));
	result.m[1][1] = float(std::cos(radius));
	return result;
}

Matrix4x4 myMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	Matrix4x4 SRResult = Multiply(scaleMatrix, rotateMatrix);
	Matrix4x4 SRTResult = Multiply(SRResult, translateMatrix);
	return SRTResult;
}

Vector3 myMath::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = matrix.m[0][0] * vector.x + matrix.m[1][0] * vector.y + matrix.m[2][0] * vector.z + matrix.m[3][0];
	result.y = matrix.m[0][1] * vector.x + matrix.m[1][1] * vector.y + matrix.m[2][1] * vector.z + matrix.m[3][1];
	result.z = matrix.m[0][2] * vector.x + matrix.m[1][2] * vector.y + matrix.m[2][2] * vector.z + matrix.m[3][2];
	float w = matrix.m[0][3] * vector.x + matrix.m[1][3] * vector.y + matrix.m[2][3] * vector.z + matrix.m[3][3];
	if (w != 0.0f)
		result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

// Vector4 版本的 Transform 函数
Vector4 myMath::Transform(const Vector4& vector, const Matrix4x4& matrix) {
	Vector4 result;
	result.x = matrix.m[0][0] * vector.x + matrix.m[1][0] * vector.y + matrix.m[2][0] * vector.z + matrix.m[3][0] * vector.w;
	result.y = matrix.m[0][1] * vector.x + matrix.m[1][1] * vector.y + matrix.m[2][1] * vector.z + matrix.m[3][1] * vector.w;
	result.z = matrix.m[0][2] * vector.x + matrix.m[1][2] * vector.y + matrix.m[2][2] * vector.z + matrix.m[3][2] * vector.w;
	result.w = matrix.m[0][3] * vector.x + matrix.m[1][3] * vector.y + matrix.m[2][3] * vector.z + matrix.m[3][3] * vector.w;
	return result;
}

Vector3 myMath::TransformNormal(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = matrix.m[0][0] * vector.x + matrix.m[1][0] * vector.y + matrix.m[2][0] * vector.z;
	result.y = matrix.m[0][1] * vector.x + matrix.m[1][1] * vector.y + matrix.m[2][1] * vector.z;
	result.z = matrix.m[0][2] * vector.x + matrix.m[1][2] * vector.y + matrix.m[2][2] * vector.z;

	return result;
}


float Dot(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Length(const KamataEngine::Vector3& v)
{
	return std::sqrt(Dot(v, v));
}

KamataEngine::Vector3 myMath::Normalize(const KamataEngine::Vector3& v)
{
	float length = Length(v);
	assert(length != 0.0f);
	return { v.x / length, v.y / length, v.z / length };
}

KamataEngine::Vector3 myMath::Add(const KamataEngine::Vector3& vec1, const KamataEngine::Vector3& vec2)
{
	return KamataEngine::Vector3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

// 減算
KamataEngine::Vector3 myMath::Subtract(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; }

KamataEngine::Vector3 myMath::Multiply(float scalar, const KamataEngine::Vector3& vector) {
	return { scalar * vector.x, scalar * vector.y, scalar * vector.z };
}

float myMath::Length(const KamataEngine::Vector3& v) {
	return std::sqrt(Dot(v, v));
}


// 添加距离计算函数实现
float myMath::Distance(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2) {
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	float dz = v1.z - v2.z;
	return std::sqrt(dx * dx + dy * dy + dz * dz);
}