/*
    EarthSoft DV Index File (.dvi)
    
    16 bytes per frame, 8 bits per byte. Multiple byte data is big endian.
    
      offset (bytes)    length (bytes)
    -    0               4  A/V Frame's offset / 4096
    -    4               2  A/V Frame's size / 4096
    -    6               6  Amount of audio frames until the earlier A/V Frame
    -   12               2  A/V Frame's amount of audio frames
    -   14               1  A/V Frame's encoding quality (0-255)
    -   15               1  Reserved
    -   16               -  Index data for the next A/V Frame
*/
typedef struct
{
    uint32_t offset;
    uint16_t size;
    uint64_t frames_until_earlier;
    uint16_t aframe_count;
    uint8_t  quality;
} dvi_frame_index;

/*
    Earthsoft DV File (.dv)
    
    One header, followed by an arbitrary amount of A/V frames.
    
    8 bits per byte. Multiple byte data is big endian.
    
    Header:
    
      offset (bytes)    length (bytes)
    -       0               1   'P'
    -       1               1   'V'
    -       2               1   '3'
    -       3               1   Codec Version
    -       4               1   Horizontal resolution / 16
    -       5               1   Vertical resolution / 8
    -       6               1   Encoding type (0: interlaced, 1: progressive)
    -       7             249   Reserved
    -     256             128   Luminance Quantization Table
    -     384             128   Chroma Quantization Table
    -     512           15872   Reserved
    -   16384               -   Beginning of the first A/V frame
    
    A/V Frame:
    
      offset (bytes)    length (bytes)
    -       0               6   Amount of audio frames until the earlier A/V Frame
    -       6               2   Amount of audio frames in this A/V Frame
    -       8               4   Audio sampling frequency
    -      12             244   Reserved
    -     256               2   Aspect ratio: horizontal
    -     258               2   Aspect ratio: vertical
    -     260               1   Encoding quality used (0-255)
    -     261             123   Reserved
    -     384               4   Data size of the video block 0 (a multiple of 32)
    -     388               4   Data size of the video block 1 (a multiple of 32)
    -     392               4   Data size of the video block 2 (a multiple of 32, only with interlaced encoding)
    -     396               4   Data size of the video block 3 (a multiple of 32, only with interlaced encoding)
    -     400             112   Reserved
    -     512        Variable   Audio data
    - 4096xn0        Variable   Video block 0
    -   32xn1        Variable   Video block 1
    -   32xn2        Variable   Video block 2 (only with interlaced encoding)
    -   32xn3        Variable   Video block 3 (only with interlaced encoding)
    - 4096xn4               -   Beginning of the next A/V Frame
*/
typedef struct
{
    uint8_t[3]      marker;
    uint8_t         codec_ver;
    uint8_t         h_res;
    uint8_t         v_res;
    uint8_t         enc_type;
    uint16_t[64]    luma_quant_table;
    uint16_t[64]    chroma_quant_table;
} dv_header;

typedef struct
{
    uint64_t frames_until_earlier;
    uint16_t aframe_count;
    uint32_t audio_frequency;
    uint16_t aspect_x;
    uint16_t aspect_y;
    uint8_t  quality;
    uint32_t vb0_size;
    uint32_t vb1_size;
    uint32_t vb2_size;
    uint32_t vb3_size;
} dv_frame;