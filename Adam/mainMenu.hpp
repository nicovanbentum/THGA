#ifndef _MAINMENU_HPP
#define _MAINMENU_HPP
// ==========================================================================
//
// File      :mainMenu.hpp
//
// ==========================================================================

// this file contains Doxygen lines
///file mainMenu.hpp
/// \brief-a class that holds the functionality to use the main menu
/// \details
/// This class serves as way to enter a main menu when you start the game.
#include "menu.hpp"
// ==========================================================================
//
// File      :ingameMenu.hpp
//
// ==========================================================================

// this file contains Doxygen lines
///file ingameMenu.hpp
/// \brief-a class that holds the functionality to use a menu ingame
/// \details
/// This class serves as way to easily acces a menu ingame and give the player a way to pause the game and make the necessary changes.

class mainMenu : public Menu
{
private:
	/// \brief font of characters
	/// \detail
	/// font of characters of the menu
	sf::Font font;
	/// \brief amount of tiles
	/// \detail
	/// amount of tiles that the menu can show
	std::vector<sf::Text> menuChoices;
	
public:
	/// \brief default constructor
	/// \detail
	/// default constructor that makes the menu when the game starts
	mainMenu(Character &player);
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
	int chooseTile(std::shared_ptr<Menu> &currentMenu, Character & player, sf::Window & window, AnimationManager & ani) override;

};




#endif // !MENU_HPP
