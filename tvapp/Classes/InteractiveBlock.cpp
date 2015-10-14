//
//  InteractiveBlock.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 09/12/14.
//
//

#include "InteractiveBlock.h"
#include "Utility.h"
#include "Player.h"
#include "Brazier.h"
#include "SoundComponent.h"
#include "AnimationComponent.h"

bool InteractiveBlock::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHSprite::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _observers.clear();
        _movablePlatform = nullptr;
        _soundFall = SoundComponent::create(SoundComponent::SOUND_INTERACTIVE_FALL);
        _isFalling = false;
        _eoloStartOffset = Point(0.0f, 0.0f);
        _windRootNode = nullptr;
        
        this->addChild(_soundFall);
        this->scheduleUpdate();
    }
    
    return status;
}

void InteractiveBlock::setIsOnMovablePlatform(LHNodeProtocol* platform, bool begin)
{
    if (begin)
    {
        _movablePlatform = platform;
    }
    else
    {
        _movablePlatform = nullptr;
    }
}

void InteractiveBlock::update(float dt)
{
    this->updateDistanceFromPlayer();
    this->updateFalling();
}

void InteractiveBlock::updateDistanceFromPlayer()
{
    auto blockAsset = dynamic_cast<LHAsset*>(this->getParent());
    
    if (blockAsset)
    {
        b2Body* body = this->getBox2dBody();
        b2Vec2 playerBodyPosition = _player->getBox2dBody()->GetPosition();
        b2Vec2 blockBodyPosition = body->GetPosition();
        b2Vec2 distanceBodyVector = blockBodyPosition - playerBodyPosition;
        
        const float kPixelsPerMeter = PTM_RATIO;
        float widthFactor = ((_player->getContentSize().width * fabs(_player->getScaleX())) / 2) + ((blockAsset->getContentSize().width * fabs(blockAsset->getScaleX())) / 2);
        float heightFactor = ((_player->getContentSize().height * fabs(_player->getScaleY())) / 2) + ((blockAsset->getContentSize().height * fabs(blockAsset->getScaleY())) / 2);
        widthFactor /= kPixelsPerMeter;
        heightFactor /= kPixelsPerMeter;
        
        if ((fabs(distanceBodyVector.x) > (widthFactor + 0.1f + fabs(sin(body->GetAngle()))))
            || ((fabs(distanceBodyVector.y) > heightFactor) && fabs(sin(body->GetAngle())) < .1f ))
        {
            float horizontalVelocity = body->GetLinearVelocity().x;
            
            if (fabs(horizontalVelocity) > 0.1f)
            {
                Utility::setDensity(this, 1.0f);
                body->SetGravityScale(10.0f);
                
                if (_player->getIsPushingLeft())
                {
                    _player->setPushingSide(-1.0f, false);
                }
                else if (_player->getIsPushingRight())
                {
                    _player->setPushingSide(1.0, false);
                }
            }
        }
    }
}

void InteractiveBlock::updateFalling()
{
    auto blockAsset = dynamic_cast<LHAsset*>(this->getParent());
    
    if (blockAsset)
    {
        b2Body* body = this->getBox2dBody();
        float linearVelocityY = fabs(body->GetLinearVelocity().y);
        
        if ( linearVelocityY >= 25.0f && !_isFalling)
        {
            CCLOG("Interactive sta cadendo!!!");
            
            _isFalling = true;
            this->notify(eEventType::INTERACTIVE_FALL);
        }
        if ( linearVelocityY < 1.0f && _isFalling)
        {
            _isFalling = false;
 //           this->playFallingSound();
        }
        
        if (_movablePlatform)
        {
            auto movablePhysic = dynamic_cast<LHPhysicsProtocol*>(_movablePlatform);
            
            if (movablePhysic)
            {
                this->getBox2dBody()->SetLinearVelocity(b2Vec2(this->getBox2dBody()->GetLinearVelocity().x,
                                                               movablePhysic->getBox2dBody()->GetLinearVelocity().y));
            }
        }
    }
}

void InteractiveBlock::playFallingSound()
{
    if (!this->containsTag(gameTags::GOATSKIN_TAG) && _isFalling)
    {
        _soundFall->playOnce();
        _isFalling = false;
    }
}

#pragma mark - Observer Pattern

void InteractiveBlock::addObserver(Observer *observer)
{
    _observers.push_back(observer);
}

void InteractiveBlock::removeObserver(Observer *observer)
{
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}

void InteractiveBlock::notify(eEventType event)
{
    auto observersCopy = _observers;
    for (std::vector<Observer*>::const_iterator iterator = observersCopy.begin(); iterator != observersCopy.end(); ++iterator)
    {
        if (*iterator != NULL)
        {
            (*iterator)->update(event, this);
        }
    }
}

void InteractiveBlock::setUnderEoloWind(LHNode* node, bool begin)
{
    if (begin)
    {
        auto asset = dynamic_cast<LHAsset*>(this->getParent());
        if (asset)
        {
            auto brazier = dynamic_cast<Brazier*>(asset->getChildNodeWithName(brazierDefs::BRAZIER_SHAPE));
            if (brazier)
            {
                brazier->setUnderEoloEffect();
            }
        }
        
        auto gameWorld = node->getParent();
        auto objectAsset = this->getParent();
        
        if (gameWorld && objectAsset)
        {
            Point screenCoord = gameWorld->convertToWorldSpace(node->getPosition());
            Point relativePosition = objectAsset->convertToNodeSpace(screenCoord);
            
            _eoloStartOffset = this->getPosition() - relativePosition;
            
            this->startWindAnimation();
        }
    }
}

