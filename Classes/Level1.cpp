/*
	Level1.cpp

	Joe O'Regan
	K00203642
	02/02/2018

	The first level of the game
	Inherits from Level base class

	20180202	Added Audio class with singleton access
*/

#include "Level1.h"
//#include "Achievement.h"

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "PluginSdkboxPlay/PluginSdkboxPlay.h"							// For leaderboard and achievements
//#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"					// 20180307 Google Analytics
//#endif

Scene* Level1::createScene() {
	cocos2d::Scene* scene = Scene::create();								// 'scene' is an autorelease object, JOR replaced auto specifier   
	s_pLayerInstance = Level1::create();									// 'layer' is an autorelease object, JOR replaced auto specifier  
	//Level1->setName("Level1");											// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(s_pLayerInstance);										// Add layer as a child to scene	    
    return scene;															// Return the scene
}

/* 
	Initialise level 1 making additions to level base class
*/
bool Level1::init() {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	sdkbox::PluginGoogleAnalytics::logEvent("Achievement", 
//		"Unlocked", "Level 1 Started", 5);									// Google Analytics
//#endif

	//Game::Instance()->setLevel(1);										// For parallax node init

	Level::init();															// 20180221 Added Level base class

	m_pHUD->setLevelLabel();												// Update HUD Level text display

	if ( !Layer::init() ) { return false; }									// super init first
		
	Game::Instance()->resetAsteroidKills();									// Reset the number of asteroids destroyed
	Game::Instance()->resetEnemyShipKIlls();								// Reset the number of enemy ships destroyed
		
	// ParallaxNode
	this->addChild(m_backgroundNode, -1);									// Add the parallax background
	m_backgroundNode->init();												// ParallaxNodeExtras.SpaceQuest: Initialise the parallax scrolling background
	
	this->scheduleUpdate();
	
    return true;
}

/*
	Initialise the enemy ships (Different each level)
*/
void Level1::initEnemyShips() {
	for (int i = 0; i < NUM_ENEMIES; ++i) {									// 3 of enemy ship type 1
		EnemyShip* enemyShip1 = EnemyShip::create(m_visibleSize);
		this->addChild(enemyShip1);
		m_enemyShipList->pushBack(enemyShip1);
	}
}

/*
	Update Level 1
*/
void Level1::update(float dt) {
	Level::update(dt);														// Call base class update function	
}

/*
	Check collisions
*/
void Level1::checkCollisions() {
	Level::checkCollisions();												// Call base class function
	// Check collisions with different objects in different levels
}

/*
	End the level
*/
void Level1::endScene(EndReason endReason) {
	/*
		If the player has successfully completed the level
		Unlock the level complete achievement
	*/
	/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (endReason == KENDREASONWIN) {
		sdkbox::PluginSdkboxPlay::unlockAchievement("Level 1 Complete");	// Achievement
	}
		sdkbox::PluginGoogleAnalytics::logEvent("Achievement", 
			"Unlocked", "Level 1 Finished", 5);								// Google Analytics
#endif
*/
	//endLevelAchievement(endReason);											// Generic complete level achievements for console and analytics

	//if (endReason == KENDREASONWIN) Achievement::Instance()->endLevelAchievement();
	//else if (endReason == KENDREASONLOSE) Achievement::Instance()->updateLoserBoard();

	Level::endScene(endReason);												// End the scene
}