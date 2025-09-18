// Hakuensai Launcher 2025 一部AI要素、おふざけあり　タスキルでのみ終了できます
#include <math.h>//数学関数
#include <windows.h>//Windows API
#include <DxLib.h>// DxLibヘッダファイル
#include <time.h>// 時刻関数
#include<string>//文字列関数
#include <vector>//ベクター関数
#include<cstdlib>//標準ライブラリ
#include <iostream>//入出力ストリーム
#pragma warning(disable : 4996)// 4996警告を無視する
#define PI 3.141592653589793238462643 //円周率設定
// グローバル変数で元のプロシージャを保存
WNDPROC g_OldWndProc = nullptr;
// Alt+F4や×ボタンを無効化するプロシージャ
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_CLOSE) {
		return 0; // WM_CLOSEを無視
	}
	// 他のメッセージは必ず元のプロシージャへ
	return CallWindowProc(g_OldWndProc, hWnd, msg, wParam, lParam);
}
//FPS設定(固定)達
class Fps {
	int mStartTime;         //測定開始時刻
	int mCount;             //カウンター
	float mFps;             //fps
	int N = 60;//平均を取るサンプル数
public:int FPS = 120;	//設定したFPS

public:
	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update() {
		if (mCount == 0) { //1フレーム目なら時刻を記憶
			mStartTime = GetNowCount();
		}
		if (mCount == N) { //60フレーム目なら平均を計算する
			int t = GetNowCount();
			mFps = 1000.f / ((t - mStartTime) / (float)N);
			mCount = 0;
			mStartTime = t;
		}
		mCount++;
		return true;
	}

	void Draw() {
		DrawFormatString(0, 0, GetColor(0, 0, 0), "%.1f", mFps);//FPS表示
	}

	void Wait() {
		int tookTime = GetNowCount() - mStartTime;	//かかった時間
		int waitTime = mCount * 1000 / FPS - tookTime;	//待つべき時間
		if (waitTime > 0) {
			Sleep(waitTime);	//待機
		}
	}
};

std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\r\n");
	size_t last = str.find_last_not_of(" \t\r\n");
	if (first == std::string::npos || last == std::string::npos) return "";
	return str.substr(first, (last - first + 1));
}

std::string GetExeDir() {
	char path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	std::string exePath(path);
	size_t pos = exePath.find_last_of("\\/");
	return exePath.substr(0, pos); // 実行ファイルのディレクトリ
}

// タスクバーを非表示にする
void HideTaskbar() {
	HWND hTaskbar = FindWindowA("Shell_TrayWnd", NULL);
	if (hTaskbar) {
		ShowWindow(hTaskbar, SW_HIDE);
	}
	HWND hStart = FindWindowA("Button", NULL); // スタートボタン
	if (hStart) {
		ShowWindow(hStart, SW_HIDE);
	}
}

