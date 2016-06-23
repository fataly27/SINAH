#ifndef GRAPHICS_H_
#define GRAPHICS_H_

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

#include <memory>

class Graphics
{
	public:
		Graphics();
		~Graphics();

		void setup(unsigned long systemHandle);
		void createScene();

		void defineTerrain(long x, long y);
		void initBlendMaps(Ogre::Terrain* terrain);
		void configureTerrainDefaults(Ogre::Light* light);
		void getTerrainImage(Ogre::Image& img);

		void setCameraPosition(float x, float y, float z);
		void setCameraLookAt(float x, float y, float z);

		shared_ptr<Ogre::Root> getRoot();

	private:
		shared_ptr<Ogre::Root>                 mRoot;
		shared_ptr<Ogre::Camera>               mCamera;
		shared_ptr<Ogre::SceneManager>         mSceneMgr;
		shared_ptr<Ogre::RenderWindow>         mWindow;

		shared_ptr<Ogre::TerrainGroup>		   mTerrainGroup;
		shared_ptr<Ogre::TerrainGlobalOptions> mTerrainGlobals;
		bool								   mTerrainsImported;

		Ogre::String            		       m_ResourcePath;
		Ogre::String           			       mResourcesCfg;
		Ogre::String          			       mPluginsCfg;

};

#endif // GRAPHICS_H_
