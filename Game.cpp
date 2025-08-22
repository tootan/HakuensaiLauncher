// Hakuensai Launcher 2025 一部AI要素、おふざけあり
#include <math.h>//数学関数
#include <windows.h>//Windows API
#include <DxLib.h>// DxLibヘッダファイル
#include <time.h>// 時刻関数
#include<string>//文字列関数
#include <vector>//ベクター関数
#pragma warning(disable : 4996)// 4996警告を無視する
#define PI 3.141592653589793238462643 //円周率設定
// グローバル変数で元のプロシージャを保存
WNDPROC g_OldWndProc = nullptr;
// Alt+F4や×ボタンを無効化するプロシージャ
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_CLOSE) {
		return 0; // WM_CLOSEを無視
	}
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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) /*おまじない♪*/ {
	ChangeWindowMode(TRUE);//ウインドウ設定
	SetGraphMode(1920, 1080, 32);//画面サイズ指定(FHD、簡単には閉じれないぞ・・・ﾆﾋﾋ)
	SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定
	SetMainWindowText("Hakuensai Launcher 2025");//ウインドウ名設定
	SetAlwaysRunFlag(true);//バックグラウンド実行
	SetBackgroundColor(255, 255, 255);//背景設定
	SetWindowStyleMode(4);//ウインドウスタイルをボーダーレスウインドウに設定
	SetDoubleStartValidFlag(FALSE); //二重起動禁止
	if (DxLib_Init() == -1)//起動失敗したらエラー吐かせる
		return -1;
	HWND hwnd = (HWND)GetMainWindowHandle();
	g_OldWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MyWndProc);
	SetDrawScreen(DX_SCREEN_BACK);//裏描画に設定
	int font = CreateFontToHandle("BIZ UDPゴジック", 30, 3, DX_FONTTYPE_ANTIALIASING);//通常フォント生成
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(0/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int BGM = LoadSoundMem("Audio/BGM.ogg");//BGM読み込み
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(1/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int Communicationmark = LoadGraph("Image/Communicationmark.png");//コミュニケーションマーク読み込み
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(2/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int BGMCount = 0;//BGM時間測定用変数
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(3/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	double Communicationmarkangle = 4;//コミュニケーションマーク角度変数
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(4/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int titlefont = CreateFontToHandle("UD デジタル 教科書体 N", 100, 8, DX_FONTTYPE_ANTIALIASING);//タイトルフォント生成
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(5/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int timefont = CreateFontToHandle("BIZ UDPゴジック", 60, 3, DX_FONTTYPE_ANTIALIASING);//時刻フォント生成
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(6/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int mouseX, mouseY;//マウス座標格納変数
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(7/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int onpuon = LoadGraph("image/onpuon2.png");//♪ボタンオン読み込み
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(8/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int onpuoff = LoadGraph("image/onpuoff2.png");//♪ボタンオフ読み込みClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(9/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int BGMinfo = 1; //BGM情報格納変数（1:ON, 0:OFF）ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(10/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int prevMouseInput = 0; // 前回のマウス入力状態を格納する変数
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(11/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int mouseInput;// マウス入力状態を格納する変数
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(12/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int botan = LoadSoundMem("Audio/botan.ogg");//ボタン音読み込み
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(13/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	struct tm local;//時刻格納用構造体ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(14/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	char timebuf[128];//時刻格納用バッファ
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(15/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	ChangeVolumeSoundMem(255, BGM); //BGMの音量を最大に設定
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(16/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	Fps fps;
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(17/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int hidaribotan = LoadGraph("Image/hidaribotan.png");//左ボタン読み込み
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "NowLoading...(18/19)", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int migibotan = LoadGraph("Image/migibotan.png");//右ボタン読み込み
	ClearDrawScreen(); //裏画面をクリア
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "LoadFinish!", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int GameInfoFile = FileRead_open("launcherInfo.txt", FALSE);//GameInfo.txtを開く
	FileRead_seek(GameInfoFile, 0, SEEK_SET); // ファイルの先頭に移動
	char gameInfoLine[256]; //GameInfo.txtの内容を格納する文字列
	int gameyousocheck;
	int gameyouso = 0; //Gameの個数をカウントする変数
	int gamecheck = 0;//StringgameInfoLineの配列をカウントする変数
	std::vector<int> gametitleimage; //Gameのタイトル画像のインデックスを格納するベクター
	char chargameyouso[256];//Gameの個数を格納する文字列
	std::string StringgameInfoLine; //GameInfo.txtの内容を格納する文字列
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "GameScanning...", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
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
				return -1; //titleimageがなかったら異常終了
			}
			else {
				StringgameInfoLine = StringgameInfoLine.substr(11); // "titleimage:"の後ろの文字列を取得
				gametitleimage.push_back(LoadGraph(StringgameInfoLine.c_str())); // 文字列をグラフィックハンドルに変換してベクターに追加
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
	sprintf(chargameyouso, "%d", gameyouso); //Gameの個数を文字列に変換
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);//画面全体を黒で塗りつぶす
	DrawStringToHandle(0, 0, "Complete!", GetColor(255, 255, 255), font);//タイトル描画
	ScreenFlip();//裏画面を表画面に
	int feding = 225; //フェードイン用変数
	int page = 1;//ページ番号変数
	int pagekasan = 0;
	int pagetotal = gameyouso / 7 + 1;
	while (ProcessMessage() == 0 /*恒例のプロセスメッセージ*/ && ClearDrawScreen() == 0/*スクリーンクリーン（初期化っていうか全削除っていうか・・・もうなんでもいいや）*/) {
		fps.Update();	//FPS更新
		time_t t = time(NULL);//現在時刻を取得
		localtime_s(&local, &t);//現在時刻をローカル時間に変換
		strftime(timebuf, sizeof(timebuf), "%H:%M", &local);//時刻をフォーマットして文字列に変換
		GetMousePoint(&mouseX, &mouseY); //マウス座標取得
		SetDrawMode(DX_DRAWMODE_BILINEAR);//描画モードをバイリニアフィルタに設定(アンチエイリアス効果)
		DrawRotaGraph(60, 50, 1.0, PI * Communicationmarkangle, Communicationmark, TRUE); //コミュニケーションマーク描画
		SetDrawMode(DX_DRAWMODE_NEAREST);//描画モードをニアレストネイバーに設定(アンチエイリアス効果なし)
		DrawStringToHandle(450, 10, "柏苑祭", GetColor(0, 0, 0), titlefont);//タイトル描画
		DrawStringToHandle(785, 10, "2025", GetColor(255, 0, 0), titlefont);//タイトル描画2
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
		DrawStringToHandle(70, 150, "ほああああ", GetColor(0, 0, 0), font);
		DrawBox(790, 173, 1430, 533, GetColor(0, 0, 0), TRUE);//黒いボックス描画
		DrawBox(90, 613, 265, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画0
		DrawBox(285, 613, 460, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画1
		DrawBox(480, 613, 655, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画2
		DrawBox(675, 613, 850, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画3
		DrawBox(870, 613, 1045, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画4
		DrawBox(1065, 613, 1240, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画5
		DrawBox(1260, 613, 1435, 788, GetColor(0, 0, 0), TRUE);//黒いボックス描画6
		DrawGraph(90, 613, gametitleimage[0 + pagekasan], false);//これか？
		if (gameyouso == 2 + pagekasan) {
			DrawGraph(285, 613, gametitleimage[1 + pagekasan], false);//これか？
			if(gameyouso == 3 + pagekasan) {
				DrawGraph(480, 613, gametitleimage[2 + pagekasan], false);//これか？
				if (gameyouso == 4 + pagekasan) {
					DrawGraph(675, 613, gametitleimage[3 + pagekasan], false);//これか？
					if (gameyouso == 5 + pagekasan) {
						DrawGraph(870, 613, gametitleimage[4 + pagekasan], false);//これか？
						if (gameyouso == 6 + pagekasan) {
							DrawGraph(1065, 613, gametitleimage[5 + pagekasan], false);//これか？
							if (gameyouso == 7 + pagekasan) {
								DrawGraph(1260, 613, gametitleimage[6 + pagekasan], false);//これか？
							}
						}
					}
				}
			}
		}
		fps.Draw();		//FPS描画
		DrawFormatString(55, 0, GetColor(0, 0, 0), "BGMCount:%d", BGMCount);//BGMCount変数表示
		DrawFormatString(180, 0, GetColor(0, 0, 0), "MouseX:%d", mouseX);//マウスX座標表示
		DrawFormatString(280, 0, GetColor(0, 0, 0), "MouseY:%d", mouseY);//マウスY座標表示
		if (feding != -1) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, feding); //フェードインのためのブレンドモード設定
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); //画面全体を黒で塗りつぶす
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //ブレンドモードを元に戻す
			feding -= 1; //フェードインのために徐々に透明度を下げる
		}
		ScreenFlip();//裏画面を表画面に
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
				if (page > 1) { //ページが1より大きいなら
					page -= 1; //ページを1つ戻す
					pagekasan -= 7; //ページカウントを戻す
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
				}
			}
			else if (mouseX >= 1420 && mouseX <= 1520 && mouseY >= 650 && mouseY <= 750)/*右ボタンの範囲内なら*/{
				if (page != pagetotal) {
					page += 1; //ページを1つ進める
					pagekasan += 7; //ページカウントを進める
					PlaySoundMem(botan, DX_PLAYTYPE_BACK); // ボタン音再生
				}
			} 
		}
		prevMouseInput = mouseInput; // ループの最後で状態を保存
	}

	DxLib_End();//終わりー
	return 0; //正常終了
}