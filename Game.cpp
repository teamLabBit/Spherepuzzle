#include "Game.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include <time.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

static int mImageHandle1;
static int mImageHandle2;
static int mImageHandle3;
static int mImageHandle4;
static int mImageHandle5;
static int mImageHandle6;

vector<vector<int>> combine(9, vector<int>(10, 0));     //結合の状態と番号を入れておく配列
vector<vector<int>> delete_mem(9, vector<int>(10, 0));  //消すか否かを記録する配列
LPCSTR font_path = "fonts/ookamikun.ttf";

void Game_Initialize() {
    if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
    }
    else {
        MessageBox(NULL, "FONT LOAD ERROR", "", MB_OK);
    }
	mImageHandle1 = LoadGraph("images/game.png");
    mImageHandle2 = LoadGraph("images/sphere_blue.png");
    mImageHandle3 = LoadGraph("images/sphere_yellow.png");
    mImageHandle4 = LoadGraph("images/sphere_red.png");
    mImageHandle5 = LoadGraph("images/sphere_green.png");
    mImageHandle6 = LoadGraph("images/Gameover.png");
}

void Game_Finalize() {
    if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
    }
    else {
        MessageBox(NULL, "remove failure", "", MB_OK);
    }
	DeleteGraph(mImageHandle1);
    DeleteGraph(mImageHandle2);
    DeleteGraph(mImageHandle3);
    DeleteGraph(mImageHandle4);
    DeleteGraph(mImageHandle5);
    DeleteGraph(mImageHandle6);
}

