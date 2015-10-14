//
//  EditorMenuManager.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 06/11/14.
//
//

#ifdef EDITOR_MODE

#include "EditorMenuManager.h"
#include "VisibleRect.h"
#include "InputMenuItem.h"
#include "SheepMenuItem.h"
#include "NoteMenuItem.h"
#include "GoatskinMenuItem.h"
#include "EditorTableView.h"
#include "GameLogic.h"
#include "Utility.h"

EditorMenuManager::EditorMenuManager()
{
    
}

EditorMenuManager::~EditorMenuManager()
{
    removeAllChildren();
}

bool EditorMenuManager::init()
{
    bool initDone = false;
    
    if (Layer::init())
    {
        initDone = true;
    }
    
    return initDone;
}

void EditorMenuManager::createItems()
{
    Rect rect = VisibleRect::getVisibleRect();
    this->setZOrder(2);
    
    auto scene = dynamic_cast<GameLogic*>(this->getParent());
    
    if (scene)
    {
        /*--------------------- Input Menu -------------------------*/
        auto player = scene->getPlayer();
        
        if (player)
        {
            auto inputMenu = InputMenuItem::create();
            inputMenu->setPosition(0, rect.getMaxY());
            
            inputMenu->setPlayer(player);
            inputMenu->initLevelVar();
            this->addChild(inputMenu, 0, INPUT_MENU);
            
            auto inputMenuSize = inputMenu->getContentSize();
            auto inputTabLayer = LayerColor::create(Color4B(50,50,50,200), inputMenuSize.width / 17, inputMenuSize.height / 3);
            inputTabLayer->setPosition(0, -(inputMenuSize.height / 3));
            inputMenu->addChild(inputTabLayer, 0, 1);
            
            Sprite* inputTabSprite = Sprite::create("Pan_Tab.png");
            inputTabSprite->setScale(0.6f, 0.6f);
            MenuItemSprite* inputTabItem = MenuItemSprite::create(inputTabSprite, inputTabSprite, inputTabSprite, CC_CALLBACK_1(EditorMenuManager::inputTabPressed, this));
            
            Menu *inputTab = Menu::create(inputTabItem, NULL);
            inputTab->setPosition(44, 48);
            inputTabLayer->addChild(inputTab, 0, 1);
        }
        
        /*--------------------- Sheep Menu -------------------------*/
        __Array* sheepArray = scene->getSheep();
        
        if (sheepArray->count() > 0)
        {
            auto sheepMenu = SheepMenuItem::create();
            sheepMenu->setPosition(0, rect.getMaxY());
            
            sheepMenu->setScene(scene);
            sheepMenu->initLevelVar();
            this->addChild(sheepMenu, 0, SHEEP_MENU);
            
            auto sheepMenuSize = sheepMenu->getContentSize();
            auto sheepTabLayer = LayerColor::create(Color4B(50,50,50,200), sheepMenuSize.width / 17, sheepMenuSize.height / 3);
            sheepTabLayer->setPosition(sheepMenuSize.width / 17, -(sheepMenuSize.height / 3));
            sheepMenu->addChild(sheepTabLayer, 0, 1);
            
            Sprite* sheepTabSprite = Sprite::create("Pecora_Tab.png");
            sheepTabSprite->setScale(0.8f, 0.8f);
            MenuItemSprite* sheepTabItem = MenuItemSprite::create(sheepTabSprite, sheepTabSprite, sheepTabSprite, CC_CALLBACK_1(EditorMenuManager::sheepTabPressed, this));
            
            Menu *sheepTab = Menu::create(sheepTabItem, NULL);
            sheepTab->setPosition(34, 34);
            sheepTabLayer->addChild(sheepTab, 0, 2);
        }
        
        /*--------------------- Note Menu --------------------------*/
        if (player && (sheepArray->count() >0))
        {
            auto noteMenu = NoteMenuItem::create();
            noteMenu->setPosition(0, rect.getMaxY());
            
            noteMenu->setPlayer(player);
            noteMenu->setScene(scene);
            noteMenu->initLevelVar();
            this->addChild(noteMenu, 0, NOTE_MENU);
            
            auto noteMenuSize = noteMenu->getContentSize();
            auto noteTabLayer = LayerColor::create(Color4B(50,50,50,200), noteMenuSize.width / 17, noteMenuSize.height / 3);
            noteTabLayer->setPosition(noteMenuSize.width / 8.5, -(noteMenuSize.height / 3));
            noteMenu->addChild(noteTabLayer, 0, 1);
            
            Sprite* noteTabSprite = Sprite::create("Nota_Tab.png");
            noteTabSprite->setScale(0.8f, 0.8f);
            MenuItemSprite* noteTabItem = MenuItemSprite::create(noteTabSprite, noteTabSprite, noteTabSprite, CC_CALLBACK_1(EditorMenuManager::noteTabPressed, this));
            
            Menu *noteTab = Menu::create(noteTabItem, NULL);
            noteTab->setPosition(34, 34);
            noteTabLayer->addChild(noteTab, 0, 3);
        }
        
        /*--------------------- Goatskin Menu --------------------------*/
        __Array* goatskinArray = scene->getGoatskins();
        
        if (goatskinArray->count() > 0)
        {
            auto goatskinMenu = GoatskinMenuItem::create();
            goatskinMenu->setPosition(0, rect.getMaxY());
            
            goatskinMenu->setScene(scene);
            goatskinMenu->initLevelVar();
            this->addChild(goatskinMenu, 0, GOATSKIN_MENU);
    
            auto goatskinMenuSize = goatskinMenu->getContentSize();
            auto goatskinTabLayer = LayerColor::create(Color4B(50,50,50,200), goatskinMenuSize.width / 17, goatskinMenuSize.height / 3);
            goatskinTabLayer->setPosition(goatskinMenuSize.width / 5.67, -(goatskinMenuSize.height / 3));
            goatskinMenu->addChild(goatskinTabLayer, 0, 1);
    
            Sprite* goatskinTabSprite = Sprite::create("rock_2x2.png");
            goatskinTabSprite->setScale(0.3f, 0.3f);
            MenuItemSprite* goatskinTabItem = MenuItemSprite::create(goatskinTabSprite, goatskinTabSprite, goatskinTabSprite, CC_CALLBACK_1(EditorMenuManager::goatskinTabPressed, this));
    
            Menu *goatskinTab = Menu::create(goatskinTabItem, NULL);
            goatskinTab->setPosition(74, 70);
            goatskinTabLayer->addChild(goatskinTab, 0, 4);
        }
    }
    
    /*----------------- Save Data selection --------------------*/
    auto saveTabLayer = LayerColor::create(Color4B(50,50,50,200), rect.getMaxX() / 17, rect.getMaxY() / 12);
    saveTabLayer->setPosition((rect.getMaxX() - (rect.getMaxX() / 8.5)), (rect.getMaxY() - (rect.getMaxY() / 12)));
    this->addChild(saveTabLayer, 0, SAVE);
    
    auto saveLabel = Label::createWithSystemFont("SAVE", "Marker Felt", 15);
    MenuItemLabel* saveTabItem = MenuItemLabel::create(saveLabel, CC_CALLBACK_1(EditorMenuManager::saveTabPressed, this));
    
    Menu *saveTab = Menu::create(saveTabItem, NULL);
    saveTab->setPosition(28, 26);
    saveTabLayer->addChild(saveTab, 0, 1);
    
    /*--------------- Back to Level selection ------------------*/
    auto backTabLayer = LayerColor::create(Color4B(50,50,50,200), rect.getMaxX() / 17, rect.getMaxY() / 12);
    backTabLayer->setPosition((rect.getMaxX() - (rect.getMaxX() / 17)), (rect.getMaxY() - (rect.getMaxY() / 12)));
    this->addChild(backTabLayer, 0, BACK);
    
    auto backLabel = Label::createWithSystemFont("BACK", "Marker Felt", 15);
    MenuItemLabel* backTabItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(EditorMenuManager::backTabPressed, this));
    
    Menu *backTab = Menu::create(backTabItem, NULL);
    backTab->setPosition(28, 26);
    backTabLayer->addChild(backTab, 0, 1);
}

