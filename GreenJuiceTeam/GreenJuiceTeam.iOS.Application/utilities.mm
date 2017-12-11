#include "utilities.h"
#include <Foundation/Foundation.h>

FILE *iosfopen(const char *filename, const char *mode) {
NSMutableString* adjusted_relative_path =
    [[NSMutableString alloc] initWithString:@"/res/"];
    [adjusted_relative_path appendString:
     [[NSString alloc] initWithCString:filename encoding:NSASCIIStringEncoding]];
    const char * filePath = [[[NSBundle mainBundle] pathForResource:adjusted_relative_path ofType:nil]
cStringUsingEncoding:NSASCIIStringEncoding];
    
    return fopen(filePath, mode);
}
 