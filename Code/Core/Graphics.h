#ifndef GRAPHICS_H_
#define GRAPHICS_H_

//Includes Ogre 1.9
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <string>
#include <memory>

#include "Terrain.h"

#include "Constants.h"

class Graphics
{
	public:
		Graphics();
		~Graphics();

		void setup(unsigned long systemHandle);
		void createTerrain(std::array<std::array<std::shared_ptr<Terrain>, 5>, 5>& map);
		void createScene();

		void defineTerrain(long x, long y, std::string& path);
		void initBlendMaps(Ogre::Terrain* terrain);
		void configureTerrainDefaults(Ogre::Light* light);
		void getTerrainImage(Ogre::Image& img, std::string& path);

		void setCameraPosition(float x, float y, float z);
		void setCameraLookAt(float x, float y, float z);

		std::shared_ptr<Ogre::Root> getRoot();

	private:
		std::shared_ptr<Ogre::Root>                 mRoot;
		std::shared_ptr<Ogre::Camera>               mCamera;
		std::shared_ptr<Ogre::SceneManager>         mSceneMgr;
		std::shared_ptr<Ogre::RenderWindow>         mWindow;

		std::shared_ptr<Ogre::TerrainGroup>         mTerrainGroup;
		std::shared_ptr<Ogre::TerrainGlobalOptions> mTerrainGlobals;
		bool                                        mTerrainsImported;

		Ogre::String                                mResourcePath;
		Ogre::String                                mResourcesCfg;
		Ogre::String                                mPluginsCfg;

};

#endif // GRAPHICS_H_
