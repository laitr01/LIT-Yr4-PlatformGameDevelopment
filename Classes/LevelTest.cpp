/*
	Level1.cpp

	Joe O'Regan
	K00203642
	02/02/2018

	The first level of the game

	20180202	Added Audio class with singleton access
*/
#include "LevelTest.h"	// was level1
#include "Level2.h"
#include "MainMenu.h"
//#include "Audio.h"
#include "DPad.h"
#include <string>
#include <sstream>
//#include "HUD.h"
#include "Input.h"

//#define KNUMASTEROIDS 15						// Number of asteroids
//#define KNUMLASERS 5							// Number of lasers

// Because cocos2d-x requres createScene to be static, we need to make other non-pointer members static
//std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> Input::keys;

//DPad *controller;								// Add directional pad for mobile device
/*
Audio* Audio::s_pInstance;						// Singleton so only one instance of Audio exists in the game, for easy access
HUD* HUD::s_pInstance;							// Singleton for Heads Up Display
Input* Input::s_pInstance;						// Singleton for Input
GameScene* GameScene::s_pInstance;				// Game Singleton 
*/

Scene* LevelTest::createScene() {
	cocos2d::Scene* scene = Scene::create();	// 'scene' is an autorelease object, JOR replaced auto specifier   
	LevelTest* layer = LevelTest::create();			// 'layer' is an autorelease object, JOR replaced auto specifier  
	layer->setName("LevelTest");					// Set name for layer to access (//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();)
    scene->addChild(layer);						// Add layer as a child to scene	    
    return scene;								// Return the scene
}

