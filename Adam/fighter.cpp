#include "pch.h"
#include "fighter.h"



fighter::fighter(statistic health_c, int lvl_c)	
{
	health = health_c;
	lvl = lvl_c;
	Collision::CreateTextureAndBitmask(AABB_H, "assets/AABB_H.png");
}

bool fighter::fight(fighter * opponent, Audio & sound) {

	if (Collision::PixelPerfectTest(getHitbox(), opponent->getBox())) {
		updateFollowPosition(0);
		opponent->take_damage(20+lvl*lvl);
		opponent->update_info();
		return true;
	}
	return false;

}


void fighter::take_damage(int amount)
{
	if (!checkDead()) 
	{
		health.current = health.current - amount;
	}
	else
	{
		health.zero();
	}

	
}

bool fighter::checkDead() {
	return health.is_zero();
}
