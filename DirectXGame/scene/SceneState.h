#pragma once
/// <summary>
/// 現在または次に表示するシーン種別を表す列挙型。
/// </summary>
enum SceneState {
	TITLE,   // 标题场景
	STAGE_SELECT,
	LOADING, // 加载场景
	GAME,    // 游戏场景
	RESULT   // 结果场景
};
