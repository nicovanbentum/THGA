#include "pch.h"
#include "mainMenu.hpp"
#include "newGameMenu.hpp"

mainMenu::mainMenu(Character &player)

{
	if (!font.loadFromFile("fonts/stranger.ttf"))
	{
		std::cout << "error loading font" << '\n';
	}

	menuChoices.emplace_back().setString("New Game");
	menuChoices.emplace_back().setString("Continue Game");
	menuChoices.emplace_back().setString("Load Game");
	menuChoices.emplace_back().setString("Options");
	menuChoices.emplace_back().setString("Quit Game");

	int pos = 0;

	for (auto& choice : menuChoices) {
		choice.setFont(font);
		choice.setCharacterSize(160);
		choice.setFillColor(sf::Color::White);
		choice.setPosition(sf::Vector2f((player.getPosition().x) - 200, player.getPosition().y + 160 * (pos - 2) - 120));

		pos++;
	}

	menuChoices[0].setFillColor(sf::Color::Red);

	selectedItem = 0;
	setMain();
}

void mainMenu::draw(sf::RenderWindow & window, sf::View & main_camera, sf::Sprite & bgMain, Character & player)
{
	main_camera.setSize(1920, 1080);
	sf::Vector2f playerposforbg = player.getPosition();
	bgMain.setPosition(sf::Vector2f(playerposforbg.x - 960, playerposforbg.y - 540));
	window.draw(bgMain);

	for (auto& choice : menuChoices) {
		window.draw(choice);
	}

	window.setView(main_camera);
}

void mainMenu::moveUp()
{
	menuChoices[selectedItem].setFillColor(sf::Color::White);
	if (selectedItem - 1 >= 0)
	{
		selectedItem--;
	}
	else
	{
		selectedItem = menuChoices.size() - 1 ;
	}
	menuChoices[selectedItem].setFillColor(sf::Color::Red);
}
void mainMenu::moveDown()
{
	menuChoices[selectedItem].setFillColor(sf::Color::White);
	if (selectedItem + 1 < menuChoices.size())
	{
		
		selectedItem++;
		
	}
	else 
	{
		selectedItem = 0;
	}
	menuChoices[selectedItem].setFillColor(sf::Color::Red);
}

int mainMenu::chooseTile(std::shared_ptr<Menu> &currentMenu, Character & player, sf::Window & window, AnimationManager & ani)
{
	if (selectedItem == 0) {
		currentMenu = std::make_shared<newGameMenu>(player);
		setNewGame();
	}
	else if (selectedItem == 1) {
		if (player.role == std::string("")) {
			currentMenu = std::make_shared<newGameMenu>(player);
			setNewGame();
		}
		else {
			setInGame();
			return 2;
		}
	}
	else if (selectedItem == 2) {
	}
	else if (selectedItem == 3) {
	}
	else if (selectedItem == 4) {
		window.close();
	}
	return 0;
}