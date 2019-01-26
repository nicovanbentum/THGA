#include "pch.h"
#include "mainMenu.hpp"
#include "Collision.h"
#include "Character.h"
#include "Game.hpp"
#include "HUD.hpp"
#include "statistic.h"
#include "AnimationManager.h"
#include "Audio.hpp"

using namespace std;
using namespace sf;

//"trick" struct to declare objects without calling constructors
template<typename T>
struct deffered
{
	deffered() {}
	~deffered() { object.~T(); }
	template<typename...TArgs>
	void construct(TArgs&&...args)
	{
		new (&object) T(std::forward<TArgs>(args)...);
	}
public:
	union
	{
		T object;
	};
};

//renders and displays a rotating loading icon
void render_loading_screen(sf::RenderWindow & w, bool & loaded)
{
	sf::Sprite loader;
	sf::Texture loader_png;
	loader_png.loadFromFile("assets/loader.png");
	loader.setTexture(loader_png);
	loader.setOrigin(sf::Vector2f(loader_png.getSize().x / 2, loader_png.getSize().y / 2));
	loader.setPosition(sf::Vector2f(w.getSize().x / 2, w.getSize().y / 2));
	loader.setScale(sf::Vector2f(0.2, 0.2));

	int i = 0;
	while (!loaded)
	{
		w.clear();
		w.draw(loader);
		loader.rotate(10);
		w.display();
	}
}

int main()
{
	RenderWindow window(VideoMode(1920, 1080, 32), "Project: ADAM", sf::Style::None);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(false);

	//declare objects without constructing
	deffered<AnimationManager> ani;
	deffered<Audio> geluidje;
	deffered<Character> player;
	deffered<HUD> hud;
	deffered<Game> game;

	//create and launch a thread that calls constructors
	bool loaded = false;
	std::thread t([&]() 
	{
		ani.construct("assets/animations/animations.txt");
		geluidje.construct("audio/audio.txt");
		player.construct(sf::Vector2f(890, 690), sf::Vector2f(0.2, 0.2), ani.object.animations["mage"], sf::Vector2f(0, 0), statistic(200, 200), statistic(300, 300), statistic(80, 0));
		hud.construct(player.object);
		game.construct(window, player.object, hud.object, ani.object, geluidje.object);

		loaded = true;
	});

	//while loading render a loading icon/screen
	render_loading_screen(window, loaded);

	t.join(); //wait for objects to finish constructing before moving on

	
	while (window.isOpen())
	{
		if (window.hasFocus()) game.object.handleInput();
		game.object.update();
		game.object.render();

	}

	return 0;
}