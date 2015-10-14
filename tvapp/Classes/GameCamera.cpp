//
//  GameCameraNew.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 02/04/15.
//
//

#include "GameCamera.h"

GameCamera::~GameCamera()
{
    
}

GameCamera::GameCamera(Node * target, const Rect worldBoundaries, const bool clampOnBoundaries)
: _target(target)
, _clampOnBoundaries(clampOnBoundaries)
, _softBoundariesX(false)
, _softBoundariesY(false)
, _softBoundariesScheduledX(false)
, _softBoundariesScheduledY(false)

, _followedNode(nullptr)
, _followEnabled(false)
, _followedOffset(Point::ZERO)
, _followSpeed(10.0f)
, _followSmoothMovement(false)
, _followBoundingArea(Rect::ZERO)

, _zoomDestination(1.0f)
, _zoomSpeed(10.0f)
, _zoomSmoothMovement(false)

, _panningDestination(Point::ZERO)
, _panningSpeed(10.0f)
, _panningSmoothMovement(false)

,  _attractorsEnabled(false)
,  _railsEnabled(false)

, _debugEnabled(false)
{
    _screenOrigin = Director::getInstance()->getVisibleOrigin();
    _screenSize = Director::getInstance()->getVisibleSize();
    _screenCenter = Size(_screenOrigin.x + _screenSize.width / 2.0f, _screenOrigin.y + _screenSize.height / 2.0f);
    this->setWorldBoundaries(worldBoundaries);
}

void GameCamera::setWorldBoundaries(const Rect worldBoundaries)
{
    Point origin = worldBoundaries.origin;
    Size size = worldBoundaries.size;
    if (worldBoundaries.size.width < _screenSize.width || worldBoundaries.size.height < _screenSize.height)
    {
        origin = _screenOrigin;
        size = _screenSize;
    }
    _worldBoundaries = Rect(origin.x, origin.y, size.width, size.height);
}

float GameCamera::getMinZoomScaleForBoundaries()
{
    return this->getMinZoomScaleForSize(_worldBoundaries.size);
}

float GameCamera::getMinZoomScaleForSize(Size size)
{
    return fmax(_screenSize.width/size.width, _screenSize.height/size.height);
}

void GameCamera::setSoftBoundariesX(const bool soft)
{
    if (true == soft)
    {
        _softBoundariesScheduledX = true;
    }
    else
    {
        _softBoundariesScheduledX = false;
        _softBoundariesX = false;
    }
}

void GameCamera::setSoftBoundariesY(const bool soft)
{
    if (true == soft)
    {
        _softBoundariesScheduledY = true;
    }
    else
    {
        _softBoundariesScheduledY = false;
        _softBoundariesY = false;
    }
}

float GameCamera::getCurrentZoom()
{
    if (_target)
    {
        return _target->getScale();
    }
    else
    {
        return 0.0f;
    }
}

Point GameCamera::getCurrentPosition()
{
    if (_target)
    {
        return _target->convertToNodeSpace(_screenCenter);
    }
    else
    {
        return Point::ZERO;
    }
}

#pragma mark -
#pragma mark - Servizi
#pragma mark -

#pragma mark - Follow

void GameCamera::setFollowNode(Node * followedNode, const float speed, const bool smoothMovement)
{
    _followedNode = followedNode;
    _followSpeed = fabsf(speed);
    _followSmoothMovement = smoothMovement;
}

void GameCamera::setFollowedNodeOffset(const Point offset)
{
    _followedOffset = offset;
}

void GameCamera::setFollowNode(Node * followedNode)
{
    _followedNode = followedNode;
}

void GameCamera::setFollowedNodeSpeed(const float speed, const bool smoothMovement)
{
    _followSpeed = fabsf(speed);
    _followSmoothMovement = smoothMovement;
}

void GameCamera::setFollowedArea(const Rect followArea)
{
    _followBoundingArea = followArea;
}

