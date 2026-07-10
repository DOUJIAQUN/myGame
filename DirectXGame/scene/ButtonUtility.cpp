#include "ButtonUtility.h"

void ButtonUtility::UpdateHoverSprite(
    KamataEngine::Sprite* sprite,
    const KamataEngine::Vector2& basePosition,
    const KamataEngine::Vector2& normalSize,
    const KamataEngine::Vector2& hoverSize,
    bool isMouseOver,
    bool wasMouseOver
) {
    if (!sprite) {
        return;
    }

    // 状態が変わっていない場合は何もしない
    if (isMouseOver == wasMouseOver) {
        return;
    }

    if (isMouseOver) {
        sprite->SetSize(hoverSize);

        KamataEngine::Vector2 sizeDiff = {
            (hoverSize.x - normalSize.x) / 2.0f,
            (hoverSize.y - normalSize.y) / 2.0f
        };

        sprite->SetPosition({
            basePosition.x - sizeDiff.x,
            basePosition.y - sizeDiff.y
            });
    }
    else {
        sprite->SetSize(normalSize);
        sprite->SetPosition(basePosition);
    }
}
