import Entity;
import globalVar;
import constVar;
import textureVar;
import turnWait;
import World;
import Sticker;
import Flame;
import Player;

bool Entity::runAnimation(bool shutdown)
{
	//if (isPlayer) prt(L"Player�� runAnimation�� ����Ǿ���.\n");
	//else prt(L"Entity %p�� runAnimation�� ����Ǿ���.\n",this);

	if (getAniType() == aniFlag::move)//���� �÷��̾� �ν��Ͻ��� ��ǥ�� ������ǥ�� �ٸ� ���
	{
		//8������-4(���ǵ�2)
		//6������-3(���ǵ�3)
		//4������-2(���ǵ�4)

		// 1 / 60�ʸ��� runAnimation�� �����


		static float totalDistX = 0;
		static float totalDistY = 0;
		static bool footChanged = false;
		
		float speed = 2.5;
		if (Player::ins()->getSpriteInfimum() == sprInf::run) speed = 3.5;
		else if (Player::ins()->getSpriteInfimum() == sprInf::walk) speed = 3.0;
		else if (Player::ins()->getSpriteInfimum() == sprInf::walk) speed = 2.0;
		else if (Player::ins()->getSpriteInfimum() == sprInf::walk) speed = 2.0;
		
		addTimer();

		if (getTimer() == 1)
		{
			footChanged = false;
			totalDistX = 0;
			totalDistY = 0;
		}


		if (getX() + getFakeX() > getDstX()) totalDistX -= speed;
		else if (getX() + getFakeX() < getDstX()) totalDistX += speed;
		if (getY() + getFakeY() > getDstY()) totalDistY -= speed;
		else if (getY() + getFakeY() < getDstY()) totalDistY += speed;

		setFakeX(std::floor(totalDistX));
		setFakeY(std::floor(totalDistY));

		if (isPlayer)
		{
			cameraFix = false;
			cameraX = getX() + getFakeX();
			cameraY = getY() + getFakeY();
		}

		if (std::abs(getFakeX()) >= 8.0 || std::abs(getFakeY()) >= 8.0)
		{
			if (footChanged == false)
			{
				if (getLeftFoot() == true)
				{
					setSpriteIndex(getSpriteInfimum() + 1);
					setLeftFoot(false);
				}
				else
				{
					setSpriteIndex(getSpriteInfimum() + 2);
					setLeftFoot(true);
				}
				footChanged = true;
			}
		}

		if (std::abs(getFakeX()) >= 16.0 || std::abs(getFakeY()) >= 16.0)
		{
			setSpriteIndex(getSpriteInfimum() + 0);
			resetTimer();
			setAniType(aniFlag::null);
			setFakeX(0);
			setFakeY(0);
			setGrid(getDstGridX(), getDstGridY(), getGridZ());
			turnWait(1.0);
			endMove();
			if (isPlayer) cameraFix = true;
			return true;
		}





	}
	else if (getAniType() == aniFlag::atk)
	{
		addTimer();

		char dx;
		char dy;

		switch (getDirection())
		{
		case 0: dx = 1; dy = 0; break;
		case 1: dx = 1; dy = -1; break;
		case 2: dx = 0; dy = -1; break;
		case 3: dx = -1; dy = -1; break;
		case 4: dx = -1; dy = 0; break;
		case 5: dx = -1; dy = 1; break;
		case 6: dx = 0; dy = 1; break;
		case 7: dx = 1; dy = 1; break;
		}

		Entity* address = (Entity*)World::ins()->getTile(atkTargetGridX, atkTargetGridY, atkTargetGridZ).EntityPtr;
		std::wstring stickerID = L"BASEATK" + std::to_wstring((unsigned __int64)this);

		if (shutdown == true)//������� ���� ��������
		{
			if (address != nullptr)
			{
				address->setFlashRGBA(0, 0, 0, 0);
			}
			aniUSet.erase(aniUSet.find(this));
			delete(((Sticker*)(StickerList.find(stickerID))->second));
			setFakeX(0);
			setFakeY(0);
			return true;
		}

		switch (getTimer())
		{
		case 2:
			setFakeX(getFakeX() + 3 * dx);
			setFakeY(getFakeY() + 3 * dy);
			break;
		case 3:
			setFakeX(getFakeX() + 2 * dx);
			setFakeY(getFakeY() + 2 * dy);
			break;
		case 4:
			setFakeX(getFakeX() + 1 * dx);
			setFakeY(getFakeY() + 1 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() + 2 * dx);
				address->setFakeY(address->getFakeY() + 2 * dy);
			}

			if (atkTargetPart == -1) { attack(atkTargetGridX, atkTargetGridY); }
			else { attack(atkTargetGridX, atkTargetGridY, atkTargetPart); }
			new Sticker(false, getX() + (16 * (atkTargetGridX - getGridX())), getY() + (16 * (atkTargetGridY - getGridY())), spr::effectCut1, 0, stickerID, true);
			break;
		case 5:
			setFakeX(getFakeX() - 1 * dx);
			setFakeY(getFakeY() - 1 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() - 1 * dx);
				address->setFakeY(address->getFakeY() - 1 * dy);
			}
			break;
		case 6:
			setFakeX(getFakeX() - 2 * dx);
			setFakeY(getFakeY() - 2 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() - 1 * dx);
				address->setFakeY(address->getFakeY() - 1 * dy);
			}
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(1);
			break;
		case 7:
			setFakeX(getFakeX() - 3 * dx);
			setFakeY(getFakeY() - 3 * dy);
			break;
		case 8:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(2);
			break;
		case 10:
			delete(((Sticker*)(StickerList.find(stickerID))->second));
		case 20:
			setFakeX(0);
			setFakeY(0);
			resetTimer();
			setAniType(aniFlag::null);
			if (isPlayer == true) { turnWait(endAtk()); }
			else { endAtk(); }
			return true;
		}

		if (getTimer() >= 5)
		{
			Entity* address = (Entity*)World::ins()->getTile(atkTargetGridX, atkTargetGridY, atkTargetGridZ).EntityPtr;
			if (address != nullptr)
			{
				Uint8 targetR, targetG, targetB, targetAlpha;
				address->getFlashRGBA(targetR, targetG, targetB, targetAlpha);
				if (address->getFlashType() == 1)
				{
					if (targetG > 51) { targetG -= 51; }
					else { targetG = 0; }
					if (targetB > 51) { targetB -= 51; }
					else { targetB = 0; }
					if (targetAlpha > 51) { targetAlpha -= 51; }
					else { targetAlpha = 0; }
					address->setFlashRGBA(targetR, targetG, targetB, targetAlpha);
				}
			}
		}
	}
	else if (getAniType() == aniFlag::throwing)
	{
		//�Ÿ��� ���� ���� �ǰ��ϴµ��� �ɸ��� �ð��� ��� ���� ��
		addTimer();

		char dx;
		char dy;

		switch (getDirection())
		{
		case 0: dx = 1; dy = 0; break;
		case 1: dx = 1; dy = -1; break;
		case 2: dx = 0; dy = -1; break;
		case 3: dx = -1; dy = -1; break;
		case 4: dx = -1; dy = 0; break;
		case 5: dx = -1; dy = 1; break;
		case 6: dx = 0; dy = 1; break;
		case 7: dx = 1; dy = 1; break;
		}

		Entity* address = (Entity*)World::ins()->getTile(atkTargetGridX, atkTargetGridY, atkTargetGridZ).EntityPtr;
		std::wstring stickerID = L"BASEATK" + std::to_wstring((unsigned __int64)this);

		if (shutdown == true)//������� ���� ��������
		{
			if (address != nullptr)
			{
				address->setFlashRGBA(0, 0, 0, 0);
			}
			aniUSet.erase(aniUSet.find(this));
			delete(((Sticker*)(StickerList.find(stickerID))->second));
			setFakeX(0);
			setFakeY(0);
			return true;
		}

		switch (getTimer())
		{
		case 2:
			setFakeX(getFakeX() + 3 * dx);
			setFakeY(getFakeY() + 3 * dy);
			break;
		case 3:
			setFakeX(getFakeX() + 2 * dx);
			setFakeY(getFakeY() + 2 * dy);
			break;
		case 4:
			setFakeX(getFakeX() + 1 * dx);
			setFakeY(getFakeY() + 1 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() + 2 * dx);
				address->setFakeY(address->getFakeY() + 2 * dy);
			}

			if (atkTargetPart == -1) { attack(atkTargetGridX, atkTargetGridY); }
			else { attack(atkTargetGridX, atkTargetGridY, atkTargetPart); }
			new Sticker(false, getX() + (16 * (atkTargetGridX - getGridX())), getY() + (16 * (atkTargetGridY - getGridY())), spr::effectCut1, 0, stickerID, true);
			break;
		case 5:
			setFakeX(getFakeX() - 1 * dx);
			setFakeY(getFakeY() - 1 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() - 1 * dx);
				address->setFakeY(address->getFakeY() - 1 * dy);
			}
			break;
		case 6:
			setFakeX(getFakeX() - 2 * dx);
			setFakeY(getFakeY() - 2 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() - 1 * dx);
				address->setFakeY(address->getFakeY() - 1 * dy);
			}
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(1);
			break;
		case 7:
			setFakeX(getFakeX() - 3 * dx);
			setFakeY(getFakeY() - 3 * dy);
			break;
		case 8:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(2);
			break;
		case 10:
			delete(((Sticker*)(StickerList.find(stickerID))->second));
		case 20:
			setFakeX(0);
			setFakeY(0);
			resetTimer();
			setAniType(aniFlag::null);
			if (isPlayer == true) { turnWait(endAtk()); }
			else { endAtk(); }
			return true;
		}

		if (getTimer() >= 5)
		{
			Entity* address = (Entity*)World::ins()->getTile(atkTargetGridX, atkTargetGridY, atkTargetGridZ).EntityPtr;
			if (address != nullptr)
			{
				Uint8 targetR, targetG, targetB, targetAlpha;
				address->getFlashRGBA(targetR, targetG, targetB, targetAlpha);
				if (address->getFlashType() == 1)
				{
					if (targetG > 51) { targetG -= 51; }
					else { targetG = 0; }
					if (targetB > 51) { targetB -= 51; }
					else { targetB = 0; }
					if (targetAlpha > 51) { targetAlpha -= 51; }
					else { targetAlpha = 0; }
					address->setFlashRGBA(targetR, targetG, targetB, targetAlpha);
				}
			}
		}
	}
	else if (getAniType() == aniFlag::shotSingle)
	{
		//�Ÿ��� ���� ���� �ǰ��ϴµ��� �ɸ��� �ð��� ��� ���� ��
		addTimer();

		char dx;
		char dy;

		switch (getDirection())
		{
		case 0: dx = 1; dy = 0; break;
		case 1: dx = 1; dy = -1; break;
		case 2: dx = 0; dy = -1; break;
		case 3: dx = -1; dy = -1; break;
		case 4: dx = -1; dy = 0; break;
		case 5: dx = -1; dy = 1; break;
		case 6: dx = 0; dy = 1; break;
		case 7: dx = 1; dy = 1; break;
		}

		Entity* address = (Entity*)World::ins()->getTile(atkTargetGridX, atkTargetGridY, atkTargetGridZ).EntityPtr;
		std::wstring stickerID = L"BASEATK" + std::to_wstring((unsigned __int64)this);

		if (shutdown == true)//������� ���� ��������
		{
			if (address != nullptr)
			{
				address->setFlashRGBA(0, 0, 0, 0);
			}
			aniUSet.erase(aniUSet.find(this));
			delete(((Sticker*)(StickerList.find(stickerID))->second));
			setFakeX(0);
			setFakeY(0);
			return true;
		}

		switch (getTimer())
		{
		case 2:
			setFakeX(getFakeX() - 3 * dx);
			setFakeY(getFakeY() - 3 * dy);
			break;
		case 3:
			setFakeX(getFakeX() - 2 * dx);
			setFakeY(getFakeY() - 2 * dy);
			break;
		case 4:
			setFakeX(getFakeX() - 1 * dx);
			setFakeY(getFakeY() - 1 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() + 2 * dx);
				address->setFakeY(address->getFakeY() + 2 * dy);
			}

			if (atkTargetPart == -1)
			{
				//���߷� ��� �����
				attack(atkTargetGridX, atkTargetGridY);
			}
			else
			{
				attack(atkTargetGridX, atkTargetGridY, atkTargetPart);
			}
			new Sticker(false, getX() + (16 * (atkTargetGridX - getGridX())), getY() + (16 * (atkTargetGridY - getGridY())), spr::effectCut1, 0, stickerID, true);
			break;
		case 5:
			setFakeX(getFakeX() + 1 * dx);
			setFakeY(getFakeY() + 1 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() - 1 * dx);
				address->setFakeY(address->getFakeY() - 1 * dy);
			}
			break;
		case 6:
			setFakeX(getFakeX() + 2 * dx);
			setFakeY(getFakeY() + 2 * dy);
			if (address != nullptr)
			{
				address->setFakeX(address->getFakeX() - 1 * dx);
				address->setFakeY(address->getFakeY() - 1 * dy);
			}
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(1);
			break;
		case 7:
			setFakeX(getFakeX() + 3 * dx);
			setFakeY(getFakeY() + 3 * dy);
			break;
		case 8:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(2);
			break;
		case 10:
			delete(((Sticker*)(StickerList.find(stickerID))->second));
		case 20:
			setFakeX(0);
			setFakeY(0);
			resetTimer();
			setAniType(aniFlag::null);
			if (isPlayer == true) { turnWait(endAtk()); }
			else { endAtk(); }
			return true;
		}

		if (getTimer() >= 5)
		{
			Entity* address = (Entity*)World::ins()->getTile(atkTargetGridX, atkTargetGridY, atkTargetGridZ).EntityPtr;
			if (address != nullptr)
			{
				Uint8 targetR, targetG, targetB, targetAlpha;
				address->getFlashRGBA(targetR, targetG, targetB, targetAlpha);
				if (address->getFlashType() == 1)
				{
					if (targetG > 51) { targetG -= 51; }
					else { targetG = 0; }
					if (targetB > 51) { targetB -= 51; }
					else { targetB = 0; }
					if (targetAlpha > 51) { targetAlpha -= 51; }
					else { targetAlpha = 0; }
					address->setFlashRGBA(targetR, targetG, targetB, targetAlpha);
				}
			}
		}
	}
	else if (getAniType() == aniFlag::fireStorm)
	{
		addTimer();

		std::wstring stickerID = L"FIRESTORM";

		switch (getTimer())
		{
		case 1:
			new Sticker(false, getX() + (16 * (getSkillTarget().x - getGridX())), getY() + (16 * (getSkillTarget().y - getGridY())), spr::fireStorm, 0, stickerID, true);
			new Flame(getSkillTarget().x, getSkillTarget().y, getGridZ(), flameFlag::BIG);


			break;
		case 5:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(1);

			new Flame(getSkillTarget().x + 1, getSkillTarget().y, getGridZ(), flameFlag::NORMAL);
			new Flame(getSkillTarget().x - 1, getSkillTarget().y, getGridZ(), flameFlag::NORMAL);
			new Flame(getSkillTarget().x, getSkillTarget().y + 1, getGridZ(), flameFlag::NORMAL);
			new Flame(getSkillTarget().x, getSkillTarget().y - 1, getGridZ(), flameFlag::NORMAL);


			break;
		case 9:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(2);

			new Flame(getSkillTarget().x, getSkillTarget().y - 2, getGridZ(), flameFlag::SMALL);
			new Flame(getSkillTarget().x - 1, getSkillTarget().y - 1, getGridZ(), flameFlag::SMALL);
			new Flame(getSkillTarget().x - 2, getSkillTarget().y, getGridZ(), flameFlag::SMALL);
			new Flame(getSkillTarget().x - 1, getSkillTarget().y + 1, getGridZ(), flameFlag::SMALL);
			new Flame(getSkillTarget().x, getSkillTarget().y + 2, getGridZ(), flameFlag::SMALL);
			new Flame(getSkillTarget().x + 1, getSkillTarget().y + 1, getGridZ(), flameFlag::SMALL);
			new Flame(getSkillTarget().x + 2, getSkillTarget().y, getGridZ(), flameFlag::SMALL);
			new Flame(getSkillTarget().x + 1, getSkillTarget().y - 1, getGridZ(), flameFlag::SMALL);
			break;
		case 13:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(3);
			break;
		case 17:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(4);
			break;
		case 21:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(5);
			break;
		case 25:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(6);
			break;
		case 29:
			((Sticker*)(StickerList.find(stickerID))->second)->setSpriteIndex(7);
			break;
		case 33:
			delete(((Sticker*)(StickerList.find(stickerID))->second));
			resetTimer();
			setAniType(aniFlag::null);
			return true;
			break;
		}
	}

	return false;
}