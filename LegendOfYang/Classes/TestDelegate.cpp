#include "AppDelegate.h"
#include "OverworldScene.hpp"
#include "Player.h"
#include "Enemy.h"
#include "GameState.hpp"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("LegendOfYang", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("LegendOfYang");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = OverworldScene::createWithTileMap("TileMap.tmx");
    //auto scene = HelloWorld::createScene();
    
    // run
    //director->runWithScene(scene);
    Catch::Session session;
    session.run();

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}



TEST_CASE("gamestate-singleton") {
    GameState::defaultInstance->state["key"] = Value(true);
    REQUIRE(GameState::defaultInstance->state["key"].asBool());
}

TEST_CASE("keyboardMenu-setItems") {
    auto menu = KeyboardMenu::create({{"item1", nullptr}, {"item2", nullptr}});
    menu->setItems({{"item3", nullptr}});
    REQUIRE(menu->selectedLabelText() == "item3");
}

TEST_CASE("keyboardMenu-setContentSize") {
    auto menu = KeyboardMenu::create({{"item1", nullptr}, {"item2", nullptr}});
    menu->setContentSize(Size::ZERO);
    REQUIRE(!menu->getContentSize().equals(Size::ZERO)); // Note: should not be zero despite the call above.
}

TEST_CASE("keyboardMenu-setContentSize2") {
    auto menu = KeyboardMenu::create({{"item1", nullptr}, {"item2", nullptr}});
    menu->setContentSize(Size(1000, 1000));
    REQUIRE(!menu->getContentSize().equals(Size(1000, 1000))); // Note: should not be zero despite the call above.
}

TEST_CASE("keyboardMenu-selectedLabelText") {
    auto menu = KeyboardMenu::create({{"item1", nullptr}, {"item2", nullptr}});
    REQUIRE(menu->selectedLabelText() == "item1");
}

TEST_CASE("keyboardMenu-autoResize") {
    auto menu = KeyboardMenu::create({{"item1", nullptr}, {"item2", nullptr}});
    REQUIRE(!menu->getContentSize().equals(Size::ZERO));
}

TEST_CASE("entity-face") {
    auto e1 = Entity::create();
    auto e2 = Entity::create();
    e1->setPosition(Vec2(0, 1));
    e2->face(e1);
    REQUIRE(e2->getOrientation().equals(Vec2(0,1)));
}

TEST_CASE("entity-face2") {
    auto e1 = Entity::create();
    auto e2 = Entity::create();
    e1->setPosition(Vec2(1, 0));
    e2->face(e1);
    REQUIRE(e2->getOrientation().equals(Vec2(1,0)));
}

TEST_CASE("entity-setOrientation") {
    auto e1 = Entity::create();
    e1->setOrientation(Vec2(100, 0));
    REQUIRE(e1->getOrientation().equals(Vec2(1,0)));
}

TEST_CASE("entity-setOrientation2") {
    auto e1 = Entity::create();
    e1->setOrientation(Vec2(-100, 0));
    REQUIRE(e1->getOrientation().equals(Vec2(-1,0)));
}

TEST_CASE("entity-getCollisionBox") {
    auto e1 = Entity::create();
    e1->setPosition(1, 2);
    e1->setContentSize(Size(3, 4));
    REQUIRE(e1->getCollisionBox().equals(Rect(1, 2, 3, 4)));
}

TEST_CASE("entity-getCollisionBox2") {
    auto e1 = Entity::create();
    e1->setPosition(4, 3);
    e1->setContentSize(Size(2, 1));
    REQUIRE(e1->getCollisionBox().equals(Rect(4, 3, 2, 1)));
}

TEST_CASE("entity-getAllEntities") {
    auto e1 = Entity::create();
    auto e2 = Entity::create();
    auto e3 = Entity::create();
    
    e1->addChild(e2); e2->addChild(e3);
    REQUIRE(e1->getAllEntities().equals({e2, e3}));
}

TEST_CASE("Player Tests")
{
    REQUIRE(Player::getLv() == 1);
    REQUIRE(Player::getExp() == 0);
    REQUIRE(Player::getGold() == 0);
    REQUIRE(Player::getMaxHp() == 60);
    REQUIRE(Player::getCurrentHp() == 60);
    REQUIRE(Player::getAtk() == 10);
    REQUIRE(Player::getDef() == 0);
    Player::takeDamage(10);
    REQUIRE(Player::getCurrentHp() == 50);
    Player::gainExp(5);
    REQUIRE(Player::getExp() == 5);
    Player::gainExp(5);
    REQUIRE(Player::getLv() == 2);
    REQUIRE(Player::getExp() == 0);
    REQUIRE(Player::getMaxHp() == 70);
    REQUIRE(Player::getCurrentHp() == 70);
    REQUIRE(Player::getAtk() == 20);
    REQUIRE(Player::getDef() == 10);
    Player::gainGold(10);
    REQUIRE(Player::getGold() == 10);
}

TEST_CASE("Enemy Tests")
{
    Enemy enemy(0);
    REQUIRE(enemy.getExp() == 4);
    REQUIRE(enemy.getGold() == 5);
    REQUIRE(enemy.getHp() == 30);
    REQUIRE(enemy.getAtk() == 2);
    REQUIRE(enemy.getDef() == 0);
    REQUIRE(enemy.getPosition() == Vec2(400, 500));
    REQUIRE(!enemy.isDead());
    enemy.takeDamage(10);
    REQUIRE(enemy.getHp() == 20);
    enemy.takeDamage(20);
    REQUIRE(enemy.getHp() == 0);
    REQUIRE(enemy.isDead());
}
