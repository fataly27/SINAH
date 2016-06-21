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
	if(mOverlaySystem)
		delete mOverlaySystem;
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
		//mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

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

	// Create your scene here :)
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));

	Ogre::Vector3 lightdir(-0.5, -0.2, -5.0);
	lightdir.normalise();

	Ogre::Light* light = mSceneMgr->createLight("TestLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(0.7, 0.7, 0.7));

	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	configureTerrainDefaults(light);

	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);

	mTerrainGroup->loadAllTerrains(true);

	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup->freeTemporaryResources();
}
void Graphics::getTerrainImage(Ogre::Image& img)
{
	img.load("Landscapes/Heightmaps/first-try.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

void Graphics::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	bool exists = Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename);

	if (exists)
		mTerrainGroup->defineTerrain(x, y);
	else
	{
		Ogre::Image img;
		getTerrainImage(img);
		mTerrainGroup->defineTerrain(x, y, &img);

		mTerrainsImported = true;
	}
}

void Graphics::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::Real minHeight0 = 70;
	Ogre::Real fadeDist0 = 40;
	Ogre::Real minHeight1 = 70;
	Ogre::Real fadeDist1 = 15;

	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend0++ = val;

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}

	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}

void Graphics::configureTerrainDefaults(Ogre::Light* light)
{
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(6000);

	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
	importData.terrainSize = 513;
	importData.worldSize = 12000.0;
	importData.inputScale = 600;
	importData.minBatchSize = 33;
	importData.maxBatchSize = 65;
	importData.layerList.resize(3);
	importData.layerList[0].worldSize = 100;
	importData.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	importData.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	importData.layerList[1].worldSize = 30;
	importData.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	importData.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	importData.layerList[2].worldSize = 200;
	importData.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	importData.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

// -----------------------

Ogre::Root* Graphics::getRoot()
{
	return mRoot;
}

void Graphics::setCameraPosition(float x, float y, float z)
{
	mCamera->setPosition(Ogre::Vector3(x, y, z));
}
void Graphics::setCameraLookAt(float x, float y, float z)
{
	mCamera->lookAt(Ogre::Vector3(x, y, z));
}
