#include "pch.h"
#include "Level.h"

void Level::draw(sf::RenderTarget & w)
{
	w.draw(sprites["background"].first);
	w.draw(sprites["foreground"].first);
	w.draw(sprites["foreground_dmg"].first);
	w.draw(sprites["foreground_bounce"].first);
	w.draw(sprites["lvl_end"].first);
	w.draw(sprites["infinity"].first);
}

std::future<void> Level::addSprite(const std::string & name, const std::string & location)
{
	std::cout << "Loading sprite: " << name << " at: " << location << '\n';
	//sprites[name].second.setSmooth(true);
	sprites.insert({ name, {sf::Sprite(), std::make_shared<sf::Texture>() } });
	
	return std::async(std::launch::async, ([name, location, this]() {
		Collision::CreateTextureAndBitmask(sprites[name].second, location);
		sprites[name].first.setTexture(*sprites[name].second);
	}));
}

void Level::enemy_factory(std::string s, AnimationManager & ani)
{
	std::ifstream lvls_file(s);
	if (lvls_file.is_open())
	{
		std::string png, health, posx, posy = "";

		while (lvls_file >> png >> posx >> posy >> health)
		{
			auto& enemy = enemies.emplace_back(sf::Vector2f(stoi(posx), stoi(posy)), sf::Vector2f(0.2, 0.2), ani.animations[png], sf::Vector2f(0, 0), statistic(std::stoi(health), std::stoi(health)));
			enemy.setAnimation("IDLEright", Animation::intervals::idle);
			enemy.setTexture(enemy.currentAnimation->getCurrentFrame());
		}

	}
}

bool getVectorString(std::ifstream & input, std::vector<std::string> & npc_text) {
	char c;
	std::string tempString = "";
	if (!(input >> c)) {
		return false;
	}
	if (c != '{') {
		std::cout << "-------------------------" << c << "\n";
		return false;
	}
	while (c != '}') {
		while ((input >> c) && (c != ',' && c != '}') ){
			if (c == '#') {
				tempString += " ";
			}
			else {
				tempString += c;
			}
		}
		std::cout << "--------------------------------------" << tempString << "\n";
		
		npc_text.push_back(tempString);
		tempString = "";
	}
	return true;
}



void Level::npc_factory(std::string s, AnimationManager & ani)
{
	std::ifstream lvls_file(s);
	if (lvls_file.is_open())
	{
		std::string png, posx, posy = "";
		std::vector<std::string> npc_text = {};
		while (lvls_file >> png >> posx >> posy && getVectorString(lvls_file, npc_text))
		{
			auto& npc = npcs.emplace_back(sf::Vector2f(stoi(posx), stoi(posy)), sf::Vector2f(0.2, 0.2), ani.animations[png], sf::Vector2f(0, 0), npc_text );
			npc.setAnimation("IDLEright", Animation::intervals::idle);
			npc.setTexture(npc.currentAnimation->getCurrentFrame());
			npc_text.clear();
		}

	}
}
sf::Sprite & Level::getLayer(const std::string & name)
{
	return sprites[name].first;
}
void Level::setCharacterSpawn(Character & player)
{
	player.set_spawn(player_spawn_point);
	player.respawn();
}

void Level::set_player_spawn_point(sf::Vector2f & spawn_point)
{
	player_spawn_point = spawn_point;
}

void Level::set_next_level(const std::string & next_level)
{
	next_level_name = next_level;
}

std::string Level::get_next_level()
{
	return next_level_name;
}

std::vector<npc> & Level::getNPCs()
{
	return npcs;
}

std::vector<Enemy> & Level::getEnemies()
{
	return enemies;
}



