#include "pch.h"
#include "Animated.hpp"

Animateable::Animateable(std::map<std::string, Animation> &animations):

	animations(&animations)
{
	currentAnimation = &animations.begin()->second;
}

Animation* Animateable::getAnimation(std::string animation) {
	return &(*animations)[animation];
}

bool Animateable::updateAnimation()
{
	if (timer.getElapsedTime().asMilliseconds() > animation_interval)
	{
		currentAnimation->nextFrame();
		timer.restart();
		return true;
	}

	return false;
}

std::string Animateable::getCurrentAnimation() {
	return currentAnimation->name;
}

void Animateable::setAnimation(std::string animation, int interval) {
	animation_interval = interval;
	currentAnimation = &(*animations)[animation];
	currentAnimation->reset_animation();
}

void Animateable::setAnimationMap(std::map<std::string, Animation> & newAnimations)
{
	animations = &newAnimations;
}