// on "init" you need to initialize your instance
bool LevelTest::init() {
	Level::init();																						// 20180221 Added Level base class

	if ( !Layer::init() ) { return false; }																// super init first

	Game::Instance()->setLevel(1); // for parallax node init

	//_lives = 3;
	Game::Instance()->setLives(3);																		// JOR set the number of lives for the player

    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //Point origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	//cocos2d::MenuItemImage* closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
	//	CC_CALLBACK_1(Level1::menuCloseCallback, this));												// JOR replaced auto specifier

	//closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
	//	origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	//cocos2d::Menu* menu = Menu::create(closeItem, NULL);												// JOR replaced auto specifier
	//menu->setPosition(Point::ZERO);
	//this->addChild(menuClose, 1);
	
	//Director::getInstance()->getRunningScene()->getChildByName("Level1")->addChild();

	//  GALAXY
	//_batchNode = SpriteBatchNode::create("Sprites.pvr.ccz");
	//this->addChild(_batchNode);

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");
		
	// Player sprite
	//player = new Player(this);
	//_batchNode->addChild(player->getSprite(), 1);

	// 1) Create the ParallaxNode
	//_backgroundNode = ParallaxNodeExtras::create();
	this->addChild(_backgroundNode, -1);
	_backgroundNode->init();																								// ParallaxNodeExtras.cpp: Initialise the parallax scrolling background
	
	LevelTest::addChild(ParticleSystemQuad::create("Stars1.plist"));
	LevelTest::addChild(ParticleSystemQuad::create("Stars2.plist"));
	LevelTest::addChild(ParticleSystemQuad::create("Stars3.plist"));

	//_asteroids = new Vector<Sprite*>(KNUMASTEROIDS);																		// List of asteroids
	//for (int i = 0; i < KNUMASTEROIDS; ++i) {
	//	cocos2d::Sprite* asteroid = Sprite::createWithSpriteFrameName("asteroid.png");										// Asteroid sprite, JOR replaced auto specifier
	//	asteroid->setVisible(false);
	//	_batchNode->addChild(asteroid);
	//	_asteroids->pushBack(asteroid);
	//}

	// EnemyShip
	//EnemyShipList = new Vector<Sprite*>(3);																				// List of enemy ships
	for (int i = 0; i < 3; ++i) {
		//EnemyShip = Sprite::create("EnemyShip.png");
		//EnemyShip->setPosition(visibleSize.width, visibleSize.height / 2);
		//this->addChild(EnemyShip);
		cocos2d::Sprite* enemyShip1 = Sprite::create("EnemyShip.png");														// Asteroid sprite, JOR replaced auto specifier
		enemyShip1->setVisible(false);
		this->addChild(enemyShip1);
		EnemyShipList->pushBack(enemyShip1);
	}

	/*
	//_shipLasers = new Vector<Sprite*>(KNUMLASERS);																		// List of lasers
	for (int i = 0; i < KNUMLASERS; ++i) {
		cocos2d::Sprite* shipLaser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");								// Laser sprite, JOR replaced auto specifier
		shipLaser->setVisible(false);
		_batchNode->addChild(shipLaser);
		_shipLasers->pushBack(shipLaser);
	}
	*/
	//Device::setAccelerometerEnabled(true);																				// Enable accelerometer
	//cocos2d::EventListenerAcceleration* accelerationListener = 
	//EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::onAcceleration, this));									// JOR replaced auto specifier
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(accelerationListener, this);

	//cocos2d::EventListenerTouchAllAtOnce* touchListener = EventListenerTouchAllAtOnce::create();							// JOR replaced auto specifier
	touchListener->onTouchesBegan = CC_CALLBACK_2(LevelTest::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// Player Lives																											// Number of lives
	//for (int i = 0; i < _lives; i++) {
	for (unsigned int i = 0; i < MAX_LIVES; i++) {
		playerLife = Sprite::create("PlayerLife.png");
		playerLife->setPosition(visibleSize.width * 0.05 + (i * 52), visibleSize.height * 0.05);
		this->addChild(playerLife);
		livesList[i] = playerLife;																							// Add life sprite to list of lives
	}
	livesList[3]->setVisible(false);
	livesList[4]->setVisible(false);

	double curTime = getTimeTick();																							// Current game time
	//_gameOverTime = curTime + 30000;																						// Time to finish game
	_gameOverTime = curTime + LEVEL_TIME;																					// Time to finish game

	//currentTime = getTimeTick();																							// Current game time, for timer
	//currentTime = 0.0f;																										// Current game time, for timer, changed to float to solve Android timer issue

	//Audio::Instance()->init();																							// Initialise the game audio
	Game::Instance()->init();																								// Inite score and level
	HUD::Instance()->init(time, this);																						// Display score, level number, and time

	//time = LEVEL_TIME / 1000;
	Game::Instance()->setTimer(LEVEL_TIME/1000);

	Input::Instance()->init(this, this->_eventDispatcher);																	// Ship Movement

	__String *tempScore = __String::createWithFormat("Score: %i", Game::Instance()->getScore());
	__String *tempTime = __String::createWithFormat("Time: %i", time);
	/*
	// Score & Timer set size
	//timeLabel->setPosition(Point(visibleSize.width - timeLabel->getWidth() - 250, visibleSize.height * 0.95 + origin.y));
	if (visibleSize.height == 1080) {
		scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.075f);
		timeLabel = Label::createWithTTF(tempTime->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.075f);
		timeLabel->setPosition(Point(visibleSize.width - timeLabel->getWidth() - 150, visibleSize.height * 0.95 + origin.y));
	}
	else {
		scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.05f);
		timeLabel = Label::createWithTTF(tempTime->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.05f);
		timeLabel->setPosition(Point(visibleSize.width - timeLabel->getWidth() - 80, visibleSize.height * 0.95 + origin.y));
	}

	scoreLabel->setColor(Color3B::WHITE);
	scoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.95 + origin.y));
	*/
	this->addChild(scoreLabel, 10000);
	this->addChild(timeLabel);
	
	
	// D-pad (Display on mobile device)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		/*
		if (visibleSize.height == 1080)
			controller = DPad::create("DPad/Base300.png", "DPad/Arrow96.png", "DPad/Arrow96Pressed.png", Point(250, 250));
		else
			controller = DPad::create("DPad/Base150.png", "DPad/Arrow.png", "DPad/ArrowPressed.png", Point(150, 150));
		*/
		this->addChild(controller);
	}

	/*
	//controller->init(this);																									// NOT WORKING ANDROID: D-pad (Display on mobile device)
	//DPad::Instance()->init(this);																								// NOT WORKING ANDROID: D-pad (Display on mobile device)
	controller = DPad::Instance()->create("DPad/Base300.png", "DPad/Arrow96.png", "DPad/Arrow96Pressed.png", Point(250, 250));
	this->addChild(controller);
	*/

	this->addChild(mplayer);

	this->scheduleUpdate();
	
    return true;
}

