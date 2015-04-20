What is mameBitmap ?
===============
mameBitmap is a header only library for netpbm format images.

Example
===============
```cpp
#include "mameBitmap.h"

int main( int argc, char** argv )
{
    const char* input_path = "input_path_to_your_env";
    const char* output_path = "output_path_to_your_env";

    uint16_t width;
    uint16_t height;
    mameBitmap_format format;
    uint16_t max_value;
    mameBitmap_getInfoFromFile( input_path, &width, &height, &format, &max_value );

    size_t decode_image_buffer_size;
    mameBitmap_getImageSize( width, height, format, &decode_image_buffer_size );

    uint8_t* decode_image_buffer[ decode_image_buffer_size ];
    size_t decode_image_size;
    mameBitmap_decodeFromFile( input_path,
                decode_image_buffer,
                decode_image_buffer_size,
                &width,
                &height,
                &format,
                &max_value,
                &decode_image_size );

    size_t encode_image_size;
    mameBitmap_encodeToFile( decode_image_buffer,
                decode_image_size,
                output_path,
                width,
                height,
                format,
                max_value,
                &encode_image_size );

    return 0;
}
```

Features
===============
* only include mameBitmap.h.
* support encoding and decoding for  raw pbm, ascii pbm, raw pgm, ascii pgm, raw ppm and ascii pgm.
* read and write data by input and output callback functions.
