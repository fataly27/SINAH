#include "Graphics.h"

Graphics::Graphics() : mRoot(0), mCamera(0), mSceneMgr(0), mWindow(0), mOverlaySystem(0), mTerrainGroup(0), mTerrainGlobals(0), mTerrainsImported(false)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
	m_ResourcePath = "";
#endif

#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
	mResourcesCfg = m_ResourcePath + "resources_d.cfg";
	mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
	mResourcesCfg = m_ResourcePath + "resources.cfg";
	mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif
#endif
}
Graphics::~Graphics()
{
	if(mOverlaySystem) delete mOverlaySystem;
	delete mTerrainGroup;
	delete mTerrainGlobals;
	delete mRoot;
}

void Graphics::setup(unsigned long systemHandle)
{
	mRoot = new Ogre::Root(mPluginsCfg);

	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app.
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location.
			if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
				archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	mRoot->setRenderSystem(mRoot->getAvailableRenderers()[0]);
	mRoot->initialise(false);

	if (mRoot->showConfigDialog())
	{
		//mOgreWindow = mRoot->initialise(true, "TutorialApplication Render Window");

		Ogre::NameValuePairList misc;

#ifdef _WIN32
		unsigned long winGlContext = reinterpret_cast<unsigned long>(wglGetCurrentContext());

		misc["externalWindowHandle"] = Ogre::StringConverter::toString(systemHandle);
		misc["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
		misc["externalGLControl"] = Ogre::String("True");
#else
		misc["currentGLContext"] = String("True");
#endif
		// Create a render window | Note: Window Title and Size are not important here.
		mWindow = mRoot->createRenderWindow("Ogre Window", 800, 600, false, &misc);
		mWindow->setVisible(true);
	}

	// Get the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Graphics::createScene()
{
	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
	mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));
	mCamera->setNearClipDistance(0.1);

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		mCamera->setFarClipDistance(0);
	else
		mCamera->setFarClipDistance(50000);

	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}