void LevelTest::update(float dt) {
	Level::update(dt);																						// Call base class update function
	//float curTimeMillis = getTimeTick();																	// Current game time
	//winSize = Director::getInstance()->getWinSize();														// Dimensions of game screen (Needs to update here so lasers fire etc.)
	
    //scoreLabel->setString("Score: " + to_string(Game::Instance()->getScore()));								// Update the displayed score
	//HUD::Instance()->update();																			// Update the score (Not working)

	//getInput();																								// Get keyboard input for Windows, Get DPad input for Android

	// Update timer this class
	//updateTimer(curTimeMillis);																			// Update the countdown timer, pass in curTimeMillies solves Android Timer issue

	// Update timer from Game class
	Game::Instance()->updateTimer(curTimeMillis);															// Update the countdown timer, pass in curTimeMillies solves Android Timer issue
	timeLabel->setString("Time: " + to_string(Game::Instance()->getTimer()));

	//time -= ((int) dt % 10);
	//time += dt;
	//this->schedule(schedule_selector(Level1::updateTimer), 1.0f);											// Call the function every 1 second
	//if (curTimeMillis > timerTime) {
	//	timerTime = curTimeMillis + 1000;
	//	time--;
	//}

	//timeLabel->setString("Time: " + to_string(time));

	//_backgroundNode->update(dt);																			// Scroll the background objects
	spawnAsteroids(curTimeMillis);																			// Spawn asteroids
	spawnEnemyShips(curTimeMillis);																			// Spawn asteroids
	checkCollisions();																						// Check have game objects collided with each other
	checkGameOver(curTimeMillis);																			// Check is the game over or not

	//player->update();																						// Update player sprite position

		// Update displayed lives
	//if (_lives < 3 && !_gameOver) {																		// If the players lives are less than 3
	if (Game::Instance()->getLives() < MAX_LIVES && !_gameOver) {											// If the players lives are less than the max num lives
		livesList[Game::Instance()->getLives()]->setVisible(false);											// Set the lives invisible (2,1,0)
	}

	// Update the enemy ship position
	//EnemyShip->setPosition(EnemyShip->getPosition().x - 2, EnemyShip->getPosition().y);
}
/*
// Moved to Level base class
void Level1::getInput() {
	// Android DPad (maybe change to returning a point (0,0)(1,0)(0,1),(-1,0),(0,-1)
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
		if (controller->getButton(8)->isSelected()) {
			player->moveUp();
			//CCLOG("Down button is pressed!");
		}
		else if (controller->getButton(2)->isSelected()) {
			player->moveDown();
			//CCLOG("Down button is pressed!");
		}
		if (controller->getButton(4)->isSelected()) {
			player->moveLeft();
			//CCLOG("Down button is pressed!");
		}
		else if (controller->getButton(6)->isSelected()) {
			player->moveRight();
			//CCLOG("Down button is pressed!");
		}
	}	
}
*/
void LevelTest::updateTimer(float dt) {
	if (dt > currentTime) {
		currentTime = dt + 1000.0f;
		time--;
	}

	//if (getTimeTick() >= currentTime + 1000) {
	//	currentTime = getTimeTick();
	//	time--;
	//	if (time == 0) this->unschedule(schedule_selector(Level1::updateTimer));
	timeLabel->setString("Time: " + to_string(time));
	//}
}

void LevelTest::spawnAsteroids(float curTimeMillis) {
	if (curTimeMillis > _nextAsteroidSpawn) {
		float randMillisecs = randomValueBetween(0.20F, 1.0F) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;

		float randY = randomValueBetween(0.0F, winSize.height);												// Random Y position for asteroid
		float randDuration = randomValueBetween(2.0F, 10.0F);

		Sprite *asteroid = _asteroids->at(_nextAsteroid);
		_nextAsteroid++;																					// Increment the asteroid

		if (_nextAsteroid >= _asteroids->size()) _nextAsteroid = 0;											// Loop back around to start of asteroids list

		asteroid->stopAllActions();																			// CCNode.cpp
		asteroid->setPosition(winSize.width + asteroid->getContentSize().width / 2, randY);
		asteroid->setVisible(true);
		asteroid->runAction(
			Sequence::create(
				MoveBy::create(randDuration, Point(-winSize.width - asteroid->getContentSize().width, 0)),
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
				NULL /* DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)*/)
		);
	}
}

