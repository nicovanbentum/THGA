#include "pch.h"
#include "shooter.hpp"

shooter::shooter()
{
}

shooter::shooter(int & size, std::map<std::string, Animation> & projectileAnimations):
	size(size),
	projectileAnimations(&projectileAnimations)
{
	for (int i = 0; i < size; i++) {
		projectiles.push_back(shoot(sf::Vector2f(1, 1), sf::Vector2f(1, 1), &projectileAnimations));
	}
}

void shooter::shootProjectile(sf::Vector2f pos, sf::Vector2f direction, float angle){
	for (auto &prj : projectiles) {
		if (prj->isDeath()) {
			prj->revive();
			prj->setPosition(pos);
			prj->setRotation(angle);
			prj->setVelocity(direction);
			return;
		}
	}
}

void shooter::setProjectile(std::function<std::shared_ptr<projectile>(sf::Vector2f, sf::Vector2f, std::map<std::string, Animation>*)> newS, std::map<std::string, Animation>& animations){
	shoot = newS;
	projectileAnimations = &animations;
	for (auto & prj: projectiles) {
		prj = shoot(sf::Vector2f(1,1), sf::Vector2f(1,1), projectileAnimations);
	}
}

void shooter::setVeloDeco(std::function<void(sf::Vector2f&)> NveloDeco){
	for (auto & prj: projectiles) {
		prj->setVeloDeco(NveloDeco);
	}
}

void shooter::setMovDeco(std::function<void(sf::Vector2f&)> NmovDeco){
	for (auto & prj : projectiles) {
		prj->setMovDeco(NmovDeco);
	}
}
