#pragma once
enum ItemType{PotionOfTeleport, DrinkMe, EatMe, MagicFan, InvisiHat, Rose};
class Items
{
private:
	unsigned enhance_damage;
	bool can_teleport;
public:
	void use();
};