void LevelTest::spawnEnemyShips(float curTimeMillis) {
	if (curTimeMillis > nextEnemyShipSpawnTime) {
		float randMillisecs = randomValueBetween(0.20F, 1.0F) * 2500;
		nextEnemyShipSpawnTime = randMillisecs + curTimeMillis;												// Set the time to spawn the next ship

		float randY = randomValueBetween(0.0F, winSize.height);												// Random Y position for enemy ship
		float randDuration = randomValueBetween(2.0F, 10.0F);

		Sprite *enemyShip = EnemyShipList->at(nextEnemyShip);
		nextEnemyShip++;																					// Increment the enemy ship on the list

		if (nextEnemyShip >= EnemyShipList->size()) nextEnemyShip = 0;										// Loop back around to start of enemy ship list

		enemyShip->stopAllActions();																		// CCNode.cpp
		enemyShip->setPosition(winSize.width + enemyShip->getContentSize().width / 2, randY);
		enemyShip->setVisible(true);

		enemyShip->runAction(
			Sequence::create(
				MoveBy::create(randDuration, Point(-winSize.width - enemyShip->getContentSize().width, 0)),
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
				NULL /* TERMINATE WITH NULL */)
		);
	}
}

void LevelTest::moveShip(float dt) {
	// Acceleration
	/*
	float maxY = winSize.height - _ship->getContentSize().height / 2;										// The maximum distance the ship can move down
	float minY = _ship->getContentSize().height / 2;														// The maximum distance the ship can move up
	float diff = (_shipPointsPerSecY * dt);
	//float newX;
	float newY = _ship->getPosition().y + diff;
	newY = MIN(MAX(newY, minY), maxY);
	_ship->setPosition(_ship->getPosition().x, newY);
	*/
	/*
	// Only moves when key is pressed
	auto eventListener = EventListenerKeyboard::create();
	
	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			//event->getCurrentTarget()->setPosition(--loc.x, loc.y);
			event->getCurrentTarget()->setPosition(loc.x - 0.1f, loc.y);			
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			//event->getCurrentTarget()->setPosition(++loc.x, loc.y);
			event->getCurrentTarget()->setPosition(loc.x + 0.1f, loc.y);
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			//event->getCurrentTarget()->setPosition(loc.x, ++loc.y);
			event->getCurrentTarget()->setPosition(loc.x, loc.y + 0.1f);
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			//event->getCurrentTarget()->setPosition(loc.x, --loc.y);
			event->getCurrentTarget()->setPosition(loc.x, loc.y - 0.1f);
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, _ship);
	*/
}
/*
void Level1::checkCollisions() {
	// Asteroids Collisions
	for (cocos2d::Sprite* asteroid : *_asteroids) {															// JOR replaced auto specifier
		if (!(asteroid->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *_shipLasers) {													// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {
				Audio::Instance()->explodeFX();
				shipLaser->setVisible(false);
				asteroid->setVisible(false);
				//score += 10;																				// Award 10 points for destroying an asteroid
				Game::Instance()->updateScore(10);
			}
		}

		// Check collisions between the player ship and asteroids
		if (player->getSprite()->getBoundingBox().intersectsRect(asteroid->getBoundingBox())) {				// If the ship collides with an asteroid
			asteroid->setVisible(false);																	// Destroy the asteroid
			player->getSprite()->runAction(Blink::create(1.0F, 9));											// Flash the Player ship
			//_lives--;																						// Decrement the number of lives
			Game::Instance()->takeLife();																	// Decrement the number of lives
			livesList[Game::Instance()->getLives()]->setVisible(false);										// Set the lives invisible (2,1,0)
		}
	}

	// Enemy ship collisions
	for (cocos2d::Sprite* enemyShip : *EnemyShipList) {														// JOR replaced auto specifier
		if (!(enemyShip->isVisible())) continue;

		for (cocos2d::Sprite* shipLaser : *_shipLasers) {													// JOR replaced auto specifier
			if (!(shipLaser->isVisible())) continue;

			if (shipLaser->getBoundingBox().intersectsRect(enemyShip->getBoundingBox())) {
				Audio::Instance()->explodeFX();
				shipLaser->setVisible(false);
				enemyShip->setVisible(false);
				//score += 20;																				// Award 20 points for destroying an enemy ship
				Game::Instance()->updateScore(20);
			}
		}
	}
}
*/
void LevelTest::checkGameOver(float currenTime) {
	//if (_lives <= 0) {																					// If the player has run out of lives
	if (Game::Instance()->getLives() <= 0) {																// If the player has run out of lives
		player->getSprite()->stopAllActions();																// CCNode.cpp
		player->getSprite()->setVisible(false);																// Destroy the ship
		this->endScene(KENDREASONLOSE);																		// Player has died
	}
	else if (currenTime >= _gameOverTime) {
		this->endScene(KENDREASONWIN);																		// Player stays playing for the length of time
	}
}
/*
void Level1::onAcceleration(Acceleration* acc, Event* event) {
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTSPERSEC (winSize.height*0.5)        
#define KMAXDIFFX 0.2

	double rollingX;

	// Cocos2DX inverts X and Y accelerometer depending on device orientation
	// in landscape mode right x=-y and y=x !!! (Strange and confusing choice)
	acc->x = acc->y;
	rollingX = (acc->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));
	float accelX = acc->x - rollingX;
	Size winSize = Director::getInstance()->getWinSize();
	float accelDiff = accelX - KRESTACCELX;
	float accelFraction = accelDiff / KMAXDIFFX;
	_shipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;
}
float Level1::randomValueBetween(float low, float high) {
	// from http://stackoverflow.com/questions/686353/c-random-float-number-generation
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}
float Level1::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
}
void Level1::setInvisible(Node * node) {
	node->setVisible(false);
}
*/

