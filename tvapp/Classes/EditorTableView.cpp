//
//  EditorTableView.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 06/11/14.
//
//

#ifdef EDITOR_MODE

#include "EditorTableView.h"
#include "VisibleRect.h"
#include "dirent.h"
#include "GameLogic.h"
#include "GameContext.h"
#include "SoundManager.h"
#include "SettingsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool EditorTableView::init()
{
    if ( !Layer::init())
    {
        return false;
    }
    
    if(!populateLevelList())
        return false;
    
    // Inizializzo il SoundManager
    if (SettingsManager::getInstance()->loadUserActiveMusic())
    {
        SoundManager::getInstance()->setUserVolumeMusic(SettingsManager::getInstance()->loadUserVolumeMusic(), false);
    }
    else
    {
        SoundManager::getInstance()->setUserVolumeMusic(0.0f, false);
    }
    
    if (SettingsManager::getInstance()->loadUserActiveFX())
    {
        SoundManager::getInstance()->setUserVolumeFX(SettingsManager::getInstance()->loadUserVolumeFX(), false);
    }
    else
    {
        SoundManager::getInstance()->setUserVolumeFX(0.0f, false);
    }
    
    SoundManager::getInstance()->preloadAudio();
    
    Size winSize = Director::getInstance()->getWinSize();
    
    TableView* tableView = TableView::create(this, Size(500, 500));
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    tableView->setPosition(Vec2(winSize.width/2.5,winSize.height/8));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::BOTTOM_UP);
    this->addChild(tableView);
    tableView->reloadData();
    
    glClearColor(0, 0, 0, 1.0f);
    
    return true;
}

void EditorTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
    
    std::string level(_levelList[cell->getIdx()].data());
    
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir ("./publishedResources")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            std::string tmp(ent->d_name);
            if (tmp.find(level.c_str()) != std::string::npos)
            {
                CCLOG("%s\n", tmp.c_str());
                
                GameContext context(level, "");
                
                auto scene = GameLogic::create(context);
                Director::getInstance()->replaceScene(scene);
            }
        }
        closedir (dir);
    }
    else
    {
        /* could not open directory */
        perror ("");
    }

}

Size EditorTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(100, 100);
}

TableViewCell* EditorTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    CCLOG("%s", const_cast<char*>(_levelList[idx].data()));

    auto string = String::createWithFormat("%s", const_cast<char*>(_levelList[idx].data()));
    TableViewCell *cell = table->dequeueCell();
    if (!cell)
    {
        cell = new (std::nothrow) CustomTableViewCell();
        cell->autorelease();

        auto label = Label::createWithSystemFont(string->getCString(), "Helvetica", 20.0);
        label->setColor(Color3B::GRAY);
        label->setPosition(Vec2::ZERO);
        label->setAnchorPoint(Vec2::ZERO);
        label->setTag(123);
        cell->addChild(label);
    }
    else
    {
        auto label = dynamic_cast<Label*>(cell->getChildByTag(123));
        label->setString(string->getCString());
    }
    
    
    return cell;
}

ssize_t EditorTableView::numberOfCellsInTableView(TableView *table)
{
    return _levelList.size();
}

bool EditorTableView::populateLevelList()
{
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir ("./publishedResources")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            std::string tmp(ent->d_name);
            if (tmp.find(".lhplist") != std::string::npos)
            {
                _levelList.push_back(tmp);
                CCLOG("%s\n", tmp.c_str());
            }
        }
        closedir (dir);
        
        return true;
    }
    else
    {
        /* could not open directory */
        perror ("");
        return false;
    }
}

void CustomTableViewCell::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    TableViewCell::draw(renderer, transform, flags);
}

#endif // EDITOR_MODE
