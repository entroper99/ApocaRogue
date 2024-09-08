#include <SDL.h>

import HUD;
import std;
import util;
import globalVar;
import constVar;
import textureVar;
import Player;
import Sprite;
import World;
import drawSprite;
import drawText;
import globalTime;
import checkCursor;
import Vehicle;
import mouseGrid;

namespace segmentIndex
{
	int zero = 0;
	int one = 1;
	int two = 2;
	int three = 3;
	int four = 4;
	int five = 5;
	int six = 6;
	int seven = 7;
	int eight = 8;
	int nine = 9;
	int dash = 10;
	int colon = 11;
	int pm = 12;
	int am = 13;
	float popUpDist = 360;
};

void HUD::drawGUI()
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	Sprite* targetBtnSpr = nullptr;


	drawStatusEffects();

	drawStadium(letterbox.x, letterbox.y, letterbox.w, letterbox.h + 10, { 0,0,0 }, 150, 5);
	if (ctrlVeh != nullptr) drawSpriteCenter(spr::vehicleHUD, 0, cameraW / 2, cameraH + 73);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (isAdvancedMode == false)
	{

		SDL_SetTextureBlendMode(texture::minimap, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(texture::minimap, 160);
		setZoom(4.0);
		drawTextureCenter(texture::minimap, 94, 94);
		setZoom(1.0);


		drawSprite(spr::minimapEdge, 1, 14, 14);
		Sprite* targetBtnSpr;
		if (inputType == input::gamepad)
		{
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) { targetBtnSpr = spr::buttonsPressed; }
			else { targetBtnSpr = spr::buttons; }
			drawSpriteCenter(targetBtnSpr, keyIcon::duelSense_L1, 14 + 15, 14 + 15);
		}
		else if (inputType == input::mouse)
		{
			targetBtnSpr = spr::buttons;
			drawSpriteCenter(targetBtnSpr, keyIcon::keyboard_M, 14 + 15, 14 + 15);
		}


		int vShift = 384 * (ctrlVeh != nullptr);

		if (1)//플레이어 이름 그리기
		{
			setFontSize(12);
			SDL_SetRenderDrawColor(renderer, lowCol::yellow.r, lowCol::yellow.g, lowCol::yellow.b, 0xff);
			drawText(L"Jackson, Practitioner of Elivilon ******", letterbox.x + 18 + vShift, letterbox.y + 1);
		}

		//HP바 그리기
		{
			int pivotX = letterbox.x + 11 + 1.02*vShift;
			int pivotY = letterbox.y + 21;
			drawSprite(spr::icon16, 72, pivotX, pivotY);

			setFontSize(9);
			drawText(col2Str(lowCol::green) + L"HEALTH", pivotX + 16, pivotY + 1);

			drawSprite(spr::hpBlankGauge, pivotX + 55, pivotY + 1);

			//페이크 HP
			float ratioFakeHP = myMax((float)0.0, (Player::ins()->entityInfo.fakeHP) / (float)(Player::ins()->entityInfo.maxHP));
			SDL_Rect fakeRect = { pivotX + 55 + 3, pivotY + 1 + 3, 176 * ratioFakeHP, 5 };
			drawFillRect(fakeRect, lowCol::white, Player::ins()->entityInfo.fakeHPAlpha);
			float ratioHP = myMax((float)0.0, (float)(Player::ins()->entityInfo.HP) / (float)(Player::ins()->entityInfo.maxHP));
			SDL_Rect realRect = { pivotX + 55 + 3, pivotY + 1 + 3, 176 * ratioHP, 5 };
			if (ratioHP > 0 && realRect.w == 0) { realRect.w = 1; }
			drawFillRect(realRect, lowCol::green);

			std::wstring hpStr = std::to_wstring(Player::ins()->entityInfo.HP) + L" / " + std::to_wstring(Player::ins()->entityInfo.maxHP);
			setFontSize(8);
			drawTextCenter(col2Str(lowCol::black) + hpStr, pivotX + 142 + 1, pivotY + 7);
			drawTextCenter(col2Str(lowCol::black) + hpStr, pivotX + 142 - 1, pivotY + 7);
			drawTextCenter(col2Str(lowCol::black) + hpStr, pivotX + 142, pivotY + 7 + 1);
			drawTextCenter(col2Str(lowCol::black) + hpStr, pivotX + 142, pivotY + 7 - 1);
			drawTextCenter(col2Str(lowCol::white) + hpStr, pivotX + 142, pivotY + 7);
		}

		//MP바 그리기
		{
			int pivotX = letterbox.x + 11 + 1.02 * vShift;
			int pivotY = letterbox.y + 21 +15;
			drawSprite(spr::icon16, 73, pivotX, pivotY);

			setFontSize(9);
			drawText(col2Str(lowCol::blue) + L"MENTAL", pivotX + 16, pivotY + 1);

			drawSprite(spr::hpBlankGauge, pivotX + 55, pivotY + 1);
			
			float ratioFakeMP = myMax((float)0.0, (Player::ins()->entityInfo.fakeMP) / (float)(Player::ins()->entityInfo.maxMP));
			SDL_Rect fakeRect = { pivotX + 55 + 3, pivotY + 1 + 3, 176 * ratioFakeMP, 5 };
			drawFillRect(fakeRect, lowCol::white, Player::ins()->entityInfo.fakeMPAlpha);
			float ratioMP = myMax((float)0.0, (float)(Player::ins()->entityInfo.MP) / (float)(Player::ins()->entityInfo.maxMP));
			SDL_Rect realRect = { pivotX + 55 + 3, pivotY + 1 + 3, 176 * ratioMP, 5 };
			if (ratioMP > 0 && realRect.w == 0) { realRect.w = 1; }
			drawFillRect(pivotX + 55 + 3, pivotY + 1 + 3, 176* ratioMP, 5, lowCol::blue);

			std::wstring mpStr = std::to_wstring(Player::ins()->entityInfo.MP) + L" / " + std::to_wstring(Player::ins()->entityInfo.maxMP);
			setFontSize(8);
			drawTextCenter(col2Str(lowCol::black) + mpStr, pivotX + 142 + 1, pivotY + 7);
			drawTextCenter(col2Str(lowCol::black) + mpStr, pivotX + 142 - 1, pivotY + 7);
			drawTextCenter(col2Str(lowCol::black) + mpStr, pivotX + 142, pivotY + 7 + 1);
			drawTextCenter(col2Str(lowCol::black) + mpStr, pivotX + 142, pivotY + 7 - 1);
			drawTextCenter(col2Str(lowCol::white) + mpStr, pivotX + 142, pivotY + 7);
		}


		if (ctrlVeh == nullptr)
		{
			//스테미나
			{
				int pivotX = letterbox.x + 18 + 238;
				int pivotY = letterbox.y + 4;
				setFontSize(10);

				int sprIndex = (int)(16.0 * ((double)Player::ins()->entityInfo.STA / (double)Player::ins()->entityInfo.maxSTA));
				drawSprite(spr::staminaGauge, sprIndex, pivotX, pivotY);
				drawTextCenter(col2Str(lowCol::yellow) + L"STA", pivotX + 24, pivotY + 16);

				std::wstring STAStr = std::to_wstring(Player::ins()->entityInfo.STA) + L"/" + std::to_wstring(Player::ins()->entityInfo.maxSTA);
				setFontSize(8);
				drawTextCenter(col2Str(lowCol::black) + STAStr, pivotX + 24 + 1, pivotY + 29);
				drawTextCenter(col2Str(lowCol::black) + STAStr, pivotX + 24 - 1, pivotY + 29);
				drawTextCenter(col2Str(lowCol::black) + STAStr, pivotX + 24, pivotY + 29 + 1);
				drawTextCenter(col2Str(lowCol::black) + STAStr, pivotX + 24, pivotY + 29 - 1);
				drawTextCenter(col2Str(lowCol::white) + STAStr, pivotX + 24, pivotY + 29);
			}

			setFontSize(12);
			drawText(col2Str(col::lightGray) + L"SPEED", letterbox.x + 18 + 296, letterbox.y + 3 + 15 * 1);
			drawText(col2Str(lowCol::green) + L"120%", letterbox.x + 18 + 44 + 296, letterbox.y + 3 + 15 * 1);
			drawText(col2Str(col::lightGray) + L"MENTAL", letterbox.x + 18 + 296, letterbox.y + 3 + 15 * 2);
			drawText(col2Str(lowCol::green) + L"XD", letterbox.x + 18 + 44 + 296, letterbox.y + 3 + 15 * 2);

			//시간 표시 기능
			{
				int timeIndex;
				// PM or AM
				if (getHour() >= 12) { timeIndex = segmentIndex::pm; }
				else { timeIndex = segmentIndex::am; }
				drawSprite(spr::segment, timeIndex, letterbox.x + 18 + 446 + 14 * 0, letterbox.y + 10);
				//xx시
				drawSprite(spr::segment, getHour() / 10, letterbox.x + 18 + 446 + 14 * 1, letterbox.y + 10);
				drawSprite(spr::segment, getHour() % 10, letterbox.x + 18 + 446 + 14 * 2, letterbox.y + 10);
				//:
				drawSprite(spr::segment, segmentIndex::colon, letterbox.x + 18 + 446 + 14 * 3, letterbox.y + 10);
				//xx분
				drawSprite(spr::segment, getMin() / 10, letterbox.x + 18 + 446 + 14 * 4, letterbox.y + 10);
				drawSprite(spr::segment, getMin() % 10, letterbox.x + 18 + 446 + 14 * 5, letterbox.y + 10);
				//xx초
				setZoom(0.7);
				drawSprite(spr::segment, getSec() / 10, letterbox.x + 18 + 446 + 14 * 6, letterbox.y + 15);
				drawSprite(spr::segment, getSec() % 10, letterbox.x + 18 + 446 + 14 * 6 + 10, letterbox.y + 15);

				//xxxx년
				drawSprite(spr::segment, getYear() / 1000, letterbox.x + 18 + 460 + 10 * -1, letterbox.y + 7 + 25);
				drawSprite(spr::segment, (getYear() % 1000) / 100, letterbox.x + 18 + 460 + 10 * 0, letterbox.y + 7 + 25);
				drawSprite(spr::segment, ((getYear() % 100) / 10), letterbox.x + 18 + 460 + 10 * 1, letterbox.y + 7 + 25);
				drawSprite(spr::segment, getYear() % 10, letterbox.x + 18 + 460 + 10 * 2, letterbox.y + 7 + 25);
				//:
				drawSprite(spr::segment, segmentIndex::dash, letterbox.x + 18 + 460 + 10 * 3, letterbox.y + 7 + 25);
				//xx월
				drawSprite(spr::segment, getMonth() / 10, letterbox.x + 18 + 460 + 10 * 4, letterbox.y + 7 + 25);
				drawSprite(spr::segment, getMonth() % 10, letterbox.x + 18 + 460 + 10 * 5, letterbox.y + 7 + 25);
				//:
				drawSprite(spr::segment, segmentIndex::dash, letterbox.x + 18 + 460 + 10 * 6, letterbox.y + 7 + 25);
				//xx일
				drawSprite(spr::segment, getDay() / 10, letterbox.x + 18 + 460 + 10 * 7, letterbox.y + 7 + 25);
				drawSprite(spr::segment, getDay() % 10, letterbox.x + 18 + 460 + 10 * 8, letterbox.y + 7 + 25);
				setZoom(1.0);
			}

			drawSprite(spr::ecliptic, 2, letterbox.x + 18 + 374, letterbox.y);

			int cx, cy;
			int pz = Player::ins()->getGridZ();
			World::ins()->changeToChunkCoord(Player::ins()->getGridX(), Player::ins()->getGridY(), cx, cy);
			if (World::ins()->getChunkWeather(cx, cy, pz) == weatherFlag::sunny)
			{
				static int index = 0;
				int sprSize = 6;
				if (timer::timer600 % 12 == 0)
				{
					index++;
					if (index == sprSize) index = 0;
				}
				drawSpriteCenter(spr::symbolSunny, index, letterbox.x + 426, letterbox.y + 19);
			}
			else if (World::ins()->getChunkWeather(cx, cy, pz) == weatherFlag::cloudy)
			{
				static int index = 0;
				int sprSize = 9;
				if (timer::timer600 % 12 == 0)
				{
					index++;
					if (index >= sprSize) index = 0;
				}
				drawSpriteCenter(spr::symbolCloudy, index, letterbox.x + 426, letterbox.y + 19);
			}
			else if (World::ins()->getChunkWeather(cx, cy, pz) == weatherFlag::rain)
			{
				static int index = 0;
				int sprSize = 3;
				if (timer::timer600 % 12 == 0)
				{
					index++;
					if (index >= sprSize) index = 0;
				}
				drawSpriteCenter(spr::symbolRain, index, letterbox.x + 426, letterbox.y + 19);
			}
			else if (World::ins()->getChunkWeather(cx, cy, pz) == weatherFlag::storm)
			{
				static int index = 0;
				int sprSize = 3;
				if (timer::timer600 % 12 == 0)
				{
					index++;
					if (index >= sprSize) index = 0;
				}
				drawSpriteCenter(spr::symbolStorm, index, letterbox.x + 426, letterbox.y + 19);
			}
			else if (World::ins()->getChunkWeather(cx, cy, pz) == weatherFlag::snow)
			{
				static int index = 0;
				int sprSize = 15;
				if (timer::timer600 % 12 == 0)
				{
					index++;
					if (index >= sprSize) index = 0;
				}
				drawSpriteCenter(spr::symbolSnow, index, letterbox.x + 426, letterbox.y + 19);
			}

			SDL_SetRenderDrawColor(renderer, lowCol::white.r, lowCol::white.g, lowCol::white.b, 0xff);
			drawTextCenter(L"15℃", letterbox.x + 18 + 374 + 36, letterbox.y + 16 + 22);

			drawSprite(spr::batteryGauge, 4, letterbox.x + 18 + 562, letterbox.y + 3);
			setFontSize(10);

			drawTextCenter(col2Str(lowCol::black) + L"72%", letterbox.x + 18 + 562 + 16 + 1, letterbox.y + 3 + 24);
			drawTextCenter(col2Str(lowCol::black) + L"72%", letterbox.x + 18 + 562 + 16, letterbox.y + 3 + 24 - 1);
			drawTextCenter(col2Str(lowCol::black) + L"72%", letterbox.x + 18 + 562 + 16 - 1, letterbox.y + 3 + 24);
			drawTextCenter(col2Str(lowCol::black) + L"72%", letterbox.x + 18 + 562 + 16, letterbox.y + 3 + 24 + 1);
			drawTextCenter(col2Str(lowCol::white) + L"72%", letterbox.x + 18 + 562 + 16, letterbox.y + 3 + 24);
		}

		//팝업 버튼
		drawSprite(spr::menuPopUp, 0, letterbox.x + letterbox.w - 42, letterbox.y - 36);
		{
			SDL_Color popUpBtnColor;
			if (checkCursor(&letterboxPopUpButton))
			{
				if (click == true) { popUpBtnColor = lowCol::deepBlue; }
				else { popUpBtnColor = lowCol::blue; }
			}
			else if (state[SDL_SCANCODE_RETURN]) { popUpBtnColor = lowCol::blue; }
			else { popUpBtnColor = lowCol::black; }

			drawStadium(letterboxPopUpButton.x, letterboxPopUpButton.y, letterboxPopUpButton.w, letterboxPopUpButton.h, popUpBtnColor, 200, 5);

			if (inputType == input::gamepad)
			{
				if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y)) { targetBtnSpr = spr::buttonsPressed; }
				else { targetBtnSpr = spr::buttons; }
				drawSpriteCenter(targetBtnSpr, keyIcon::duelSense_TRI, letterboxPopUpButton.x + 15, letterboxPopUpButton.y + 15);
			}
			else if (inputType != input::keyboard)
			{
				if (y == 0) drawSpriteCenter(spr::windowArrow, 1, letterboxPopUpButton.x + 15, letterboxPopUpButton.y + 15);
				else drawSpriteCenter(spr::windowArrow, 3, letterboxPopUpButton.x + 15, letterboxPopUpButton.y + 15);
			}
			else
			{
				if (state[SDL_SCANCODE_RETURN]) { targetBtnSpr = spr::buttonsPressed; }
				else { targetBtnSpr = spr::buttons; }
				drawSpriteCenter(targetBtnSpr, keyIcon::keyboard_Enter, letterboxPopUpButton.x + 15, letterboxPopUpButton.y + 15);
			}
		}

	}
	else
	{

	}

	drawBarAct();
	drawTab();
	drawQuickSlot();
}


