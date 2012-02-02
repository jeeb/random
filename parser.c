#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <libavutil/bswap.h> 
#include "earthsoft_dvi.h"

int main()
{
    int derp;
    FILE *file = NULL;
    char str[33];
    char str2[33];
    uint64_t fsize;
    uint8_t left;
    uint32_t frames;
    
    file = fopen("sola OP1.dvi", "rb");
    
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    left = fsize % 16;
    frames = fsize / 16;
    
    snprintf(str, sizeof str, "%" PRIu64, fsize);
    printf( "File size: %s\n", str );
    snprintf(str, sizeof str, "%" PRIu8, left);
    printf( "Left: %s\n", str );
    snprintf(str, sizeof str, "%" PRIu32, frames);
    printf( "Amount of frames: %s\n\n", str );

    frame_index index[frames];
    
    for( uint32_t i = 0; i < frames; i++ )
    {
        derp = fread( &index[i].offset, sizeof(uint32_t), 1, file );
        if( derp != 1 )
        {
            printf( "Derped\n" );
            return 1;
        }

        index[i].offset = av_bswap32(index[i].offset);
        index[i].offset *= 4096;
        
        derp = fread( &index[i].size, sizeof(uint16_t), 1, file );
        if( derp != 1 )
        {
            printf( "Derped\n" );
            return 1;
        }
        
        index[i].size = av_bswap16(index[i].size);
        index[i].size *= 4096;
        
        derp = fread( &index[i].frames_until_earlier, sizeof(uint16_t)+sizeof(uint32_t), 1, file );
        if( derp != 1 )
        {
            printf( "Derped\n" );
            return 1;
        }
        
        index[i].frames_until_earlier = av_bswap64(index[i].frames_until_earlier);
        
        derp = fread( &index[i].aframe_count, sizeof(uint16_t), 1, file );
        if( derp != 1 )
        {
            printf( "Derped\n" );
            return 1;
        }
        
        index[i].aframe_count = av_bswap16(index[i].aframe_count);
        
        derp = fread( &index[i].quality, sizeof(uint8_t), 1, file );
        if( derp != 1 )
        {
            printf( "Derped\n" );
            return 1;
        }
        
        derp = fread( &index[i].reserved, sizeof(uint8_t), 1, file );
        if( derp != 1 )
        {
            printf( "Derped\n" );
            return 1;
        }
        
        // snprintf(str2, sizeof str2, "%" PRIu32, i);
        
        // snprintf(str, sizeof str, "%" PRIu64, index[i].offset);
        // printf( "[frame %s] : Offset %s\n", str2, str );
        // snprintf(str, sizeof str, "%" PRIu16, index[i].size);
        // printf( "[frame %s] : Size: %s\n", str2, str );
        // snprintf(str, sizeof str, "%" PRIu64, index[i].frames_until_earlier);
        // printf( "[frame %s] : FuE: %s\n", str2, str );
        // snprintf(str, sizeof str, "%" PRIu16, index[i].aframe_count);
        // printf( "[frame %s] : Amount of AFrames: %s\n", str2, str );
        // snprintf(str, sizeof str, "%" PRIu8, index[i].quality);
        // printf( "[frame %s] : Quality: %s\n", str2, str );
    }

    fclose(file);
    
    return 0;
}