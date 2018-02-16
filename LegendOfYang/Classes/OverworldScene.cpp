//
//  OverworldScene.cpp
//  LegendOfYang
//
//  Created by John Rehbein on 2/7/18.
//

#include "OverworldScene.hpp"
#include "PagedTextBox.hpp"
#include "KeyboardMenu.hpp"
#include "Inventory.hpp"
#include "PlayerStatsDisplay.hpp"
#include "YangPhysics.hpp"

// initialize
bool OverworldScene::init() {
    return Scene::initWithPhysics();
}

Scene* OverworldScene::createWithTileMap(std::string filename) {
    // Setup node Layers
    auto scene = OverworldScene::create();
    scene->gui = Node::create();
    scene->world = Node::create();
    scene->addChild(scene->world, 0);
    scene->addChild(scene->gui, 1); // GUI always has higher event priority over anything else
    
    // Setup Tile map
    scene->tileMap = TMXTiledMap::create(filename);
    scene->meta = scene->tileMap->getLayer("Meta");
    scene->world->addChild(scene->tileMap, 0, 99);
    
    // Add player
    scene->player = Entity::create("CloseNormal.png");
    scene->player->setContentSize(Size(24, 24));
    scene->player->setPosition(Vec2(64.0f, 1024.0f));
    scene->player->setAnchorPoint(Vec2(0.0f, 0.0f));
    scene->world->addChild(scene->player);
    scene->world->runAction(Follow::create(scene->player));
    
    // Setup keyboard listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(OverworldScene::onKeyPressed, scene);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(OverworldScene::onKeyReleased, scene);
    scene->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, scene);
    
    // schedule update
    scene->scheduleUpdateWithPriority(LOOP_UPDATE_ORDER_INPUT);
    
    // Add physics handler to the world
    scene->world->addChild(YangPhysics::createWithTileMap(scene->tileMap));
    
    return scene;
}

void OverworldScene::update(float delta) {
    float vx = heldKey[(int)EventKeyboard::KeyCode::KEY_RIGHT_ARROW] - heldKey[(int)EventKeyboard::KeyCode::KEY_LEFT_ARROW];
    float vy = heldKey[(int)EventKeyboard::KeyCode::KEY_UP_ARROW] - heldKey[(int)EventKeyboard::KeyCode::KEY_DOWN_ARROW];
    
    auto velocityDirection = Vec2(vx, vy);
    velocityDirection.normalize(); // Components shouldn't compound speed should be the same in any direction
    
    player->velocity = velocityDirection * 200.0f;
    
}

Vec2 OverworldScene::tileCoordForPosition(Vec2 position) {
    Size tileSize = tileMap->getTileSize();
    int x = position.x / tileSize.width;
    int y = ((tileMap->getMapSize().height * tileSize.height) - position.y) / tileSize.height;
    return Vec2(x, y);
}

#pragma mark input

void OverworldScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {
    heldKey[(int)keyCode] = true;
    
    if (keyCode == EventKeyboard::KeyCode::KEY_E) {
        // Present Inventory
        std::vector<LabelAndCallback> items;
        LabelAndCallback item1, item2, item3;
        item1.first = "Inventory";
        item1.second = [this](Node *sender) { // TODO : self should be weak, shared_ptr, or autorelease
            this->gui->addChild(Inventory::create());
        };
        item2.first = "Stats";
        item2.second = [this](Node *sender) {
            this->gui->addChild(PlayerStatsDisplay::create());
        };
        item3.first = "Close";
        item3.second = [](Node *sender) {
            sender->removeFromParent();
        };
        items.push_back(item1);
        items.push_back(item2);
        items.push_back(item3);
        auto menu = KeyboardMenu::create(items);
        gui->addChild(menu);
    }
    
}

void OverworldScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
    heldKey[(int)keyCode] = false;
}
