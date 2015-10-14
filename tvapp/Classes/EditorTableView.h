//
//  EditorTableView.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 06/11/14.
//
//

#ifdef EDITOR_MODE

#ifndef __SOP_EDITOR_TABLE_VIEW__
#define __SOP_EDITOR_TABLE_VIEW__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class EditorTableView : public cocos2d::Layer, cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    virtual bool init();
    
    CREATE_FUNC(EditorTableView);
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
private:
    bool populateLevelList();
    
    std::vector<std::string> _levelList;
};

class CustomTableViewCell : public cocos2d::extension::TableViewCell
{
public:
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
};

#endif // __SOP_EDITOR_TABLE_VIEW__

#endif // EDITOR_MODE