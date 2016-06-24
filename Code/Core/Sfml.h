#ifndef SFML_H_
#define SFML_H_

#include "Includes.h"

class Sfml
{
		public:
			Sfml();
			Sfml::Sfml(int x, int y, std::string name);
			~Sfml();
			//------------
			std::shared_ptr<sf::RenderWindow> getWindow();
			std::shared_ptr<sf::Event> getEvent();

			int getWidth();
			int getHeight();
			std::string getName();
			//------------
			void setWidth(int x);
			void setHeight(int y);
			void setName(std::string name);

		private:
			std::shared_ptr<sf::RenderWindow> mWindow;
			std::shared_ptr<sf::Event> mEvent;

			int mWidth;
			int mHeight;
			std::string mName;
};

#endif // SFML_H_
