#pragma once

#include <KamataEngine.h>

class ButtonUtility {
public:
    static void UpdateHoverSprite(
        KamataEngine::Sprite* sprite,
        const KamataEngine::Vector2& basePosition,
        const KamataEngine::Vector2& normalSize,
        const KamataEngine::Vector2& hoverSize,
        bool isMouseOver,
        bool wasMouseOver
    );
};