#include "Includes.h"

using namespace std;

int main()
{
	//----------------- Setup

	Physics motPhys;
	Graphics motGraph;
	Sfml bibSfml;

	motGraph.setup(reinterpret_cast<unsigned long>(bibSfml.getWindow()->getSystemHandle()));
	motGraph.createScene();

	Camera MaCamera(45.0f);

	//----------------- Go

	// run the main loop
	bool running = true;
	while (running)
	{
		//Ogre::WindowEventUtilities::messagePump();

		// handle events
		while (bibSfml.getWindow()->pollEvent(*bibSfml.getEvent()))
		{
			if (bibSfml.getEvent()->type == sf::Event::Closed)
			{
				// end the program
				running = false;
			}
			else if (bibSfml.getEvent()->type == sf::Event::MouseWheelScrolled)
			{
				if (bibSfml.getEvent()->mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
					MaCamera.zoomCamera(-(bibSfml.getEvent()->mouseWheelScroll.delta));
			}
		}

		try
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
					MaCamera.moveCamera(Vecteur(0, 0, -1));
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					MaCamera.moveCamera(Vecteur(0, 0, 1));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
					MaCamera.moveCamera(Vecteur(-1, 0, 0));
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					MaCamera.moveCamera(Vecteur(1, 0, 0));

			MaCamera.releaseZoom();
		}
		catch (exception const& exc)
		{
			cout << "! ERREUR : " << exc.what() << endl;
		}
		
		motGraph.setCameraPosition(MaCamera.getPosition().x, MaCamera.getPosition().y, MaCamera.getPosition().z);
		motGraph.setCameraLookAt(MaCamera.getFocus().x, MaCamera.getFocus().y, MaCamera.getFocus().z);

		// clear the buffers
		bibSfml.getWindow()->clear();

		// draw...
		motGraph.getRoot()->renderOneFrame();

		// end the current frame (internally swaps the front and back buffers)
		bibSfml.getWindow()->display();
	}

	return 0;
}
