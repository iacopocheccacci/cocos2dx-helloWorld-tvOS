//
//  GameCameraNew.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 02/04/15.
//
//

#ifndef __SongOfPan__GameCameraNew__
#define __SongOfPan__GameCameraNew__

#include "cocos2d.h"
#include "LinearInterpolation.h"

USING_NS_CC;

    
class GameCamera
{
    
public:
    
    enum class SizeConstraint
    {
        WIDTH = 0,
        HEIGHT,
        BOTH
    };
    
    virtual ~GameCamera();
    GameCamera(Node * target, const Rect worldBoundaries, const bool clampOnBoundaries);
    
    //
    // GENERAL
    //
    
    void update(float dt);
    
    //
    // FOLLOW
    //
    
    /*
     Imposta il nodo da seguire, alla velocità data e, eventualmente, con movimento smussato.
     */
    void setFollowNode(Node * followedNode, const float speed, const bool smoothMovement);
    
    /*
     Imposta l'offset che definisce il punto da inquadrare rispetto al followed node
     */
    void setFollowedNodeOffset(const Point offset);
    
    /*
     Imposta il nodo da seguire, lasciando invariati velocità e opzione di movimento smussato.
     */
    void setFollowNode(Node * followedNode);
    
    /*
     Imposta la velocità con cui la telecamera si avvicina al followed node, con eventuale opzione di movimento smussato.
     */
    void setFollowedNodeSpeed(const float speed, const bool smoothMovement);
    
    /*
     Imposta il rettangolo, relativo alla telecamera, entro il quale non avviene follow
     */
    void setFollowedArea(const Rect followArea);
    
    /*
     Attiva/Disattiva la modalità follow
     */
    void setFollowEnabled(const bool followEnabled);
    
    /*
     Forza l'aggiornamento della destinazione di panning sul followNode + offset.
     */
    void resetFollow();
    
    //
    // ZOOM
    //
    
    /*
     Effettua lo zoom della telecamera (scale del target) al valore dato alla velocità data e, eventualmente,
     con effetto smussato.
     */
    void zoomToScale(const float scale, const float speed, const bool smoothMovement);
    
    /*
     Aggiorna il valore di zoom da raggiungere, lasciando inalterati la velocità e la preferenza
     per il movimento smussato precedentemente indicati.
     */
    void setZoomScale(const float scale);
    
    /*
     Aggiorna il valore di velocità dello zoom, con eventuale opzione di movimento smussato.
     */
    void setZoomSpeed(const float speed, const bool smoothMovement);
    
    /*
     Forza istantaneamente la telecamera ad uno specifico zoom. Non altera i valori di speed e smooth
     precedentemente assegnati. NOTA: settare forzatamente lo zoom viola le regole comportamento della
     telecamera, per cui usare limitatamente (es.: inizializzazione)
     */
    void forceZoomToScale(const float scale);
    
    //
    // PANNING
    //
    
    /*
     Effettua il panning della telecamera alla posizione data alla velocità data e, eventualmente,
     con movimento smussato.
     */
    void panToLocation(const Point location, const float speed, const bool smoothMovement);
    
    /*
     Effettua il panning istantaneo della telecamera sul nodo dato, con l'offset dato.
     */
    void centerCameraOnNode(const Node * node, const Point offset);
    
    // Utilities
    float zoomClampToSizeForScale(const float scale);
    
    //
    // ATTRACTORS
    //
    
    /*
     Aggiunge un attrattore con posizione, raggio interno ed esterno dati. Lo zoom che si avrà 
     all'interno del raggio interno sarà quello fa coincidere lo schermo dentro al size dato,
     vincolandolo in ampiezza, altenzza o entrambi in base al sizeConstraint dato. La chiave data
     verrà utilizzata per l'eventuale rimozione dell'attrattore.
     */
    void addAttractor(const Point position, const Size targetScreenSize, const float innerRadius, const float outerRadius, const SizeConstraint sizeConstraint, const std::string key);
    
    /*
     Aggiunge un attrattore con posizione, zoom, raggio interno ed esterno dati. La chiave data
     verrà utilizzata per l'eventuale rimozione dell'attrattore.
     */
    void addAttractor(const Point position, const float zoom, const float innerRadius, const float outerRadius, const std::string key);
    
    /*
     Rimuove l'attrattore corrispondente alla chiave data
     */
    void removeAttractor(const std::string key);
    
    /*
     Attiva/disattiva gli attrattori
     */
    void setAttractorsEnabled(const bool enabled);
    
    //
    // RAILS
    //
    
    void addRail(const Rect areaOfEffect, const Point startProjectionPoint, const Point endProjectionPoint, const std::vector<Point> pathPoints, const Size pathScreenSize, const std::string key);
    
    void removeRail(const std::string key);
    
    void setRailsEnabled(const bool enabled);
    
    //
    // ACCESSORS
    //
    
    Node * getTarget()                                              { return _target; };
    
