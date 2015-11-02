//
//  FakeSplash.hpp
//  tvapp
//
//  Created by Iacopo Checcacci on 29/10/15.
//
//

#ifndef FAKE_SPLASH_HPP
#define FAKE_SPLASH_HPP

#include "LevelHelper2API.h"
#include "Definitions.h"

class FakeSplash : public LHNode
{
public:
    NODEWITHDICT_FUNC(FakeSplash);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt) override;
    
    void update(float dt) override;
    
private:
    void onEnterTransitionDidFinish() override;
    void initAudio();
    bool isLoadingEnded();
};

#endif // FAKE_SPLASH_HPP