void GameCamera::setFollowEnabled(const bool followEnabled)
{
    _followEnabled = followEnabled;
    
    // Punto fin da subito al followNode, così da evitare scatti (es.: dovuti alla bounding area)
    this->resetFollow();
}

void GameCamera::resetFollow()
{
    if (_followEnabled)
    {
        this->setPanningDestinationX(this->getFollowedNodePositionOnTarget().x);
        this->setPanningDestinationY(this->getFollowedNodePositionOnTarget().y);
    }
}

#pragma mark - Zooming

void GameCamera::zoomToScale(const float scale, const float speed, const bool smoothMovement)
{
    _zoomDestination = this->zoomClampToSizeForScale(scale);
    _zoomSpeed = fabsf(speed);
    _zoomSmoothMovement = smoothMovement;
}

void GameCamera::setZoomScale(const float scale)
{
    _zoomDestination = scale;
}

void GameCamera::setZoomSpeed(const float speed, const bool smoothMovement)
{
    _zoomSpeed = fabsf(speed);
    _zoomSmoothMovement = smoothMovement;
}

void GameCamera::forceZoomToScale(const float scale)
{
    _zoomDestination = this->zoomClampToSizeForScale(scale);
    this->setScaleAndAdjustPosition(_zoomDestination);
}

#pragma mark - Panning

void GameCamera::panToLocation(const Point location, const float speed, const bool smoothMovement)
{
    this->setPanningDestinationX(location.x);
    this->setPanningDestinationY(location.y);
    _panningSpeed = fabsf(speed);
    _panningSmoothMovement = smoothMovement;
}

void GameCamera::centerCameraOnNode(const Node * node, const Point offset)
{
    if (nullptr == node || nullptr == node->getParent()) return;

    Point locationOnTarget = _target->convertToNodeSpace(node->convertToWorldSpace(Point(node->getContentSize()/2.0f) + offset));
//    Point locationOnTarget = _target->convertToNodeSpace(node->getParent()->convertToWorldSpace(node->getPosition() + offset));
    
    this->setPanningDestinationX(locationOnTarget.x);
    this->setPanningDestinationY(locationOnTarget.y);
    
    Point screenCenterOnTarget = _target->convertToNodeSpace(_screenCenter);
    Point panningDirection = (_panningDestination - screenCenterOnTarget) * _target->getScale();
    
    this->setTargetPosition(_target->getPosition() - panningDirection);
}

#pragma mark - Attractors

void GameCamera::addAttractor(const Point position, const Size targetScreenSize, const float innerRadius, const float outerRadius, const SizeConstraint sizeConstraint, const std::string key)
{
    float zoom = 0.0f;
    if (SizeConstraint::WIDTH == sizeConstraint)
    {
        zoom = this->getMinZoomScaleForSize(Size(targetScreenSize.width, FLT_MAX));
    }
    else if (SizeConstraint::HEIGHT == sizeConstraint)
    {
        zoom = this->getMinZoomScaleForSize(Size(FLT_MAX, targetScreenSize.height));
    }
    else
    {
        float minZoomWidth = this->getMinZoomScaleForSize(Size(targetScreenSize.width, FLT_MAX));
        float minZoomHeight = this->getMinZoomScaleForSize(Size(FLT_MAX, targetScreenSize.height));
        zoom = fminf(minZoomWidth, minZoomHeight);
    }
    this->addAttractor(position, zoom, innerRadius, outerRadius, key);
}

void GameCamera::addAttractor(const Point position, const float zoom, const float innerRadius, const float outerRadius, const std::string key)
{
    if (zoom > 0.0f && innerRadius > 0.0f && outerRadius >= innerRadius && false == key.empty())
    {
        _attractors[key] = std::shared_ptr<Attractor>(new Attractor(position, zoom, innerRadius, outerRadius));
        if (_debugEnabled)
        {
            CCLOG("Camera Attractor %s aggiunto:\nPosizione: (%.2f, %.2f)\nZoom: %.2f\nInnerRadius: %.2f\nOuterRadius: %.2f", key.c_str(), position.x, position.y, zoom, innerRadius, outerRadius);
        }
    }
    else
    {
        CCLOG("Impossibile aggiungere attrattore! Dati non validi");
    }
}