void LevelTest::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event){
	Audio::Instance()->laserFX();
	//cocos2d::Size winSize = Director::getInstance()->getWinSize();									// JOR replaced auto specifier

	//spawnLaser();
	spawnLasers(3);
	//Level::spawnLasers(2, this);
}

// Up to 4 different types of laser, with different spawn points and rotations
void LevelTest::spawnLasers(int amount) {																		// 20180221
	//int i;
	//(amount == 2) ? i = 1 : i = 0;
	int yVal = 0, yPos = 0, rot = 0;	// y value for point to aim for, y position of laser spawn point, rotation of laser	
	//if (amount == 2) yVal = 100;
	// 100 * 0
	// 100 * -1
	// 100 * 1


	for (int i = 0; i < amount; i++) {
		cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);										// Next laser in the list, JOR replaced auto specifier
		if (_nextShipLaser >= _shipLasers->size())
			_nextShipLaser = 0;																				// Reset laser list index to 0 (go back to start of list)

		//if (i == 0) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 0));		// middle
		//if (i == 1) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, -12));	// bottom
		//if (i == 2) shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 12));	// top

		// Set laser spawn points
		if (amount < 4)
			(i == 0) ? yPos = 0 : (i == 1) ? yPos = -12 : yPos = 12;										// 0 = midd
		else
			(i == 0) ? yPos = 8 : (i == 1) ? yPos = -12 : (i == 2) ? yPos = 12 : yPos = -8;					// 0. = 5, 1. = 12, 2. = 12, 3. = -5

		shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, yPos));
		if (amount == 2) (i == 0) ? rot = -5 : rot = 5;														// Top, bottom lasers
		if (amount == 3) (i == 0) ? rot = 0 : (i == 1) ? rot = 5 : rot = -5;								// Middle, bottom, top lasers
		if (amount == 4) (i == 0) ? rot = -3 : (i == 1) ? rot = 5 : (i == 2 ) ? rot = -5: rot = 3;			// laser 1: i = 2 (5), laser 2: i = 0 (3), laser 3: i = ,laser 4: i = 1 (-5)
		shipLaser->setRotation(rot);

		shipLaser->setVisible(true);
		shipLaser->stopAllActions();

		if (amount == 2) (i == 0) ? yVal = 60 : yVal = -60;													// if 2 lasers, first one goes up, second goes down
		if (amount == 3) (i == 1) ? yVal = -100 : (i == 2) ? yVal = 100 : yVal = 0;							// if 3 lasers, first goes straight, second goes down, third goes up
		if (amount == 4) (i == 1) ? yVal = -120 : (i == 2) ? yVal = 120 : (i==3) ? yVal = -40 : yVal = 40;	// if 3 lasers, first goes straight, second goes down, third goes up

		shipLaser->runAction(
			Sequence::create(MoveBy::create(0.5, Point(winSize.width, yVal)), // change to plus 100 for up - 100 for down
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
	}
}

