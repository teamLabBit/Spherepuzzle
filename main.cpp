#include "Dxlib.h"
#include "SceneMgr.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	SceneMgr_Initialize();

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !GameEnd_Check()) {
		SceneMgr_Update();
		SceneMgr_Draw();
	}

	SceneMgr_Finalize();

	DxLib_End();
	return 0;
}