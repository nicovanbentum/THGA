#include "pch.h"
#include "mainMenu.hpp"
#include "Collision.h"
#include "Character.h"
#include "Game.hpp"
#include "HUD.hpp"
#include "statistic.h"
#include "AnimationManager.h"
#include "Audio.h"

using namespace std;
using namespace sf;

//renders and displays a rotating loading icon
void render_loading_screen(sf::RenderWindow & window, bool & loaded)
{
	sf::Sprite loader;
	sf::Sprite background;
	sf::Texture loader_png;
	sf::Texture background_texture;
	loader_png.loadFromFile("assets/loader.png");
	background_texture.loadFromFile("assets/backgrounds/loadingScreen.png");
	loader_png.setSmooth(true);
	loader.setTexture(loader_png);
	background.setTexture(background_texture);
	loader.setOrigin(sf::Vector2f(loader_png.getSize().x / 2, loader_png.getSize().y / 2));
	loader.setPosition(sf::Vector2f(window.getSize().x - loader.getTextureRect().width * 0.2, window.getSize().y - loader.getTextureRect().height * 0.2));
	loader.setScale(sf::Vector2f(0.2, 0.2));

	sf::FloatRect spriteSize=background.getGlobalBounds();
	background.setOrigin(spriteSize.width/2.,spriteSize.height/2.);

	sf::Clock clock;

	while (!loaded)
	{
		sf::Event e;
		while (window.pollEvent(e)) {}
		window.clear();
		background.setPosition(window.getSize().x/2., window.getSize().y/2.);
		background.setScale(sf::Vector2f(1.33f, 1.33f));
		window.draw(background);
		window.draw(loader);
		loader.rotate(clock.restart().asSeconds() * 100);
		window.display();
	}
}

int main()
{
	RenderWindow window(VideoMode(1920, 1080, 32), "Project: ADAM", sf::Style::Titlebar | sf::Style::Close);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(true);
	window.setMouseCursorGrabbed(false);
	window.setFramerateLimit(0);

	ImGui::SFML::Init(window);

	//declare objects without constructing

	std::unique_ptr<AnimationManager> anim;
	std::unique_ptr<Audio> audio;
	std::unique_ptr<Character> player;
	std::unique_ptr<HUD> hud;
	std::unique_ptr<Game> game;

	//create and launch a thread that calls constructors
	bool loaded = false;
	std::thread t([&]() {
		anim = std::make_unique<AnimationManager>("assets/animations/animations.txt");
		audio = std::make_unique<Audio>("audio/audio.txt");
		player = std::make_unique<Character>(sf::Vector2f(890, 690), sf::Vector2f(0.2, 0.2), anim->animations["mage"], sf::Vector2f(0, 0), 5, statistic(200, 200), statistic(300, 300), statistic(80, 0));
		hud = std::make_unique<HUD>(*player);
		game = std::make_unique<Game>(window, *player, *hud, *anim, *audio);

		loaded = true;
	});

	//while loading render a loading icon/screen
	render_loading_screen(window, loaded);

	t.join(); //wait for objects to finish constructing before moving on

	sf::Clock clock;

	while (window.isOpen())
	{
		if (window.hasFocus()) {
			game->handleInput();
		} else {
			sf::Event ev;
			while (window.pollEvent(ev)) {}
		}

		game->update(clock.restart().asSeconds());
		game->render();
	}

	return 0;
}