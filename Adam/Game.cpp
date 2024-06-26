#include "pch.h"
#include "Game.hpp"
#include "Menu.hpp"
#include "npc.hpp"
#include "Audio.h"

typedef Animateable::states state;

Game::Game(sf::RenderWindow &w, Character &player, HUD &hud, AnimationManager & ani, Audio & geluidje) :

	window(w),
	player(player),
	hud(hud),
	ani(ani),
	geluidje(geluidje),
	lvl(ani),
	cln_h(lvl.getLevel()->getLayer("foreground")),
	world_physics(&player, cln_h)
{
	damage_texture.loadFromFile("assets/damage.png");
	damage_overlay.setTexture(damage_texture);
	auto c = damage_overlay.getColor();
	damage_overlay.setColor(sf::Color(c.r, c.g, c.b, 0));

	Collision::CreateTextureAndBitmask(menuTex, "assets/backgrounds/forest.png");
	bgMain = sf::Sprite(menuTex);

	lvl.getLevel()->setCharacterSpawn(player);
	enemies = lvl.getLevel()->getEnemies();
	npcs = lvl.getLevel()->getNPCs();
	main_camera.setCenter(player.getPosition());

	bgMain.setTexture(menuTex);
	sf::Vector2f playerposforbg = player.getPosition();
	bgMain.setPosition(sf::Vector2f(playerposforbg.x - 960, playerposforbg.y - 540));

	mouse_texture.loadFromFile("assets/cursor.png");
	cursor.setTexture(mouse_texture);
	cursor.setScale(sf::Vector2f(0.03f,0.03f));

	currentMenu = std::make_shared<mainMenu>(player);

	ai = std::make_shared<AI>();

	for (auto & enemy : enemies) {
		enemy.collide_others = false;
		world_physics.moveables.push_back(&enemy);
	}
	for (auto & np : npcs) {
		np.collide_others = false;
		world_physics.moveables.push_back(&np);
	}

	geluidje.playMusic("audio/music1.wav", 35);

	state = STATE::MENU;

	for (const auto& projectile : player.projectiles) {
		world_physics.moveables.push_back(projectile.get());
	}
}