void Game_Update() {
    clock_t game_start = clock();
	if (GetAsyncKeyState(VK_SPACE) & 0x01) {
		SceneMgr_ChangeScene(eScene_Gameover);
	}
    vector<vector<int>> puzzle = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    vector<vector<int>> puzzle_temp = puzzle;

    int next_sphere = sphere_Gene();
    int now_sphere = sphere_Gene();
    int sphere_pos = 0;
    long long score = 0;
    Game_Draw(puzzle, score, sphere_pos, now_sphere, next_sphere, game_start);
    clock_t prev_stacktime = clock();

    while (ProcessMessage() == 0) {
        clock_t start = clock();
        if (GetAsyncKeyState(VK_LEFT) & 0x01) {
            if (sphere_pos >= 1 && sphere_pos <= 9) {
                sphere_pos--;
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x01) {
            if (sphere_pos >= 0 && sphere_pos <= 8) {
                sphere_pos++;
            }
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x01) {
            for (int i = 0; i < puzzle.size(); i++) {
                if (puzzle[i][sphere_pos] == 0) {
                    puzzle[i][sphere_pos] = now_sphere;
                    now_sphere = next_sphere;
                    next_sphere = sphere_Gene();
                    break;
                }
            }
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x01) {
            break;
        }
        Game_Draw(puzzle, score, sphere_pos, now_sphere, next_sphere, game_start);
        // ここから消す処理と落下処理
        bool rep_flag = true;
        int rensa_count = 0;
        while (rep_flag) {
            rensa_count++;
            // combine(つながっているか否かを保持する配列)とdelete_mem(消すかどうかを保持する配列)を初期化
            for (int y = 0; y < puzzle.size(); y++) {
                for (int x = 0; x < puzzle[0].size(); x++) {
                    combine[y][x] = 0;
                    delete_mem[y][x] = 0;
                }
            }

            rep_flag = false; // 消す処理を繰り返すかどうかのフラグ
            for (int y = 0; y < puzzle.size(); y++) {
                for (int x = 0; x < puzzle[0].size(); x++) {
                    int i = 0; // チェックするスフィアの番号
                    if (combine[y][x] == 0 && puzzle[y][x] != 0) {
                        i++;
                        vector<vector<int>> temp_combine = combine;              // 結合状態の配列を一時保持(後で戻せるように)
                        int count = CheckCombine(puzzle, x, y, puzzle[y][x], i); // CheckCombineで返ってくる結合数を取得
                        if (count >= 4) {
                            rep_flag = true; // 消す処理を再度行うようにセット(連鎖の確認)
                            for (int temp_y = 0; temp_y < puzzle.size(); temp_y++) {
                                for (int temp_x = 0; temp_x < puzzle[0].size(); temp_x++) {
                                    if (combine[y][x] != 0) {
                                        delete_mem[y][x] = 1; // CheckCombine()で結合状態と判断された場所を消す様に設定する
                                        score += count * rensa_count * 10;
                                    }
                                }
                            }
                        }
                        else {
                            combine = temp_combine; // 4個以上つながっていない場合は消さないので結合の配列をもとに戻す
                        }
                    }
                }
            }
            // 消す処理
            for (int y = 0; y < puzzle.size(); y++) {
                for (int x = 0; x < puzzle[0].size(); x++) {
                    if (delete_mem[y][x] == 1) {
                        int temp_combine_no = combine[y][x]; // 結合の番号を記録
                        for (int y2 = 0; y2 < puzzle.size(); y2++) {
                            for (int x2 = 0; x2 < puzzle[0].size(); x2++) {
                                if (combine[y2][x2] == temp_combine_no) {
                                    puzzle[y2][x2] = 0; // 結合の番号が同じものを消す
                                }
                            }
                        }
                    }
                }
            }
            // 落下処理
            bool fall_flag = true;
            while (fall_flag) {
                fall_flag = false;
                for (int y = 1; y < puzzle.size(); y++) {
                    for (int x = 0; x < puzzle[0].size(); x++) {
                        if (puzzle[y - 1][x] == 0 && puzzle[y][x] != 0) {
                            fall_flag = true;
                            puzzle[y - 1][x] = puzzle[y][x];
                            puzzle[y][x] = 0;
                        }
                    }
                }
            }
            if (rep_flag) {
                Sleep(1000);
                prev_stacktime = clock();
                Game_Draw(puzzle, score, sphere_pos, now_sphere, next_sphere, game_start);
            }
        }
        if (clock() - prev_stacktime >= 3000) {
            prev_stacktime = clock();
            vector<int> stack(puzzle[0].size(), 0);
            for (int i = 0; i < stack.size(); i++) {
                stack[i] = sphere_Gene();
            }
            for (int y = puzzle.size() - 2; y >= 0; y--) {
                for (int x = 0; x < puzzle[0].size(); x++) {
                    puzzle[y + 1][x] = puzzle[y][x];
                }
            }
            for (int i = 0; i < stack.size(); i++) {
                puzzle[0][i] = stack[i];
            }
            if (is_gameover(puzzle)) {
                break;
            }
        }
        Game_Draw(puzzle, score, sphere_pos, now_sphere, next_sphere, game_start);
        if (clock() - start < 16) {
            Sleep(16 - (clock() - start));
        }

        if (is_gameover(puzzle)) {
            break;
        }
    }
    gameover_disp(puzzle, score);
    while (true) {
        if (GetAsyncKeyState('R') & 0x01) {
            SceneMgr_ChangeScene(eScene_Game);
            break;
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x01) {
            SceneMgr_ChangeScene(eScene_Menu);
            break;
        }
    }
}

void Game_Draw(vector<vector<int>> puzzle, long long score, int sphere_pos, int now_sphere, int next_sphere, clock_t game_start) {
    ClearDrawScreen();
	DrawGraph(0, 0, mImageHandle1, FALSE);
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle[0].size(); j++) {
            if (puzzle[i][j] != 0) {
                switch (puzzle[i][j]) {
                case 1:
                    DrawGraph(40 + (j * 40), 40 + (i * 40), mImageHandle2, TRUE);
                    break;
                case 2:
                    DrawGraph(40 + (j * 40), 40 + (i * 40), mImageHandle3, TRUE);
                    break;
                case 3:
                    DrawGraph(40 + (j * 40), 40 + (i * 40), mImageHandle4, TRUE);
                    break;
                case 4:
                    DrawGraph(40 + (j * 40), 40 + (i * 40), mImageHandle5, TRUE);
                    break;
                }
            }
        }
    }

    switch (now_sphere) {
    case 1:
        DrawGraph(40 + (sphere_pos * 40), 400, mImageHandle2, TRUE);
        break;
    case 2:
        DrawGraph(40 + (sphere_pos * 40), 400, mImageHandle3, TRUE);
        break;
    case 3:
        DrawGraph(40 + (sphere_pos * 40), 400, mImageHandle4, TRUE);
        break;
    case 4:
        DrawGraph(40 + (sphere_pos * 40), 400, mImageHandle5, TRUE);
        break;
    }

    switch (next_sphere) {
    case 1:
        DrawGraph(520, 280, mImageHandle2, TRUE);
        break;
    case 2:
        DrawGraph(520, 280, mImageHandle3, TRUE);
        break;
    case 3:
        DrawGraph(520, 280, mImageHandle4, TRUE);
        break;
    case 4:
        DrawGraph(520, 280, mImageHandle5, TRUE);
        break;
    }

    SetFontSize(22);
    ChangeFont("おおかみくんフォント", DX_CHARSET_DEFAULT);

    stringstream sssec;
    sssec << setfill('0') << right << setw(2) << ((clock()-game_start)/1000)%60;
    stringstream ssmin;
    ssmin << setfill('0') << right << setw(2) << ((clock() - game_start) / 1000) / 60;
    string stime;
    stime = ssmin.str() + ":" + sssec.str();
    DrawString(503, 120, stime.c_str(), GetColor(255, 255, 255));

    stringstream ss;
    ss << setfill('0') << right << setw(8) << score;
    string s = ss.str();
    DrawString(469, 200, s.c_str(), GetColor(255, 255, 255));
    ScreenFlip();
}