void GameCamera::removeAttractor(const std::string key)
{
    _attractors.erase(key);
    if (_debugEnabled && _target)
    {
        if (_target->getChildByName(key + "inner"))
        {
            _target->removeChildByName(key + "inner");
        }
        if (_target->getChildByName(key + "outer"))
        {
            _target->removeChildByName(key + "outer");
        }
    }
}

void GameCamera::setAttractorsEnabled(const bool enabled)
{
    _attractorsEnabled = enabled;
}

#pragma mark - Rails

void GameCamera::addRail(const Rect areaOfEffect, const Point startProjectionPoint, const Point endProjectionPoint, const std::vector<Point> pathPoints, const Size pathScreenSize, const std::string key)
{
    if (pathPoints.size() > 0 && false == key.empty())
    {
        float zoom;
        if (pathScreenSize.width == 0 && pathScreenSize.height == 0)
        {
            zoom = -1;
        }
        else
        {
            zoom = this->getMinZoomScaleForSize(pathScreenSize);
        }
        
        _rails[key] = std::shared_ptr<Rail>(new Rail(areaOfEffect, startProjectionPoint, endProjectionPoint, pathPoints, zoom));
        if (_debugEnabled)
        {
            CCLOG("Camera Rail %s aggiunto:\nArea di influenza: (%.2f, %.2f, %.2f, %.2f)\nPunto di proiezione iniziale: (%.2f, %.2f)\nPunto di proiezione finale: (%.2f, %.2f)\nNumero di punti del binario: %d", key.c_str(), areaOfEffect.origin.x, areaOfEffect.origin.y, areaOfEffect.size.width, areaOfEffect.size.height, startProjectionPoint.x, startProjectionPoint.y, endProjectionPoint.x, endProjectionPoint.y, (int)pathPoints.size());
        }
    }
    else
    {
        CCLOG("Impossibile aggiungere binario! Dati non validi");
    }
}

void GameCamera::removeRail(const std::string key)
{
    _rails.erase(key);
    if (_debugEnabled && _target)
    {
        
    }
}

void GameCamera::setRailsEnabled(const bool enabled)
{
    _railsEnabled = enabled;
}

#pragma mark - General

void GameCamera::update(float dt)
{
    dt = 0.016f;
    
    if (_target == nullptr) return;
    
    this->updateFollow(dt);
    //Essendo dopo il updateFollow, la bounding area viene ignorata, se si è nell'area di influenza di un rail
    this->updateRails(dt);
    this->updateAttractors(dt);
    this->updateZoom(dt);
    this->updatePanning(dt);
    
    if(_clampOnBoundaries)
    {
        this->handleBoundaries();
    }
    
    // Debug
    this->updateDebug();
}

#pragma mark -
#pragma mark - Metodi privati
#pragma mark -

#pragma mark - Updates

void GameCamera::updateFollow(float dt)
{
    if (_followEnabled)
    {
        _panningSmoothMovement = _followSmoothMovement;
        _panningSpeed = _followSpeed;
        
        if (false == this->checkFollowedNodeInAreaX())
        {
            this->setPanningDestinationX(this->getFollowedNodePositionOnTarget().x);
        }
        if (false == this->checkFollowedNodeInAreaY())
        {
            this->setPanningDestinationY(this->getFollowedNodePositionOnTarget().y);
        }
    }
}

