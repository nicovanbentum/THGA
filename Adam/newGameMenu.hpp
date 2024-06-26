#ifndef NEWGAMEMENU_HPP
#define NEWGAMEMENU_HPP
// ==========================================================================
//
// File      :newGameMenu.hpp
//
// ==========================================================================

// this file contains Doxygen lines
///file newGameMenu.hpp
/// \brief-a class that holds the functionality to use a menu ingame
/// \details
/// This class serves as way to choose make the menu that show what roles the player can choose to play the game with.
#include "menu.hpp"

#define newGameTiles 5

class newGameMenu : public Menu
{
public:
	/// \brief default constructor
	/// \detail
	/// default constructor that makes the menu when called upon ingame
	newGameMenu(Character & player);
	/// \brief draw function for menu
	/// \detail
	/// implementation to draw the menu when ingame
	void draw(sf::RenderWindow & window, sf::View & main_camera, sf::Sprite & bgMain, Character & player) override;
	/// \brief moving in menu options
	/// \detail
	/// a way to move through the menu tiles and make a selection
	void moveUp() override;
	/// \brief moving in menu options
	/// \detail
	/// a way to move through the menu tiles and make a selection
	void moveDown() override;
	/// \brief a way to choose the selected tile
	/// \detail
	/// function that chooses the selected tile, needs all parameters to choose the Role the player want 'mage or warrior'
	int chooseTile(std::shared_ptr<Menu> & currentMenu, Character & player, sf::Window & window, AnimationManager & ani) override;

private:
	/// \brief font of characters
	/// \detail
	/// font of characters of the menu
	sf::Font font;
	/// \brief amount of tiles
	/// \detail
	/// amount of tiles that the menu can show
	sf::Text menuChoices[newGameTiles];
};



#endif NEWGAMEMENU_HPP
