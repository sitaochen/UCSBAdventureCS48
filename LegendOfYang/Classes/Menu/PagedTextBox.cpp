//
//  PagedTextBox.cpp
//  LegendOfYang
//
//  Created by John Rehbein on 2/7/18.
//

#include "PagedTextBox.hpp"

bool PagedTextBox::init() {
    if (!TextBox::init()) { return false; }
    auto keyboardListener = EventListenerKeyboard::create();
    auto eventDispatcher = getEventDispatcher();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PagedTextBox::onKeyPressed, this);
    eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    return true;
}

TextBox* PagedTextBox::create(const std::vector<std::string> &text, const Size size) {
    auto box = PagedTextBox::create();
    box->setContentSize(size);
    box->currentPage = 0;
    box->_text = text;
    box->updateText(text.at(box->currentPage));
    
    
    return box;
}

void PagedTextBox::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {
    event->stopPropagation();
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_ENTER:
        case EventKeyboard::KeyCode::KEY_SHIFT:
            if (currentPage + 1 < _text.size()) {
                currentPage += 1;
                updateText(_text.at(currentPage));
            } else {
                // Close Text Box
                removeFromParent();
            }
            
            break;
            
        default:
            break;
    }
}