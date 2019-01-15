#pragma once
#include "SFML/Graphics.hpp"
#include "Character.h"

namespace Adam
{
	class collision_handler
	{
	public:
		sf::Sprite & collision_layer;

		collision_handler(sf::Sprite & collision_layer);
		~collision_handler();

		template<typename LAMBDA>
		bool handle_collision(bool condition, LAMBDA f)
		{
			if (condition) f();
			return condition;
		}

		bool handle_world_collision(Character & object);
		bool handle_sprite_collision(sf::Sprite & object1, sf::Sprite & object2);

	};
}