void InteractiveBlock::updateUnderEoloWind(LHNode *node)
{
    auto gameWorld = node->getParent();
    auto objectAsset = this->getParent();
    
    if (gameWorld && objectAsset)
    {
        Point screenCoord = gameWorld->convertToWorldSpace(node->getPosition());
        Point relativePosition = objectAsset->convertToNodeSpace(screenCoord);

        relativePosition += _eoloStartOffset;
        this->setPosition(relativePosition);
        
        auto asset = dynamic_cast<LHAsset*>(this->getParent());
        if (asset)
        {
            auto brazier = dynamic_cast<Brazier*>(asset->getChildNodeWithName(brazierDefs::BRAZIER_SHAPE));
            if (brazier)
            {
                brazier->setPosition(relativePosition + Point(0.f, this->getContentSize().height));
            }
        }
    }
}

void InteractiveBlock::exitUnderEoloWind()
{
    this->stopWindAnimation();
}

void InteractiveBlock::startWindAnimation()
{
    static const float minHorizontalDistance = 20.0f;
    static const float maxHorizontalDistance = 80.0f;
    static const float minRotation = 10.0f;
    static const float maxRotation = 30.0f;
    static const float easeRate = 2.0f;
    
    float randomFactor = CCRANDOM_0_1();
    float horizontalDistance = randomFactor * ( maxHorizontalDistance - minHorizontalDistance ) + minHorizontalDistance;
    float rotation = randomFactor * ( maxRotation - minRotation ) + minRotation;
    float timeFactor = horizontalDistance / 1000.0f;
    
    this->stopWindAnimation();
    
    this->setOpacity(0);
    
    if (!_windRootNode)
    {
        _windRootNode = Node::create();
        _windRootNode->setPosition( - _eoloStartOffset + Point(this->getContentSize() / 2.0f));
        this->addChild(_windRootNode);
        
        Sprite * spriteWind = Sprite::createWithSpriteFrameName(this->getSpriteFrameName());
        spriteWind->setPosition(Point::ZERO);
        _windRootNode->addChild(spriteWind);
        
        ActionInterval * start;
        ActionInterval * middle;
        ActionInterval * end;
        
        // Oscillazione orizzontale per tornado che cresce
        
        start = EaseOut::create(MoveTo::create(2.0f * timeFactor, Point(horizontalDistance / 4.0f, spriteWind->getPositionY())), easeRate);
        middle = EaseInOut::create(MoveTo::create(4.0f * timeFactor, Point( - horizontalDistance / 2.0f, spriteWind->getPositionY())), easeRate);
        end = EaseIn::create(MoveTo::create(2.0f * timeFactor, Point(0.0f, spriteWind->getPositionY())), easeRate);
        Sequence * bounceStart = Sequence::create(start, middle, end, NULL);
        
        // Oscillazione orizzontale per tornado che loopa
        start = EaseOut::create(MoveTo::create(2.0f * timeFactor, Point(horizontalDistance, spriteWind->getPositionY())), easeRate);
        middle = EaseInOut::create(MoveTo::create(4.0f * timeFactor, Point( - horizontalDistance, spriteWind->getPositionY())), easeRate);
        end = EaseIn::create(MoveTo::create(2.0f * timeFactor, Point(0.0f, spriteWind->getPositionY())), easeRate);
        Sequence * bounceLoop = Sequence::create(start, middle, end, NULL);
        
        // Uso un Repeat perché il RepeatForever non funziona all'interno di una Sequence
        spriteWind->runAction(Sequence::create(bounceStart, Repeat::create(bounceLoop, 10), NULL));
        
        // Oscillazione di rotazione per tornado che cresce
        start = EaseOut::create(RotateTo::create(2.0f * timeFactor, - 90.0f + rotation / 4.0f), easeRate);
        middle = EaseInOut::create(RotateTo::create(4.0f * timeFactor, - 90.0 -  + rotation / 2.0f), easeRate);
        end = EaseIn::create(RotateTo::create(2.0f * timeFactor, - 90.0f), easeRate);
        Sequence * rotateStart = Sequence::create(start, middle, end, NULL);
        
        // Oscillazione di rotazione per tornado che loopa
        start = EaseOut::create(RotateTo::create(2.0f * timeFactor, - 90.0f + rotation), easeRate);
        middle = EaseInOut::create(RotateTo::create(4.0f * timeFactor, - 90.0f - rotation), easeRate);
        end = EaseIn::create(RotateTo::create(2.0f * timeFactor, - 90.0f), easeRate);
        Sequence * rotateLoop = Sequence::create(start, middle, end, NULL);
        
        // Uso un Repeat perché il RepeatForever non funziona all'interno di una Sequence
        spriteWind->runAction(Sequence::create(rotateStart, Repeat::create(rotateLoop, 10), NULL));
    }
}

void InteractiveBlock::stopWindAnimation()
{
    if (_windRootNode)
    {
        _windRootNode->removeFromParentAndCleanup(true);
        _windRootNode = nullptr;
    }
    this->setOpacity(255);
}