void GameCamera::updateZoom(float dt)
{
    float zoomGap = _zoomDestination - _target->getScale();
    
    // Calcolo lo step di zoom a questo giro
    float zoomStep;
    if (_zoomSmoothMovement)
    {
        zoomStep = fabsf(zoomGap) * _zoomSpeed * dt;
    }
    else
    {
        zoomStep = _zoomSpeed * dt;
    }
    
    // Se sto facendo zoom out, devo sottrarre
    if (zoomGap < 0.0f)
    {
        zoomStep *= -1.0f;
    }
    
    // Se devo ancora zoomare lo faccio, altrimenti setto lo zoom del target allo zoom atteso
    if (fabsf(zoomGap) > fabsf(zoomStep))
    {
        this->setScaleAndAdjustPosition(_target->getScale() + zoomStep);
    }
    else
    {
        this->setScaleAndAdjustPosition(_zoomDestination);
    }
}

void GameCamera::updatePanning(float dt)
{
    Point screenCenterOnTarget = _target->convertToNodeSpace(_screenCenter);
    Point panningDirection = (_panningDestination - screenCenterOnTarget);// * _target->getScale();
    float panningGap = panningDirection.length() * _target->getScale();
    
    // Calcolo lo step di panning a questo giro
    float panningStep;
    if (_panningSmoothMovement)
    {
        panningStep = panningGap * _panningSpeed * dt;
    }
    else
    {
        panningStep = _panningSpeed * dt;
    }
    
    panningDirection.normalize();
    panningDirection *= panningStep;
    
    if (panningStep < panningGap)
    {
        this->setTargetPosition(_target->getPosition() - panningDirection);
    }
}

void GameCamera::updateAttractors(float dt)
{
    if (false == _attractorsEnabled) return;
    
    Point attractedPosition = _panningDestination;
    float attractedZoom = _zoomDestination;
    
    bool insideAttractor = false;
    
    for (auto iter = _attractors.begin(); iter != _attractors.end(); iter++)
    {
        Point currentAttractedPosition = _panningDestination;
        float currentAttractedZoom = _zoomDestination;
        
        Attractor * attractor = (*iter).second.get();
        Point followedNodeScreenPosition = _followedNode->getParent()->convertToWorldSpace(_followedNode->getPosition());
        Point followedNodePosition = _target->convertToNodeSpace(followedNodeScreenPosition);
        float distanceToAttractor = (attractor->getPosition() - followedNodePosition).length();
        
        if (distanceToAttractor < attractor->getOuterRadius())
        {
            if (distanceToAttractor < attractor->getInnerRadius())
            {
                currentAttractedPosition = attractor->getPosition();
                currentAttractedZoom = attractor->getZoom();
            }
            else
            {
                float alpha = 1.0f - (distanceToAttractor - attractor->getInnerRadius())/(attractor->getOuterRadius() - attractor->getInnerRadius());
                
                currentAttractedPosition = _panningDestination.lerp(attractor->getPosition(), alpha);
                currentAttractedZoom = MathUtil::lerp(_zoomDestination, attractor->getZoom(), alpha);
            }
            
            if (false == insideAttractor)
            {
                attractedPosition = currentAttractedPosition;
                attractedZoom = currentAttractedZoom;
            }
            else
            {
                attractedPosition = attractedPosition.getMidpoint(currentAttractedPosition);
                attractedZoom = ( attractedZoom + currentAttractedZoom ) / 2.0f;
            }
            
            insideAttractor = true;
        }
    }
    
    this->setPanningDestinationX(attractedPosition.x);
    this->setPanningDestinationY(attractedPosition.y);
    _zoomDestination = this->zoomClampToSizeForScale(attractedZoom);
}

void GameCamera::updateRails(float dt)
{
    if (false == _railsEnabled) return;
    
    bool insideRailArea = false;
    Rail * rail = nullptr;
    
    Point followedNodeScreenPosition = _followedNode->getParent()->convertToWorldSpace(_followedNode->getPosition());
    Point followedNodePosition = _target->convertToNodeSpace(followedNodeScreenPosition);
    
    // Se il followed node è nell'area di più binari, prendo il primo che trovo
    auto iter = _rails.begin();
    while (false == insideRailArea && iter != _rails.end())
    {
        rail = (*iter).second.get();
        if (rail->isPointOnRail(followedNodePosition))
        {
            insideRailArea = true;
        }
        iter++;
    }
    
    if (insideRailArea)
    {
        Point position = this->getFollowedNodePositionOnTarget();
        bool res;
        
        Point newPositionOnRail = Point::ZERO;
        res = rail->getPositionOnRail(position, newPositionOnRail);
        if (res)
        {
            this->setPanningDestinationX(newPositionOnRail.x);
            this->setPanningDestinationY(newPositionOnRail.y);
        }
        
        float newZoomOnRail = 0;
        res = rail->getZoomOnRail(newZoomOnRail);
        if (res)
        {
            _zoomDestination = this->zoomClampToSizeForScale(newZoomOnRail);
        }
    }
}