void HUD::drawTab()
{
	SDL_Color btnColor = { 0x00, 0x00, 0x00 };
	if (checkCursor(&tab))
	{
		if (click == false) { btnColor = lowCol::blue; }
		else { btnColor = lowCol::deepBlue; }
	}

	switch (tabType)
	{
	case tabFlag::autoAtk:

		if (checkCursor(&tabSmallBox))
		{
			if (click == false) { drawSprite(spr::tab, 3, tab.x, tab.y - 2); }
			else { drawSprite(spr::tab, 4, tab.x, tab.y - 2); }
		}
		else if (checkCursor(&tab))
		{
			if (click == false) { drawSprite(spr::tab, 1, tab.x, tab.y - 2); }
			else { drawSprite(spr::tab, 2, tab.x, tab.y - 2); }
		}
		else
		{
			drawSprite(spr::tab, 0, tab.x, tab.y - 2);
		}

		//drawStadium(tab.x, tab.y, tab.w, tab.h, btnColor, 150, 5);
		//drawSpriteCenter(spr::icon48, 2, tab.x + 40, tab.y + 39);
		//drawSpriteCenter(spr::icon48, 1, tab.x + 80, tab.y + 45);
		setFontSize(13);
		SDL_SetRenderDrawColor(renderer, lowCol::white.r, lowCol::white.g, lowCol::white.b, 0xff);
		drawTextCenter(sysStr[1], tab.x + 60, tab.y + 92);
		drawTextCenter(sysStr[2], tab.x + 60, tab.y + 92 + 14);
		break;
	case tabFlag::aim:
		drawStadium(tab.x, tab.y, tab.w, tab.h, btnColor, 150, 5);
		setZoom(1.5);
		drawSpriteCenter(spr::icon48, 42, tab.x + 60, tab.y + 52);
		setZoom(1.0);
		setFontSize(13);
		SDL_SetRenderDrawColor(renderer, lowCol::white.r, lowCol::white.g, lowCol::white.b, 0xff);
		drawTextCenter(sysStr[104], tab.x + 60, tab.y + 92 + 7);
		break;
	case tabFlag::closeWin:
		//창 닫기
		drawStadium(tab.x, tab.y, tab.w, tab.h, btnColor, 150, 5);
		drawSpriteCenter(spr::icon48, 14, tab.x + 60, tab.y + 52);
		setFontSize(13);
		SDL_SetRenderDrawColor(renderer, lowCol::white.r, lowCol::white.g, lowCol::white.b, 0xff);
		drawTextCenter(sysStr[14], tab.x + 60, tab.y + 92 + 7);
		break;
	case tabFlag::back:
		//뒤로가기
		drawStadium(tab.x, tab.y, tab.w, tab.h, btnColor, 150, 5);
		drawSpriteCenter(spr::icon48, 31, tab.x + 60, tab.y + 52);
		setFontSize(13);
		SDL_SetRenderDrawColor(renderer, lowCol::white.r, lowCol::white.g, lowCol::white.b, 0xff);
		drawTextCenter(sysStr[31], tab.x + 60, tab.y + 92 + 7);
		break;
	case tabFlag::confirm:
		//뒤로가기
		drawStadium(tab.x, tab.y, tab.w, tab.h, btnColor, 150, 5);
		drawSpriteCenter(spr::icon48, 39, tab.x + 60, tab.y + 52);
		setFontSize(13);
		SDL_SetRenderDrawColor(renderer, lowCol::white.r, lowCol::white.g, lowCol::white.b, 0xff);
		drawTextCenter(sysStr[91], tab.x + 60, tab.y + 92 + 7);
		break;
	default:
		errorBox("undefined tabFalg");
		break;
	}

	Sprite* targetBtnSpr;
	if (inputType == input::gamepad)
	{
		if (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 0) { targetBtnSpr = spr::buttonsPressed; }
		else { targetBtnSpr = spr::buttons; }
		drawSpriteCenter(targetBtnSpr, keyIcon::duelSense_R2, tab.x + 4, tab.y + 4);
	}
}

