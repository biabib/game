#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <string>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	int frame = character.GetFrameIndexOfBitmap();
	background.SetTopLeft(background.GetLeft()+speedX, background.GetTop()+speedY);
	hitbox[0].SetTopLeft(hitbox[0].GetLeft() + speedX, hitbox[0].GetTop() + speedY);
	//for (int i = 0; i < hitboxnum; i++) {
		//hitbox[i].SetTopLeft(hitbox[i].GetLeft() + speedX, hitbox[i].GetTop() + speedY);
	//}
	if (speedX != 0 || speedY != 0) {
		if (characterFrameCounter == 4) {
			if (speedY < 0) {
				character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()) % 2 + 1);
			}
			else if (speedY > 0) {
				character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()+1) % 2 + 6);
			}
			else if (speedX < 0) {
				character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()+1) % 2 + 8);
			}
			else if (speedX > 0) {
				character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()) % 2 + 3);
			}
			characterFrameCounter = 0;
		}
		else {
			characterFrameCounter+=1;
		}
	}
	else
	{
		if (frame == 1 || frame ==2) {
				character.SetFrameIndexOfBitmap(0);
		}
		else if (frame == 3 || frame == 4) {
			character.SetFrameIndexOfBitmap(3);
		}
		else if (frame == 6 || frame == 7) {
			character.SetFrameIndexOfBitmap(5);
		}
		else if (frame == 8 || frame == 9) {
			character.SetFrameIndexOfBitmap(8);
		}
	}
	
	
}


void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	background.LoadBitmapByString({
		"resources/home_2f.bmp",
		"resources/home_1f.bmp",
		"resources/map1_1.5.bmp",
		});//,RGB(100,100,100));
	background.SetTopLeft(250, 190);
	/*background.SetTopLeft(-215, -1885);*/
	
	character.LoadBitmapByString({ "Resources/character/red_front.bmp","resources/character/red_front1.bmp","resources/character/red_front2.bmp","resources/character/red_left.bmp","resources/character/red_left1.bmp","resources/character/red_back.bmp","resources/character/red_back1.bmp","resources/character/red_back2.bmp","resources/character/red_right.bmp","resources/character/red_right1.bmp" },RGB(255,255,255));
	character.SetTopLeft(300, 272);
	character.SetAnimation(1000, true);

	textbox.LoadBitmapByString({"Resources/text.bmp"});
	textbox.SetTopLeft(0,378);
	
	ifstream ifs("map/home2f.txt");
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			ifs >> home2fmap[i][j];
		}
	}
	ifs.close();							//讀地圖txt進來
	int hit = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (home2fmap[i][j] % 2 == 1) {
				hitboxnum++;
				hitbox[hit].LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
				hitbox[hit].SetTopLeft(250 + i * 24 , 190 + j * 24);
				hit++;
			}
		}
	}
	hitbox[0].LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
	hitbox[0].SetTopLeft(250 , 190 );
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (nChar == VK_RETURN) {
		if (phase == 1) {
			if ((((90 <= background.GetLeft()) && (background.GetLeft() <= 110))) && ((190 <= background.GetTop()) &&(background.GetTop() <= 210))) {
				phase = 2;
				background.SetFrameIndexOfBitmap(0);
				background.SetTopLeft(205,210);
				character.SetFrameIndexOfBitmap(3);
				Sleep(500);
			}
			else if ((((180 <= background.GetLeft()) && (background.GetLeft() <= 200))) && ((10 <= background.GetTop()) && (background.GetTop() <= 30))) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-215, -1885);
				character.SetFrameIndexOfBitmap(0);
				Sleep(500);
			}
		}
		else if (phase == 2) {
			if (((190 <= background.GetLeft()) && (background.GetLeft() <= 210)) && ((200 <= background.GetTop()) && (background.GetTop() <= 220))) {
				phase = 1;
				background.SetFrameIndexOfBitmap(1);
				background.SetTopLeft(110,200);
				character.SetFrameIndexOfBitmap(3);
				Sleep(500);
			}
		}
		else if (phase == 3) {
			if (((-225 <= background.GetLeft()) && (background.GetLeft() <= -205)) && ((-1895 <= background.GetTop()) && (background.GetTop() <= -1875))) {
				phase = 1;
				background.SetFrameIndexOfBitmap(1);
				background.SetTopLeft(190, 20);
				character.SetFrameIndexOfBitmap(5);
				Sleep(500);
			}
		}

	}

	if(nChar == VK_LEFT)
	{	
		speedX = 5;
		speedY = 0;
	}
	else if (nChar == VK_UP)
	{
		speedY = 5;
		speedX = 0;
	}
	else if (nChar == VK_DOWN)
	{
		speedY = -5;
		speedX = 0;
	}
	else if (nChar == VK_RIGHT)
	{
		speedX = -5;
		speedY = 0;
	}

	
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT)
	{
		speedX = 0;

	}
	else if (nChar == VK_UP)
	{
		speedY = 0;
	}
	else if (nChar == VK_DOWN)
	{
		speedY = 0;
	}
	else if (nChar == VK_RIGHT)
	{
		speedX = 0;
	}

}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	show_image_by_phase();
	show_text_by_phase();
	character.ShowBitmap();
}

void CGameStateRun::show_image_by_phase() {
	/*if (phase = 1) {
		
		background.SetFrameIndexOfBitmap(0);
		background.ShowBitmap();
	}
	else if (phase = 2) {
		
		
	}*/

	background.ShowBitmap();
	character.ShowBitmap();
	//textbox.ShowBitmap();
	//for (int i = 0; i < hitboxnum; i++) {
	//	hitbox[i].ShowBitmap();
	//}
	hitbox[0].ShowBitmap();
}

void CGameStateRun::show_text_by_phase() {
	CDC* pDC = CDDraw::GetBackCDC();
	string x = to_string(background.GetLeft());
	string y = to_string(background.GetTop());

	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(255, 0, 0), 800);
	CTextDraw::Print(pDC, 0, 0, x);
	CTextDraw::Print(pDC, 90, 0, y);

	CDDraw::ReleaseBackCDC();
}