#pragma mark - Utilities

void GameCamera::handleBoundaries()
{
    if (_target == nullptr)
    {
        return;
    }
    
    float worldOriginX = _worldBoundaries.origin.x;
    float worldOriginY = _worldBoundaries.origin.y;
    float worldWidthWithMargins = _worldBoundaries.size.width;
    float worldHeightWithMargins = _worldBoundaries.size.height;
    
    // Riposizionamento del target per compensare lo ZoomOut
    Point screenTargetOrigin = _target->convertToWorldSpace(Point(worldOriginX, worldOriginY));
    Point screenTargetDestination = _target->convertToWorldSpace(Point(worldOriginX + worldWidthWithMargins,
                                                                       worldOriginY + worldHeightWithMargins));
    float overstep = 0.0f;
    
    // Controllo il margine sinistro
    if (screenTargetOrigin.x > _screenOrigin.x)
    {
        overstep = screenTargetOrigin.x - _screenOrigin.x;
        this->setTargetPosition(Point(_target->getPosition().x - overstep, _target->getPosition().y));
    }
    // Controllo il margine inferiore
    if (screenTargetOrigin.y > _screenOrigin.y)
    {
        overstep = screenTargetOrigin.y - _screenOrigin.y;
        this->setTargetPosition(Point(_target->getPosition().x, _target->getPosition().y - overstep));
    }
    // Controllo il margine destro
    if (screenTargetDestination.x < _screenOrigin.x + _screenSize.width)
    {
        overstep = _screenOrigin.x + _screenSize.width - screenTargetDestination.x;
        this->setTargetPosition(Point(_target->getPosition().x + overstep, _target->getPosition().y));
    }
    // Controllo il margine superiore
    if (screenTargetDestination.y < _screenOrigin.y + _screenSize.height)
    {
        overstep = _screenOrigin.y + _screenSize.height - screenTargetDestination.y;
        this->setTargetPosition(Point(_target->getPosition().x, _target->getPosition().y + overstep));
    }
}

float GameCamera::zoomClampToSizeForScale(const float scale)
{
    float minZoomScale = this->getMinZoomScaleForBoundaries();
    if ( scale < minZoomScale )
    {
        return minZoomScale;
    } else {
        return scale;
    }
}

void GameCamera::setScaleAndAdjustPosition(const float newScale)
{
    if (_target == nullptr)
    {
        return;
    }
    
    // Calcolo la posizione attuale del centro dello schermo nel sistema di coordinate del nodo target
    Point screenCenterOnTarget = _target->convertToNodeSpace(_screenCenter); // Cocos2D Target coordinates
    
    // Applico lo scale
    this->setTargetScale(newScale);
    
    // Calcolo la posizione del punto nel nodo target che corrispondeva al centro dello schermo e lo esprimo nelle
    // coordinate dello schermo (cioè della scena)
    Point nextScreenCenterOnView = _target->convertToWorldSpace(screenCenterOnTarget); // Cocos2D World Coordinates
    
    // Aggiusto la posizione del target della differenza di questi due punti per compensare la traslazione del punto osservato
    // a causa dello "stretch" del nodo target dovuto al cambio di scale
    this->setTargetPosition(_target->getPosition() + ( _screenCenter - nextScreenCenterOnView ));
    
}