// タスクバーを再表示する
void ShowTaskbar() {
	HWND hTaskbar = FindWindowA("Shell_TrayWnd", NULL);
	if (hTaskbar) {
		ShowWindow(hTaskbar, SW_SHOW);
	}
	HWND hStart = FindWindowA("Button", NULL); // スタートボタン
	if (hStart) {
		ShowWindow(hStart, SW_SHOW);
	}
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) /*おまじない♪*/ {
	ChangeWindowMode(TRUE);//ウインドウ設定
	SetGraphMode(1920, 1080, 32);//画面サイズ指定(FHD、簡単には閉じれないぞ・・・ﾆﾋﾋ)
	SetMainWindowText("Hakuensai Launcher 2025");//ウインドウ名設定
	SetAlwaysRunFlag(true);//バックグラウンド実行
	SetBackgroundColor(255, 255, 255);//背景設定
	SetWindowStyleMode(2);//ウインドウスタイルをボーダーレスウインドウに設定
	SetDoubleStartValidFlag(FALSE); //二重起動禁止
	SetUseDateNameLogFile(TRUE); //ログファイルに日付を付ける
	if (DxLib_Init() == -1)//起動失敗したらエラー吐かせる
		return -1;
	ErrorLogTabAdd(); //ログタブ追加
	ErrorLogAdd("DXライブラリが正常に初期化されました。\n");//ログに初期化成功を記録
	HWND hwnd = (HWND)GetMainWindowHandle();//メインウインドウハンドルを取得
	g_OldWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MyWndProc);//ウインドウプロシージャをフックする
	ErrorLogAdd("ウインドウプロシージャをフックしました。\n");//ログにフック成功を記録
	//HideTaskbar(); //タスクバーを非表示にする
	ErrorLogAdd("タスクバーを非表示にしました。\n");//ログにタスクバー非表示成功を記録
	SetDrawScreen(DX_SCREEN_BACK);//裏描画に設定
	ErrorLogAdd("裏画面に描画を設定しました。\n");//ログに裏画面設定成功を記録
	int font = CreateFontToHandle("BIZ UDPゴジック", 30, 3, DX_FONTTYPE_ANTIALIASING);//通常フォント生成
	ErrorLogAdd("フォントを生成しました。\n");//ログにフォント生成成功を記録
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(0/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int BGM = LoadSoundMem("Audio/BGM.ogg");//BGM読み込み
	ErrorLogAdd("BGMを読み込みました。\n");//ログにBGM読み込み成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(1/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int Communicationmark = LoadGraph("Image/Communicationmark.png");//コミュニケーションマーク読み込み
	ErrorLogAdd("コミュニケーションマークを読み込みました。\n");//ログにコミュニケーションマーク読み込み成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(2/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int BGMCount = 0;//BGM時間測定用変数
	ErrorLogAdd("BGMの時間測定用変数を初期化しました。\n");//ログにBGM時間測定用変数初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(3/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	double Communicationmarkangle = 4;//コミュニケーションマーク角度変数
	ErrorLogAdd("コミュニケーションマーク角度変数を初期化しました。\n");//ログにコミュニケーションマーク角度変数初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(4/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int titlefont = CreateFontToHandle("UD デジタル 教科書体 N", 100, 8, DX_FONTTYPE_ANTIALIASING);//タイトルフォント生成
	ErrorLogAdd("タイトルフォントを生成しました。\n");//ログにタイトルフォント生成成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(5/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int timefont = CreateFontToHandle("BIZ UDPゴジック", 60, 3, DX_FONTTYPE_ANTIALIASING);//時刻フォント生成
	ErrorLogAdd("時刻フォントを生成しました。\n");//ログに時刻フォント生成成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(6/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int mouseX, mouseY;//マウス座標格納変数
	ErrorLogAdd("マウス座標格納変数を初期化しました。\n");//ログにマウス座標格納変数初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(7/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int onpuon = LoadGraph("image/onpuon2.png");//♪ボタンオン読み込み
	ErrorLogAdd("♪ボタンオンを読み込みました。\n");//ログに♪ボタンオン読み込み成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(8/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int onpuoff = LoadGraph("image/onpuoff2.png");//♪ボタンオフ読み込み
	ErrorLogAdd("♪ボタンオフを読み込みました。\n");//ログに♪ボタンオフ読み込み成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(9/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int BGMinfo = 1; //BGM情報格納変数（1:ON, 0:OFF）
	ErrorLogAdd("BGM情報格納変数を初期化しました。\n");//ログにBGM情報格納変数初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(10/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int prevMouseInput = 0; // 前回のマウス入力状態を格納する変数
	ErrorLogAdd("前回のマウス入力状態を格納する変数を初期化しました。\n");//ログに前回のマウス入力状態変数初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(11/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int mouseInput;// マウス入力状態を格納する変数
	ErrorLogAdd("マウス入力状態を格納する変数を初期化しました。\n");//ログにマウス入力状態変数初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(12/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int botan = LoadSoundMem("Audio/botan.ogg");//ボタン音読み込み
	ErrorLogAdd("ボタン音を読み込みました。\n");//ログにボタン音読み込み成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(13/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	struct tm local;//時刻格納用構造体
	ErrorLogAdd("時刻格納用構造体を初期化しました。\n");//ログに時刻格納用構造体初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(14/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	char timebuf[128];//時刻格納用バッファ
	ErrorLogAdd("時刻格納用バッファを初期化しました。\n");//ログに時刻格納用バッファ初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(15/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	ChangeVolumeSoundMem(255, BGM); //BGMの音量を最大に設定
	ErrorLogAdd("BGMの音量を最大に設定しました。\n");//ログにBGM音量設定成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(16/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	Fps fps;
	ErrorLogAdd("FPSクラスを初期化しました。\n");//ログにFPSクラス初期化成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(17/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int hidaribotan = LoadGraph("Image/hidaribotan.png");//左ボタン読み込み
	ErrorLogAdd("左ボタンを読み込みました。\n");//ログに左ボタン読み込み成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(18/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int migibotan = LoadGraph("Image/migibotan.png");//右ボタン読み込み
	ErrorLogAdd("右ボタンを読み込みました。\n");//ログに右ボタン読み込み成功を記録
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "LoadFinish!", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int GameInfoFile = FileRead_open("launcherInfo.txt", FALSE);//GameInfo.txtを開く
	ErrorLogAdd("GameInfo.txtを開きました。\n");//ログにGameInfo.txt開く成功を記録
	FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
	ErrorLogAdd("GameInfo.txtの先頭に移動しました。\n");//ログにGameInfo.txt先頭移動成功を記録
	char gameInfoLine[256]; //GameInfo.txtの内容を格納する文字列
	ErrorLogAdd("GameInfo.txtの内容を格納する文字列を初期化しました。\n");//ログにGameInfo.txt内容格納文字列初期化成功を記録
	int gameyousocheck;
	ErrorLogAdd("Gameの個数をカウントする変数を初期化しました。\n");//ログにGame個数カウント変数初期化成功を記録
	int gameyouso = 0; //Gameの個数をカウントする変数
	ErrorLogAdd("Gameのタイトル画像のインデックスを格納するベクターを初期化しました。\n");//ログにGameタイトル画像インデックス格納ベクター初期化成功を記録
	int gamecheck = 0;//StringgameInfoLineの配列をカウントする変数
	ErrorLogAdd("StringgameInfoLineの配列をカウントする変数を初期化しました。\n");//ログにStringgameInfoLine配列カウント変数初期化成功を記録
	std::vector<int> gametitleimage; //Gameのタイトル画像のインデックスを格納するベクター
	ErrorLogAdd("Gameのタイトル画像のインデックスを格納するベクターを初期化しました。\n");//ログにGameタイトル画像インデックス格納ベクター初期化成功を記録
	char chargameyouso[256];//Gameの個数を格納する文字列
	ErrorLogAdd("Gameの個数を格納する文字列を初期化しました。\n");//ログにGame個数格納文字列初期化成功を記録
	std::string StringgameInfoLine; //GameInfo.txtの内容を格納する文字列
	ErrorLogAdd("GameInfo.txtの内容を格納する文字列を初期化しました。\n");//ログにGameInfo.txt内容格納文字列初期化成功を記録
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "GameScanning...", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	ErrorLogAdd("GameInfo.txtの内容を読み込み開始しました。\n");//ログにGameInfo.txt内容読み込み開始を記録
	while (FileRead_eof(GameInfoFile) == 0) {
		FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
		StringgameInfoLine = std::string(gameInfoLine); // char型のgameInfoLineをstring型に変換して追加
		gameyousocheck = StringgameInfoLine.find("{");
		if (gameyousocheck != std::string::npos) {
			gameyouso += 1;
			FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
			StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
			gameyousocheck = StringgameInfoLine.find("titleimage:");
			if (gameyousocheck == std::string::npos) {
				ErrorLogAdd("Error: titleimageが見つかりませんでした。\n");//ログにtitleimageが見つからなかったことを記録
				MessageBox(NULL, "Error: titleimageが見つかりませんでした。", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
				ShowTaskbar(); //タスクバーを再表示する
				return -1; //titleimageがなかったら異常終了
			}
			else {
				StringgameInfoLine = StringgameInfoLine.substr(11); // "titleimage:"の後ろの文字列を取得
				gametitleimage.push_back(LoadGraph(StringgameInfoLine.c_str())); // 文字列をグラフィックハンドルに変換してベクターに追加
				if(gametitleimage[gamecheck]==-1){
					ErrorLogAdd("Error: titleimageの画像が見つかりませんでした。\n");//ログにtitleimageの画像が見つからなかったことを記録
					MessageBox(NULL, "Error: titleimageの画像が見つかりませんでした。", "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
					ShowTaskbar(); //タスクバーを再表示する
					return -1; //titleimageの画像がなかったら異常終了
				}
				FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
				StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
				gameyousocheck = StringgameInfoLine.find("}");// "}"の位置を探す
				if (gameyousocheck == std::string::npos) {
					return -1;// "}"がなかったら異常終了
				}
			}
		}
	}
	FileRead_close(GameInfoFile); // ファイルを閉じる
	ErrorLogAdd("GameInfo.txtの内容を読み込みました。\n");//ログにGameInfo.txt内容読み込み成功を記録
	sprintf(chargameyouso, "%d", gameyouso); //Gameの個数を文字列に変換
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "Complete!", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int feding; //フェードイン用変数
	ErrorLogAdd("フェードイン用変数を初期化しました。\n");//ログにフェードイン用変数初期化成功を記録
	int page = 1;//ページ番号変数
	ErrorLogAdd("ページ番号変数を初期化しました。\n");//ログにページ番号変数初期化成功を記録
	int pagekasan = 0;
	ErrorLogAdd("ページ番号加算変数を初期化しました。\n");//ログにページ番号加算変数初期化成功を記録
	int pagetotal = gameyouso / 7 + 1;
	ErrorLogAdd("ページ総数を計算しました。\n");//ログにページ総数計算成功を記録
	int gametypecheck = 0; //ゲームの種類を判別する変数
	ErrorLogAdd("ゲームの種類を判別する変数を初期化しました。\n");//ログにゲーム種類判別変数初期化成功を記録
	int gametitleinfo = 0;
	ErrorLogAdd("ゲームタイトル情報変数を初期化しました。\n");//ログにゲームタイトル情報変数初期化成功を記録
	std::string gametitle;
	ErrorLogAdd("ゲームタイトル変数を初期化しました。\n");//ログにゲームタイトル変数初期化成功を記録
	std::string gameex;
	ErrorLogAdd("ゲーム説明変数を初期化しました。\n");//ログにゲーム説明変数初期化成功を記録
	int gameeximage;
	ErrorLogAdd("ゲーム説明画像変数を初期化しました。\n");//ログにゲーム説明画像変数初期化成功を記録
	int gameeximageinfo = 0;
	ErrorLogAdd("ゲーム説明画像情報変数を初期化しました。\n");//ログにゲーム説明画像情報変数初期化成功を記録
	std::string gameexe;
	ErrorLogAdd("ゲーム実行ファイル変数を初期化しました。\n");//ログにゲーム実行ファイル変数初期化成功を記録
	int gamescan = 0;
	ErrorLogAdd("ゲームスキャン変数を初期化しました。\n");//ログにゲームスキャン変数初期化成功を記録
	std::string searchkey;
	ErrorLogAdd("検索キー変数を初期化しました。\n");//ログに検索キー変数初期化成功を記録
	int Endresult;
	ErrorLogAdd("終了結果変数を初期化しました。\n");//ログに終了結果変数初期化成功を記録
	int playbotan = LoadGraph("Image/play.png");//プレイボタン読み込み
	ErrorLogAdd("プレイボタンを読み込みました。\n");//ログにプレイボタン読み込み成功を記録
	int gameexinfo = 0;
	ErrorLogAdd("ゲーム説明情報変数を初期化しました。\n");//ログにゲーム説明情報変数初期化成功を記録
	int gameexeinfo = 0;
	ErrorLogAdd("ゲーム実行ファイル情報変数を初期化しました。\n");//ログにゲーム実行ファイル情報初期化成功を記録
	std::string commandline;
	ErrorLogAdd("コマンドライン変数を初期化しました。\n");//ログにコマンドライン変数初期化成功を記録
	DWORD exitCode;
	ErrorLogAdd("終了コード変数を初期化しました。\n");//ログに終了コード変数初期化成功を記録
	std::string gamedir;
	ErrorLogAdd("ゲームディレクトリ変数を初期化しました。\n");//ログにゲームディレクトリ変数初期化成功を記録
	std::string basedir;
	ErrorLogAdd("ベースディレクトリ変数を初期化しました。\n");//ログにベースディレクトリ変数初期化成功を記録
	std::string fulldir;
	ErrorLogAdd("フルディレクトリ変数を初期化しました。\n");//ログにフルディレクトリ変数初期化成功を記録
	std::string fullexepath;
	ErrorLogAdd("フル実行ファイルパス変数を初期化しました");//ログにフル実行ファイルパス変数初期化成功を記録
	DWORD exitcode;
	ErrorLogAdd("終了コード変数を初期化しました。\n");//ログに終了コード変数初期化成功を記録
	int logo = LoadGraph("Image/logo.png");//ロゴ読み込み
	ErrorLogAdd("ロゴを読み込みました。\n");//ログにロゴ読み込み成功を記録
	int timer = 0;
	ErrorLogAdd("タイマー変数を初期化しました。\n");//ログにタイマー変数初期化成功を記録
	int timerinfo = 0;
	ErrorLogAdd("タイマー情報変数を初期化しました。\n");//ログにタイマー情報変数初期化成功を記録
	int focuspage = 0;;
	ErrorLogAdd("フォーカスページ変数を初期化しました。\n");//ログにフォーカスページ変数初期化成功を記録
	int focusgame = 0;
	ErrorLogAdd("フォーカスゲーム変数を初期化しました。\n");//ログにフォーカスゲーム変数初期化成功を記録
	int focus = LoadGraph("Image/focus.png");//フォーカス画像読み込み
	ErrorLogAdd("フォーカス画像を読み込みました。\n");//ログにフォーカス画像読み込み成功を記録
	int timerhyouzi = 0;
	ErrorLogAdd("タイマー表示変数を初期化しました。");
	ErrorLogAdd("すべてのデータの読み込みに成功しました!\n"); //ログにすべてのデータ読み込み成功を記録
	if(gameyouso == 0){
		ErrorLogAdd("Warning: Gameが一つも見つかりませんでした。\n");//ログにGameが一つも見つからなかったことを記録
		MessageBox(NULL, "Warning: Gameが一つも見つかりませんでした。launcherInfo.txtの内容を確認してください。", "Error", MB_OK | MB_ICONWARNING | MB_TOPMOST);
	}
	ErrorLogAdd(("ゲーム数：" + std::to_string(gameyouso) + "\n").c_str());//ログにゲーム数を記録
	ErrorLogAdd(("ページ総数：" + std::to_string(pagetotal) + "\n").c_str());//ログにページ総数を記録
	ErrorLogAdd(("pagekasan:" + std::to_string(pagekasan) + "\n").c_str());//ログにpagekasanを記録
	fukki:
	for (feding = 225; feding != -1; --feding) {
		ClearDrawScreen(); //裏画面をクリア
		DrawGraph(1000, 700, logo, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, feding); //フェードインのためのブレンドモード設定
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); //画面全体を黒で塗りつぶす
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //ブレンドモードを元に戻す
		ScreenFlip(); //描画した内容を画面に反映
		ProcessMessage();
	}
	while (ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		if (CheckHitKey(KEY_INPUT_F1) && CheckHitKey(KEY_INPUT_F4) && CheckHitKey(KEY_INPUT_F10)) {
			// 3つのキーが同時に押されたら確認
			ErrorLogAdd("終了操作が要求されました。\n");//ログに終了操作要求を記録
			Endresult = MessageBox(NULL, "この操作は管理者専用です。いいえを押して退出してください。", "確認", MB_YESNO | MB_ICONWARNING | MB_TOPMOST);
			if (Endresult == IDYES) {
				ErrorLogAdd("終了操作が確認されました。プログラムを終了します。\n");//ログに終了操作確認を記録
				MessageBox(NULL, "プログラムを終了します。", "終了", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
				// ウインドウプロシージャのフック解除
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)g_OldWndProc);
				ShowTaskbar();
				DxLib_End(); // DXライブラリの終了処理
				return 0; // プログラムの終了
			}
			else {
				ErrorLogAdd("終了操作がキャンセルされました。\n");//ログに終了操作キャンセルを記録
				MessageBox(NULL, "処理を中断しました。", "警告", MB_OK | MB_ICONWARNING | MB_TOPMOST);
			}
		}
		if(CheckHitKey(KEY_INPUT_F7)){
			if (timerinfo == 1) {
				if (timerhyouzi == 0) {
					timerhyouzi = 1;
					ErrorLogAdd("タイマーが表示されました。\n");
				}
				else if (timerhyouzi == 1) {
					timerhyouzi = 0;
					ErrorLogAdd("タイマーが非表示にされました。\n");
				}
				Sleep(100); // 連続判定防止
			}
		}
		if (CheckHitKey(KEY_INPUT_F5)) {
			if (timerinfo == 0) {
				ErrorLogAdd("タイマー有効化操作が要求されました。\n");//ログにタイマー有効化操作要求を記録
				int result = MessageBox(NULL, "タイマーを有効にしますか？", "確認", MB_YESNO | MB_ICONWARNING | MB_TOPMOST);
				if (result == IDYES) {
					GameInfoFile = FileRead_open("timer.txt", FALSE);//GameInfo.txtを開く
					ErrorLogAdd("timer.txtを開きました。\n");//ログにtimer.txt開く成功を記録
					FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
					ErrorLogAdd("timer.txtの先頭に移動しました。\n");//ログにtimer.txt先頭移動成功を記録
					FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
					timer = atoi(gameInfoLine);
					FileRead_close(GameInfoFile); // ファイルを閉じる
					ErrorLogAdd("timer.txtを読み込みました。\n");//ログにtimer.txt読み込み成功を記録
					timerinfo = 1;
					ErrorLogAdd("タイマーを有効にしました。\n");
					MessageBox(NULL, "タイマーを有効にしました。", "情報", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
				}
				else {
					ErrorLogAdd("処理が中断されました。\n");
					MessageBox(NULL, "処理を中断しました。", "警告", MB_OK | MB_ICONWARNING | MB_TOPMOST);
				}
			}else if(timerinfo == 1){
				ErrorLogAdd("タイマー無効化操作が要求されました。\n");//ログにタイマー無効化操作要求を記録
				int result = MessageBox(NULL, "この操作は管理者専用です。いいえをクリックして戻ってください。", "確認", MB_YESNO | MB_ICONWARNING | MB_TOPMOST);
				if (result == IDYES) {
					timerinfo = 0;
					ErrorLogAdd("タイマーを無効にしました。\n");
					MessageBox(NULL, "タイマーを無効にしました。", "情報", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
				}
				else {
					ErrorLogAdd("処理が中断されました。\n");
					MessageBox(NULL, "処理を中断しました。", "警告", MB_OK | MB_ICONWARNING | MB_TOPMOST);
				}
			}
			// F5の連続判定防止のため、少し待機
			Sleep(200);
		}
		if (timerinfo == 1) {
			if (CheckHitKey(KEY_INPUT_LCONTROL) && CheckHitKey(KEY_INPUT_LALT) && CheckHitKey(KEY_INPUT_X)) {
				ErrorLogAdd("タイマーリセット操作が要求されました。\n");//ログにタイマー初期化操作要求を記録
				int result = MessageBox(NULL, "この操作は管理者専用です。いいえを押して戻ってください。", "確認", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
				if (result == IDYES) {
					int GameInfoFile = FileRead_open("timer.txt", FALSE);
					char gameInfoLine[256];
					FileRead_seek(GameInfoFile, 0, SEEK_SET);
					FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);
					timer = atoi(gameInfoLine);
					FileRead_close(GameInfoFile);
					ErrorLogAdd("タイマーがリセットされました。\n");
					MessageBox(NULL, "タイマーをリセットしました。", "情報", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
				}
				else {
					ErrorLogAdd("処理が中断されました。。\n");
					MessageBox(NULL, "処理を中断しました。", "警告", MB_OK | MB_ICONWARNING | MB_TOPMOST);
				}
				Sleep(200); // 連続判定防止
			}
		}
		fps.Update();	//FPS更新
		time_t t = time(NULL);//現在時刻を取得
		localtime_s(&local, &t);//現在時刻をローカル時間に変換
		strftime(timebuf, sizeof(timebuf), "%H:%M", &local);//時刻をフォーマットして文字列に変換
		GetMousePoint(&mouseX, &mouseY); //マウス座標取得
		SetDrawMode(DX_DRAWMODE_BILINEAR);//描画モードをバイリニアフィルタに設定(アンチエイリアス効果)
		DrawRotaGraph(60, 50, 1.0, PI * Communicationmarkangle, Communicationmark, TRUE); //コミュニケーションマーク描画
		SetDrawMode(DX_DRAWMODE_NEAREST);//描画モードをニアレストネイバーに設定(アンチエイリアス効果なし)
		if (gametitleinfo == 0) {
			DrawStringToHandle(450, 10, "柏苑祭", GetColor(0, 0, 0), titlefont);//タイトル描画
			DrawStringToHandle(785, 10, "2025", GetColor(255, 0, 0), titlefont);//タイトル描画2
		}
		else {
			DrawStringToHandle(150, 10, gametitle.c_str(), GetColor(0, 0, 0), titlefont);//ゲームタイトル描画
		}
		DrawBox(0, 105, 1920, 110, GetColor(255, 0, 0), TRUE);//赤い線描画
		DrawStringToHandle(1320, 5, "♪昼下がり気分", GetColor(0, 0, 0), font);//BGM名描画
		if (BGMinfo == 1) { //BGMがONなら
			DrawGraph(1400, 35, onpuon, TRUE); //♪ボタンオン描画
		}
		else if (BGMinfo == 0) { //BGMがOFFなら
			DrawGraph(1400, 35, onpuoff, TRUE); //♪ボタンオフ描画
		}
		DrawBox(0, 600, 1920, 800, GetColor(255, 160, 160), TRUE);//赤いボックス描画
		DrawBox(0, 800, 1920, 900, GetColor(100, 100, 100), TRUE);//灰色のボックス描画
		DrawStringToHandle(1375, 803, timebuf, GetColor(255, 255, 255), timefont);//時刻描画
		DrawLineAA(1370, 800, 1370, 900, GetColor(255, 255, 255));//時刻の縦線描画
		DrawFormatStringToHandle(5, 803, GetColor(255, 255, 255), timefont, "Page:%d/%d", page, pagetotal);//ページ番号描画
		DrawGraph(10, 650, hidaribotan, TRUE); //左ボタン描画
		DrawGraph(1420, 650, migibotan, TRUE); //右ボタン描画
		DrawBox(60, 145, 1470, 560, GetColor(200, 200, 200), TRUE);//灰色ボックス描画
		if (gameexeinfo == 1) {
			DrawGraph(70, 450, playbotan, TRUE); //プレイボタン描画
		}
		//DrawBox(70, 450, 778, 550, GetColor(255, 0, 0), TRUE);//赤色ボックス描画(テスト)
		if (gameexinfo == 1) {
			DrawStringToHandle(70, 150, gameex.c_str(), GetColor(0, 0, 0), font);//ゲーム説明描画
		}
		if (gameeximageinfo == 1) {
			DrawGraph(790, 173, gameeximage, TRUE);//ゲーム説明画像描画
		}
		if (page == focuspage && focusgame >= 1 && focusgame <= 7) {
			int x = 80 + (focusgame - 1) * 195;
			DrawGraph(x, 603, focus, TRUE);
		}
		//if (focusgame == 1 && page == focuspage) {
		//	DrawGraph(80, 603, focus, TRUE);
		//}
		//else if (focusgame == 2 && page == focuspage) {
		//	DrawGraph(275, 603, focus, TRUE);
		//}
		//else if (focusgame == 3 && page == focuspage) {
		//	DrawGraph(470, 603, focus, TRUE);
		//}
		//else if (focusgame == 4 && page == focuspage) {
		//	DrawGraph(665, 603, focus, TRUE);
		//}
		//else if (focusgame == 5 && page == focuspage) {
		//	DrawGraph(860, 603, focus, TRUE);
		//}
		//else if (focusgame == 6 && page == focuspage) {
		//	DrawGraph(1055, 603, focus, TRUE);
		//}
		//else if (focusgame == 7 && page == focuspage) {
		//	DrawGraph(1250, 603, focus, TRUE);
		//}
		DrawBox(790, 173, 1430, 533, GetColor(0, 0, 0), TRUE);//黒いボックス描画
		DrawBox(90, 613, 265, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画0
		DrawBox(285, 613, 460, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画1
		DrawBox(480, 613, 655, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画2
		DrawBox(675, 613, 850, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画3
		DrawBox(870, 613, 1045, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画4
		DrawBox(1065, 613, 1240, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画5
		DrawBox(1260, 613, 1435, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画6
		for (int i = 0; i < 7; ++i) {
			if (gameyouso > i + pagekasan) {
				int x = 90 + i * 195;
				DrawGraph(x, 613, gametitleimage[i + pagekasan], false);
						}
					}
		if (timerhyouzi == 1) {
			DrawFormatString(0, 0, GetColor(0, 0, 0), "%d",timer);
		}
		fps.Wait();		//待機
		if (BGMCount % 28200 == 0) { // 235秒に一度
			BGMCount = 0;//カウントリセット
			PlaySoundMem(BGM, DX_PLAYTYPE_BACK); // 効果音を再生する
		}

		BGMCount++;//プラス
		Communicationmarkangle += 0.0005;//回転!
		if (Communicationmarkangle == 4)/*一回転したら*/ {
			Communicationmarkangle = 2;//角度リセット
		}

		mouseInput = GetMouseInput();
			if ((mouseInput & MOUSE_INPUT_LEFT) != 0 && (prevMouseInput & MOUSE_INPUT_LEFT) == 0) { // 押された瞬間
				if (mouseX >= 1400 && mouseX <= 1500 && mouseY >= 35 && mouseY <= 135) { //♪ボタンの範囲内なら
					fps.FPS = 1;
					if (BGMinfo == 1) {//BGMがONなら
						ChangeVolumeSoundMem(0, BGM); //BGMの音量を0に設定
						BGMinfo = 0; //BGM情報をOFFに変更
						PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					}
					else { //BGMがOFFなら
						ChangeVolumeSoundMem(255, BGM); //BGMの音量を最大に設定
						BGMinfo = 1; //BGM情報をONに変更
						PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 10 && mouseX <= 110 && mouseY >= 650 && mouseY <= 750) { //左ボタンの範囲内なら
					fps.FPS = 1;
					if (page > 1) { //ページが1より大きいなら
						page -= 1; //ページを1つ戻す
						pagekasan -= 7; //ページカウントを戻す
						PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 1420 && mouseX <= 1520 && mouseY >= 650 && mouseY <= 750)/*右ボタンの範囲内なら*/ {
					fps.FPS = 1;
					if (page != pagetotal) {
						page += 1; //ページを1つ進める
						pagekasan += 7; //ページカウントを進める
						PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 90 && mouseX <= 265 && mouseY >= 613 && mouseY <= 788) { // ゲーム1の範囲内なら
					fps.FPS = 1;
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					if (gameyouso >= 1 + pagekasan) {
						gamescan = 0;
						GameInfoFile = FileRead_open("GamaInfo.txt", FALSE);//GameInfo.txtを開く
						FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
						while (FileRead_eof(GameInfoFile) == 0 && gamescan == 0) {
							FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
							StringgameInfoLine = trim(std::string(gameInfoLine));
							searchkey = std::to_string(pagekasan) + ":{";
							gamecheck = StringgameInfoLine.find(searchkey);
							if (gamecheck != std::string::npos) {
								FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
								StringgameInfoLine = trim(std::string(gameInfoLine));
								gamecheck = StringgameInfoLine.find("type:");
								if (gamecheck != std::string::npos) {
									StringgameInfoLine = trim(StringgameInfoLine.substr(5));
									if (StringgameInfoLine == "note") {
										gametypecheck = 1;
									}
									else if (StringgameInfoLine == "game") {
										gametypecheck = 2;
									}
									else {
										ErrorLogAdd("typeが指定された文字列ではありません。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがnoteでもgameでもなかったら異常終了
									}
									FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
									StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
									gamecheck = StringgameInfoLine.find("title:");
									if (gamecheck != std::string::npos) {
										StringgameInfoLine = trim(StringgameInfoLine.substr(6)); // "title:"の後ろの文字列を取得
										gametitleinfo = 1;
										gametitle = StringgameInfoLine;
										FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
										StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
										gamecheck = StringgameInfoLine.find("ex:");
										if (gamecheck != std::string::npos) {
											StringgameInfoLine = trim(StringgameInfoLine.substr(3)); // "ex:"の後ろの文字列を取得
											gameex = StringgameInfoLine;
											gameexinfo = 1;
											FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
											StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
											gamecheck = StringgameInfoLine.find("eximage:");
											if (gamecheck != std::string::npos) {
												StringgameInfoLine = trim(StringgameInfoLine.substr(8)); // "eximage:"の後ろの文字列を取得
												gameeximage = LoadGraph(StringgameInfoLine.c_str());
												gameeximageinfo = 1;
												if (gametypecheck == 1) {
													gamescan = 1;
													gameexeinfo = 0;
													gameexe = "";
													gamedir = "";
													basedir = "";
													fulldir = "";
													fullexepath = "";
													break;
												}
												if (gametypecheck == 2) {
													FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
													StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
													gamecheck = StringgameInfoLine.find("dir:");// "dir:"の位置を探す
													if (gamecheck != std::string::npos) {
														StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "dir:"の後ろの文字列を取得
														gamedir = StringgameInfoLine;
														FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
														StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
														if (gamecheck != std::string::npos) {
															StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "exe:"の後ろの文字列を取得
															gameexe = StringgameInfoLine;
															gameexeinfo = 1;
															FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
															StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
															gamecheck = StringgameInfoLine.find("}");// "}"の位置を探す
															if (gamecheck != std::string::npos) {
																gamescan = 1;
															}
															else {
																ErrorLogAdd("}が不明です。\n");//ログにゲーム情報読み込み失敗を記録
																MessageBox(NULL, "ゲーム情報の終端が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
																ShowTaskbar();
																return -1; //"}"がなかったら異常終了
															}
														}
														else {
															ErrorLogAdd("exeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
															MessageBox(NULL, "ゲームの実行ファイルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
															ShowTaskbar();
															return -1; //exeがなかったら異常終了
														}
													}
													else {
														ErrorLogAdd("dirが不明です。\n");//ログにゲーム情報読み込み失敗を記録
														MessageBox(NULL, "ゲームのディレクトリが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
														ShowTaskbar();
														return -1; //dirがなかったら異常終了
													}
												}
												else {
													ErrorLogAdd("eximageが不明です。\n");//ログにゲーム情報読み込み失敗を記録
												}
											}
											else {
												ErrorLogAdd("exが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											}
										}
										else {
											ErrorLogAdd("titleが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											MessageBox(NULL, "ゲームタイトルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
											ShowTaskbar();
											return -1; //titleがなかったら異常終了
										}
									}
									else {
										ErrorLogAdd("typeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがなかったら異常終了
									}
								}
								else {
									continue;
								}
							}
						}
						FileRead_close(GameInfoFile); // ファイルを閉じる
						if (gamescan == 0) {
							ErrorLogAdd("ゲーム情報が見つかりません。\n");//ログにゲーム情報読み込み失敗を記録
							MessageBox(NULL, "ゲーム情報が見つかりません。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
							ShowTaskbar();
							return -1; //ゲーム情報が見つからなかったら異常終了
						}
						ErrorLogAdd("ゲーム情報を読み込みました。\n");//ログにゲーム情報読み込み成功を記録
						ErrorLogAdd(("Scan:" + std::to_string(gamescan) + "\n").c_str());//ログにスキャンしたゲーム番号を記録
						ErrorLogAdd(("ゲーム種類:" + std::to_string(gametypecheck) + "\n").c_str());//ログにゲーム種類を記録
						ErrorLogAdd(("ゲームタイトル:" + gametitle + "\n").c_str());//ログにゲームタイトルを記録
						focusgame = 1;
						focuspage = page;
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 285 && mouseX <= 460 && mouseY >= 613 && mouseY <= 788) { // ゲーム2の範囲内なら
					fps.FPS = 1;
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					if (gameyouso >= 2 + pagekasan) {
						gamescan = 0;
						GameInfoFile = FileRead_open("GamaInfo.txt", FALSE);//GameInfo.txtを開く
						FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
						while (FileRead_eof(GameInfoFile) == 0 && gamescan == 0) {
							FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
							StringgameInfoLine = trim(std::string(gameInfoLine));
							searchkey = std::to_string(1 + pagekasan) + ":{";
							gamecheck = StringgameInfoLine.find(searchkey);
							if (gamecheck != std::string::npos) {
								FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
								StringgameInfoLine = trim(std::string(gameInfoLine));
								gamecheck = StringgameInfoLine.find("type:");
								if (gamecheck != std::string::npos) {
									StringgameInfoLine = trim(StringgameInfoLine.substr(5));
									if (StringgameInfoLine == "note") {
										gametypecheck = 1;
									}
									else if (StringgameInfoLine == "game") {
										gametypecheck = 2;
									}
									else {
										ErrorLogAdd("typeが指定された文字列ではありません。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがnoteでもgameでもなかったら異常終了
									}
									FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
									StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
									gamecheck = StringgameInfoLine.find("title:");
									if (gamecheck != std::string::npos) {
										StringgameInfoLine = trim(StringgameInfoLine.substr(6)); // "title:"の後ろの文字列を取得
										gametitleinfo = 1;
										gametitle = StringgameInfoLine;
										FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
										StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
										gamecheck = StringgameInfoLine.find("ex:");
										if (gamecheck != std::string::npos) {
											StringgameInfoLine = trim(StringgameInfoLine.substr(3)); // "ex:"の後ろの文字列を取得
											gameex = StringgameInfoLine;
											gameexinfo = 1;
											FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
											StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
											gamecheck = StringgameInfoLine.find("eximage:");
											if (gamecheck != std::string::npos) {
												StringgameInfoLine = trim(StringgameInfoLine.substr(8)); // "eximage:"の後ろの文字列を取得
												gameeximage = LoadGraph(StringgameInfoLine.c_str());
												gameeximageinfo = 1;
												if (gametypecheck == 1) {
													gamescan = 1;
													gameexeinfo = 0;
													gameexe = "";
													gamedir = "";
													basedir = "";
													fulldir = "";
													fullexepath = "";
													break;
												}
												if (gametypecheck == 2) {
													FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
													StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
													gamecheck = StringgameInfoLine.find("dir:");// "dir:"の位置を探す
													if (gamecheck != std::string::npos) {
														StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "dir:"の後ろの文字列を取得
														gamedir = StringgameInfoLine;
														FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
														StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
														if (gamecheck != std::string::npos) {
															StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "exe:"の後ろの文字列を取得
															gameexe = StringgameInfoLine;
															gameexeinfo = 1;
															FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
															StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
															gamecheck = StringgameInfoLine.find("}");// "}"の位置を探す
															if (gamecheck != std::string::npos) {
																gamescan = 1;
															}
															else {
																ErrorLogAdd("}が不明です。\n");//ログにゲーム情報読み込み失敗を記録
																MessageBox(NULL, "ゲーム情報の終端が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
																ShowTaskbar();
																return -1; //"}"がなかったら異常終了
															}
														}
														else {
															ErrorLogAdd("exeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
															MessageBox(NULL, "ゲームの実行ファイルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
															ShowTaskbar();
															return -1; //exeがなかったら異常終了
														}
													}
													else {
														ErrorLogAdd("dirが不明です。\n");//ログにゲーム情報読み込み失敗を記録
														MessageBox(NULL, "ゲームのディレクトリが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
														ShowTaskbar();
														return -1; //dirがなかったら異常終了
													}
												}
												else {
													ErrorLogAdd("eximageが不明です。\n");//ログにゲーム情報読み込み失敗を記録
												}
											}
											else {
												ErrorLogAdd("exが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											}
										}
										else {
											ErrorLogAdd("titleが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											MessageBox(NULL, "ゲームタイトルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
											ShowTaskbar();
											return -1; //titleがなかったら異常終了
										}
									}
									else {
										ErrorLogAdd("typeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがなかったら異常終了
									}
								}
								else {
									continue;
								}
							}
						}
						FileRead_close(GameInfoFile); // ファイルを閉じる
						if (gamescan == 0) {
							ErrorLogAdd("ゲーム情報が見つかりません。\n");//ログにゲーム情報読み込み失敗を記録
							MessageBox(NULL, "ゲーム情報が見つかりません。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
							ShowTaskbar();
							return -1; //ゲーム情報が見つからなかったら異常終了
						}
						ErrorLogAdd("ゲーム情報を読み込みました。\n");//ログにゲーム情報読み込み成功を記録
						ErrorLogAdd(("Scan:" + std::to_string(gamescan) + "\n").c_str());//ログにスキャンしたゲーム番号を記録
						ErrorLogAdd(("ゲーム種類:" + std::to_string(gametypecheck) + "\n").c_str());//ログにゲーム種類を記録
						ErrorLogAdd(("ゲームタイトル:" + gametitle + "\n").c_str());//ログにゲームタイトルを記録
						focusgame = 2;
						focuspage = page;
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 480 && mouseX <= 655 && mouseY >= 613 && mouseY <= 788) { // ゲーム3の範囲内なら
					fps.FPS = 1;
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					if (gameyouso >= 3 + pagekasan) {
						gamescan = 0;
						GameInfoFile = FileRead_open("GamaInfo.txt", FALSE);//GameInfo.txtを開く
						FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
						while (FileRead_eof(GameInfoFile) == 0 && gamescan == 0) {
							FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
							StringgameInfoLine = trim(std::string(gameInfoLine));
							searchkey = std::to_string(2 + pagekasan) + ":{";
							gamecheck = StringgameInfoLine.find(searchkey);
							if (gamecheck != std::string::npos) {
								FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
								StringgameInfoLine = trim(std::string(gameInfoLine));
								gamecheck = StringgameInfoLine.find("type:");
								if (gamecheck != std::string::npos) {
									StringgameInfoLine = trim(StringgameInfoLine.substr(5));
									if (StringgameInfoLine == "note") {
										gametypecheck = 1;
									}
									else if (StringgameInfoLine == "game") {
										gametypecheck = 2;
									}
									else {
										ErrorLogAdd("typeが指定された文字列ではありません。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがnoteでもgameでもなかったら異常終了
									}
									FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
									StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
									gamecheck = StringgameInfoLine.find("title:");
									if (gamecheck != std::string::npos) {
										StringgameInfoLine = trim(StringgameInfoLine.substr(6)); // "title:"の後ろの文字列を取得
										gametitleinfo = 1;
										gametitle = StringgameInfoLine;
										FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
										StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
										gamecheck = StringgameInfoLine.find("ex:");
										if (gamecheck != std::string::npos) {
											StringgameInfoLine = trim(StringgameInfoLine.substr(3)); // "ex:"の後ろの文字列を取得
											gameex = StringgameInfoLine;
											gameexinfo = 1;
											FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
											StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
											gamecheck = StringgameInfoLine.find("eximage:");
											if (gamecheck != std::string::npos) {
												StringgameInfoLine = trim(StringgameInfoLine.substr(8)); // "eximage:"の後ろの文字列を取得
												gameeximage = LoadGraph(StringgameInfoLine.c_str());
												gameeximageinfo = 1;
												if (gametypecheck == 1) {
													gamescan = 1;
													gameexeinfo = 0;
													gameexe = "";
													gamedir = "";
													basedir = "";
													fulldir = "";
													fullexepath = "";
													break;
												}
												if (gametypecheck == 2) {
													FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
													StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
													gamecheck = StringgameInfoLine.find("dir:");// "dir:"の位置を探す
													if (gamecheck != std::string::npos) {
														StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "dir:"の後ろの文字列を取得
														gamedir = StringgameInfoLine;
														FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
														StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
														if (gamecheck != std::string::npos) {
															StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "exe:"の後ろの文字列を取得
															gameexe = StringgameInfoLine;
															gameexeinfo = 1;
															FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
															StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
															gamecheck = StringgameInfoLine.find("}");// "}"の位置を探す
															if (gamecheck != std::string::npos) {
																gamescan = 1;
															}
															else {
																ErrorLogAdd("}が不明です。\n");//ログにゲーム情報読み込み失敗を記録
																MessageBox(NULL, "ゲーム情報の終端が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
																ShowTaskbar();
																return -1; //"}"がなかったら異常終了
															}
														}
														else {
															ErrorLogAdd("exeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
															MessageBox(NULL, "ゲームの実行ファイルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
															ShowTaskbar();
															return -1; //exeがなかったら異常終了
														}
													}
													else {
														ErrorLogAdd("dirが不明です。\n");//ログにゲーム情報読み込み失敗を記録
														MessageBox(NULL, "ゲームのディレクトリが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
														ShowTaskbar();
														return -1; //dirがなかったら異常終了
													}
												}
												else {
													ErrorLogAdd("eximageが不明です。\n");//ログにゲーム情報読み込み失敗を記録
												}
											}
											else {
												ErrorLogAdd("exが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											}
										}
										else {
											ErrorLogAdd("titleが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											MessageBox(NULL, "ゲームタイトルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
											ShowTaskbar();
											return -1; //titleがなかったら異常終了
										}
									}
									else {
										ErrorLogAdd("typeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがなかったら異常終了
									}
								}
								else {
									continue;
								}
							}
						}
						FileRead_close(GameInfoFile); // ファイルを閉じる
						if (gamescan == 0) {
							ErrorLogAdd("ゲーム情報が見つかりません。\n");//ログにゲーム情報読み込み失敗を記録
							MessageBox(NULL, "ゲーム情報が見つかりません。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
							ShowTaskbar();
							return -1; //ゲーム情報が見つからなかったら異常終了
						}
						ErrorLogAdd("ゲーム情報を読み込みました。\n");//ログにゲーム情報読み込み成功を記録
						ErrorLogAdd(("Scan:" + std::to_string(gamescan) + "\n").c_str());//ログにスキャンしたゲーム番号を記録
						ErrorLogAdd(("ゲーム種類:" + std::to_string(gametypecheck) + "\n").c_str());//ログにゲーム種類を記録
						ErrorLogAdd(("ゲームタイトル:" + gametitle + "\n").c_str());//ログにゲームタイトルを記録
						focusgame = 3;
						focuspage = page;
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 675 && mouseX <= 850 && mouseY >= 613 && mouseY <= 788) { // ゲーム4の範囲内なら
					fps.FPS = 1;
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					if (gameyouso >= 4 + pagekasan) {
						gamescan = 0;
						GameInfoFile = FileRead_open("GamaInfo.txt", FALSE);//GameInfo.txtを開く
						FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
						while (FileRead_eof(GameInfoFile) == 0 && gamescan == 0) {
							FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
							StringgameInfoLine = trim(std::string(gameInfoLine));
							searchkey = std::to_string(3 + pagekasan) + ":{";
							gamecheck = StringgameInfoLine.find(searchkey);
							if (gamecheck != std::string::npos) {
								FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
								StringgameInfoLine = trim(std::string(gameInfoLine));
								gamecheck = StringgameInfoLine.find("type:");
								if (gamecheck != std::string::npos) {
									StringgameInfoLine = trim(StringgameInfoLine.substr(5));
									if (StringgameInfoLine == "note") {
										gametypecheck = 1;
									}
									else if (StringgameInfoLine == "game") {
										gametypecheck = 2;
									}
									else {
										ErrorLogAdd("typeが指定された文字列ではありません。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがnoteでもgameでもなかったら異常終了
									}
									FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
									StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
									gamecheck = StringgameInfoLine.find("title:");
									if (gamecheck != std::string::npos) {
										StringgameInfoLine = trim(StringgameInfoLine.substr(6)); // "title:"の後ろの文字列を取得
										gametitleinfo = 1;
										gametitle = StringgameInfoLine;
										FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
										StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
										gamecheck = StringgameInfoLine.find("ex:");
										if (gamecheck != std::string::npos) {
											StringgameInfoLine = trim(StringgameInfoLine.substr(3)); // "ex:"の後ろの文字列を取得
											gameex = StringgameInfoLine;
											gameexinfo = 1;
											FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
											StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
											gamecheck = StringgameInfoLine.find("eximage:");
											if (gamecheck != std::string::npos) {
												StringgameInfoLine = trim(StringgameInfoLine.substr(8)); // "eximage:"の後ろの文字列を取得
												gameeximage = LoadGraph(StringgameInfoLine.c_str());
												gameeximageinfo = 1;
												if (gametypecheck == 1) {
													gamescan = 1;
													gameexeinfo = 0;
													gameexe = "";
													gamedir = "";
													basedir = "";
													fulldir = "";
													fullexepath = "";
													break;
												}
												if (gametypecheck == 2) {
													FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
													StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
													gamecheck = StringgameInfoLine.find("dir:");// "dir:"の位置を探す
													if (gamecheck != std::string::npos) {
														StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "dir:"の後ろの文字列を取得
														gamedir = StringgameInfoLine;
														FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
														StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
														if (gamecheck != std::string::npos) {
															StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "exe:"の後ろの文字列を取得
															gameexe = StringgameInfoLine;
															gameexeinfo = 1;
															FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
															StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
															gamecheck = StringgameInfoLine.find("}");// "}"の位置を探す
															if (gamecheck != std::string::npos) {
																gamescan = 1;
															}
															else {
																ErrorLogAdd("}が不明です。\n");//ログにゲーム情報読み込み失敗を記録
																MessageBox(NULL, "ゲーム情報の終端が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
																ShowTaskbar();
																return -1; //"}"がなかったら異常終了
															}
														}
														else {
															ErrorLogAdd("exeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
															MessageBox(NULL, "ゲームの実行ファイルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
															ShowTaskbar();
															return -1; //exeがなかったら異常終了
														}
													}
													else {
														ErrorLogAdd("dirが不明です。\n");//ログにゲーム情報読み込み失敗を記録
														MessageBox(NULL, "ゲームのディレクトリが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
														ShowTaskbar();
														return -1; //dirがなかったら異常終了
													}
												}
												else {
													ErrorLogAdd("eximageが不明です。\n");//ログにゲーム情報読み込み失敗を記録
												}
											}
											else {
												ErrorLogAdd("exが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											}
										}
										else {
											ErrorLogAdd("titleが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											MessageBox(NULL, "ゲームタイトルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
											ShowTaskbar();
											return -1; //titleがなかったら異常終了
										}
									}
									else {
										ErrorLogAdd("typeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがなかったら異常終了
									}
								}
								else {
									continue;
								}
							}
						}
						FileRead_close(GameInfoFile); // ファイルを閉じる
						if (gamescan == 0) {
							ErrorLogAdd("ゲーム情報が見つかりません。\n");//ログにゲーム情報読み込み失敗を記録
							MessageBox(NULL, "ゲーム情報が見つかりません。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
							ShowTaskbar();
							return -1; //ゲーム情報が見つからなかったら異常終了
						}
						ErrorLogAdd("ゲーム情報を読み込みました。\n");//ログにゲーム情報読み込み成功を記録
						ErrorLogAdd(("Scan:" + std::to_string(gamescan) + "\n").c_str());//ログにスキャンしたゲーム番号を記録
						ErrorLogAdd(("ゲーム種類:" + std::to_string(gametypecheck) + "\n").c_str());//ログにゲーム種類を記録
						ErrorLogAdd(("ゲームタイトル:" + gametitle + "\n").c_str());//ログにゲームタイトルを記録
						focusgame = 4;
						focuspage = page;
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 870 && mouseX <= 1045 && mouseY >= 613 && mouseY <= 788) { // ゲーム5の範囲内なら
					fps.FPS = 1;
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					if (gameyouso >= 5 + pagekasan) {
						gamescan = 0;
						GameInfoFile = FileRead_open("GamaInfo.txt", FALSE);//GameInfo.txtを開く
						FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
						while (FileRead_eof(GameInfoFile) == 0 && gamescan == 0) {
							FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
							StringgameInfoLine = trim(std::string(gameInfoLine));
							searchkey = std::to_string(4 + pagekasan) + ":{";
							gamecheck = StringgameInfoLine.find(searchkey);
							if (gamecheck != std::string::npos) {
								FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
								StringgameInfoLine = trim(std::string(gameInfoLine));
								gamecheck = StringgameInfoLine.find("type:");
								if (gamecheck != std::string::npos) {
									StringgameInfoLine = trim(StringgameInfoLine.substr(5));
									if (StringgameInfoLine == "note") {
										gametypecheck = 1;
									}
									else if (StringgameInfoLine == "game") {
										gametypecheck = 2;
									}
									else {
										ErrorLogAdd("typeが指定された文字列ではありません。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがnoteでもgameでもなかったら異常終了
									}
									FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
									StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
									gamecheck = StringgameInfoLine.find("title:");
									if (gamecheck != std::string::npos) {
										StringgameInfoLine = trim(StringgameInfoLine.substr(6)); // "title:"の後ろの文字列を取得
										gametitleinfo = 1;
										gametitle = StringgameInfoLine;
										FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
										StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
										gamecheck = StringgameInfoLine.find("ex:");
										if (gamecheck != std::string::npos) {
											StringgameInfoLine = trim(StringgameInfoLine.substr(3)); // "ex:"の後ろの文字列を取得
											gameex = StringgameInfoLine;
											gameexinfo = 1;
											FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
											StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
											gamecheck = StringgameInfoLine.find("eximage:");
											if (gamecheck != std::string::npos) {
												StringgameInfoLine = trim(StringgameInfoLine.substr(8)); // "eximage:"の後ろの文字列を取得
												gameeximage = LoadGraph(StringgameInfoLine.c_str());
												gameeximageinfo = 1;
												if (gametypecheck == 1) {
													gamescan = 1;
													gameexeinfo = 0;
													gameexe = "";
													gamedir = "";
													basedir = "";
													fulldir = "";
													fullexepath = "";
													break;
												}
												if (gametypecheck == 2) {
													FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
													StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
													gamecheck = StringgameInfoLine.find("dir:");// "dir:"の位置を探す
													if (gamecheck != std::string::npos) {
														StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "dir:"の後ろの文字列を取得
														gamedir = StringgameInfoLine;
														FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
														StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
														if (gamecheck != std::string::npos) {
															StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "exe:"の後ろの文字列を取得
															gameexe = StringgameInfoLine;
															gameexeinfo = 1;
															FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
															StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
															gamecheck = StringgameInfoLine.find("}");// "}"の位置を探す
															if (gamecheck != std::string::npos) {
																gamescan = 1;
															}
															else {
																ErrorLogAdd("}が不明です。\n");//ログにゲーム情報読み込み失敗を記録
																MessageBox(NULL, "ゲーム情報の終端が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
																ShowTaskbar();
																return -1; //"}"がなかったら異常終了
															}
														}
														else {
															ErrorLogAdd("exeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
															MessageBox(NULL, "ゲームの実行ファイルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
															ShowTaskbar();
															return -1; //exeがなかったら異常終了
														}
													}
													else {
														ErrorLogAdd("dirが不明です。\n");//ログにゲーム情報読み込み失敗を記録
														MessageBox(NULL, "ゲームのディレクトリが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
														ShowTaskbar();
														return -1; //dirがなかったら異常終了
													}
												}
												else {
													ErrorLogAdd("eximageが不明です。\n");//ログにゲーム情報読み込み失敗を記録
												}
											}
											else {
												ErrorLogAdd("exが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											}
										}
										else {
											ErrorLogAdd("titleが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											MessageBox(NULL, "ゲームタイトルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
											ShowTaskbar();
											return -1; //titleがなかったら異常終了
										}
									}
									else {
										ErrorLogAdd("typeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがなかったら異常終了
									}
								}
								else {
									continue;
								}
							}
						}
						FileRead_close(GameInfoFile); // ファイルを閉じる
						if (gamescan == 0) {
							ErrorLogAdd("ゲーム情報が見つかりません。\n");//ログにゲーム情報読み込み失敗を記録
							MessageBox(NULL, "ゲーム情報が見つかりません。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
							ShowTaskbar();
							return -1; //ゲーム情報が見つからなかったら異常終了
						}
						ErrorLogAdd("ゲーム情報を読み込みました。\n");//ログにゲーム情報読み込み成功を記録
						ErrorLogAdd(("Scan:" + std::to_string(gamescan) + "\n").c_str());//ログにスキャンしたゲーム番号を記録
						ErrorLogAdd(("ゲーム種類:" + std::to_string(gametypecheck) + "\n").c_str());//ログにゲーム種類を記録
						ErrorLogAdd(("ゲームタイトル:" + gametitle + "\n").c_str());//ログにゲームタイトルを記録
						focusgame = 5;
						focuspage = page;
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 1065 && mouseX <= 1240 && mouseY >= 613 && mouseY <= 788) { // ゲーム6の範囲内なら
					fps.FPS = 1;
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					if (gameyouso >= 6 + pagekasan) {
						gamescan = 0;
						GameInfoFile = FileRead_open("GamaInfo.txt", FALSE);//GameInfo.txtを開く
						FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
						while (FileRead_eof(GameInfoFile) == 0 && gamescan == 0) {
							FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
							StringgameInfoLine = trim(std::string(gameInfoLine));
							searchkey = std::to_string(5 + pagekasan) + ":{";
							gamecheck = StringgameInfoLine.find(searchkey);
							if (gamecheck != std::string::npos) {
								FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
								StringgameInfoLine = trim(std::string(gameInfoLine));
								gamecheck = StringgameInfoLine.find("type:");
								if (gamecheck != std::string::npos) {
									StringgameInfoLine = trim(StringgameInfoLine.substr(5));
									if (StringgameInfoLine == "note") {
										gametypecheck = 1;
									}
									else if (StringgameInfoLine == "game") {
										gametypecheck = 2;
									}
									else {
										ErrorLogAdd("typeが指定された文字列ではありません。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがnoteでもgameでもなかったら異常終了
									}
									FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
									StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
									gamecheck = StringgameInfoLine.find("title:");
									if (gamecheck != std::string::npos) {
										StringgameInfoLine = trim(StringgameInfoLine.substr(6)); // "title:"の後ろの文字列を取得
										gametitleinfo = 1;
										gametitle = StringgameInfoLine;
										FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
										StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
										gamecheck = StringgameInfoLine.find("ex:");
										if (gamecheck != std::string::npos) {
											StringgameInfoLine = trim(StringgameInfoLine.substr(3)); // "ex:"の後ろの文字列を取得
											gameex = StringgameInfoLine;
											gameexinfo = 1;
											FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
											StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
											gamecheck = StringgameInfoLine.find("eximage:");
											if (gamecheck != std::string::npos) {
												StringgameInfoLine = trim(StringgameInfoLine.substr(8)); // "eximage:"の後ろの文字列を取得
												gameeximage = LoadGraph(StringgameInfoLine.c_str());
												gameeximageinfo = 1;
												if (gametypecheck == 1) {
													gamescan = 1;
													gameexeinfo = 0;
													gameexe = "";
													gamedir = "";
													basedir = "";
													fulldir = "";
													fullexepath = "";
													break;
												}
												if (gametypecheck == 2) {
													FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
													StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
													gamecheck = StringgameInfoLine.find("dir:");// "dir:"の位置を探す
													if (gamecheck != std::string::npos) {
														StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "dir:"の後ろの文字列を取得
														gamedir = StringgameInfoLine;
														FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
														StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
														if (gamecheck != std::string::npos) {
															StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "exe:"の後ろの文字列を取得
															gameexe = StringgameInfoLine;
															gameexeinfo = 1;
															FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
															StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
															gamecheck = StringgameInfoLine.find("}");// "}"の位置を探す
															if (gamecheck != std::string::npos) {
																gamescan = 1;
															}
															else {
																ErrorLogAdd("}が不明です。\n");//ログにゲーム情報読み込み失敗を記録
																MessageBox(NULL, "ゲーム情報の終端が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
																ShowTaskbar();
																return -1; //"}"がなかったら異常終了
															}
														}
														else {
															ErrorLogAdd("exeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
															MessageBox(NULL, "ゲームの実行ファイルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
															ShowTaskbar();
															return -1; //exeがなかったら異常終了
														}
													}
													else {
														ErrorLogAdd("dirが不明です。\n");//ログにゲーム情報読み込み失敗を記録
														MessageBox(NULL, "ゲームのディレクトリが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
														ShowTaskbar();
														return -1; //dirがなかったら異常終了
													}
												}
												else {
													ErrorLogAdd("eximageが不明です。\n");//ログにゲーム情報読み込み失敗を記録
												}
											}
											else {
												ErrorLogAdd("exが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											}
										}
										else {
											ErrorLogAdd("titleが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											MessageBox(NULL, "ゲームタイトルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
											ShowTaskbar();
											return -1; //titleがなかったら異常終了
										}
									}
									else {
										ErrorLogAdd("typeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがなかったら異常終了
									}
								}
								else {
									continue;
								}
							}
						}
						FileRead_close(GameInfoFile); // ファイルを閉じる
						if (gamescan == 0) {
							ErrorLogAdd("ゲーム情報が見つかりません。\n");//ログにゲーム情報読み込み失敗を記録
							MessageBox(NULL, "ゲーム情報が見つかりません。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
							ShowTaskbar();
							return -1; //ゲーム情報が見つからなかったら異常終了
						}
						ErrorLogAdd("ゲーム情報を読み込みました。\n");//ログにゲーム情報読み込み成功を記録
						ErrorLogAdd(("Scan:" + std::to_string(gamescan) + "\n").c_str());//ログにスキャンしたゲーム番号を記録
						ErrorLogAdd(("ゲーム種類:" + std::to_string(gametypecheck) + "\n").c_str());//ログにゲーム種類を記録
						ErrorLogAdd(("ゲームタイトル:" + gametitle + "\n").c_str());//ログにゲームタイトルを記録
						focusgame = 6;
						focuspage = page;
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 1260 && mouseX <= 1435 && mouseY >= 613 && mouseY <= 788) { // ゲーム7の範囲内なら
					fps.FPS = 1;
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					if (gameyouso >= 7 + pagekasan) {
						gamescan = 0;
						GameInfoFile = FileRead_open("GamaInfo.txt", FALSE);//GameInfo.txtを開く
						FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
						while (FileRead_eof(GameInfoFile) == 0 && gamescan == 0) {
							FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
							StringgameInfoLine = trim(std::string(gameInfoLine));
							searchkey = std::to_string(6 + pagekasan) + ":{";
							gamecheck = StringgameInfoLine.find(searchkey);
							if (gamecheck != std::string::npos) {
								FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
								StringgameInfoLine = trim(std::string(gameInfoLine));
								gamecheck = StringgameInfoLine.find("type:");
								if (gamecheck != std::string::npos) {
									StringgameInfoLine = trim(StringgameInfoLine.substr(5));
									if (StringgameInfoLine == "note") {
										gametypecheck = 1;
									}
									else if (StringgameInfoLine == "game") {
										gametypecheck = 2;
									}
									else {
										ErrorLogAdd("typeが指定された文字列ではありません。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがnoteでもgameでもなかったら異常終了
									}
									FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
									StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
									gamecheck = StringgameInfoLine.find("title:");
									if (gamecheck != std::string::npos) {
										StringgameInfoLine = trim(StringgameInfoLine.substr(6)); // "title:"の後ろの文字列を取得
										gametitleinfo = 1;
										gametitle = StringgameInfoLine;
										FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
										StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
										gamecheck = StringgameInfoLine.find("ex:");
										if (gamecheck != std::string::npos) {
											StringgameInfoLine = trim(StringgameInfoLine.substr(3)); // "ex:"の後ろの文字列を取得
											gameex = StringgameInfoLine;
											gameexinfo = 1;
											FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
											StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
											gamecheck = StringgameInfoLine.find("eximage:");
											if (gamecheck != std::string::npos) {
												StringgameInfoLine = trim(StringgameInfoLine.substr(8)); // "eximage:"の後ろの文字列を取得
												gameeximage = LoadGraph(StringgameInfoLine.c_str());
												gameeximageinfo = 1;
												if (gametypecheck == 1) {
													gamescan = 1;
													gameexeinfo = 0;
													gameexe = "";
													gamedir = "";
													basedir = "";
													fulldir = "";
													fullexepath = "";
													break;
												}
												if (gametypecheck == 2) {
													FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
													StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
													gamecheck = StringgameInfoLine.find("dir:");// "dir:"の位置を探す
													if (gamecheck != std::string::npos) {
														StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "dir:"の後ろの文字列を取得
														gamedir = StringgameInfoLine;
														FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
														StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
														if (gamecheck != std::string::npos) {
															StringgameInfoLine = trim(StringgameInfoLine.substr(4)); // "exe:"の後ろの文字列を取得
															gameexe = StringgameInfoLine;
															gameexeinfo = 1;
															FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);//指定されたサイズ－１バイト分の文字列があった所までの文字列が格納されるため注意
															StringgameInfoLine = gameInfoLine; // char型のgameInfoLineをstring型に変換して追加
															gamecheck = StringgameInfoLine.find("}");// "}"の位置を探す
															if (gamecheck != std::string::npos) {
																gamescan = 1;
															}
															else {
																ErrorLogAdd("}が不明です。\n");//ログにゲーム情報読み込み失敗を記録
																MessageBox(NULL, "ゲーム情報の終端が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
																ShowTaskbar();
																return -1; //"}"がなかったら異常終了
															}
														}
														else {
															ErrorLogAdd("exeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
															MessageBox(NULL, "ゲームの実行ファイルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
															ShowTaskbar();
															return -1; //exeがなかったら異常終了
														}
													}
													else {
														ErrorLogAdd("dirが不明です。\n");//ログにゲーム情報読み込み失敗を記録
														MessageBox(NULL, "ゲームのディレクトリが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
														ShowTaskbar();
														return -1; //dirがなかったら異常終了
													}
												}
												else {
													ErrorLogAdd("eximageが不明です。\n");//ログにゲーム情報読み込み失敗を記録
												}
											}
											else {
												ErrorLogAdd("exが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											}
										}
										else {
											ErrorLogAdd("titleが不明です。\n");//ログにゲーム情報読み込み失敗を記録
											MessageBox(NULL, "ゲームタイトルが不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
											ShowTaskbar();
											return -1; //titleがなかったら異常終了
										}
									}
									else {
										ErrorLogAdd("typeが不明です。\n");//ログにゲーム情報読み込み失敗を記録
										MessageBox(NULL, "ゲームの種類が不明です。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
										ShowTaskbar();
										return -1; //typeがなかったら異常終了
									}
								}
								else {
									continue;
								}
							}
						}
						FileRead_close(GameInfoFile); // ファイルを閉じる
						if (gamescan == 0) {
							ErrorLogAdd("ゲーム情報が見つかりません。\n");//ログにゲーム情報読み込み失敗を記録
							MessageBox(NULL, "ゲーム情報が見つかりません。GameInfo.txtの記述を確認してください。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
							ShowTaskbar();
							return -1; //ゲーム情報が見つからなかったら異常終了
						}
						ErrorLogAdd("ゲーム情報を読み込みました。\n");//ログにゲーム情報読み込み成功を記録
						ErrorLogAdd(("Scan:" + std::to_string(gamescan) + "\n").c_str());//ログにスキャンしたゲーム番号を記録
						ErrorLogAdd(("ゲーム種類:" + std::to_string(gametypecheck) + "\n").c_str());//ログにゲーム種類を記録
						ErrorLogAdd(("ゲームタイトル:" + gametitle + "\n").c_str());//ログにゲームタイトルを記録
						focusgame = 7;
						focuspage = page;
					}
					fps.FPS = 120; //FPSを元に戻す
				}
				else if (mouseX >= 70 && mouseX <= 778 && mouseY >= 450 && mouseY <= 550 && gameexeinfo == 1) { // プレイボタンの範囲内なら
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
					for (feding = 0; feding <= 255; ++feding) {
						ClearDrawScreen(); //裏画面をクリア
						DrawGraph(1000, 700, logo, FALSE);
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, feding); //フェードインのためのブレンドモード設定
						DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); //画面全体を黒で塗りつぶす
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //ブレンドモードを元に戻す
						ScreenFlip(); //描画した内容を画面に反映
						ChangeVolumeSoundMem(255 - feding, BGM); //徐々にBGMの音量を下げる
						ProcessMessage();
					}
					fps.FPS = 30; //FPSを30にする
					STARTUPINFOA si = { sizeof(si) };
					PROCESS_INFORMATION pi;
					basedir = GetExeDir();
					fulldir = basedir + "\\" + gamedir;
					fullexepath = gamedir + "\\" + gameexe;
					ErrorLogAdd(("ゲームディレクトリ:" + fulldir + "\n").c_str());//ログにゲームディレクトリを記録
					ErrorLogAdd(("ゲーム実行ファイルパス:" + fullexepath + "\n").c_str());//ログにゲーム実行ファイルパスを記録
					ErrorLogAdd("ゲームを起動します。\n");//ログにゲーム起動を記録
					if (CreateProcessA(NULL, (LPSTR)fullexepath.c_str(), NULL, NULL, FALSE, 0, NULL, fulldir.c_str(), &si, &pi)) {//ゲーム起動
						ErrorLogAdd("ゲームを起動しました。\n");//ログにゲーム起動成功を記録
						DWORD exitcode = 0; // 初期化
						while (1) {
							GetExitCodeProcess(pi.hProcess, &exitcode);//プロセスの終了コードを取得
							if (exitcode != STILL_ACTIVE) {
								break;
							}
							else {
								fps.Update();	//FPS更新
								if (timerinfo == 1) {
									timer -= 1;
									if (timer <= 0) {
										TerminateProcess(pi.hProcess, 0);
										CloseHandle(pi.hProcess);
										CloseHandle(pi.hThread);
										ErrorLogAdd("タイムオーバーによりゲームを強制終了しました。\n");//ログにゲーム強制終了を記録
										MessageBox(NULL, "時間切れです!遊んでいただきありがとうございました!", "エラー", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
										goto zikangire;
									}
								}

							}
						}
						CloseHandle(pi.hProcess);
						CloseHandle(pi.hThread);
					}
					else {
						ErrorLogAdd("ゲームの起動に失敗しました。\n");//ログにゲーム起動失敗を記録
						MessageBox(NULL, "ゲームの起動に失敗しました。", "エラー", MB_OK | MB_ICONERROR | MB_TOPMOST);
						ShowTaskbar();
						return -1; //ゲーム起動に失敗したら異常終了
					}
					fps.FPS = 120; //FPSを元に戻す
					SetForegroundWindow(hwnd);
					SetActiveWindow(hwnd);
					SetFocus(hwnd);
					if (CheckSoundMem(BGM) == 0) {
						PlaySoundMem(BGM, DX_PLAYTYPE_BACK);
					}
					for (feding >= 0; feding >= 0; --feding) {
						ClearDrawScreen(); //裏画面をクリア
						DrawGraph(1000, 700, logo, FALSE);
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, feding); //フェードインのためのブレンドモード設定
						DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); //画面全体を黒で塗りつぶす
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //ブレンドモードを元に戻す
						ScreenFlip(); //描画した内容を画面に反映
						ChangeVolumeSoundMem(255 - feding, BGM); //徐々にBGMの音量を上げる
						ProcessMessage();
					}
				}
			}
		// 毎フレーム最後に前回の状態を更新
		prevMouseInput = mouseInput;
		pagekasan = (page - 1) * 7;
		ScreenFlip(); //描画した内容を画面に反映
	}
zikangire:
	fps.FPS = 120;
		for (feding >= 0; feding >= 0; --feding) {
			ClearDrawScreen(); //裏画面をクリア
			DrawGraph(1000, 700, logo, FALSE);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, feding); //フェードインのためのブレンドモード設定
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); //画面全体を黒で塗りつぶす
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //ブレンドモードを元に戻す
			ScreenFlip(); //描画した内容を画面に反映
			ChangeVolumeSoundMem(255 - feding, BGM); //徐々にBGMの音量を上げる
			ProcessMessage();
		}
		fps.FPS = 30;
		while (1) {
			ClearDrawScreen(); //裏画面をクリア
			DrawGraph(1000, 700, logo, FALSE);
			ScreenFlip(); //描画した内容を画面に反映
			fps.Update();	//FPS更新
			ProcessMessage();
			DrawFormatString(0, 0, GetColor(0, 0, 0), "timeup");
			if (CheckHitKey(KEY_INPUT_LCONTROL) && CheckHitKey(KEY_INPUT_LALT) && CheckHitKey(KEY_INPUT_X)) {
				ErrorLogAdd("通常復帰操作が要求されました。\n");//ログにタイマー初期化操作要求を記録
				int result = MessageBox(NULL, "この操作は管理者専用です。いいえを押して戻ってください。", "確認", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
				if (result == IDYES) {
					int GameInfoFile = FileRead_open("timer.txt", FALSE);
					char gameInfoLine[256];
					FileRead_seek(GameInfoFile, 0, SEEK_SET);
					FileRead_gets(gameInfoLine, sizeof(gameInfoLine), GameInfoFile);
					timer = atoi(gameInfoLine);
					FileRead_close(GameInfoFile);
					ErrorLogAdd("通常状態に復帰しました。\n");
					MessageBox(NULL, "タイマーをリセットしました。\n通常状態に戻ります。", "情報", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
					fps.FPS = 120;
					goto fukki;
				}
				else {
					ErrorLogAdd("処理が中断されました。。\n");
					MessageBox(NULL, "処理を中断しました。", "警告", MB_OK | MB_ICONWARNING | MB_TOPMOST);
				}
				Sleep(200); // 連続判定防止
			}
		}
	DxLib_End(); // DXライブラリの終了処理
	return 0; // プログラムの終了
}