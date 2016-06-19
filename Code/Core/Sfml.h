#ifndef SFML_H_
#define SFML_H_

// SFML v2.1 Includes
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Constants.h"

class Sfml
{
		public:
			Sfml();
			Sfml::Sfml(int x, int y, std::string name);
			~Sfml();
			//------------
			sf::RenderWindow* getWindow();
			sf::Event* getEvent();

			int getWidth();
			int getHeight();
			std::string getName();
			//------------
			void setWidth(int x);
			void setHeight(int y);
			void setName(std::string name);

		private:
			sf::RenderWindow* mWindow;
			sf::Event* mEvent;

			int mWidth;
			int mHeight;
			std::string mName;
};

#endif // SFML_H_