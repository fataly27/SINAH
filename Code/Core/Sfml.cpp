#include "Sfml.h"

Sfml::Sfml() : Sfml(1200, 700, "Ogre3D v1.9 and SFML v2.1") {}
Sfml::Sfml(int x, int y, std::string name) : mEvent(new sf::Event), mWidth(1200), mHeight(700), mName("Ogre3D v1.9 and SFML v2.1"),
											 mWindow(new sf::RenderWindow(sf::VideoMode(mWidth, mHeight), mName, sf::Style::Default, sf::ContextSettings(32)))							 
{
	setWidth(x);
	setHeight(y);
	setName(name);
	mWindow->setFramerateLimit(GeneralConstants::nbFrames);
}
Sfml::~Sfml() {}

// -------------------------------------------

std::shared_ptr<sf::RenderWindow> Sfml::getWindow()
{
	return mWindow;
}
std::shared_ptr<sf::Event> Sfml::getEvent()
{
	return mEvent;
}

int Sfml::getWidth()
{
	return mWidth;
}
int Sfml::getHeight()
{
	return mHeight;
}
std::string Sfml::getName()
{
	return mName;
}
// -----------------------------------------------

void Sfml::setWidth(int x)
{
	if (x > 0 && x < 10000)
	{
		mWidth = x;
		mWindow->setSize(sf::Vector2u(mWidth, mHeight));
	}
}
void Sfml::setHeight(int y)
{
	if (y > 0 && y < 10000)
	{
		mHeight = y;
		mWindow->setSize(sf::Vector2u(mWidth, mHeight));
	}
}
void Sfml::setName(std::string name)
{
	mName = name;
	mWindow->setTitle(mName);
}
