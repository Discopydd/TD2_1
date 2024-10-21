#include <Novice.h>
#include "Player.h"
#include "MapChip.h"
enum GameState {
    TITLE,
    PLAYING,
    GAME_OVER
};
const char kWindowTitle[] = "じゃんじゃんじゃんぷ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1020,720);

	GameState gameState = TITLE;  // 初始状态为标题画面

	Player player;
	MapChip mapChip;

	 mapChip.LoadMap("NoviceResources/map.csv");

	 int backgroundTextureHandle = Novice::LoadTexture("BG01.png");

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
        switch (gameState) {
        case TITLE: {
            // 按下空格键进入游戏
            if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
                gameState = PLAYING;
                player = Player();  // 重置玩家状态
                mapChip.LoadMap("NoviceResources/map.csv"); // 重新加载地图
               player.cooldownTimer = 0.1f; 
            }
            break;
        }

        case PLAYING: {
            // 更新和绘制游戏内容

          
        player.Update(mapChip);
   
            mapChip.UpdatePlatforms();

            Novice::DrawSpriteRect(
                0, 0,
                0, 0,
                720, 720,
                backgroundTextureHandle,
                1.0f, 1.0f,
                0.0f,
                0xFFFFFFFF
            );
            player.Draw();
            mapChip.DrawPlatforms(player.cameraOffsetX, player.cameraOffsetY);
            mapChip.DrawMap(player.cameraOffsetX, player.cameraOffsetY);

            // 如果玩家死亡，切换到游戏结束状态
            if (!player.playerLife) {
                gameState = GAME_OVER;
            }
            break;
        }

        case GAME_OVER: {


            // 按下空格键回到标题界面
            if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
                gameState = TITLE;
                player = Player();  // 重置玩家状态
                mapChip.LoadMap("NoviceResources/map.csv"); // 重新加载地图
            }
            break;
        }
        }

    // フレームの終了
    Novice::EndFrame();

    // ESCキーが押されたらループを抜ける
    if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
        break;
    }
}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
