#import "UtilityPlatform.h"
#import <Foundation/NSString.h>
#import <Foundation/NSBundle.h>

const char * getMediaPath(const char *fileName)
{
    NSString * str = [NSString stringWithFormat:@"%@/Media/%s", [[NSBundle mainBundle] resourcePath], fileName];
    return [str UTF8String];
}

void utilityInit(void **extraDriverData)
{
    
}