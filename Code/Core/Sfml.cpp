#include "Sfml.h"

Sfml::Sfml() : mWindow(0), mEvent(new sf::Event()), mWidth(1200), mHeight(700), mName("Ogre3D v1.9 and SFML v2.1")
{
	mWindow = new sf::RenderWindow(sf::VideoMode(mWidth, mHeight), mName, sf::Style::Default, sf::ContextSettings(32));
	mWindow->setFramerateLimit(GeneralConstants::nbFrames);
}
Sfml::Sfml(int x, int y, std::string name) : mWindow(0), mEvent(new sf::Event), mWidth(1200), mHeight(700), mName("Ogre3D v1.9 and SFML v2.1")
{
	mWindow = new sf::RenderWindow(sf::VideoMode(mWidth, mHeight), mName, sf::Style::Default, sf::ContextSettings(32));
	setWidth(x);
	setHeight(y);
	setName(name);
}
Sfml::~Sfml()
{
	delete mWindow;
	delete mEvent;
}

// -------------------------------------------

sf::RenderWindow* Sfml::getWindow()
{
	return mWindow;
}
sf::Event* Sfml::getEvent()
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