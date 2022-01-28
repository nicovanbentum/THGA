#include "pch.h"
#include "physics.h"

namespace Adam
{
	physics::physics(movable* player, collision_handler & clh_in)
	{
		moveables.push_back(player);
		clh = &clh_in;
	}

	void physics::step_x_moveables(float dt)
	{
		for (auto & moveable : moveables) {
			moveable->move(sf::Vector2f(moveable->getVelocity().x * dt, 0));
		}

		for (auto & moveable : moveables) {
			if (!moveable->bCanCollide) continue;

			//handle collision for every moveable with the world
			clh->handle_collision(clh->collides_with_world(moveable), 
			[&]() 
			{
				moveable->move(sf::Vector2f(-moveable->getVelocity().x * dt, 0));
				moveable->setVelocity(sf::Vector2f(0, moveable->getVelocity().y));
			});

			//handle collision for current moveable with every other moveable
			for (auto & other_moveable : moveables) {
				if (moveable == other_moveable)			continue;
				if (!other_moveable->collide_others)	continue;

				clh->handle_collision(clh->collides_with_sprite(moveable->getBox(), other_moveable->getBox()),
				[&]() {
					moveable->move(sf::Vector2f(-moveable->getVelocity().x * dt, 0));
					moveable->setVelocity(sf::Vector2f(0, moveable->getVelocity().y));
				});
			}
		}
	}

	void physics::step_y_moveables(float dt)
	{
		for (auto & moveable : moveables) {
			moveable->move(sf::Vector2f(0, moveable->getVelocity().y * dt));
		}

		for (auto & moveable : moveables) {
			if (!moveable->bCanCollide) continue;

			if (clh->handle_collision(clh->collides_with_world(moveable) && moveable->getVelocity().y > 0,
				[&]() {
					while (clh->collides_with_world(moveable)) {
						if (!moveable->canJump) { 
							moveable->canJump = true; 
							moveable->jumpCount = 0; 
						}
						moveable->move(sf::Vector2f(0, -moveable->getVelocity().y  * dt));
					}
					moveable->setVelocity(sf::Vector2f(moveable->getVelocity().x, 0));
			})) continue;

			if (clh->handle_collision(clh->collides_with_world(moveable) && moveable->getVelocity().y < 0,
				[&]() {
					while (clh->collides_with_world(moveable))
					{
						moveable->move(sf::Vector2f(0, -moveable->getVelocity().y  * dt));
					}
					moveable->setVelocity(sf::Vector2f(moveable->getVelocity().x, 0));
			})) continue;

			moveable->setVelocity(moveable->getVelocity() + (gravity * dt));



		}
	}
}