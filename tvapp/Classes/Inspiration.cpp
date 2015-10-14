//
//  Inspiration.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 09/06/15.
//
//

#include "Inspiration.h"
#include "SoundComponent.h"
#include "LHUserPropertyProtocol.h"

void Inspiration::handleStartInspiration(LHNodeProtocol *node)
{
    auto nodeArray = node->getChildrenWithTags({inspirationDefs::NODE_TAG});
    Ref* ref = nullptr;
    LHSprite* sprite = nullptr;
    
    // Audio
    static const char * soundComponentName = "soundComponentChild";
    Node * nodeObj = LHNodeProtocol::LHGetNode(node);
    SoundComponent * soundComponent = nodeObj->getChildByName<SoundComponent *>(soundComponentName);
    if (!soundComponent)
    {
        InspirationAudio * audioProperty = dynamic_cast<InspirationAudio *>(node->getUserProperty());
        if (audioProperty)
        {
            soundComponent = SoundComponent::create(audioProperty->getSoundName());
            soundComponent->setName(soundComponentName);
            nodeObj->addChild(soundComponent);
        }
    }
    if (soundComponent)
    {
        soundComponent->playOnce();
    }
    
    // Animazione
    CCARRAY_FOREACH(nodeArray, ref)
    {
        sprite = dynamic_cast<LHSprite*>(ref);
    
        if (sprite)
        {
            auto newAnimation = sprite->getAnimationWithName(inspirationDefs::START_ANIMATION);
            if (newAnimation)
            {
                newAnimation->setActive(true);
                newAnimation->restart();
            }
        }
    }
}

void Inspiration::handleEndInspiration(LHAnimation *animation)
{
    if (animation->name() == inspirationDefs::START_ANIMATION)
    {
        auto sprite = dynamic_cast<LHSprite*>(animation->node());
        if (sprite)
        {
            auto newAnimation = sprite->getAnimationWithName(inspirationDefs::END_ANIMATION);
            if (newAnimation)
            {
                newAnimation->setActive(true);
                newAnimation->restart();
            }
        }
    }
}