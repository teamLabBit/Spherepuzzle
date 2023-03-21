#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"

static int mImageHandle;

void Menu_Initialize() {
	mImageHandle = LoadGraph("images/Scene_Menu.png");
}

void Menu_Finalize() {
	DeleteGraph(mImageHandle);
}

void Menu_Update() {
	if (GetAsyncKeyState(VK_SPACE) & 0x01) {
		SceneMgr_ChangeScene(eScene_Standby);
	}
}

void Menu_Draw() {
	DrawGraph(0, 0, mImageHandle, FALSE);
	DrawString(0, 0, "���j���[��ʂł��B", GetColor(255, 255, 255));
	DrawString(0, 20, "SPACE�L�[�������ƃQ�[����ʂɐi�݂܂��B", GetColor(255, 255, 255));
}