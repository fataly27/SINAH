#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Includes.h"

#include <memory>

class Graphics
{
	public:
		Graphics();
		~Graphics();

		void setup(unsigned long systemHandle);
		void createTerrain(std::shared_ptr<std::array<std::array<std::shared_ptr<Terrain>, 5>, 5>>& map);
		void createScene();

		void defineTerrain(long x, long y, std::string& path);
		void initBlendMaps(std::shared_ptr<Ogre::Terrain> terrain);
		void configureTerrainDefaults(std::shared_ptr<Ogre::Light> light);
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