void Game::handleInput()
{
	//do game stuff
	switch (state)
	{
	case STATE::MENU:
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			ImGui::SFML::ProcessEvent(window, ev);

			if (ev.type == sf::Event::Closed)
			{
				geluidje.playSound("gameOver", 40);
				window.close();
			}
			switch (ev.type)
			{
			case sf::Event::KeyPressed:
			{
				switch (ev.key.code)
				{
				case sf::Keyboard::Up:
				{
					geluidje.playSoundTwo("menuMove", 40);
					currentMenu->moveUp();
					break;
				}
				case sf::Keyboard::Down:
				{
					geluidje.playSoundTwo("menuMove", 40);
					currentMenu->moveDown();
					break;
				}
				case sf::Keyboard::Enter:
				{
					geluidje.playSoundTwo("menuEnter", 40);
					int menuResult = currentMenu->chooseTile(currentMenu, player, window, ani);
					// if 0, do nothing
					if (menuResult == 1) {
						state = STATE::MENU;
					}
					else if (menuResult == 2 && currentMenu->menu_states == Menu::menu_states::INGAME) {
						main_camera.setCenter(player.getPosition());
						main_camera.setSize(560, 315);
						state = STATE::PLAYING;
					}
					else if (menuResult == 2 && currentMenu->menu_states == Menu::menu_states::MAIN) {
						lvl.make_lvl("lvl0");
						lvl.getLevel()->setCharacterSpawn(player);
						player.respawn();
						cln_h.collision_layer = &lvl.getLevel()->getLayer("foreground");
						world_physics.clh = &cln_h;
						enemies = lvl.getLevel()->getEnemies();
						npcs = lvl.getLevel()->getNPCs();

						world_physics.moveables.clear();
						world_physics.moveables.push_back(&player);
						for (auto & enemy : enemies) {
							enemy.collide_others = false;
							world_physics.moveables.push_back(&enemy);
						}
						for (auto & np : npcs) {
							np.collide_others = false;
							world_physics.moveables.push_back(&np);
						}

						for (const auto& projectile : player.projectiles) {
							world_physics.moveables.push_back(projectile.get());
						}

						main_camera.setCenter(player.getPosition());
						main_camera.setSize(560, 315);
						state = STATE::PLAYING;
						currentMenu->setInGame();
					}
					else if (menuResult == 3) {
						state = STATE::GAMEOVER;
					}
					break;
				}
				case sf::Keyboard::BackSpace:
				{
					geluidje.playSoundTwo("menuReturn", 40);
					currentMenu = std::make_shared<mainMenu>(player);
					break;
				}
				}
			}
			}
		}
	}

	case STATE::PLAYING:
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			ImGui::SFML::ProcessEvent(window, ev);

			switch (ev.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}

			}

			if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Space)
			{
				player.canJump = false;
				if (!player.checkDead() && player.jumpCount < 2)
				{
					geluidje.playSoundTwo("jump", 40);
					player.setVelocity(sf::Vector2f(player.getVelocity().x, -player.jumpHeight));
					player.jumpCount++;
				}
			}


			else if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Button::Left && !player.checkDead())
			{
				//set the slashing state
				player.setVelocity(sf::Vector2f(0, player.getVelocity().y));
				if (player.state != state::SLASHING) {
					player.state = state::SLASHING;

					for (auto & enemy : enemies) {
						if (player.fight(&enemy, geluidje))
						{
							geluidje.playSoundTwo("Sword", 40);
							geluidje.playSound("maleAttack", 40.0);

							if (player.getPosition().x < enemy.getPosition().x)
							{
								enemy.setVelocity(sf::Vector2f(50, -100));
							}
							else
							{
								enemy.setVelocity(sf::Vector2f(-50, -100));
							}

							if (enemy.checkDead()) {
								enemy.die();
								geluidje.playSound("monsterDeath", 40);
								player.health.add(15);
								player.mana.add(50);
							}
						}
					}
				}

			}
		}

		//release the slashing state if slashing animation is done
		if (player.state == state::SLASHING && player.currentAnimationIsDone())
		{
			player.state = state::IDLE;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			state = STATE::MENU;
			currentMenu = std::make_shared<inGameMenu>(player);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !player.checkDead())
		{
			//we only allow input if the player is not currently slashing
			if (player.state != state::SLASHING && player.state != state::DEAD)
			{
				player.state = state::WALKING;
				player.setScale(sf::Vector2f(0.2, 0.2));
				player.current_direction = movable::direction::RIGHT;
				player.setVelocity(sf::Vector2f(125, player.getVelocity().y));
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !player.checkDead())
		{
			//we only allow input if the player is not currently slashing or dead
			if (player.state != state::SLASHING && player.state != state::DEAD)
			{
				player.state = state::WALKING;
				player.setScale(sf::Vector2f(-0.2, 0.2));
				player.current_direction = movable::direction::LEFT;
				player.setVelocity(sf::Vector2f(-125, player.getVelocity().y));
			}
		}
		else if (player.state != state::SLASHING && player.state != state::DEAD) //if the player is not slashing , not dead, and no button was pressed, the player is idle
		{
			player.state = state::IDLE;
			player.setVelocity(sf::Vector2f(0, player.getVelocity().y));
		}


		if (ev.type == sf::Event::KeyReleased && ev.key.code == sf::Keyboard::W && !player.checkDead()) 
		{
				for (auto & npc : npcs) {
					npc.updateText();;
					if (npc.getPosition().x - player.getPosition().x < 50 && npc.getPosition().x - player.getPosition().x >-50)
					{
						geluidje.playSound("npc", 40);
					}
				}
		}

		if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Button::Right && !player.checkDead()) {
				//set player state to slashing and lock x movement
				player.setVelocity(sf::Vector2f(0, player.getVelocity().y));
				if(player.state != state::SLASHING) player.state = state::SLASHING;

				//get mousePosition
				auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				//player pos stuff
				auto pPos = player.getPosition();
				pPos.x = pPos.x + (sf::Sprite(player).getGlobalBounds().width / 2);

				//flip player sprite to show which way its shooting
				if (mouse_pos.x < player.getPosition().x) {
					player.setScale(sf::Vector2f(-0.2, 0.2));
					player.current_direction = movable::direction::LEFT;
					pPos.y = pPos.y + (sf::Sprite(player).getGlobalBounds().height / 2);
				}
				else {
					player.setScale(sf::Vector2f(0.2, 0.2));
					player.current_direction = movable::direction::RIGHT;
				}

				//do math stuff to get mouse angle for projectile
				auto delta = mouse_pos - pPos;
				float angle_r = atan2(delta.y, delta.x);
				auto angle_degrees = angle_r * (180 / 3.14);
				auto delta_normalized = delta / float(sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
				sf::Vector2f shoot_vector(delta_normalized * 100.f);

				//shoot the projectile
				player.shootProjectile(pPos, shoot_vector, angle_degrees); 

				//play sound according to role chosen
				if (!player.mana.is_zero()) {
					if (player.role == "mage")
						geluidje.playSoundTwo("Fireball", 40);
					else
						geluidje.playSoundTwo("maleAttack", 40);
				}

		}


		switch (player.state)
		{
			case state::IDLE:
			{
				if (player.getCurrentAnimation() != std::string("IDLEright"))
				{
					player.setAnimation("IDLEright", Animation::intervals::idle);
					player.setTexture(player.currentAnimation->nextFrame());
				}
				break;
			}
			case state::SLASHING:
			{
				if (player.getCurrentAnimation() != std::string("SLASHINGright"))
				{
					player.setAnimation("SLASHINGright", Animation::intervals::attack);
					player.setTexture(player.currentAnimation->nextFrame());
				}
				break;
			}
			case state::WALKING:
			{
				if (player.getCurrentAnimation() != std::string("WALKright"))
				{
					player.setAnimation("WALKright", Animation::intervals::walk);
					player.setTexture(player.currentAnimation->nextFrame());
				}
				break;
			}
			case state::DEAD:
			{
				if (player.getCurrentAnimation() != std::string("DYINGright"))
				{
					player.setAnimation("DYINGright", Animation::intervals::dying);
					player.setTexture(player.currentAnimation->nextFrame());
				}
				break;
			}
		}
	}
	}
}
void Game::update(float dt) {
	ImGui::SFML::Update(window, sf::seconds(dt));

	auto& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2(20, 200), ImGuiCond_Once);
	ImGui::Begin("Settings", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::SliderFloat("Gravity", &world_physics.gravity.y, 0.0, 1000.0f, "%.2f");
	ImGui::SliderFloat("Jump Height", &player.jumpHeight, 0.0, 1000.0f, "%.2f");
	ImGui::SliderInt("Bounce Height", &lvl.getLevel()->bounce_velocity, 0.0, 1000);
	ImGui::End();

	switch (state)
	{
	case STATE::MENU:
	{
		break;
	}

	case STATE::PLAYING:
	{

		auto c = damage_overlay.getColor();
		if (damage_overlay.getColor().a > 0) damage_overlay.setColor(sf::Color(c.r, c.g, c.b, c.a - 1));

		for (auto & np : npcs) {
			ai->walkRandomly(&np);

			if (np.updateAnimation())
			{
				np.setTexture(np.currentAnimation->getCurrentFrame());
			}
		}

		if (player.updateAnimation() && player.canJump)
		{
			player.setTexture(player.currentAnimation->getCurrentFrame());
		}

		for (auto & enemy : enemies) {
			if (!enemy.checkDead()) {
				if (enemy.updateAnimation())
				{
					enemy.setTexture(enemy.currentAnimation->getCurrentFrame());
				}
			}
			else {
				enemy.setTexture(enemy.currentAnimation->getLastFrame());
			}
		}

		world_physics.step_x_moveables(dt);
		world_physics.step_y_moveables(dt);

		if (lvl.check_interaction(player, geluidje)) 
		{
			cln_h.collision_layer = &lvl.getLevel()->getLayer("foreground");
			world_physics.clh = &cln_h;
			enemies = lvl.getLevel()->getEnemies();
			npcs = lvl.getLevel()->getNPCs();
			world_physics.moveables.clear();
			world_physics.moveables.push_back(&player);
			for (auto & enemy : enemies) {
				enemy.collide_others = false;
				world_physics.moveables.push_back(&enemy);
			}
			for (auto & np : npcs) {
				np.collide_others = false;
				world_physics.moveables.push_back(&np);
			}

			for (const auto& projectile : player.projectiles) {
				world_physics.moveables.push_back(projectile.get());
			}
		}

		for (auto& prj : player.projectiles) {
			if (!prj->isDeath()) {

				// tick away projectile HP over time
				float totalTime = projectileClock.getElapsedTime().asSeconds();
				if (totalTime > 0.025f) {
					prj->updateLive(1);
					projectileClock.restart();
				}

				for (auto& enemie : enemies) {
					if (prj->fight(&enemie, geluidje)) {
						player.update_exp(20);
						player.mana.add(50);
						player.health.add(15);
						enemie.die();
						geluidje.playSound("monsterDeath", 40);
					}
				}
				if (Collision::PixelPerfectTest(lvl.getLevel()->getLayer("foreground"), *prj)) {
					prj->updateLive(50);
				}

				prj->updateAnimation();
				prj->setTexture(prj->currentAnimation->getCurrentFrame());
			}
			
		}

		for (auto & np : npcs) {
			np.showText(player);
		}

		hud.update();
			
		for (auto & enemy : enemies) {
			enemy.update_info_pos(window);
		}

		if (player.checkDead()) {
			player.state = state::DEAD;
			player.die();
			geluidje.playSoundTwo("death", 40);
			geluidje.playSound("revive", 40);
		}

		if (player.health.current < player.health.max)
		{
			if (healthClock.getElapsedTime().asSeconds() > 2.0)
			{
				if (player.role == "knight")
				{
					player.health.add(4);
					healthClock.restart();
				}
				else
				{
					player.health.add(2);
					healthClock.restart();
				}
			}
		}

		if (player.mana.current < player.mana.max)
		{
			if (manaClock.getElapsedTime().asSeconds() > 2.0)
			{
				if (player.role == "knight")
				{
					player.mana.add(2);
					manaClock.restart();
				}
				else
				{
					player.mana.add(4);
					manaClock.restart();
				}
				
			}
			
		}

		if (player.getPosition().y > 30000) {
			player.respawn();
			geluidje.playSound("revive", 88);
			player.setVelocity(sf::Vector2f(0, 0));
		}

		for (auto& enemy : enemies) {
			if (!enemy.checkDead()) {

				if (ai->shouldFollow_followDirection(&enemy, &player, geluidje))
				{
					geluidje.playSound("MaleHurtPain", 40);
					auto c = damage_overlay.getColor();
					damage_overlay.setColor(sf::Color(c.r, c.g, c.b, 100));
				}
			}
		}
	}
	break;

	}
}

