//
//  FakeLoading.h
//  SongOfPan
//
//  Created by Giuseppe Lapenta on 17/03/15.
//
//

#ifndef __SongOfPan__FakeLoading__
#define __SongOfPan__FakeLoading__

#include "cocos2d.h"

USING_NS_CC;

class FakeLoading : public Layer
{
public:
    
    FakeLoading();
    virtual ~FakeLoading();
    
    static Scene* scene();
    CREATE_FUNC(FakeLoading);
    
    virtual bool init() override;
    
    void update(float dt) override;

private:
    
    void onEnterTransitionDidFinish() override;
    
    void initAudio();
    bool isLoadingEnded();
};

#endif /* defined(__SongOfPan__FakeLoading__) */
