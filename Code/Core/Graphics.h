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
#include <OgreOverlaySystem.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

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
		void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);

		void setCameraPosition(float x, float y, float z);
		void setCameraLookAt(float x, float y, float z);

		Ogre::Root* getRoot();

	private:
		Ogre::Root*                 mRoot;
		Ogre::Camera*               mCamera;
		Ogre::SceneManager*         mSceneMgr;
		Ogre::RenderWindow*         mWindow;
		Ogre::OverlaySystem*        mOverlaySystem;

		Ogre::TerrainGroup*			mTerrainGroup;
		Ogre::TerrainGlobalOptions* mTerrainGlobals;
		bool						mTerrainsImported;

		Ogre::String                m_ResourcePath;
		Ogre::String                mResourcesCfg;
		Ogre::String                mPluginsCfg;

};

#endif // GRAPHICS_H_