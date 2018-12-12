#pragma once
//created by Game; has an Inventory, Characters
#include "Character.h"
#include "Inventory.h"
#include "Pickup.h"

class Player {
public:
	Player();
	void addCharacter(Character *c);
	Character *getFirstCharacter();
	Character *getCurrentCharacter();
	void addItem(Pickup item);
	void prevWeapon();
	void nextWeapon();
	void fireWeapon();
	void setControllable(bool c);
	void setNextCharacter();
	void adjustAim(float dTime);
	void setAimUp(bool b);
	void setAimDown(bool b);
	void clearInput();
	void moveLeft(bool v);
	void moveRight(bool v);
	void jump(bool v);
	void RemoveCharacter(int index);
	Inventory getWeapons();
	int playerID;
	std::vector<Character *> chars;
	float aim_angle;
private:
	Inventory *weaps;
	int currentCharacterIndex;
	bool aimUp, aimDown;
};