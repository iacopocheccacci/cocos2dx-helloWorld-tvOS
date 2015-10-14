//
//  Note.cpp
//  SOP_Proto
//
//  Created by Giuseppe Lapenta on 18/11/14.
//
//

#include "Note.h"
#include "Goatskin.h"
#include "Utility.h"

const bool Note::NOTE_REGENERATION = true;

bool Note::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        // do nothing
    }
    
    return status;
}

void Note::moveIfInWindRange(Goatskin* source)
{
    if (Utility::isInXrange(source, this) && Utility::isInYrange(source, this))
    {
        Point point(0.0f, 0.0f);
        float movementSize = source->getRange();
        
        if (source->containsTag(gameTags::DIRECTION_RIGHT_TAG))
        {
            float sourcePosX = (source->getBox2dBody()->GetPosition().x * PTM_RATIO) + (source->getContentSize().width / 4);
            float notePosY = this->getBox2dBody()->GetPosition().y * PTM_RATIO;
            
            point = Point(sourcePosX + movementSize, notePosY);
        }
        else if (source->containsTag(gameTags::DIRECTION_LEFT_TAG))
        {
            float sourcePosX = (source->getBox2dBody()->GetPosition().x * PTM_RATIO) - (source->getContentSize().width / 4);
            float notePosY = this->getBox2dBody()->GetPosition().y * PTM_RATIO;
            
            point = Point(sourcePosX - movementSize, notePosY);
        }
        else if (source->containsTag(gameTags::DIRECTION_UP_TAG))
        {
            float sourcePosY = (source->getBox2dBody()->GetPosition().y * PTM_RATIO) + (source->getContentSize().width / 4);
            float notePosX = this->getBox2dBody()->GetPosition().x * PTM_RATIO;
            
            point = Point(notePosX, sourcePosY + movementSize);
        }
        
        if (point != Point(0.0f, 0.0f))
        {
            moveByWind(point);
        }
    }
}

void Note::moveByWind(Point destination)
{
    if (this->getBox2dBody()->IsActive())
    {
        this->getParent()->runAction(MoveTo::create(0.3f, destination));
    }
}

void Note::handleNotePicked()
{
    if (NOTE_REGENERATION)
    {
        this->getBox2dBody()->SetActive(false);
        this->setCascadeOpacityEnabled(true);
        this->setOpacity(0);
        CallFuncN * regenerateNoteCallback = CallFuncN::create([this](Node * noteNode)
        {
            this->regenerateNote(false);
        });
        
        auto sequence = Sequence::create(DelayTime::create(_regenerationTime), regenerateNoteCallback, NULL);
        sequence->setTag(CURRENT_ACTION);
        this->runAction(sequence);
    }
    else
    {
        this->removeFromParent();
    }
}

void Note::regenerateNote(bool manual)
{
    if (!this->getBox2dBody()->IsActive())
    {
        CallFunc * interactionNoteCallback = CallFunc::create([this]
        {
            this->getBox2dBody()->SetActive(true);
        });
        
        if (manual)
        {
            auto oldSequence = this->getActionByTag(CURRENT_ACTION);
            if (oldSequence)
            {
                this->stopActionByTag(CURRENT_ACTION);
            }
        }
        
        this->runAction(Sequence::create(FadeIn::create(0.5f), interactionNoteCallback, NULL));
    }
}