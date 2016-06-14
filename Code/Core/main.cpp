#include "Physics.h"
#include "Graphics.h"
#include "Sfml.h"

using namespace std;

int main()
{
	//----------------- Setup

	Physics motPhys;
	Graphics motGraph;
	Sfml bibSfml;

	motGraph.setup(reinterpret_cast<unsigned long>(bibSfml.getWindow()->getSystemHandle()));

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
		}

		// clear the buffers
		bibSfml.getWindow()->clear();

		// draw...
		//mRoot->renderOneFrame();

		// end the current frame (internally swaps the front and back buffers)
		bibSfml.getWindow()->display();
	}

	return 0;
}

