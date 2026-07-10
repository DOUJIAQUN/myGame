#pragma once

#include <KamataEngine.h>

/// <summary>
/// ボタン用 Sprite の共通処理をまとめたユーティリティクラス。
/// </summary>
class ButtonUtility {
public:
    static void UpdateHoverSprite(
        KamataEngine::Sprite* sprite,
        const KamataEngine::Vector2& basePosition,//SR2_02_04
        const KamataEngine::Vector2& normalSize,
        const KamataEngine::Vector2& hoverSize,
        bool isMouseOver,
        bool wasMouseOver
    );
};
