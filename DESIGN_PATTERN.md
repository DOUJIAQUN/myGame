# 採用デザインパターン：Strategy Pattern

## 対象リポジトリ
- （ここにGitHub URLを記載）

## 採用箇所（クラス名／ファイル名）
- `DirectXGame/play/Goal.h`
- `DirectXGame/play/Goal.cpp`
- `DirectXGame/play/IGoalMoveStrategy.h`（追加）
- `DirectXGame/play/GoalMoveStrategies.h` / `.cpp`（追加）※ファイル名は実装に合わせて変更

## どの部分に何のデザインパターンを採用したか
ゴール（Goal）の移動処理に Strategy Pattern を採用した。  
既存実装では `MoveDirection`（例：Horizontal / Vertical / Circular）に応じて `Goal::Update()` 内で条件分岐（switch/case）し、移動アルゴリズムが1箇所に集中していた。

そこで、移動アルゴリズムを「戦略（Strategy）」としてクラスに分離し、`Goal` は保持している戦略オブジェクトに処理を委譲する構造に変更した。

- `HorizontalMoveStrategy`：水平移動のアルゴリズム
- `VerticalMoveStrategy`：垂直移動のアルゴリズム
- `CircularMoveStrategy`：円運動のアルゴリズム

## 何の目的で採用したか（理由）
- `Goal::Update()` の責務を軽くし、分岐増加による可読性・保守性の低下を防ぐため
- 新しい移動パターン追加時に、既存の `Goal` 本体を大きく修正せずに拡張できるようにするため（拡張性）
- 移動アルゴリズムを独立させることで、挙動ごとの修正・テストを局所化するため
