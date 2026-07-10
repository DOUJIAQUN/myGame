#pragma once
#include "KamataEngine.h"
#include <cmath>

namespace MyEngine {
// アフィン変換行列の作成
/// <summary>
/// スケール、回転、平行移動を合成したアフィン変換行列を作成する。
/// </summary>
KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rot, const KamataEngine::Vector3& translate);

// 行列の掛け算
/// <summary>
/// 2つの 4x4 行列を行×列の積で掛け合わせた結果を返す。
/// </summary>
KamataEngine::Matrix4x4 MatrixMultiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);

/// <summary>
/// 2つの Vector3 の内積を計算して返す。
/// </summary>
float Dot(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
/// <summary>
/// Vector3 の長さを計算して返す。
/// </summary>
float Length(const KamataEngine::Vector3& v);

/// <summary>
/// Vector や Matrix に関する計算処理をまとめた数学ユーティリティクラス。
/// </summary>
class myMath {
public:
	/// <summary>
/// ベクトルのスカラー倍または行列積を計算して返す。
/// </summary>
	static KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
	/// <summary>
/// ベクトルのスカラー倍または行列積を計算して返す。
/// </summary>
	static KamataEngine::Vector3 Multiply(float scalar, const KamataEngine::Vector3& vector);
	/// <summary>
/// 指定された移動量を表す平行移動行列を作成する。
/// </summary>
	static KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);
	/// <summary>
/// 指定された倍率を表す拡大縮小行列を作成する。
/// </summary>
	static KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);
	/// <summary>
/// X 軸回転を表す行列を作成する。
/// </summary>
	static KamataEngine::Matrix4x4 MakeRotateXMatrix(float radius);
	/// <summary>
/// Y 軸回転を表す行列を作成する。
/// </summary>
	static KamataEngine::Matrix4x4 MakeRotateYMatrix(float radius);
	/// <summary>
/// Z 軸回転を表す行列を作成する。
/// </summary>
	static KamataEngine::Matrix4x4 MakeRotateZMatrix(float radius);
	/// <summary>
/// スケール、回転、平行移動を合成したアフィン変換行列を作成する。
/// </summary>
	static KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translate);
	/// <summary>
/// ベクトルを指定された行列で変換し、結果の座標を返す。
/// </summary>
	static KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);
	/// <summary>
/// ベクトルを指定された行列で変換し、結果の座標を返す。
/// </summary>
	static KamataEngine::Vector4 Transform(const KamataEngine::Vector4& vector, const KamataEngine::Matrix4x4& matrix);
	/// <summary>
/// 法線ベクトルを移動成分を除いた行列で変換する。
/// </summary>
	static KamataEngine::Vector3 TransformNormal(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);
	/// <summary>
/// ベクトルを長さ 1 の向きベクトルへ正規化する。
/// </summary>
	static KamataEngine::Vector3 Normalize(const KamataEngine::Vector3& v); 
	// 加算
	/// <summary>
/// 2つの Vector3 を成分ごとに加算して返す。
/// </summary>
	static KamataEngine::Vector3 Add(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
	//减算
	/// <summary>
/// 2つの Vector3 を成分ごとに減算して返す。
/// </summary>
	static KamataEngine::Vector3 Subtract(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);

	/// <summary>
/// Vector3 の長さを計算して返す。
/// </summary>
	static float Length(const KamataEngine::Vector3& v);

	 // 添加距离计算函数
	/// <summary>
/// 2点間の距離を Vector3 の差分から計算して返す。
/// </summary>
	static float Distance(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
};

} // namespace MyEngine
