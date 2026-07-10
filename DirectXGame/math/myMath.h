#pragma once
#include <math/Matrix4x4.h>
#include <math/Vector3.h>
#include <math/Vector4.h>
#include <cmath>

namespace MyEngine {
// アフィン変換行列の作成
/// <summary>
/// MakeAffineMatrix に関する処理を行う。
/// </summary>
KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rot, const KamataEngine::Vector3& translate);

// 行列の掛け算
/// <summary>
/// MatrixMultiply に関する処理を行う。
/// </summary>
KamataEngine::Matrix4x4 MatrixMultiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);

/// <summary>
/// Dot に関する処理を行う。
/// </summary>
float Dot(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
/// <summary>
/// Length に関する処理を行う。
/// </summary>
float Length(const KamataEngine::Vector3& v);

/// <summary>
/// Vector や Matrix に関する計算処理をまとめた数学ユーティリティクラス。
/// </summary>
class myMath {
public:
	/// <summary>
	/// Multiply に関する処理を行う。
	/// </summary>
	static KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
	/// <summary>
	/// Multiply に関する処理を行う。
	/// </summary>
	static KamataEngine::Vector3 Multiply(float scalar, const KamataEngine::Vector3& vector);
	/// <summary>
	/// MakeTranslateMatrix に関する処理を行う。
	/// </summary>
	static KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);
	/// <summary>
	/// MakeScaleMatrix に関する処理を行う。
	/// </summary>
	static KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);
	/// <summary>
	/// MakeRotateXMatrix に関する処理を行う。
	/// </summary>
	static KamataEngine::Matrix4x4 MakeRotateXMatrix(float radius);
	/// <summary>
	/// MakeRotateYMatrix に関する処理を行う。
	/// </summary>
	static KamataEngine::Matrix4x4 MakeRotateYMatrix(float radius);
	/// <summary>
	/// MakeRotateZMatrix に関する処理を行う。
	/// </summary>
	static KamataEngine::Matrix4x4 MakeRotateZMatrix(float radius);
	/// <summary>
	/// MakeAffineMatrix に関する処理を行う。
	/// </summary>
	static KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translate);
	/// <summary>
	/// Transform に関する処理を行う。
	/// </summary>
	static KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);
	/// <summary>
	/// Transform に関する処理を行う。
	/// </summary>
	static KamataEngine::Vector4 Transform(const KamataEngine::Vector4& vector, const KamataEngine::Matrix4x4& matrix);
	/// <summary>
	/// TransformNormal に関する処理を行う。
	/// </summary>
	static KamataEngine::Vector3 TransformNormal(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);
	/// <summary>
	/// Normalize に関する処理を行う。
	/// </summary>
	static KamataEngine::Vector3 Normalize(const KamataEngine::Vector3& v); 
	// 加算
	/// <summary>
	/// Add に関する処理を行う。
	/// </summary>
	static KamataEngine::Vector3 Add(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
	//减算
	/// <summary>
	/// Subtract に関する処理を行う。
	/// </summary>
	static KamataEngine::Vector3 Subtract(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);

	/// <summary>
	/// Length に関する処理を行う。
	/// </summary>
	static float Length(const KamataEngine::Vector3& v);

	 // 添加距离计算函数
	/// <summary>
	/// Distance に関する処理を行う。
	/// </summary>
	static float Distance(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
};

} // namespace MyEngine