void HUD::drawQuickSlot()
{
	int pivotX = quickSlotRegion.x;
	int pivotY = quickSlotRegion.y;

	//drawRect({ pivotX,pivotY,180,358, }, lowCol::skyBlue);

	int sprIndex = 0;
	if (checkCursor(&quickSlotPopBtn))
	{
		if (click == false) sprIndex = 1;
		else sprIndex = 2;
	}
	else sprIndex = 0;

	drawSprite(spr::topQuickSlotBtn, sprIndex, pivotX, pivotY);
	drawFillRect({ pivotX + 43,pivotY,137,38 }, col::black, 200);


	Sprite* targetBtnSpr;
	if (inputType == input::gamepad)
	{
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) { targetBtnSpr = spr::buttonsPressed; }
		else { targetBtnSpr = spr::buttons; }
		drawSpriteCenter(targetBtnSpr, keyIcon::duelSense_R1, pivotX + 23, pivotY + 18);
	}
	else
	{
		if (isQuickSlotPop == false) drawSpriteCenter(spr::windowArrow, 2, pivotX + 21, pivotY + 18);
		else drawSpriteCenter(spr::windowArrow, 0, pivotX + 21, pivotY + 18);
	}

	//개별 스킬창 그리기 1번부터 8번까지
	for (int i = 0; i < 8; i++)
	{
		if (isQuickSlotPop == true && (checkCursor(&quickSlotBtn[i]) || quickSlotCursor == i) && quickSlot[i].first != quickSlotFlag::NONE)
		{
			if (click) drawSprite(spr::singleQuickSlot, 2, quickSlotBtn[i].x, quickSlotBtn[i].y);
			else drawSprite(spr::singleQuickSlot, 1, quickSlotBtn[i].x, quickSlotBtn[i].y);
		}
		else drawSprite(spr::singleQuickSlot, 0, quickSlotBtn[i].x, quickSlotBtn[i].y);

		if (quickSlot[i].first == quickSlotFlag::NONE)
		{
			drawCross2(quickSlotBtn[i].x + 6, quickSlotBtn[i].y + 2, 0, 5, 0, 5);
			drawCross2(quickSlotBtn[i].x + 6 + 32, quickSlotBtn[i].y + 2, 0, 5, 5, 0);
			drawCross2(quickSlotBtn[i].x + 6, quickSlotBtn[i].y + 2 + 32, 5, 0, 0, 5);
			drawCross2(quickSlotBtn[i].x + 6 + 32, quickSlotBtn[i].y + 2 + 32, 5, 0, 5, 0);
			drawFillRect({ quickSlotBtn[i].x + 6, quickSlotBtn[i].y + 2 ,34,34 }, col::black, 180);
		}
		else
		{
			setZoom(2.0);
			std::wstring skillName = L"";
			drawSprite(spr::skillSet, skillDex[quickSlot[i].second].iconIndex, quickSlotBtn[i].x + 7, quickSlotBtn[i].y + 3);
			skillName = skillDex[quickSlot[i].second].name;
			setZoom(1.0);

			setFontSize(14);
			drawText(L"#FFFFFF" + skillName, quickSlotBtn[i].x + 43, quickSlotBtn[i].y + 5);

			//setFontSize(10);
			//drawText(L"#FFFF00-32 MP" + skillName, quickSlotBtn[i].x + 120, quickSlotBtn[i].y + 24);

			drawCross2(quickSlotBtn[i].x + 6, quickSlotBtn[i].y + 2, 0, 5, 0, 5);
			drawCross2(quickSlotBtn[i].x + 6 + 32, quickSlotBtn[i].y + 2, 0, 5, 5, 0);
			drawCross2(quickSlotBtn[i].x + 6, quickSlotBtn[i].y + 2 + 32, 5, 0, 0, 5);
			drawCross2(quickSlotBtn[i].x + 6 + 32, quickSlotBtn[i].y + 2 + 32, 5, 0, 5, 0);
		}
	}
}