bool GameCamera::checkFollowedNodeInAreaX()
{
    if (_target == nullptr)
    {
        return false;
    }
    
    Point screenFollowedNodePosition = _followedNode->getParent()->convertToWorldSpace(_followedNode->getPosition());
    
    // Controllo il margine sinistro
    if ( _followBoundingArea.origin.x - screenFollowedNodePosition.x > 0.0f )
    {
        return false;
    }
    // Controllo il margine destro
    if ( screenFollowedNodePosition.x - ( _followBoundingArea.size.width + _followBoundingArea.origin.x ) > 0.0f )
    {
        return false;
    }
    return true;
}

bool GameCamera::checkFollowedNodeInAreaY()
{
    if (_target == nullptr)
    {
        return false;
    }
    
    // Uso la posizione non basata sull'offset, per evitare un panning a singhiozzi in caso di movimenti verticali
    // lineari (esempio: piattaforme mobili, livello 2-1)
//    Point screenFollowedNodePosition = this->getFollowedNodePositionOnScreen();
    Point screenFollowedNodePosition = _followedNode->getParent()->convertToWorldSpace(_followedNode->getPosition());
    
    // Controllo il margine inferiore
    if (_followBoundingArea.origin.y - screenFollowedNodePosition.y > 0.0f )
    {
        return false;
    }
    // Controllo il margine superiore
    if ( screenFollowedNodePosition.y - ( _followBoundingArea.size.height + _followBoundingArea.origin.y ) > 0.0f )
    {
        return false;
    }
    return true;
}


#pragma mark - Wrappers

void GameCamera::setTargetPosition(const Point newPosition)
{
    _target->setPosition(newPosition);
}

void GameCamera::setTargetScale(const float newScale)
{
    _target->setScale(newScale);
}

void GameCamera::setPanningDestinationX(const float newDestinationX)
{
    float destinationX = newDestinationX;
    float scale = _target->getScale();
    
    float minX = _worldBoundaries.origin.x + _screenSize.width / 2.0f / scale;
    float maxX = _worldBoundaries.origin.x + _worldBoundaries.size.width - _screenSize.width / 2.0f / scale;
    float clampedX = fminf(maxX, fmaxf(minX, newDestinationX));
    if (true == _softBoundariesScheduledX && newDestinationX == clampedX)
    {
        // Se sono all'interno dell'area clamped orizzontalmente e avevo schedulato un soft boundaries X,
        // attivo il soft bounaries X e resetto il flag di schedule per la X
        _softBoundariesX = true;
        _softBoundariesScheduledX = false;
    }
    if (_softBoundariesX)
    {
        destinationX = clampedX;
    }
    
    _panningDestination = Point(destinationX, _panningDestination.y);
}

void GameCamera::setPanningDestinationY(const float newDestinationY)
{
    float destinationY = newDestinationY;
    float scale = _target->getScale();
    
    float minY = _worldBoundaries.origin.y + _screenSize.height / 2.0f / scale;
    float maxY = _worldBoundaries.origin.y + _worldBoundaries.size.height - _screenSize.height / 2.0f / scale;
    float clampedY = fminf(maxY, fmaxf(minY, newDestinationY));
    if (true == _softBoundariesScheduledY && newDestinationY == clampedY)
    {
        // Se sono all'interno dell'area clamped verticalmente e avevo schedulato un soft boundaries Y,
        // attivo il soft bounaries Y e resetto il flag di schedule per la Y
        _softBoundariesY = true;
        _softBoundariesScheduledY = false;
    }
    if (_softBoundariesY)
    {
        destinationY = clampedY;
    }
    
    _panningDestination = Point(_panningDestination.x, destinationY);
}

Point GameCamera::getFollowedNodePositionOnScreen()
{
    return _followedNode->getParent()->convertToWorldSpace(_followedNode->getPosition() + _followedOffset);
}

Point GameCamera::getFollowedNodePositionOnTarget()
{
    Point followedNodeOnWorld = this->getFollowedNodePositionOnScreen();
    return _target->convertToNodeSpace(followedNodeOnWorld);
}

