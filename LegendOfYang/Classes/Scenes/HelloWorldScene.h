#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Player.h"

USING_NS_CC;

class HelloWorld : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
    void update(float) override;
    void handleOption(Node *option);
    
    // create a create() function that calls constructor, init and autorelease
    CREATE_FUNC(HelloWorld);
    
private:
    int speed;
    bool movingUp, movingDown, movingLeft, movingRight, paused;
    Player player;
    Sprite *sprite;
    CocosDenshion::SimpleAudioEngine *audio;
    Vec2 position;
    EventListenerKeyboard *keyboardListener;
    RepeatForever *moveUp, *moveDown, *moveLeft, *moveRight, *moveUpLeft, *moveUpRight, *moveDownLeft, *moveDownRight;
};