void HUD::drawBarAct()
{
	if (ctrlVeh != nullptr)
	{
		if (typeHUD == vehFlag::car)
		{
			//핸들
			int wheelIndex = 0;
			SDL_SetTextureAlphaMod(spr::vehicleHUDSteeringWheel->getTexture(), 140);
			SDL_SetTextureBlendMode(spr::vehicleHUDSteeringWheel->getTexture(), SDL_BLENDMODE_BLEND);
			if (click)
			{
				if (checkCursor(&barButton[0])) wheelIndex = 1;
				else if (checkCursor(&barButton[1])) wheelIndex = 3;
				else if (checkCursor(&barButton[2])) wheelIndex = 2;
			}
			drawSpriteCenter(spr::vehicleHUDSteeringWheel, wheelIndex, barButton[1].x + (barButton[1].w / 2), barButton[1].y + (barButton[1].h / 2));

			drawSpriteCenter(spr::vehicleHUDParts, 15, barButton[0].x + barButton[0].w / 2, barButton[0].y + barButton[0].h / 2); //좌회전 마크
			drawSpriteCenter(spr::vehicleHUDParts, 16, barButton[1].x + barButton[1].w / 2, barButton[1].y + barButton[1].h / 2); //1턴 대기
			drawSpriteCenter(spr::vehicleHUDParts, 17, barButton[2].x + barButton[2].w / 2, barButton[2].y + barButton[2].h / 2); //우회전 마크

			drawSpriteCenter(spr::vehicleHUDParts, 0 + (checkCursor(&barButton[3]) && click), barButton[3].x + barButton[3].w / 2, barButton[3].y + barButton[3].h / 2); //엔진 스타트

			//기어(PRND) 그리기
			int gearSprIndex = 0;
			if (((Vehicle*)ctrlVeh)->gearState == gearFlag::park) gearSprIndex = 0;
			else if (((Vehicle*)ctrlVeh)->gearState == gearFlag::reverse) gearSprIndex = 1;
			else if (((Vehicle*)ctrlVeh)->gearState == gearFlag::neutral) gearSprIndex = 2;
			else gearSprIndex = 3;
			drawSpriteCenter(spr::vehicleHUDParts, 5 + gearSprIndex, barButton[4].x + barButton[4].w / 2, barButton[4].y + barButton[4].h / 2 - 12);

			drawSpriteCenter(spr::vehicleHUDParts, 10 + (checkCursor(&barButton[5]) && click), barButton[5].x + barButton[5].w / 2, barButton[5].y + barButton[5].h / 2);
			drawSpriteCenter(spr::vehicleHUDParts, 12 + (checkCursor(&barButton[6]) && click), barButton[6].x + barButton[6].w / 2, barButton[6].y + barButton[6].h / 2);
		}
		else if (typeHUD == vehFlag::heli)
		{
			Vehicle* myHeli = ((Vehicle*)ctrlVeh);

			setZoom(1.5);
			//drawSpriteCenter(spr::collectiveLever, myHeli->collectiveState + 1, barButton[0].x + barButton[0].w / 2, barButton[0].y + barButton[0].h / 2 - 16); //콜렉티브 레버
			setZoom(1);
			SDL_SetTextureAlphaMod(spr::icon48->getTexture(), 160);
			drawSpriteCenter(spr::icon48, 88 + myHeli->collectiveState, barButton[0].x + barButton[0].w / 2, barButton[0].y + barButton[0].h / 2 - 8);
			SDL_SetTextureAlphaMod(spr::icon48->getTexture(), 255);

			drawSpriteCenter(spr::vehicleHUDParts, 16, barButton[1].x + barButton[1].w / 2, barButton[1].y + barButton[1].h / 2); //1턴 대기

			setZoom(1.5);
			//drawSpriteCenter(spr::cyclicLever, myHeli->cyclicState + 1, barButton[2].x + barButton[2].w / 2, barButton[2].y + barButton[2].h / 2 - 16); //사이클릭 레버
			setZoom(1);
			SDL_SetTextureAlphaMod(spr::icon48->getTexture(), 160);
			drawSpriteCenter(spr::icon48, myHeli->cyclicState + 1 + 110, barButton[2].x + barButton[2].w / 2, barButton[2].y + barButton[2].h / 2 - 8);
			SDL_SetTextureAlphaMod(spr::icon48->getTexture(), 255);


			drawSpriteCenter(spr::vehicleHUDParts, 0 + (checkCursor(&barButton[3]) && click), barButton[3].x + barButton[3].w / 2, barButton[3].y + barButton[3].h / 2); //엔진 스타트

			setZoom(1.5);
			//drawSpriteCenter(spr::rpmLever, myHeli->rpmState, barButton[4].x + barButton[4].w / 2, barButton[4].y + barButton[4].h / 2 + 9); //RPM 레버
			setZoom(1);
			SDL_SetTextureAlphaMod(spr::icon48->getTexture(), 160);
			drawSpriteCenter(spr::icon48, myHeli->rpmState + 100, barButton[4].x + barButton[4].w / 2, barButton[4].y + barButton[4].h / 2 - 8);
			SDL_SetTextureAlphaMod(spr::icon48->getTexture(), 255);


			drawSpriteCenter(spr::tailPedalL, 0 + (checkCursor(&barButton[5]) && click), barButton[5].x + barButton[5].w / 2, barButton[5].y + barButton[5].h / 2); //좌회전 테일페달
			drawSpriteCenter(spr::tailPedalR, 0 + (checkCursor(&barButton[6]) && click), barButton[6].x + barButton[6].w / 2, barButton[6].y + barButton[6].h / 2); //우회전 테일페달
		}
		else if (typeHUD == vehFlag::train)
		{
			// std::vector<act> train = { act::rpmLever, act::wait, act::brake, act::startEngine, act::blank,act::blank,act::blank };

			Vehicle* myTrain = ((Vehicle*)ctrlVeh);

			setZoom(1);
			SDL_SetTextureAlphaMod(spr::icon48->getTexture(), 160);
			drawSpriteCenter(spr::icon48, myTrain->rpmState + 100, barButton[0].x + barButton[0].w / 2, barButton[0].y + barButton[0].h / 2 - 8);
			SDL_SetTextureAlphaMod(spr::icon48->getTexture(), 255);

			drawSpriteCenter(spr::vehicleHUDParts, 16, barButton[1].x + barButton[1].w / 2, barButton[1].y + barButton[1].h / 2); //1턴 대기

			drawSpriteCenter(spr::icon48, 92 + (checkCursor(&barButton[2]) && click), barButton[2].x + barButton[2].w / 2, barButton[2].y + barButton[2].h / 2 - 8);

			drawSpriteCenter(spr::vehicleHUDParts, 0 + (checkCursor(&barButton[3]) && click), barButton[3].x + barButton[3].w / 2, barButton[3].y + barButton[3].h / 2); //엔진 스타트

			int gearSprIndex = 0;
			if (((Vehicle*)ctrlVeh)->gearState == gearFlag::park) gearSprIndex = 0;
			else if (((Vehicle*)ctrlVeh)->gearState == gearFlag::reverse) gearSprIndex = 1;
			else if (((Vehicle*)ctrlVeh)->gearState == gearFlag::neutral) gearSprIndex = 2;
			else gearSprIndex = 3;
			drawSpriteCenter(spr::vehicleHUDParts, 5 + gearSprIndex, barButton[4].x + barButton[4].w / 2, barButton[4].y + barButton[4].h / 2 - 12);

			drawSpriteCenter(spr::tailPedalL, 0 + (checkCursor(&barButton[5]) && click), barButton[5].x + barButton[5].w / 2, barButton[5].y + barButton[5].h / 2); //좌회전 테일페달(미배정)
			drawSpriteCenter(spr::tailPedalR, 0 + (checkCursor(&barButton[6]) && click), barButton[6].x + barButton[6].w / 2, barButton[6].y + barButton[6].h / 2); //우회전 테일페달(미배정)
		}
	}

	//하단 레터박스 7버튼 그리기
	for (int i = 0; i < barAct.size(); i++)
	{
		if (ctrlVeh == nullptr)//차량 모드가 아닐 경우
		{
			SDL_Color btnColor = { 0x00, 0x00, 0x00 };
			SDL_Color outlineColor = { 0x4A, 0x4A, 0x4A };

			if (checkCursor(&barButton[i]) || barActCursor == i)
			{
				if (click == false)
				{
					btnColor = lowCol::blue;
				}
				else
				{
					btnColor = lowCol::deepBlue;
				}
				outlineColor = { 0xa6, 0xa6, 0xa6 };
			}

			drawStadium(barButton[i].x, barButton[i].y, 72, 72, btnColor, 200, 5);
			SDL_Rect letterboxInButton = { barButton[i].x + 3,  barButton[i].y + 3, 72 - 6, 72 - 6 };
			drawRect(letterboxInButton, outlineColor);
		}
		else if (i <= 2)//왼쪽부터 3개의 버튼의 경우(좌회전, 1턴대기, 우회전)
		{
			//버튼을 투명하게 그림
		}
		else if (i <= 7) //그 외의 4개의 버튼
		{
		}

		std::wstring actName = L" ";
		int actIndex = 0;
		bool deactRect = false; // true일 경우 회색으로 변함
		auto setBtnLayout = [&actName, &actIndex](std::wstring inputString, int inputActIndex)
			{
				actName = inputString;
				actIndex = inputActIndex;
			};

		if (barAct[i] == act::status) setBtnLayout(sysStr[3], 33);
		else if (barAct[i] == act::mutation) setBtnLayout(sysStr[54], 34);
		else if (barAct[i] == act::ability) setBtnLayout(sysStr[4], 4);
		else if (barAct[i] == act::inventory) setBtnLayout(sysStr[5], 79);
		else if (barAct[i] == act::bionic) setBtnLayout(sysStr[6], 47);
		else if (barAct[i] == act::talent) setBtnLayout(sysStr[7], 7);
		else if (barAct[i] == act::runMode)
		{

			if (Player::ins()->entityInfo.walkMode == walkFlag::walk) setBtnLayout(sysStr[8], 9);
			else if (Player::ins()->entityInfo.walkMode == walkFlag::run) setBtnLayout(sysStr[8], 10);
			else if (Player::ins()->entityInfo.walkMode == walkFlag::crouch) setBtnLayout(sysStr[8], 12);
			else if (Player::ins()->entityInfo.walkMode == walkFlag::crawl) setBtnLayout(sysStr[8], 11);
		}
		else if (barAct[i] == act::identify) setBtnLayout(sysStr[135], 52);
		else if (barAct[i] == act::vehicle) setBtnLayout(sysStr[128], 48);
		else if (barAct[i] == act::alchemy) setBtnLayout(sysStr[134], 51);
		else if (barAct[i] == act::armor) setBtnLayout(sysStr[129], 16);
		else if (barAct[i] == act::cooking) setBtnLayout(sysStr[130], 49);
		else if (barAct[i] == act::menu) setBtnLayout(sysStr[9], 13);
		else if (barAct[i] == act::loot) setBtnLayout(sysStr[10], 0);
		else if (barAct[i] == act::pick) setBtnLayout(sysStr[11], 17);
		else if (barAct[i] == act::wield) setBtnLayout(sysStr[12], 15);
		else if (barAct[i] == act::equip) setBtnLayout(sysStr[13], 16);
		else if (barAct[i] == act::eat) setBtnLayout(sysStr[30], 25);
		else if (barAct[i] == act::pickSelect) setBtnLayout(sysStr[25], 26);
		else if (barAct[i] == act::selectAll) setBtnLayout(sysStr[26], 27);
		else if (barAct[i] == act::searching) setBtnLayout(sysStr[27], 28);
		else if (barAct[i] == act::select) setBtnLayout(sysStr[29], 30);
		else if (barAct[i] == act::droping) setBtnLayout(sysStr[52], 18);
		else if (barAct[i] == act::throwing) setBtnLayout(sysStr[53], 19);
		else if (barAct[i] == act::craft) setBtnLayout(sysStr[75], 21);
		else if (barAct[i] == act::construct) setBtnLayout(sysStr[73], 20);
		else if (barAct[i] == act::open) setBtnLayout(sysStr[88], 38);
		else if (barAct[i] == act::test) setBtnLayout(sysStr[92], 60);
		else if (barAct[i] == act::insert) setBtnLayout(sysStr[11], 17);
		else if (barAct[i] == act::reload) setBtnLayout(sysStr[100], 40);
		else if (barAct[i] == act::reloadBulletToMagazine) setBtnLayout(sysStr[113], 41);
		else if (barAct[i] == act::unloadBulletFromMagazine) setBtnLayout(sysStr[114], 44);
		else if (barAct[i] == act::reloadMagazine) setBtnLayout(sysStr[115], 40);
		else if (barAct[i] == act::unloadMagazine) setBtnLayout(sysStr[116], 43);
		else if (barAct[i] == act::reloadBulletToGun) setBtnLayout(sysStr[113], 45);
		else if (barAct[i] == act::unloadBulletFromGun) setBtnLayout(sysStr[114], 46);
		else if (barAct[i] == act::turnLeft) setBtnLayout(sysStr[141], 0);
		else if (barAct[i] == act::wait) setBtnLayout(sysStr[142], 0);
		else if (barAct[i] == act::turnRight) setBtnLayout(sysStr[143], 0);
		else if (barAct[i] == act::startEngine) setBtnLayout(sysStr[144], 0);
		else if (barAct[i] == act::stopEngine) setBtnLayout(sysStr[145], 0);
		else if (barAct[i] == act::shiftGear) setBtnLayout(sysStr[146], 0);
		else if (barAct[i] == act::accel) setBtnLayout(sysStr[147], 0);
		else if (barAct[i] == act::brake) setBtnLayout(sysStr[148], 0);
		else if (barAct[i] == act::god) setBtnLayout(sysStr[149], 61);
		else if (barAct[i] == act::map) setBtnLayout(sysStr[150], 73);
		else if (barAct[i] == act::collectiveLever) setBtnLayout(sysStr[151], 0);
		else if (barAct[i] == act::cyclicLever) setBtnLayout(sysStr[152], 0);
		else if (barAct[i] == act::rpmLever) setBtnLayout(sysStr[153], 0);
		else if (barAct[i] == act::tailRotorPedalL) setBtnLayout(sysStr[154], 0);
		else if (barAct[i] == act::tailRotorPedalR) setBtnLayout(sysStr[155], 0);
		else if (barAct[i] == act::closeDoor) setBtnLayout(sysStr[156], 91);
		else if (barAct[i] == act::phone) setBtnLayout(sysStr[189], 150);
		else if (barAct[i] == act::message) setBtnLayout(sysStr[190], 151);
		else if (barAct[i] == act::camera) setBtnLayout(sysStr[191], 152);
		else if (barAct[i] == act::internet) setBtnLayout(sysStr[192], 154);
		else if (barAct[i] == act::settings) setBtnLayout(sysStr[193], 155);
		else if (barAct[i] == act::saveAndQuit) setBtnLayout(sysStr[194], 156);
		else if (barAct[i] == act::skillActive)
		{
			errorBox(targetSkill == nullptr, L"HUD의 targetSkill이 nullptr이다.\n");
			setBtnLayout(sysStr[157], 140);

		}
		else if (barAct[i] == act::skillToggle)
		{
			errorBox(targetSkill == nullptr, L"HUD의 targetSkill이 nullptr이다.\n");
			if (targetSkill->toggle == false)
			{
				setBtnLayout(sysStr[158], 141);
			}
			else
			{
				setBtnLayout(sysStr[159], 142);
			}
		}
		else if (barAct[i] == act::quickSlot)
		{
			errorBox(targetSkill == nullptr, L"HUD의 targetSkill이 nullptr이다.\n");
			if (targetSkill->isQuickSlot == false)
			{
				setBtnLayout(sysStr[160], 146);
			}
			else
			{
				setBtnLayout(sysStr[161], 147);
			}
		}
		else setBtnLayout(L" ", 0);

		//48*48 심볼 아이콘 그리기
		drawSpriteCenter(spr::icon48, actIndex, barButton[i].x + (barButton[i].w / 2), barButton[i].y + (barButton[i].h / 2) - 10);

		//하단 텍스트
		int fontSize = 13;
		setFontSize(fontSize);
		while (queryTextWidth(actName, true) > barButton[0].w)
		{
			fontSize--;
			setFontSize(fontSize);
		}
		SDL_SetRenderDrawColor(renderer, lowCol::white.r, lowCol::white.g, lowCol::white.b, 0xff);
		drawTextCenter(actName, barButton[i].x + (barButton[i].w / 2), barButton[i].y + (barButton[i].h / 2) + 22);

		if (checkCursor(&barButton[i]) || barActCursor == i)
		{
			int markerIndex = 0;
			if (timer::timer600 % 30 < 5) { markerIndex = 0; }
			else if (timer::timer600 % 30 < 10) { markerIndex = 1; }
			else if (timer::timer600 % 30 < 15) { markerIndex = 2; }
			else if (timer::timer600 % 30 < 20) { markerIndex = 1; }
			else { markerIndex = 0; }
			if (ctrlVeh == nullptr)
			{
				drawSpriteCenter(spr::letterboxBtnMarker, markerIndex, barButton[i].x + (barButton[i].w / 2), barButton[i].y + (barButton[i].h / 2));
			}
			else
			{
				drawSpriteCenter(spr::vehicleActCursor, markerIndex + 1, barButton[i].x + (barButton[i].w / 2), barButton[i].y + (barButton[i].h / 2));
			}
		}
		else if (ctrlVeh != nullptr)
		{
			drawSpriteCenter(spr::vehicleActCursor, 0, barButton[i].x + (barButton[i].w / 2), barButton[i].y + (barButton[i].h / 2));
		}

		if (deactRect == true) drawStadium(barButton[i].x, barButton[i].y, 72, 72, { 0,0,0 }, 120, 5);
	}
}

