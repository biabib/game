#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <string>
#include <ddraw.h>
#include<fstream>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
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

void CGameStateRun::OnMove()							// ���ʹC������
{
	if (bag == false) {
		int frame = character.GetFrameIndexOfBitmap();
		background.SetTopLeft(background.GetLeft() + speedX, background.GetTop() + speedY);
		if (speedX != 0 || speedY != 0) {
			if (characterFrameCounter == 4) {
				if (speedY < 0) {
					character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()) % 2 + 1);
				}
				else if (speedY > 0) {
					character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap() + 1) % 2 + 6);
				}
				else if (speedX < 0) {
					character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap() + 1) % 2 + 8);
				}
				else if (speedX > 0) {
					character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()) % 2 + 3);
				}
				characterFrameCounter = 0;
			}
			else {
				characterFrameCounter += 1;
			}
		}
		else
		{
			if (frame == 1 || frame == 2) {
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

		//�o�̤��U���O�a�Ϥ���  

		/*if (phase = 0){
			phase = 2;
		}
		else*/ if (phase == 1) {
			if ((((75 <= background.GetLeft()) && (background.GetLeft() <= 100))) && ((190 <= background.GetTop()) && (background.GetTop() <= 220))) {
				phase = 2;
				background.SetFrameIndexOfBitmap(0);
				background.SetTopLeft(205, 210);
				character.SetFrameIndexOfBitmap(3);
				speedX = 0;
				speedY = 0;
				Sleep(500);
			}
			else if (((((180 <= background.GetLeft()) && (background.GetLeft() <= 200))) && ((10 <= background.GetTop()) && (background.GetTop() <= 30))) && character.GetFrameIndexOfBitmap() == 0) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-245, -105);
				character.SetFrameIndexOfBitmap(0);
				speedX = 0;
				speedY = 0;
				Sleep(500);
			}
		}
		else if (phase == 2) {
			if (((185 <= background.GetLeft()) && (background.GetLeft() <= 195)) && ((205 <= background.GetTop()) && (background.GetTop() <= 225))) {
				phase = 1;
				background.SetFrameIndexOfBitmap(1);
				background.SetTopLeft(110, 200);
				character.SetFrameIndexOfBitmap(3);
				speedX = 0;
				speedY = 0;
				Sleep(500);
			}
		}
		else if (phase == 3) {
			if (((-255 <= background.GetLeft()) && (background.GetLeft() <= -235))/* && ((-1895 <= background.GetTop())*/ && (background.GetTop() == -95)) {
				phase = 1;
				background.SetFrameIndexOfBitmap(1);
				background.SetTopLeft(190, 30);
				character.SetFrameIndexOfBitmap(5);
				speedX = 0;
				speedY = 0;
				Sleep(500);
			}
		}
		//else if (phase == 4) {
		//	if (((-225 <= background.GetLeft()) && (background.GetLeft() <= -205))/* && ((-1895 <= background.GetTop())*/ && (background.GetTop() == -1885)) {
		//		phase = 1;
		//		background.SetFrameIndexOfBitmap(3);
		//		background.SetTopLeft(190, 30);
		//		character.SetFrameIndexOfBitmap(5);
		//		speedX = 0;
		//		speedY = 0;
		//		//Sleep(500);
		//	}
		//}
		//else if (phase == 5) {
		//	if (((-225 <= background.GetLeft()) && (background.GetLeft() <= -205))/* && ((-1895 <= background.GetTop())*/ && (background.GetTop() == -1885)) {
		//		phase = 1;
		//		background.SetFrameIndexOfBitmap(4);
		//		background.SetTopLeft(190, 30);
		//		character.SetFrameIndexOfBitmap(5);
		//		speedX = 0;
		//		speedY = 0;
		//		//Sleep(500);
		//	}
		//}
	}
	
}


void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	background.LoadBitmapByString({
		"resources/home_2f.bmp",
		"resources/home_1f.bmp",
		"resources/town.bmp",
		"resources/shop.bmp",
		"resources/pokemon.bmp",
		});
	background.SetTopLeft(250, 190);
	
	character.LoadBitmapByString({ "Resources/character/red_front.bmp","resources/character/red_front1.bmp","resources/character/red_front2.bmp","resources/character/red_left.bmp","resources/character/red_left1.bmp","resources/character/red_back.bmp","resources/character/red_back1.bmp","resources/character/red_back2.bmp","resources/character/red_right.bmp","resources/character/red_right1.bmp" },RGB(255,255,255));
	character.SetTopLeft(300, 272);
	character.SetAnimation(1000, true);

	textbox.LoadBitmapByString({"Resources/text.bmp","Resources/menu.bmp"});
	textbox.SetFrameIndexOfBitmap(1);
	textbox.SetTopLeft(400, 40);

	arrow.LoadBitmapByString({ "Resources/arrow.bmp" });
	

	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (nChar == VK_RETURN) {
		
		if (bag == true) {
			int arrow_y = arrow.GetTop();
			if (arrow_y == 255)		//���}�\��
				exit(0);
		}

	}
	if (nChar == VK_ESCAPE) {	//�Ȱ�����ܽb�Y
		bag = !bag;
		arrow.SetTopLeft(430, 75);
		
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
		if (bag == true) {
			if (arrow.GetTop() != 75)		//���b�Y  -->���\�ध��|�[�b�W����enter��
				arrow.SetTopLeft(430, arrow.GetTop() - 60);
		}
	}
	else if (nChar == VK_DOWN)
	{
		speedY = -5;
		speedX = 0;
		if (bag == true) {
			if (arrow.GetTop() != 255)		//���b�Y
				arrow.SetTopLeft(430, arrow.GetTop() + 60);
		}
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

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnShow()
{
	show_image_by_phase();
	show_text_by_phase();
	character.ShowBitmap();
}

void CGameStateRun::show_image_by_phase() {
	background.ShowBitmap();
	character.ShowBitmap();
	if (bag == true) {
		textbox.ShowBitmap();
		arrow.ShowBitmap();
	}
}

void CGameStateRun::show_text_by_phase() {
	CDC* pDC = CDDraw::GetBackCDC();
	string x = to_string(background.GetLeft());
	string y = to_string(background.GetTop());

	CTextDraw::ChangeFontLog(pDC, 21, "�L�n������", RGB(255, 0, 0), 800);
	CTextDraw::Print(pDC, 0, 0, x);
	CTextDraw::Print(pDC, 90, 0, y);


	if (bag == true) {				//����r
		CTextDraw::ChangeFontLog(pDC, 40, "�L�n������", RGB(0, 0, 0), 800);
		CTextDraw::Print(pDC, 450, 60, "�_�i��");
		CTextDraw::Print(pDC, 450, 120, "�I�]");
		CTextDraw::Print(pDC, 450, 180, "�s��");
		CTextDraw::Print(pDC, 450, 240, "���}");
	}

	CDDraw::ReleaseBackCDC();
}

