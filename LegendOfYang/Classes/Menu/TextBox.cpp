//
//  TextBox.cpp
//  LegendOfYang
//
//  Created by John Rehbein on 2/5/18.
//

#include "TextBox.hpp"

bool TextBox::init() {
    if (!Node::init()) return false;
    background = LayerColor::create(Color4B(0, 0, 255, 200), this->getContentSize().width, this->getContentSize().height);
    this->addChild(background);
    
    updateText("Don't use the create method use create(const Vector<std::string> &text, const Size size)");
    return true;
}

TextBox* TextBox::create(const std::string &text, const Size size) {
    auto textBox = TextBox::create();
    textBox->textLabel->removeFromParent();
    textBox->updateText(text);
    
    textBox->setContentSize(size);
    
    return textBox;
}

void TextBox::updateText(std::string text) {
    removeChild(textLabel);
    textLabel = Label::createWithTTF(text, "Fonts/OpenSans-Bold.ttf", 24);
    addChild(textLabel);
    textLabel->setTextColor(Color4B::WHITE);
    textLabel->setAnchorPoint(cocos2d::Point(0.0f, 0.0f));
    
    setContentSize(getContentSize());
}

void TextBox::setContentSize(const cocos2d::Size &contentSize) {
    Node::setContentSize(contentSize);
    
    // So text doesn't touch the edges of the text box
    float labelScale = 0.95f;

    textLabel->setPosition(contentSize * (1 - labelScale) / 2.0f);
    textLabel->setContentSize(contentSize * labelScale);
    textLabel->setMaxLineWidth(contentSize.width * labelScale);
    textLabel->setLineSpacing(20.0f);
    textLabel->setVerticalAlignment(TextVAlignment::TOP);
    background->setContentSize(contentSize);
}