void EditorMenuManager::inputTabPressed(Ref* sender)
{
    CCLOG("Pan clicked");
    moveTab(INPUT_MENU);
}

void EditorMenuManager::sheepTabPressed(Ref* sender)
{
    CCLOG("Sheep clicked");
    moveTab(SHEEP_MENU);
}

void EditorMenuManager::noteTabPressed(Ref* sender)
{
    CCLOG("Note clicked");
    moveTab(NOTE_MENU);
}

void EditorMenuManager::goatskinTabPressed(Ref* sender)
{
    CCLOG("Goatskin clicked");
    moveTab(GOATSKIN_MENU);
}

void EditorMenuManager::backTabPressed(Ref *sender)
{
    auto gameLogic = dynamic_cast<GameLogic*>(this->getParent());
    gameLogic->resetForEditor();
    
    CCLOG("Back clicked");
    Scene *scene = Scene::create();
    EditorTableView *layer = EditorTableView::create();
    scene->addChild(layer);
    
    Director::getInstance()->replaceScene(scene);
}

void EditorMenuManager::saveTabPressed(Ref *sender)
{
    CCLOG("Save clicked");
    saveData();
}

void EditorMenuManager::moveTab(eItems item)
{
    Rect rect = VisibleRect::getVisibleRect();
    float durationMove = 0.3;
    
    auto menu = dynamic_cast<EditorMenuItem*>(this->getChildByTag(item));
    bool enabled = menu->getTabEnabled();
    
    for (Vector<Node*>::iterator it = this->getChildren().begin(); it != this->getChildren().end(); ++it)
    {
        if (enabled)
        {
            if ((static_cast<Node*>(*it))->getTag() == BACK)
            {
                // Back button
                (static_cast<Node*>(*it))->runAction(MoveTo::create(durationMove, Point((rect.getMaxX() - (rect.getMaxX() / 17)),
                                                                           (rect.getMaxY() - (rect.getMaxY() / 12)))));
            }
            else if ((static_cast<Node*>(*it))->getTag() == SAVE)
            {
                // Save button
                (static_cast<Node*>(*it))->runAction(MoveTo::create(durationMove, Point((rect.getMaxX() - (rect.getMaxX() / 8.5)),
                                                                            (rect.getMaxY() - (rect.getMaxY() / 12)))));
            }
            else
            {
                (static_cast<Node*>(*it))->runAction(MoveTo::create(durationMove, Point(0, rect.getMaxY())));
            }
        }
        else
        {
            if (*it == menu)
            {
                menu->runAction(MoveTo::create(durationMove, Point(0, rect.getMaxY() * 0.75f)));
            }
            else if ((static_cast<Node*>(*it))->getTag() == BACK)
            {
                // Back button
                (static_cast<Node*>(*it))->runAction(MoveTo::create(durationMove, Point((rect.getMaxX() - (rect.getMaxX() / 17)),
                                                                           rect.getMaxY() * 1.25f)));
            }
            else if ((static_cast<Node*>(*it))->getTag() == SAVE)
            {
                // Save button
                (static_cast<Node*>(*it))->runAction(MoveTo::create(durationMove, Point((rect.getMaxX() - (rect.getMaxX() / 8.5)),
                                                                                        rect.getMaxY() * 1.25f)));
            }
            else
            {
                (static_cast<Node*>(*it))->runAction(MoveTo::create(durationMove, Point(0, rect.getMaxY() * 1.25f)));
            }
        }
    }
    
    menu->setTabEnabled(!enabled);
}

