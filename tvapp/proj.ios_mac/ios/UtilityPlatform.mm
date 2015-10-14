#import "UtilityPlatform.h"
#import <Foundation/NSString.h>
#import <Foundation/NSBundle.h>
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioSession.h>

bool gSuspendState;

const char * getMediaPath(const char *fileName)
{
    NSString * str = [NSString stringWithFormat:@"%@/Media/%s", [[NSBundle mainBundle] resourcePath], fileName];
    return [str UTF8String];
}

void interruptionListenerCallback(void *inUserData, UInt32 interruptionState)
{
    if (interruptionState == kAudioSessionBeginInterruption)
    {
        gSuspendState = true;
    }
    else if (interruptionState == kAudioSessionEndInterruption)
    {
        UInt32 sessionCategory = kAudioSessionCategory_PlayAndRecord;
        AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
        AudioSessionSetActive(true);
        
        gSuspendState = false;
    }
}

void utilityInit(void **extraDriverData)
{
    gSuspendState = false;
//    gOutputBuffer = [NSMutableString stringWithCapacity:(NUM_COLUMNS * NUM_ROWS)];
    
    AudioSessionInitialize(NULL, NULL, interruptionListenerCallback, NULL);
    
    // Default to 'play and record' so we have recording available for examples that use it
    UInt32 sessionCategory = kAudioSessionCategory_PlayAndRecord;
    AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
    AudioSessionSetActive(true);
}

void utilityUpdate(void * system)
{
    
}
