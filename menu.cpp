#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"

static int mImageHandle;
static int mImageHandle1;
static int mImageHandle2;
static int mImageHandle3;
static int mImageHandle4;
static int mImageHandle5;
int menu_num = 0;

void Menu_Initialize() {
    mImageHandle = LoadGraph("images/Title_Logo.png");
	mImageHandle1 = LoadGraph("images/bg.png");
    mImageHandle2 = LoadGraph("images/Start.png");
    mImageHandle3 = LoadGraph("images/Quit.png");
    mImageHandle4 = LoadGraph("images/Menu_Cursor.png");
    mImageHandle5 = LoadGraph("images/LabBit_Logo.png");
}

void Menu_Finalize() {
    DeleteGraph(mImageHandle);
	DeleteGraph(mImageHandle1);
    DeleteGraph(mImageHandle2);
    DeleteGraph(mImageHandle3);
    DeleteGraph(mImageHandle4);
    DeleteGraph(mImageHandle5);
}

void Menu_Update() {
    menu_num = menu_num % 2;
    if (GetAsyncKeyState(VK_UP) & 0x01) {
        if (menu_num > 0) {
            menu_num--;
        }
        else {
            menu_num = 1;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x01) {
        if (menu_num < 1) {
            menu_num++;
        }
        else {
            menu_num = 0;
        }
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x01) {
        if (menu_num == 0) {
            SceneMgr_ChangeScene(eScene_Standby);
        }
        else if (menu_num == 1) {
            SceneMgr_ChangeScene(eScene_End);
        }
    }
}

void Menu_Draw() {
	DrawGraph(0, 0, mImageHandle1, FALSE);
    DrawGraph(160, 80, mImageHandle, FALSE);
    DrawGraph(240, 240, mImageHandle2, FALSE);
    DrawGraph(240, 320, mImageHandle3, FALSE);
    DrawGraph(200, 240+(80*menu_num), mImageHandle4, FALSE);
    DrawGraph(200, 400, mImageHandle5, FALSE);
}