void EditorMenuManager::saveData()
{
    auto floatObject = Float::create(0.0f);
    auto root = Dictionary::create();
    auto string = String::create("string element value");
    root->setObject(string, "string element key");
    
#pragma mark INPUT

    auto player = dynamic_cast<GameLogic*>(getParent())->getPlayer();
    auto inputDict = Dictionary::create();
    auto inputMenu = dynamic_cast<InputMenuItem*>(this->getChildByTag(INPUT_MENU));
    
    if (player && inputMenu)
    {
        auto horizontalImpulse = dynamic_cast<cocos2d::extension::ControlSlider*>(inputMenu->getChildByTag(TYPE_INPUT_HORIZONTAL_IMPULSE));
        floatObject = Float::create(horizontalImpulse->getValue());
        inputDict->setObject(floatObject, "horizontalImpulse");
        
        auto verticalImpulse = dynamic_cast<cocos2d::extension::ControlSlider*>(inputMenu->getChildByTag(TYPE_INPUT_VERTICAL_IMPULSE));
        floatObject = Float::create(verticalImpulse->getValue());
        inputDict->setObject(floatObject, "verticalImpulse");
        
        auto horizontalLimit = dynamic_cast<cocos2d::extension::ControlSlider*>(inputMenu->getChildByTag(TYPE_INPUT_HORIZONTAL_LIMIT));
        floatObject = Float::create(horizontalLimit->getValue());
        inputDict->setObject(floatObject, "horizontalLimit");
    }
    else
    {
        auto value = Utility::loadDataFromFile("Input", "horizontalImpulse", "0.3");
        floatObject = Float::create(atof(value->getCString()));
        inputDict->setObject(floatObject, "horizontalImpulse");
        
        value = Utility::loadDataFromFile("Input", "verticalImpulse", "12.35");
        floatObject = Float::create(atof(value->getCString()));
        inputDict->setObject(floatObject, "verticalImpulse");
        
        value = Utility::loadDataFromFile("Input", "horizontalLimit", "8.0");
        floatObject = Float::create(atof(value->getCString()));
        inputDict->setObject(floatObject, "horizontalLimit");
    }
    
    root->setObject(inputDict, "Input");
    
#pragma mark SHEEP
    auto sheepDict = Dictionary::create();
    auto sheepMenu = dynamic_cast<SheepMenuItem*>(this->getChildByTag(SHEEP_MENU));
    
    if (sheepMenu)
    {
        auto sheepHorizontalImpulse = dynamic_cast<cocos2d::extension::ControlSlider*>(sheepMenu->getChildByTag(TYPE_SHEEP_HORIZONTAL_IMPULSE));
        floatObject = Float::create(sheepHorizontalImpulse->getValue());
        sheepDict->setObject(floatObject, "horizontalImpulse");
        
        auto sheepVerticalImpulse = dynamic_cast<cocos2d::extension::ControlSlider*>(sheepMenu->getChildByTag(TYPE_SHEEP_VERTICAL_IMPULSE));
        floatObject = Float::create(sheepVerticalImpulse->getValue());
        sheepDict->setObject(floatObject, "verticalImpulse");
        
        auto sheepHorizontalLimit = dynamic_cast<cocos2d::extension::ControlSlider*>(sheepMenu->getChildByTag(TYPE_SHEEP_HORIZONTAL_LIMIT));
        floatObject = Float::create(sheepHorizontalLimit->getValue());
        sheepDict->setObject(floatObject, "horizontalLimit");
        
        auto sheepDistance = dynamic_cast<cocos2d::extension::ControlSlider*>(sheepMenu->getChildByTag(TYPE_SHEEP_DISTANCE));
        floatObject = Float::create(sheepDistance->getValue());
        sheepDict->setObject(floatObject, "sheepDistance");
    }
    else
    {
        auto value = Utility::loadDataFromFile("Sheep", "horizontalImpulse", "0.5");
        floatObject = Float::create(atof(value->getCString()));
        sheepDict->setObject(floatObject, "horizontalImpulse");
        
        value = Utility::loadDataFromFile("Sheep", "verticalImpulse", "8.4");
        floatObject = Float::create(atof(value->getCString()));
        sheepDict->setObject(floatObject, "verticalImpulse");
        
        value = Utility::loadDataFromFile("Sheep", "horizontalLimit", "6.0");
        floatObject = Float::create(atof(value->getCString()));
        sheepDict->setObject(floatObject, "horizontalLimit");
        
        value = Utility::loadDataFromFile("Sheep", "sheepDistance", "0.2");
        floatObject = Float::create(atof(value->getCString()));
        sheepDict->setObject(floatObject, "sheepDistance");
    }
    
    root->setObject(sheepDict, "Sheep");
    
#pragma mark NOTE
    auto noteDict = Dictionary::create();
    auto noteMenu = dynamic_cast<NoteMenuItem*>(this->getChildByTag(NOTE_MENU));
    
    if (noteMenu)
    {
        auto noteDistance = dynamic_cast<cocos2d::extension::ControlSlider*>(noteMenu->getChildByTag(TYPE_NOTE_DISTANCE));
        floatObject = Float::create(noteDistance->getValue());
        noteDict->setObject(floatObject, "noteDistance");
        
        auto particleScale = dynamic_cast<cocos2d::extension::ControlSlider*>(noteMenu->getChildByTag(TYPE_PARTICLE_SCALE));
        floatObject = Float::create(particleScale->getValue());
        noteDict->setObject(floatObject, "particleScale");
        
        auto sheepPanDistanceScale = dynamic_cast<cocos2d::extension::ControlSlider*>(noteMenu->getChildByTag(TYPE_SHEEP_PAN_DISTANCE));
        floatObject = Float::create(sheepPanDistanceScale->getValue());
        noteDict->setObject(floatObject, "sheepPanDistance");
        
        auto noteDurationScale = dynamic_cast<cocos2d::extension::ControlSlider*>(noteMenu->getChildByTag(TYPE_NOTE_DURATION));
        floatObject = Float::create(noteDurationScale->getValue());
        noteDict->setObject(floatObject, "noteDuration");
    }
    else
    {
        auto value = Utility::loadDataFromFile("Note", "noteDistance", "9.5");
        floatObject = Float::create(atof(value->getCString()));
        noteDict->setObject(floatObject, "noteDistance");
        
        value = Utility::loadDataFromFile("Note", "particleScale", "1.25");
        floatObject = Float::create(atof(value->getCString()));
        noteDict->setObject(floatObject, "particleScale");
        
        value = Utility::loadDataFromFile("Note", "sheepPanDistance", "1.5");
        floatObject = Float::create(atof(value->getCString()));
        noteDict->setObject(floatObject, "sheepPanDistance");
        
        value = Utility::loadDataFromFile("Note", "noteDuration", "10.0");
        floatObject = Float::create(atof(value->getCString()));
        noteDict->setObject(floatObject, "noteDuration");
    }
    
    root->setObject(noteDict, "Note");
    
#pragma mark GOATSKIN
    auto goatskinDict = Dictionary::create();
    auto goatskinMenu = dynamic_cast<GoatskinMenuItem*>(this->getChildByTag(GOATSKIN_MENU));
    
    if (goatskinMenu)
    {
        auto goatskinRange = dynamic_cast<cocos2d::extension::ControlSlider*>(goatskinMenu->getChildByTag(TYPE_GOATSKIN_RANGE));
        floatObject = Float::create(goatskinRange->getValue());
        goatskinDict->setObject(floatObject, "range");
    }
    else
    {
        auto value = Utility::loadDataFromFile("Goatskin", "range", "200.0");
        floatObject = Float::create(atof(value->getCString()));
        goatskinDict->setObject(floatObject, "range");
    }
    
    root->setObject(goatskinDict, "Goatskin");
    
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fullPath = writablePath + "SOP.plist";
    root->writeToFile(fullPath.c_str());
}

#endif // EDITOR_MODE