#pragma once

#include "../scene/GameScene.h"
#include "../scene/SceneState.h"
#include "../scene/LoadingScene.h"
#include "../scene/IScene.h"

#include <vector>
#include <string>
#include <memory>


namespace MyEngine {

/// <summary>
/// 複数の GameScene を保持し、ステージ進行とシーン遷移を管理するクラス。
/// </summary>
class LevelManager : public IScene {
public:
    /// <summary>
/// ステージ一覧と LoadingScene を生成し、最初のステージから進行できる状態にする。
/// </summary>
    LevelManager();
    /// <summary>
/// 保持しているステージ一覧を破棄し、レベル管理で確保したリソースを解放する。
/// </summary>
    ~LevelManager();

    /// <summary>
    /// オブジェクトやシーンの初期化処理を行う。
    /// </summary>
    void Initialize() override;
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// </summary>
    void Update() override;
    /// <summary>
    /// 描画処理を行う。
    /// </summary>
    void Draw() override;

    // 场景状态相关
    bool IsSceneEnd() const override { return isSceneEnd_; }
    SceneState GetNextSceneState() const override { return shouldReturnToTitle_ ? TITLE : RESULT; }

    // 关卡切换状态
    /// <summary>
    /// LevelManager 内でのプレイ中、ロード中、遷移中、全クリア状態を表す列挙型。
    /// </summary>
    enum class LevelState {
        Playing,      // 游戏中
        Loading,      // 加载中
        Transition,   // 关卡切换中
        GameComplete  // 游戏完成，准备显示结果前的Loading
    };

    LevelState GetCurrentState() const { return currentState_; }

    // 关卡控制
    /// <summary>
/// 現在のステージ番号を次へ進め、存在する場合は次の GameScene を初期化する。
/// </summary>
    void GoToNextLevel();
    /// <summary>
/// 現在プレイ中の GameScene を再初期化し、同じステージを最初からやり直す。
/// </summary>
    void RestartCurrentLevel();
    /// <summary>
    /// タイトル画面へ戻るための終了フラグを設定する。
    /// </summary>
    void ReturnToTitle();

    // 获取当前关卡
    GameScene* GetCurrentLevel() const {
        return (currentLevelIndex_ < levels_.size()) ? levels_[currentLevelIndex_].get() : nullptr;
    }

    // 设置当前关卡
    /// <summary>
/// ステージ選択画面から受け取った番号を、開始するステージ index に変換して保存する。
/// </summary>
/// <param name="level">1 始まりのステージ番号。</param>
    void SetCurrentLevel(int level);

    // 获取关卡信息
    int GetCurrentLevelNumber() const {
        return (currentLevelIndex_ < levels_.size()) ? levels_[currentLevelIndex_]->GetLevelNumber() : 0;
    }

    /// <summary>
/// 現在のステージインデックスに対応する表示用ステージ名を返す。
/// </summary>
    std::string GetCurrentLevelName() const;

private:
    // 关卡数据
    // 各关卡之间不同的数值集中放在这里，CreateLevels() 只负责统一生成
    /// <summary>
    /// 外部 JSON から読み込んだ 1 ステージ分の設定情報を保持する構造体。
    /// </summary>
    struct LevelConfig {
        int levelNumber;
        std::string levelName;

        std::vector<KamataEngine::Vector3> ballPositions;
        std::vector<KamataEngine::Vector3> goalPositions;
        std::vector<int> goalRequiredCounts;
        std::vector<GoalMovementConfig> goalMovementConfigs;
    };

private:
    std::vector<std::unique_ptr<GameScene>> levels_;
    std::vector<std::string> levelNames_;

    size_t currentLevelIndex_ = 0;

    bool shouldReturnToTitle_ = false;
    bool isSceneEnd_ = false;

    // Loading场景
    std::unique_ptr<LoadingScene> loadingScene_;
    LevelState currentState_ = LevelState::Playing;

private:
    /// <summary>
/// JSON から読み込んだステージ設定をもとに、GameScene の一覧を生成する。
/// </summary>
    void CreateLevels();

    /// <summary>
/// 外部 JSON ファイルを読み込み、各ステージの生成に必要な LevelConfig 配列へ変換する。
/// </summary>
/// <param name="filePath">読み込む level_config.json のパス。</param>
/// <returns>読み込みに成功したステージ設定の配列。失敗時は空配列。</returns>
    std::vector<LevelConfig> LoadLevelConfigsFromJson(const std::string& filePath) const;
    /// <summary>
/// JSON 内の移動方向文字列を MoveDirection enum に変換する。
/// </summary>
/// <param name="directionText">Horizontal、Vertical、Circular のいずれかの文字列。</param>
/// <returns>対応する MoveDirection。未知の文字列は Horizontal として扱う。</returns>
    MoveDirection StringToMoveDirection(const std::string& directionText) const;
    
    /// <summary>
/// 保持している GameScene の一覧をクリアし、unique_ptr にリソース解放を任せる。
/// </summary>
    void CleanupLevels();
    /// <summary>
/// 次ステージまたはリザルトへ進むために Loading 状態へ切り替える。
/// </summary>
    void StartLevelTransition();

    /// <summary>
/// 現在の GameScene を更新し、ステージ終了時の遷移先を判定する。
/// </summary>
    void UpdatePlayingState();
    /// <summary>
/// LoadingScene の完了を監視し、次のステージ開始または全クリア処理へ進める。
/// </summary>
    void UpdateLoadingState();
    /// <summary>
/// ステージ切り替え中の追加演出が必要になった場合に使用する更新枠。
/// </summary>
    void UpdateTransitionState();
    /// <summary>
/// 全ステージクリア後の Loading を更新し、完了したら Result へ遷移できる状態にする。
/// </summary>
    void UpdateGameCompleteState();
};

} // namespace MyEngine