#pragma mark - Draw Debug

void GameCamera::setDebugEnable(const bool enable)
{
    _debugEnabled = enable;
    if (enable)
    {
        this->initDebugDrawScreenCenter();
        this->initDebugDrawBoundingWorld();
        this->initDebugDrawFollowArea();
        this->initDebugDrawPanDestination();
    }
}

void GameCamera::initDebugDrawScreenCenter()
{
    if (_target)
    {
        Node * scene = _target->getParent();
        if (scene)
        {
            std::string drawNodeName = "cameraScreenCenter";
            DrawNode * drawNode = dynamic_cast<DrawNode *>(_target->getChildByName(drawNodeName));
            if (drawNode)
            {
                drawNode->removeFromParentAndCleanup(true);
            }
            else
            {
                drawNode = DrawNode::create();
                drawNode->drawDot(_screenCenter, 2.0f, Color4F(1, 1, 1, 1));
                scene->addChild(drawNode, 100, drawNodeName);
            }
        }
    }
}

void GameCamera::initDebugDrawBoundingWorld()
{
    if (_target)
    {
        std::string drawNodeName = "cameraBoundingWorld";
        DrawNode * drawNode = dynamic_cast<DrawNode *>(_target->getChildByName(drawNodeName));
        if (drawNode)
        {
            drawNode->removeFromParentAndCleanup(true);
        }
        else
        {
            drawNode = DrawNode::create();
            Point boundingWorldOrigin = _worldBoundaries.origin + Point(10.0f,10.0f);
            Point boundingWorldDestination = Point(_worldBoundaries.origin.x + _worldBoundaries.size.width - 10.0f,
                                                   _worldBoundaries.origin.y + _worldBoundaries.size.height - 10.0f);
            drawNode->drawRect(boundingWorldOrigin, boundingWorldDestination, Color4F(1, 1, 1, 1));
            _target->addChild(drawNode, 100, drawNodeName);
            
        }
    }
}

void GameCamera::initDebugDrawFollowArea()
{
    if (_target)
    {
        Node * scene = _target->getParent();
        if (scene)
        {
            std::string drawNodeName = "cameraBoundingArea";
            DrawNode * drawNode = dynamic_cast<DrawNode *>(_target->getChildByName(drawNodeName));
            if (drawNode)
            {
                drawNode->removeFromParentAndCleanup(true);
            }
            else
            {
                drawNode = DrawNode::create();
                Point boundingAreaOrigin = _followBoundingArea.origin;
                Point boundingAreaDestination = Point(_followBoundingArea.origin.x + _followBoundingArea.size.width,
                                                      _followBoundingArea.origin.y + _followBoundingArea.size.height);
                drawNode->drawRect(boundingAreaOrigin, boundingAreaDestination, Color4F(1, 1, 1, 1));
                
                scene->addChild(drawNode, 100, drawNodeName);
            }
        }
    }
}

void GameCamera::initDebugDrawPanDestination()
{
    if (_target)
    {
        std::string drawNodeName = "cameraPanDestination";
        DrawNode * drawNode = dynamic_cast<DrawNode *>(_target->getChildByName(drawNodeName));
        if (drawNode)
        {
            drawNode->removeFromParentAndCleanup(true);
        }
        else
        {
            drawNode = DrawNode::create();
            _target->addChild(drawNode, 100, drawNodeName);
        }
    }
}

void GameCamera::updateDebug()
{
    if (_debugEnabled)
    {
        this->updateDebugDrawPanDestination();
        this->updateDrawTargetAnchorPoint();
        this->updateDrawFollowedNodeOffset();
        this->updateDrawAttractors();
    }
}

void GameCamera::updateDebugDrawPanDestination()
{
    DrawNode * drawNode = dynamic_cast<DrawNode *>(_target->getChildByName("cameraPanDestination"));
    if (drawNode)
    {
        drawNode->clear();
        drawNode->drawDot(_panningDestination, 8.0f, Color4F(1, 0, 0, 1));
    }
}