void initRand() {
	srand((unsigned int)time(NULL));
}

int sphere_Gene() {
    return rand() % 4 + 1;
}

bool is_gameover(vector<vector<int>> puzzle) {
    for (int i = 0; i < puzzle[0].size(); i++) {
        if (puzzle[puzzle.size() - 1][i] != 0) {
            return true;
        }
    }
    return false;
}

int CheckCombine(vector<vector<int>> puzzle, int x, int y, int sphere_no, int combine_no)
{
    if (puzzle[y][x] != sphere_no || combine[y][x] != 0) {
        return 0;
    }
    int count = 1;
    combine[y][x] = combine_no;
    if (y > 0) {
        count += CheckCombine(puzzle, x, y - 1, sphere_no, combine_no);             // 上
    }
    if (x < 9) {
        count += CheckCombine(puzzle, x + 1, y, sphere_no, combine_no);             // 右
    }
    if (y < 8) {
        count += CheckCombine(puzzle, x, y + 1, sphere_no, combine_no);				// 下
    }
    if (x > 0) {
        count += CheckCombine(puzzle, x - 1, y, sphere_no, combine_no);				// 左
    }

    return count;
}

void gameover_disp(vector<vector<int>> puzzle, long long score) {
    ClearDrawScreen();
    DrawGraph(0, 0, mImageHandle6, FALSE);
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle[0].size(); j++) {
            if (puzzle[i][j] != 0) {
                switch (puzzle[i][j]) {
                case 1:
                    DrawGraph(40 + (j * 40), 40 + (i * 40), mImageHandle2, TRUE);
                    break;
                case 2:
                    DrawGraph(40 + (j * 40), 40 + (i * 40), mImageHandle3, TRUE);
                    break;
                case 3:
                    DrawGraph(40 + (j * 40), 40 + (i * 40), mImageHandle4, TRUE);
                    break;
                case 4:
                    DrawGraph(40 + (j * 40), 40 + (i * 40), mImageHandle5, TRUE);
                    break;
                }
            }
        }
    }
    stringstream ss;
    ss << setfill('0') << right << setw(8) << score;
    string s = ss.str();
    DrawString(469, 200, s.c_str(), GetColor(255, 255, 255));
    ScreenFlip();
}