void HUD::drawStatusEffects()
{
	std::vector<std::pair<statEfctFlag, int>>& myEfcts = Player::ins()->entityInfo.statusEffects;
	for (int i = 0; i < myEfcts.size(); i++)
	{
		int pivotX = 10;
		int pivotY = 185 + 38*i;
		std::wstring statEfctName = L"";
		int statEfctIcon = 0;

		switch (myEfcts[i].first)
		{
		case statEfctFlag::confusion:
			statEfctName = L"Confusion";
			statEfctIcon = 1;
			break;
		case statEfctFlag::bleeding:
			statEfctName = L"Bleeding";
			statEfctIcon = 2;
			break;
		case statEfctFlag::hunger:
			statEfctName = L"Hunger";
			statEfctIcon = 3;
			break;
		case statEfctFlag::dehydration:
			statEfctName = L"Dehydration";
			statEfctIcon = 4;
			break;
		case statEfctFlag::blindness:
			statEfctName = L"Blindness";
			statEfctIcon = 15;
			break;
		}

		drawSprite(spr::statusEffectRect, pivotX, pivotY);

		setZoom(2.0);
		drawSprite(spr::statusIcon, statEfctIcon, pivotX + 1, pivotY + 1);
		setZoom(1.0);

		drawCross2(pivotX, pivotY, 0, 5, 0, 5);
		drawCross2(pivotX + 32, pivotY, 0, 5, 5, 0);
		drawCross2(pivotX, pivotY + 32, 5, 0, 0, 5);
		drawCross2(pivotX + 32, pivotY + 32, 5, 0, 5, 0);

		setFontSize(13);
		drawText(col2Str(lowCol::red) + statEfctName, pivotX + 37, pivotY + 1);

		if (myEfcts[i].second > 0)
		{
			setZoom(0.7);
			int seg1 = myEfcts[i].second / 100;
			int seg2 = (myEfcts[i].second % 100) / 10;
			int seg3 = myEfcts[i].second % 10;
			
			if (seg1 > 0) drawSprite(spr::segment, myEfcts[i].second / 100, pivotX + 97, pivotY + 20);
			if (seg2 > 0 || seg1 > 0) drawSprite(spr::segment, (myEfcts[i].second % 100)/10, pivotX + 97 + 11, pivotY + 20);
			if (seg3 > 0 || seg2 > 0 || seg3 > 0) drawSprite(spr::segment, myEfcts[i].second % 10, pivotX + 97 + 22, pivotY + 20);
			setZoom(1.0);
		}
		else
		{
			if (myEfcts[i].first == statEfctFlag::bleeding)
			{
				//게이지
				drawRect(pivotX + 37, pivotY + 20, 61, 11, col::gray);
				drawFillRect(pivotX + 37 + 2, pivotY + 20 + 2, 61 - 4 - 20, 11 - 4, lowCol::red);
			}
		}
	}
}