void Game::render() {
	switch (state)
	{

	case STATE::MENU:
	{
		window.clear();
		currentMenu->draw(window, lvl, main_camera, bgMain, player);
		ImGui::SFML::Render(window);
		window.display();
		break;
	}


	case STATE::PLAYING:
	{

		if (rerender) {

			window.clear();
			auto level = lvl.getLevel();
			window.draw(level->getLayer("background"));
			
			for (auto & enemy : enemies)	enemy.draw(window);
			for (auto & npc : npcs)			npc.draw(window);
			
			player.draw(window);

			window.draw(level->getLayer("foreground"));
			window.draw(level->getLayer("foreground_dmg"));
			window.draw(level->getLayer("foreground_bounce"));

			for (auto & npc : npcs)			npc.drawDialogue(window);

			for (auto &prj : player.projectiles) {
				if (!prj->isDeath()) {
					prj->draw(window);
				}
				
				if (prj->isDeath() && !prj->currentAnimationIsDone()) {

					prj->setTexture(prj->currentAnimation->nextFrame());
					prj->draw(window);
				}
			}

			window.draw(level->getLayer("lvl_end"));

			auto mouse_pos = sf::Mouse::getPosition(window);
			auto mouse_pos_relative_to_view = window.mapPixelToCoords(mouse_pos);
			cursor.setPosition(mouse_pos_relative_to_view);
			window.draw(cursor);


			window.setView(main_HUD);
			hud.draw(window);

			if (damage_overlay.getColor().a > 0) window.draw(damage_overlay);

			auto center = Collision::GetSpriteCenter(player);
			center.y -= 50;
			main_camera.setCenter(center);
			window.setView(main_camera);
			ImGui::SFML::Render(window);
			window.display();

		}
		break;
	}
	}
	return;
}
