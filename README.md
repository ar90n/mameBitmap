What is mameBitmap ?
===============
mameBitmap is a header only library for bitmap images.

Example
===============
```cpp
#include "mameBitmap.h"

int main( int argc, char** argv )
{
    uint16_t width;
    uint16_t height;
    mameBitmap_format format;
    uint16_t max_value;
    mameBitmap_getInfoFromFile( TEST_DIR "pbm_ascii.pbm", &width, &height, &format, &max_value );

    size_t decode_image_buffer_size;
    mameBitmap_getImageSize( width, height, format, &decode_image_buffer_size );

    size_t decode_image_size;
    uint8_t* decode_image_buffer = new uint8_t[ decode_image_size ];

    mameBitmap_decodeFromFile( TEST_DIR "pbm_ascii.pbm",
                decode_image_buffer,
                decode_image_buffer_size,
                &width,
                &height,
                &format,
                &max_value,
                &decode_image_buffer_size ) );

    mameBitmap_encodeFromFile( decode_image_buffer,
                decode_image_buffer_size,
                TEST_DIR "res.pbm",
                width,
                height,
                format,
                max_value,
                decode_image_buffer_size ) );

    return 0;
}
```

Features
===============
* only include mameBitmap.h.
* support encoding and decoding for  raw pbm, ascii pbm, raw pgm, ascii pgm, raw ppm and ascii pgm.
* read and write data by input and output callback functions.