void GameCamera::updateDrawTargetAnchorPoint()
{
    Point anchorPointOnTarget = _target->getAnchorPointInPoints();
    
    DrawNode * anchorPointDraw = dynamic_cast<DrawNode *>(_target->getChildByName("anchorPointDraw"));
    if (!anchorPointDraw)
    {
        anchorPointDraw = DrawNode::create();
        
        _target->addChild(anchorPointDraw, 100, "anchorPointDraw");
    }
    anchorPointDraw->clear();
    anchorPointDraw->drawDot(anchorPointOnTarget, 10.0f, Color4F(0, 1, 1, 1));
}

void GameCamera::updateDrawFollowedNodeOffset()
{
    if (true == _followEnabled)
    {
        DrawNode * drawNode = dynamic_cast<DrawNode *>(_target->getChildByName("followedNodeOffset"));
        
        if (!drawNode)
        {
            drawNode = DrawNode::create();
            
            _target->addChild(drawNode, 100, "followedNodeOffset");
        }
        drawNode->clear();
        drawNode->drawDot(this->getFollowedNodePositionOnTarget(), 4.0f, Color4F(0, 1, 0, 1));
    }
}

void GameCamera::updateDrawAttractors()
{
    if (_target)
    {
        for (auto iter = _attractors.begin(); iter != _attractors.end(); iter++)
        {
            Attractor * attractor = (*iter).second.get();
            DrawNode * drawNode;
            std::string key;
            
            key = (*iter).first + "inner";
            drawNode = dynamic_cast<DrawNode *>(_target->getChildByName(key));
            if (!drawNode)
            {
                drawNode = DrawNode::create();
                _target->addChild(drawNode, 100, key);
                drawNode->drawCircle(attractor->getPosition(), attractor->getInnerRadius(), 0.0f, 20.0f, false, 1.0f, 1.0f, Color4F::WHITE);
            }
            
            key = (*iter).first + "outer";
            drawNode = dynamic_cast<DrawNode *>(_target->getChildByName(key));
            if (!drawNode)
            {
                drawNode = DrawNode::create();
                _target->addChild(drawNode, 100, key);
                drawNode->drawCircle(attractor->getPosition(), attractor->getOuterRadius(), 0.0f, 20.0f, false, 1.0f, 1.0f, Color4F::WHITE);
            }
        }
    }
}

#pragma mark - Attractor Class

GameCamera::Attractor::Attractor(Point position, float zoom, float innerRadius, float outerRadius)
: _position(position)
, _zoom(zoom)
, _innerRadius(innerRadius)
, _outerRadius(outerRadius)
{}

#pragma mark - Rail Class

GameCamera::Rail::Rail(Rect area, Point startProjectionPoint, Point endProjectionPoint, std::vector<Point> railPoints, float railZoom)
: _area(area)
, _startProjectionPoint(startProjectionPoint)
, _endProjectionPoint(endProjectionPoint)
, _railPath(railPoints)
, _railZoom(railZoom)
{
    _projectionLength = (_endProjectionPoint - _startProjectionPoint).length();
}

bool GameCamera::Rail::isPointOnRail(Point position)
{
    return _area.containsPoint(position);
}

bool GameCamera::Rail::getPositionOnRail(Point position, Point& positionOnRail)
{
    Point absProjVector = _endProjectionPoint - _startProjectionPoint;
    Point absPosition = position - _startProjectionPoint;
    float positionOnProjVector = absPosition.dot(absProjVector) / _projectionLength;
    float percentageOnRail = positionOnProjVector / _projectionLength;
    
    bool res = _railPath.getValue(percentageOnRail, positionOnRail);
    return res;
}

bool GameCamera::Rail::getZoomOnRail(float& zoomOnRail)
{
    bool res = false;
    if (_railZoom > 0 )
    {
        zoomOnRail = _railZoom;
        res = true;
    }
    return res;
}