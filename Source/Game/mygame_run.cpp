#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
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
	int frame = character.GetFrameIndexOfBitmap();
	background.SetTopLeft(background.GetLeft()+speedX, background.GetTop()+speedY);
	if (speedX != 0 || speedY != 0) {
		if (characterFrameCounter == 7) {
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


void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	background.LoadBitmapByString({
		"resources/map.bmp",
		});
	background.SetTopLeft(0, 0);

	character.LoadBitmapByString({ "Resources/red_front.bmp","resources/red_front1.bmp","resources/red_front2.bmp","resources/red_left.bmp","resources/red_left1.bmp","resources/red_back.bmp","resources/red_back1.bmp","resources/red_back2.bmp","resources/red_right.bmp","resources/red_right1.bmp" },RGB(255,255,255));
	character.SetTopLeft(300, 272);
	character.SetAnimation(1000, true);

	textbox.LoadBitmapByString({"Resources/text.bmp"});
	textbox.SetTopLeft(0,378);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (nChar == VK_RETURN) {
	
	}

	if(nChar == VK_LEFT)
	{	
		speedX = 3;
		speedY = 0;
	}
	else if (nChar == VK_UP)
	{
		speedY = 3;
		speedX = 0;
	}
	else if (nChar == VK_DOWN)
	{
		speedY = -3;
		speedX = 0;
	}
	else if (nChar == VK_RIGHT)
	{
		speedX = -3;
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
		textbox.ShowBitmap();
}

void CGameStateRun::show_text_by_phase() {
	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 21, "�L�n������", RGB(0, 0, 0), 800);


	CDDraw::ReleaseBackCDC();
}
