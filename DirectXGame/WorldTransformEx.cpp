#include <KamataEngine.h>

using namespace KamataEngine;
using namespace MathUtility;

// 関数コメント: UpdateMatrix の処理を実行する。
void WorldTransform::UpdateMatrix() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateMatrix の役割を実現する。
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateMatrix の役割を実現する。
	Matrix4x4 matScale = MakeScaleMatrix(scale_);

	Matrix4x4 matRotX = MakeRotateXMatrix(rotation_.x);
	Matrix4x4 matRotY = MakeRotateXMatrix(rotation_.y);
	Matrix4x4 matRotZ = MakeRotateXMatrix(rotation_.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	Matrix4x4 matTrans = MakeTranslateMatrix(translation_);
	matWorld_ = matScale * matRot * matTrans;

	TransferMatrix();
}