    float getCurrentZoom();
    Point getCurrentPosition();
    
    void setWorldBoundaries(const Rect worldBoundaries);
    Rect getWorldBoundaries()                                       { return _worldBoundaries; };
    
    void setClampOnBoundaries(const bool clamp)                     { _clampOnBoundaries = clamp; };
    bool isClampOnBoundaries()                                      { return _clampOnBoundaries; };
    
    void setSoftBoundariesX(const bool soft);
    void setSoftBoundariesY(const bool soft);
    bool isSoftBoundariesX()                                        { return _softBoundariesX; }
    bool isSoftBoundariesY()                                        { return _softBoundariesX; }
    
    Rect getFollowBoundingArea()                                    { return _followBoundingArea; };
    Point getFollowedNodeOffset()                                   { return _followedOffset; };
    
    float getMinZoomScaleForBoundaries();
    float getMinZoomScaleForSize(Size size);
    
    Point getPanningDestination()                                   { return _panningDestination; };
    float getZoomDestination()                                      { return _zoomDestination; };
    
    //
    // DEBUG
    //
    
    /*
     Attiva/disattiva la modalità debug:
     - Draw della followBoundingArea (rect bianco);
     - Draw dei bordi del mondo - con un padding di 10 pixel perché sennò non si vedono (rect bianco);
     - Draw del centro dello schermo (pallino bianco);
     - Draw della destinazione di panning (pallino grande rosso)
     - Draw dell'offset del followNode (pallino piccolo verde)
     - Draw dell'anchor point del target (pallino molto grande ciano)
     */
    void setDebugEnable(const bool enable);
    
private:
    
    class Attractor
    {
    public:
        Attractor(Point position, float zoom, float innerRadius, float outerRadius);
        
        Point getPosition()                                         {return _position;}
        float getZoom()                                             {return _zoom;}
        float getInnerRadius()                                      {return _innerRadius;}
        float getOuterRadius()                                      {return _outerRadius;}
        
    private:
        Point _position;
        float _zoom;
        float _innerRadius;
        float _outerRadius;
    };
    
    class Rail
    {
    public:
        
        Rail(Rect area, Point startProjectionPoint, Point endProjectionPoint, std::vector<Point> railPoints, float railZoom);
        bool isPointOnRail(Point position);
        bool getPositionOnRail(Point position, Point& positionOnRail);
        bool getZoomOnRail(float& zoomOnRail);
        
    private:
        
        Rect _area;
        Point _startProjectionPoint;
        Point _endProjectionPoint;
        float _projectionLength;
        LinearInterpolation _railPath;
        float _railZoom;
    };
    
    Node * _target;
    
    Rect _worldBoundaries;
    bool _clampOnBoundaries;
    bool _softBoundariesX;
    bool _softBoundariesY;
    bool _softBoundariesScheduledX;
    bool _softBoundariesScheduledY;
    
    // Screen Info per accesso rapido
    Point _screenOrigin;
    Size _screenSize;
    Point _screenCenter;
    
    // Follow
    Node * _followedNode;
    bool _followEnabled;
    Point _followedOffset;
    float _followSpeed;
    bool _followSmoothMovement;
    Rect _followBoundingArea;
    
    // Zoom
    float _zoomDestination;
    float _zoomSpeed;
    bool _zoomSmoothMovement;
    
    // Panning
    Point _panningDestination;
    float _panningSpeed;
    bool _panningSmoothMovement;
    
    // Attractors
    std::unordered_map<std::string, std::shared_ptr<Attractor>> _attractors;
    bool _attractorsEnabled;
    
    // Rails
    std::unordered_map<std::string, std::shared_ptr<Rail>> _rails;
    bool _railsEnabled;
    
    // Debug
    bool _debugEnabled;
    
    //
    // PRIVATE METHODS
    //
    
    // Updates
    void updateZoom(float dt);
    void updatePanning(float dt);
    void updateFollow(float dt);
    void updateAttractors(float dt);
    void updateRails(float dt);
    
    // Utilities
    void handleBoundaries();
    void setScaleAndAdjustPosition(const float newScale);
    bool checkFollowedNodeInAreaX();
    bool checkFollowedNodeInAreaY();
    
    // Wrappers
    void setTargetPosition(const Point newPosition);
    void setTargetScale(const float newScale);
    
    void setPanningDestinationX(const float newDestinationX);
    void setPanningDestinationY(const float newDestinationY);
    
    Point getFollowedNodePositionOnScreen();
    Point getFollowedNodePositionOnTarget();
    
    // DEBUG
    void initDebugDrawScreenCenter();
    void initDebugDrawBoundingWorld();
    
    void initDebugDrawFollowArea();
    void initDebugDrawPanDestination();
    
    void updateDebug();
    void updateDebugDrawPanDestination();
    void updateDrawTargetAnchorPoint();
    void updateDrawFollowedNodeOffset();
    void updateDrawAttractors();
};


#endif /* defined(__SongOfPan__GameCameraNew__) */