/*
void Level1::spawnLaser() {
	cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);														// Next laser in the list, JOR replaced auto specifier
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;																								// Reset laser list index to 0 (go back to start of list)
	
	shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width/2, 0));
	shipLaser->setVisible(true);
	shipLaser->stopAllActions();
	shipLaser->runAction(
		Sequence::create(
			MoveBy::create(0.5, Point(winSize.width, 0)),
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)),
			NULL));
}
void Level1::spawn2Lasers() {
	
void Level1::spawnLasers(int amount) {																					// 20180221
	cocos2d::Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);														// Next laser in the list, JOR replaced auto specifier
	if (_nextShipLaser >= _shipLasers->size())
		_nextShipLaser = 0;																								// Reset laser list index to 0 (go back to start of list)
	
	shipLaser->setPosition(player->getSprite()->getPosition() + Point(shipLaser->getContentSize().width / 2, 12));
	shipLaser->setVisible(true);
	shipLaser->stopAllActions();
	shipLaser->runAction(
		Sequence::create(MoveBy::create(0.5, Point(winSize.width, 100)),
			CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL)); 

	if (amount == 2) {
		cocos2d::Sprite* shipLaser2 = _shipLasers->at(_nextShipLaser++);													// Next laser in the list, JOR replaced auto specifier
		if (_nextShipLaser >= _shipLasers->size())
			_nextShipLaser = 0;																								// Reset laser list index to 0 (go back to start of list)

		shipLaser2->setPosition(player->getSprite()->getPosition() + Point(shipLaser2->getContentSize().width / 2, -12));
		shipLaser2->setVisible(true);
		shipLaser2->stopAllActions();
		shipLaser2->runAction(
			Sequence::create(MoveBy::create(0.5, Point(winSize.width, -100)),
				CallFuncN::create(CC_CALLBACK_1(Level::setInvisible, this)), NULL));
	}
}
*/
void LevelTest::restartTapped(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, this->createScene()));							// Restart the current scene	
	this->scheduleUpdate();																									// reschedule
}
/*
void Level1::startLevel2(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, Level2::createScene()));							// Change scene, progressing to Level 2
}
void Level1::returnToMenu(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(0.5, MainMenu::createScene()));						// Return to the main menu
}
*/
void LevelTest::endScene(EndReason endReason) {
	if (_gameOver) return;																									// If already game over, skip this function
	_gameOver = true;																										// Set game over

	cocos2d::Size winSize = Director::getInstance()->getWinSize();															// JOR replaced auto specifier
	
	Game::Instance()->checkHighScore();																						// The game has ended, check if the current score is the high score and save it if it is

	// Win / Lose Message
	char message[17] = "Level 1 Complete";
	if (endReason == KENDREASONLOSE) strcpy(message, "You Lose");

	cocos2d::Label* label = Label::createWithBMFont("Arial.fnt", message);													// JOR replaced auto specifier

	if (winSize.height == 720)
		label->setScale(0.2f);
	else
		label->setScale(0.3F);																								// JOR Use larger label for 1080p

	label->setPosition(winSize.width / 2, winSize.height*0.75f);
	this->addChild(label);
	
	// Restart Level
	strcpy(message, "Restart Game");
	cocos2d::Label* restartLbl = Label::createWithBMFont("Arial.fnt", message);												// JOR replaced auto specifier
	cocos2d::MenuItemLabel* restartItem=MenuItemLabel::create(restartLbl,CC_CALLBACK_1(LevelTest::restartTapped,this));		// JOR replaced auto specifier
	//restartItem->setScale(0.1F);
	if (winSize.height == 720)
		restartItem->setScale(0.1F);
	else
		restartItem->setScale(0.15F);																						// JOR Use larger size for 1080p
	restartItem->setPosition(winSize.width / 2, winSize.height*0.5f);

	cocos2d::Menu* menu = Menu::create(restartItem, NULL);																	// JOR replaced auto specifier
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	// Continue To Next Level
	if (endReason != KENDREASONLOSE) {
		strcpy(message, "Continue");
		cocos2d::Label* continueLbl = Label::createWithBMFont("Arial.fnt", message);										// JOR replaced auto specifier
		cocos2d::MenuItemLabel* continueItem = MenuItemLabel::create(continueLbl,CC_CALLBACK_1(Level::startLevel2,this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
																															//restartItem->setScale(0.1F);
		if (winSize.height == 720)
			continueItem->setScale(0.1F);
		else
			continueItem->setScale(0.15F);																					// JOR Use larger size for 1080p
		continueItem->setPosition(winSize.width / 2, winSize.height*0.25f);

		cocos2d::Menu* menu2 = Menu::create(continueItem, NULL);															// JOR replaced auto specifier
		menu2->setPosition(Point::ZERO);
		this->addChild(menu2);
		
		continueItem->runAction(ScaleTo::create(0.5F, 1.0F));
	} else {
		// Return To Main Menu
		strcpy(message, "Main Menu");
		cocos2d::Label* menuLbl = Label::createWithBMFont("Arial.fnt", message);											// JOR replaced auto specifier
		cocos2d::MenuItemLabel* mainMenuItem = MenuItemLabel::create(menuLbl, CC_CALLBACK_1(Level::returnToMenu, this));	// JOR replaced auto specifier XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		if (winSize.height == 720)
			mainMenuItem->setScale(0.1F);
		else
			mainMenuItem->setScale(0.15F);																					// JOR Use larger size for 1080p
		mainMenuItem->setPosition(winSize.width / 2, winSize.height*0.25f);

		cocos2d::Menu* menu2 = Menu::create(mainMenuItem, NULL);															// JOR replaced auto specifier
		menu2->setPosition(Point::ZERO);
		this->addChild(menu2);

		mainMenuItem->runAction(ScaleTo::create(0.5F, 1.0F));
	}

	// clear label and menu
	restartItem->runAction(ScaleTo::create(0.5F, 1.0F));
	label->runAction(ScaleTo::create(0.5F, 1.0F));
		
	this->unscheduleUpdate();																								// Terminate update callback
}
/*
void Level1::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
*/


void LevelTest::createStatusBar(int x, int y, int w, int h, float percent, Color4F FGColor, Color4F BGColor, int orientation, int startFrom) {	// Horizontal healthbar 2017/01/20 added if statement to decide orientation of health bar

	percent = (percent > 1.0f) ? 1.0f : (percent < 0.f) ? 0.f : percent;		// If greater than 1 set to 1, if minus set to 0

	cocos2d::DrawNode* rectNode = DrawNode::create();
	Color4F white(1, 1, 1, 1);
	Vec2 foregroundRect[4];
	Vec2 backgroundRect[4];

	foregroundRect[0] = Vec2(-w/2, -h/2);
	foregroundRect[1] = Vec2(w/2, -h/2);
	foregroundRect[2] = Vec2(w/2, h/2);
	foregroundRect[3] = Vec2(-w/2, h/2);

	backgroundRect[0] = Vec2(-w / 2, -h / 2);
	backgroundRect[1] = Vec2(w / 2 * percent, -h / 2);
	backgroundRect[2] = Vec2(w / 2 * percent, h / 2);
	backgroundRect[3] = Vec2(-w, h / 2);

	rectNode->drawPolygon(backgroundRect, 4, BGColor, 2, white);
	rectNode->drawPolygon(foregroundRect, 4, BGColor, 2, white);
	rectNode->setPosition(Point(x, y));


	//SDL_Rect foregroundRect;													// The rectangle to
	//Color4F old;

//	SDL_GetRenderDrawColor(Game::Instance()->getRenderer(), &old.r, &old.g, &old.g, &old.a);
//	SDL_Rect backgroundRect = { x, y, w, h };									// Background rectangle with coordinates and dimensions entered
//	SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a);	// Draw a Solid Rectangle with the RGB colour values assigned
//	SDL_RenderFillRect(Game::Instance()->getRenderer(), &backgroundRect);
//	SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);
	/*
	// Decide if Healthbar is horizontal or vertical
	if (orientation == HORIZONTAL) {
		int newW = (int)((float)w * Percent);									// new width
		int newX = x + (w - newW);												// new x coord
		if (startFrom == START_LEFT) foregroundRect = { newX, y, newW, h };		// Decrease from left
		else if (startFrom == START_RIGHT) foregroundRect = { x, y, newW, h };	// 2017/02/17 Decrease from right
	}
	else if (orientation == VERTICAL) {
		int ph = (int)((float)h * Percent);
		int py = y + (h - ph);
		foregroundRect = { x, py, w, ph };
	}
	*/
	//SDL_RenderFillRect(Game::Instance()->getRenderer(), &foregroundRect);
	//SDL_SetRenderDrawColor(Game::Instance()->getRenderer(), old.r, old.g, old.b, old.a);
	
}