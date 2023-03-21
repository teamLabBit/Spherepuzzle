#include "Standby.h"
#include "SceneMgr.h"
#include "DxLib.h"

static int mImageHandle;

void Standby_Initialize() {
	mImageHandle = LoadGraph("images/Scene_Game.png");
}

void Standby_Finalize() {
	DeleteGraph(mImageHandle);
}

void Standby_Update() {
	if (GetAsyncKeyState(VK_SPACE) & 0x01) {
		SceneMgr_ChangeScene(eScene_Game);
	}
}

void Standby_Draw() {
	DrawGraph(0, 0, mImageHandle, FALSE);
	DrawString(0, 0, "�X�^���o�C��ʂł��B", GetColor(255, 255, 255));
	DrawString(0, 20, "SPACE�L�[�������ƃQ�[����ʂɐi�݂܂��B", GetColor(255, 255, 255));
}