#ifndef __MAME_BITMAP_HEADER__
# define __MAME_BITMAP_HEADER__

# ifdef __cplusplus
extern "C" {
# endif

# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>

/* data type definitions */
typedef bool (*mameBitmap_read_byte_callback_ptr)( void* param, uint8_t* byte );
typedef bool (*mameBitmap_write_byte_callback_ptr)( void* param, uint8_t byte );

typedef enum
{
    MAMEBITMAP_FORMAT_PBM_ASCII = 1,
    MAMEBITMAP_FORMAT_PGM_ASCII,
    MAMEBITMAP_FORMAT_PPM_ASCII,
    MAMEBITMAP_FORMAT_PBM_RAW,
    MAMEBITMAP_FORMAT_PGM_RAW,
    MAMEBITMAP_FORMAT_PPM_RAW,
    MAMEBITMAP_FORMAT_UNKNOWN,
} mameBitmap_format;

typedef struct {
    mameBitmap_read_byte_callback_ptr read_callback;
    void* read_callback_param;
    mameBitmap_write_byte_callback_ptr write_callback;
    void* write_callback_param;
    int input_cache;
    size_t used_buffer_size;
    struct {
        mameBitmap_format format;
        uint16_t width;
        uint16_t height;
        int max_value;
    }info;
} mameBitmap_context;

/* prototype definitions of mameBitmap API. */
bool mameBitmap_initialize( mameBitmap_context* context,
                            mameBitmap_read_byte_callback_ptr read_callback,
                            void* read_callback_param,
                            mameBitmap_write_byte_callback_ptr write_callback,
                            void* write_callback_param );

bool mameBitmap_encode( mameBitmap_context* context,
                        uint16_t width,
                        uint16_t height,
                        mameBitmap_format format,
                        uint16_t max_value );
bool mameBitmap_decode( mameBitmap_context* context );
bool mameBitmap_dispose( mameBitmap_context* context );
bool mameBitmap_getInfo( mameBitmap_read_byte_callback_ptr read_callback,
                         void* read_callback_param,
                         uint16_t* width,
                         uint16_t* height,
                         mameBitmap_format* format,
                         uint16_t* max_value );
bool mameBitmap_getImageSize( uint16_t width,
                              uint16_t height,
                              mameBitmap_format format,
                              size_t* image_size );


/* prototype definitions of refarence callbacks */
typedef struct {
    void* buffer_ptr;
    size_t buffer_pos;
    size_t buffer_length;
} mameBitmap_memory_callback_param;

bool mameBitmap_input_from_memory_callback( void* param, uint8_t* byte );
bool mameBitmap_output_to_memory_callback( void* param, uint8_t byte );
bool mameBitmap_input_from_file_callback( void* param, uint8_t* byte );
bool mameBitmap_output_to_file_callback( void* param, uint8_t byte );

/* prototype definitions of simple API wrapper */
bool mameBitmap_getInfoFromMemory( void* input_buffer,
                                   size_t input_buffer_size,
                                   uint16_t* width,
                                   uint16_t* height,
                                   mameBitmap_format* format,
                                   uint16_t* max_value );
bool mameBitmap_getInfoFromFile( const char* file_name,
                                 uint16_t* width,
                                 uint16_t* height,
                                 mameBitmap_format* format,
                                 uint16_t* max_value );
bool mameBitmap_encodeToMemory( void* input_buffer,
                                size_t input_size,
                                void* output_buffer,
                                size_t output_buffer_size,
                                uint16_t width,
                                uint16_t height,
                                mameBitmap_format format,
                                uint16_t max_value,
                                size_t* used_buffer_size );
bool mameBitmap_encodeToFile( void* output_buffer,
                              size_t output_buffer_size,
                              const char* file_name,
                              uint16_t width,
                              uint16_t height,
                              mameBitmap_format format,
                              uint16_t max_value,
                              size_t* used_buffer_size );
bool mameBitmap_decodeFromMemory( void* input_buffer,
                                  size_t input_size,
                                  void* output_buffer,
                                  size_t output_buffer_size,
                                  uint16_t* width,
                                  uint16_t* height,
                                  mameBitmap_format* format,
                                  uint16_t* max_value,
                                  size_t* used_buffer_size );
bool mameBitmap_decodeFromFile( const char* file_name,
                                void* output_buffer,
                                size_t output_buffer_size,
                                uint16_t* width,
                                uint16_t* height,
                                mameBitmap_format* format,
                                uint16_t* max_value,
                                size_t* used_buffer_size );

/* prototype definitions of internal functions */
bool mameBitmap_readByte( mameBitmap_context* context, uint8_t* byte );
bool mameBitmap_unreadByte( mameBitmap_context* context, uint8_t byte );
bool mameBitmap_writeByte( mameBitmap_context* context, uint8_t byte );
bool mameBitmap_is_space( char byte );
bool mameBitmap_readASCIIElem( mameBitmap_context* context, uint16_t* elem_value, uint16_t elem_length );
bool mameBitmap_writeASCIIElem( mameBitmap_context* context, uint16_t elem_value );
bool mameBitmap_getFormat( uint8_t magic_number, mameBitmap_format* format );
bool mameBitmap_getComponents( mameBitmap_format format, uint8_t* components );
bool mameBitmap_getMagicNumber( mameBitmap_format format, uint8_t* magic_number );
bool mameBitmap_skipComment( mameBitmap_context* context );
bool mameBitmap_getElemLength( mameBitmap_context* context, int* elem_length );
bool mameBitmap_getBitsPerComponent( mameBitmap_context* context, uint8_t* bits_per_component );
bool mameBitmap_decodeHeader( mameBitmap_context* context );
bool mameBitmap_decodeASCII( mameBitmap_context* context );
bool mameBitmap_decodeBINARY( mameBitmap_context* context );
bool mameBitmap_encodeHeader( mameBitmap_context* context );
bool mameBitmap_encodeASCII( mameBitmap_context* context );
bool mameBitmap_encodeBINARY( mameBitmap_context* context );

# define MAMEBITMAP_CHECK( X ) do{if( (X) == false ) return false; } while(0);
# define MAMEBITMAP_NULL_CHECK( X )  MAMEBITMAP_CHECK( (X) != NULL )

bool mameBitmap_initialize( mameBitmap_context* context,
                            mameBitmap_read_byte_callback_ptr read_callback,
                            void* read_callback_param,
                            mameBitmap_write_byte_callback_ptr write_callback,
                            void* write_callback_param )
{
    MAMEBITMAP_NULL_CHECK( context )
    MAMEBITMAP_NULL_CHECK( read_callback )
    MAMEBITMAP_NULL_CHECK( write_callback )

    context->read_callback = read_callback;
    context->read_callback_param = read_callback_param;
    context->write_callback = write_callback;
    context->write_callback_param = write_callback_param;
    context->used_buffer_size = 0;
    context->input_cache = -1;

    return true;
}

bool mameBitmap_readByte( mameBitmap_context* context, uint8_t* byte )
{
    MAMEBITMAP_NULL_CHECK( context );
    MAMEBITMAP_NULL_CHECK( byte );

    if( 0 <= context->input_cache )
    {
        *byte = (uint8_t)context->input_cache;
        context->input_cache = -1;
        return true;
    }

    return context->read_callback( context->read_callback_param, byte );
}

bool mameBitmap_unreadByte( mameBitmap_context* context, uint8_t byte )
{
    MAMEBITMAP_NULL_CHECK( context )

    if( 0 <= context->input_cache )
    {
        return false;
    }

    context->input_cache = byte;
    return true;
}

bool mameBitmap_writeByte( mameBitmap_context* context, uint8_t byte )
{
    MAMEBITMAP_NULL_CHECK( context );

    context->used_buffer_size++;
    return context->write_callback( context->write_callback_param, byte );
}

bool mameBitmap_is_space( char byte )
{
    return byte == ' ' || byte == '\t' || byte == '\n';
}

bool mameBitmap_readASCIIElem( mameBitmap_context* context, uint16_t* elem_value, uint16_t elem_length )
{
    MAMEBITMAP_NULL_CHECK( context );
    MAMEBITMAP_NULL_CHECK( elem_value );

    uint8_t val = 0;
    do
    {
        MAMEBITMAP_CHECK( mameBitmap_readByte( context, &val ) );
    } while( mameBitmap_is_space( val ) );

    *elem_value = 0;
    uint16_t work_elem_length = elem_length;
    while( !mameBitmap_is_space( val ) && ( 0 < work_elem_length ) )
    {
        *elem_value *= 10;
        *elem_value += ( val - '0' );
        work_elem_length--;

        if( 0 < work_elem_length )
        {
            bool ret = mameBitmap_readByte( context, &val );
            if( !ret )
            {
                break;
            }
        }
    }

    return true;
}

bool mameBitmap_writeASCIIElem( mameBitmap_context* context, uint16_t elem_value )
{
    MAMEBITMAP_NULL_CHECK( context )

    uint8_t elem_str[5]; /* 5 = (int)log10( 65535 ) */
    int digits = 0;

    do
    {
        elem_str[digits] = ( elem_value % 10 ) + '0';
        elem_value /= 10;
        digits++;
    } while( 0 < elem_value );

    for( int i = 0; i < digits; i++ )
    {
        uint8_t write_byte = elem_str[digits - i - 1];
        MAMEBITMAP_CHECK( mameBitmap_writeByte( context, write_byte ) );
    }

    return true;
}

bool mameBitmap_getFormat( uint8_t magic_number, mameBitmap_format* format )
{
    MAMEBITMAP_NULL_CHECK( format );

    bool ret = false;
    if( magic_number == 0 || MAMEBITMAP_FORMAT_UNKNOWN <= magic_number )
    {
        *format = MAMEBITMAP_FORMAT_UNKNOWN;
        ret = false;
    }
    else
    {
        *format = (mameBitmap_format)magic_number;
        ret = true;
    }

    return ret;
}

bool mameBitmap_getMagicNumber( mameBitmap_format format, uint8_t* magic_number )
{
    MAMEBITMAP_NULL_CHECK( magic_number );

    bool ret = false;
    if( format != MAMEBITMAP_FORMAT_UNKNOWN )
    {
        *magic_number = (uint8_t)format;
        ret = true;
    }

    return ret;
}

bool mameBitmap_skipComment( mameBitmap_context* context )
{
    MAMEBITMAP_NULL_CHECK( context );

    uint8_t val;
    MAMEBITMAP_CHECK( mameBitmap_readByte( context, &val ) );
    while( val == '#' )
    {
        do
        {
            MAMEBITMAP_CHECK( mameBitmap_readByte( context, &val ) );
        } while( val != '\n' );
        MAMEBITMAP_CHECK( mameBitmap_readByte( context, &val ) );
    }
    mameBitmap_unreadByte( context, val );

    return true;
}

bool mameBitmap_getComponents( mameBitmap_format format, uint8_t* components )
{
    MAMEBITMAP_NULL_CHECK( components );

    *components = 1;
    if( format == MAMEBITMAP_FORMAT_PPM_ASCII || format == MAMEBITMAP_FORMAT_PPM_RAW )
    {
        *components = 3;
    }

    return true;
}

bool mameBitmap_getBitsPerComponent( mameBitmap_context* context, uint8_t* bits_per_component )
{
    MAMEBITMAP_NULL_CHECK( context );
    MAMEBITMAP_NULL_CHECK( bits_per_component );

    *bits_per_component = 8;
    if( context->info.format == MAMEBITMAP_FORMAT_PBM_ASCII || context->info.format == MAMEBITMAP_FORMAT_PBM_RAW )
    {
        *bits_per_component = 1;
    }

    return true;
}

bool mameBitmap_getElemLength( mameBitmap_context* context, int* elem_length )
{
    MAMEBITMAP_NULL_CHECK( context );
    MAMEBITMAP_NULL_CHECK( elem_length );

    *elem_length = -1;
    if( context->info.format == MAMEBITMAP_FORMAT_PBM_ASCII || context->info.format == MAMEBITMAP_FORMAT_PBM_RAW )
    {
        *elem_length = 1;
    }

    return true;
}

bool mameBitmap_decodeHeader( mameBitmap_context* context )
{
    uint8_t val;
    MAMEBITMAP_CHECK( mameBitmap_readByte( context, &val ) );
    MAMEBITMAP_CHECK( val == 'P' );
    MAMEBITMAP_CHECK( mameBitmap_readByte( context, &val ) );
    uint8_t magic_number = val - '0';
    MAMEBITMAP_CHECK( mameBitmap_getFormat( magic_number, &context->info.format ) );
    MAMEBITMAP_CHECK( context->info.format != MAMEBITMAP_FORMAT_UNKNOWN );
    MAMEBITMAP_CHECK( mameBitmap_readByte( context, &val ) );
    MAMEBITMAP_CHECK( mameBitmap_is_space( val ) );

    MAMEBITMAP_CHECK( mameBitmap_skipComment( context ) );
    MAMEBITMAP_CHECK( mameBitmap_readASCIIElem( context, &(context->info.width), -1 ) );
    MAMEBITMAP_CHECK( mameBitmap_readASCIIElem( context, &(context->info.height), -1 ) );

    context->info.max_value = 1;
    if( context->info.format != MAMEBITMAP_FORMAT_PBM_ASCII && context->info.format != MAMEBITMAP_FORMAT_PBM_RAW )
    {
        uint16_t elem;
        MAMEBITMAP_CHECK( mameBitmap_readASCIIElem( context, &elem, -1 ) );
        context->info.max_value = elem;
    }

    return true;
}

bool mameBitmap_decodeASCII( mameBitmap_context* context )
{
    MAMEBITMAP_NULL_CHECK( context );

    uint8_t components;
    MAMEBITMAP_CHECK( mameBitmap_getComponents( context->info.format, &components ));

    int elem_length;
    MAMEBITMAP_CHECK( mameBitmap_getElemLength( context, &elem_length ) );

    int hor_bytes = components * context->info.width;
    for( int y = 0; y < context->info.height; y++ )
    {
        for( int x = 0; x < hor_bytes; x++ )
        {
            uint16_t val;
            MAMEBITMAP_CHECK( mameBitmap_readASCIIElem( context, &val, elem_length ) );
            MAMEBITMAP_CHECK( mameBitmap_writeByte( context, (uint8_t)val ) );
        }
    }

    return true;
}

bool mameBitmap_decodeBINARY( mameBitmap_context* context )
{
    MAMEBITMAP_NULL_CHECK( context );

    uint8_t bits_per_component;
    MAMEBITMAP_CHECK( mameBitmap_getBitsPerComponent( context, &bits_per_component ) );

    uint8_t components;
    MAMEBITMAP_CHECK( mameBitmap_getComponents( context->info.format, &components ) );

    uint16_t hor_width = components * context->info.width;
    for( int y = 0; y < context->info.height; y++ )
    {
        uint16_t remain_width = hor_width;
        while( 0 < remain_width )
        {
            uint8_t ival;
            MAMEBITMAP_CHECK( mameBitmap_readByte( context, &ival ) );

            uint8_t valid_bit_mask = ( 1 << bits_per_component ) - 1;
            uint8_t current_width = ( ( 8 / bits_per_component ) < remain_width ) ? (8 / bits_per_component ) : remain_width;
            for( int i = 0; i < current_width; i++ )
            {
                uint8_t bit_shift = 8 - bits_per_component * ( i + 1 );
                uint8_t oval = ( ival >> bit_shift ) & valid_bit_mask;

                oval = ( context->info.max_value < oval ) ? context->info.max_value : oval;
                MAMEBITMAP_CHECK( mameBitmap_writeByte( context, oval ) );
            }

            remain_width -= current_width;
        }
    }

    return true;
}

bool mameBitmap_encodeHeader( mameBitmap_context* context )
{
    MAMEBITMAP_NULL_CHECK( context );

    uint8_t magic_number[4] = { 'P', 0, '\n', '\0' };
    MAMEBITMAP_CHECK( mameBitmap_getMagicNumber( context->info.format, &( magic_number[1] ) ) );
    magic_number[1] += '0';

    MAMEBITMAP_CHECK( mameBitmap_writeByte( context, magic_number[0] ) );
    MAMEBITMAP_CHECK( mameBitmap_writeByte( context, magic_number[1] ) );
    MAMEBITMAP_CHECK( mameBitmap_writeByte( context, magic_number[2] ) );

    MAMEBITMAP_CHECK( mameBitmap_writeASCIIElem( context, context->info.width ) );
    MAMEBITMAP_CHECK( mameBitmap_writeByte( context, ' ' ) );
    MAMEBITMAP_CHECK( mameBitmap_writeASCIIElem( context, context->info.height ) );
    MAMEBITMAP_CHECK( mameBitmap_writeByte( context, '\n' ) );

    if( context->info.format != MAMEBITMAP_FORMAT_PBM_ASCII && context->info.format != MAMEBITMAP_FORMAT_PBM_RAW )
    {
        MAMEBITMAP_CHECK( mameBitmap_writeASCIIElem( context, context->info.max_value ) );
        MAMEBITMAP_CHECK( mameBitmap_writeByte( context, '\n' ) );
    }

    return true;
}

bool mameBitmap_encodeASCII( mameBitmap_context* context )
{
    MAMEBITMAP_NULL_CHECK( context );

    uint8_t components;
    MAMEBITMAP_CHECK( mameBitmap_getComponents( context->info.format, &components ));

    int elem_length;
    MAMEBITMAP_CHECK( mameBitmap_getElemLength( context, &elem_length ) );

    int hor_bytes = components * context->info.width;
    for( int y = 0; y < context->info.height; y++ )
    {
        for( int x = 0; x < hor_bytes; x++ )
        {
            uint8_t val;
            MAMEBITMAP_CHECK( mameBitmap_readByte( context, &val ) );

            val = ( context->info.max_value < val ) ? context->info.max_value : val;
            MAMEBITMAP_CHECK( mameBitmap_writeASCIIElem( context, val ) );
            MAMEBITMAP_CHECK( mameBitmap_writeByte( context, ' ' ) );
        }
        MAMEBITMAP_CHECK( mameBitmap_writeByte( context, '\n' ) );
    }

    MAMEBITMAP_CHECK( mameBitmap_writeByte( context, '\0' ) );

    return true;
}

bool mameBitmap_encodeBINARY( mameBitmap_context* context )
{
    MAMEBITMAP_NULL_CHECK( context );

    uint8_t bits_per_component;
    MAMEBITMAP_CHECK( mameBitmap_getBitsPerComponent( context, &bits_per_component ) );

    uint8_t components;
    MAMEBITMAP_CHECK( mameBitmap_getComponents( context->info.format, &components ) );

    uint16_t hor_width = components * context->info.width;
    for( int y = 0; y < context->info.height; y++ )
    {
        uint16_t remain_width = hor_width;
        while( 0 < remain_width )
        {
            uint8_t valid_bit_mask = ( 1 << bits_per_component ) - 1;
            uint8_t current_width = ( ( 8 / bits_per_component ) < remain_width ) ? (8 / bits_per_component ) : remain_width;

            uint8_t oval = 0;
            for( int i = 0; i < current_width; i++ )
            {
                uint8_t ival;
                MAMEBITMAP_CHECK( mameBitmap_readByte( context, &ival ) );

                uint8_t bit_shift = 8 - bits_per_component * ( i + 1 );
                oval |= ( ival & valid_bit_mask ) << bit_shift;
            }
            MAMEBITMAP_CHECK( mameBitmap_writeByte( context, oval ) );

            remain_width -= current_width;
        }
    }

    return true;
}

bool mameBitmap_encode( mameBitmap_context* context, uint16_t width, uint16_t height, mameBitmap_format format, uint16_t max_value )
{
    MAMEBITMAP_NULL_CHECK( context );
    MAMEBITMAP_CHECK( 0 < width );
    MAMEBITMAP_CHECK( 0 < height );
    MAMEBITMAP_CHECK( MAMEBITMAP_FORMAT_PBM_ASCII <= format && format <= MAMEBITMAP_FORMAT_PPM_RAW );
    MAMEBITMAP_CHECK( 0 < max_value );

    context->info.width = width;
    context->info.height = height;
    context->info.format = format;
    context->info.max_value = max_value;
    MAMEBITMAP_CHECK( mameBitmap_encodeHeader( context ) );

    switch( context->info.format )
    {
        case MAMEBITMAP_FORMAT_PBM_ASCII: /* fall through */
        case MAMEBITMAP_FORMAT_PGM_ASCII: /* fall through */
        case MAMEBITMAP_FORMAT_PPM_ASCII:
        {
            MAMEBITMAP_CHECK( mameBitmap_encodeASCII( context ) );
        }break;
        case MAMEBITMAP_FORMAT_PBM_RAW: /* fall through */
        case MAMEBITMAP_FORMAT_PGM_RAW: /* fall through */
        case MAMEBITMAP_FORMAT_PPM_RAW:
        {
            MAMEBITMAP_CHECK( mameBitmap_encodeBINARY( context ) );
        }break;
        default:
        {
            return false;
        }
    }

    return true;
}

bool mameBitmap_decode( mameBitmap_context* context )
{
    MAMEBITMAP_NULL_CHECK( context );

    MAMEBITMAP_CHECK( mameBitmap_decodeHeader( context ) );

    switch( context->info.format )
    {
        case MAMEBITMAP_FORMAT_PBM_ASCII: /* fall through */
        case MAMEBITMAP_FORMAT_PGM_ASCII: /* fall through */
        case MAMEBITMAP_FORMAT_PPM_ASCII:
        {
            MAMEBITMAP_CHECK( mameBitmap_decodeASCII( context ) );
        }break;
        case MAMEBITMAP_FORMAT_PBM_RAW: /* fall through */
        case MAMEBITMAP_FORMAT_PGM_RAW: /* fall through */
        case MAMEBITMAP_FORMAT_PPM_RAW:
        {
            MAMEBITMAP_CHECK( mameBitmap_decodeBINARY( context ) );
        }break;
        default:
        {
            return false;
        }
    }

    return true;
}

bool mameBitmap_dispose( mameBitmap_context* context )
{
    return true;
}

bool mameBitmap_getInfo( mameBitmap_read_byte_callback_ptr read_callback,
                         void* read_callback_param,
                         uint16_t* width,
                         uint16_t* height,
                         mameBitmap_format* format,
                         uint16_t* max_value )
{
    MAMEBITMAP_NULL_CHECK( read_callback );
    MAMEBITMAP_NULL_CHECK( read_callback_param );
    MAMEBITMAP_NULL_CHECK( width );
    MAMEBITMAP_NULL_CHECK( height );
    MAMEBITMAP_NULL_CHECK( format );
    MAMEBITMAP_NULL_CHECK( max_value );

    mameBitmap_context work_context[1];
    work_context->read_callback = read_callback;
    work_context->read_callback_param = read_callback_param;
    work_context->input_cache = -1;

    mameBitmap_decodeHeader( work_context );
    *format = work_context->info.format;
    *width = work_context->info.width;
    *height = work_context->info.height;
    *max_value = work_context->info.max_value;

    return true;
}

bool mameBitmap_getImageSize( uint16_t width,
                              uint16_t height,
                              mameBitmap_format format,
                              size_t* image_size )
{
    uint8_t components;
    MAMEBITMAP_CHECK( mameBitmap_getComponents( format, &components ) );

    *image_size = components * width * height;
    return true;
}

bool mameBitmap_input_from_memory_callback( void* param, uint8_t* byte )
{
    MAMEBITMAP_NULL_CHECK( param )
    MAMEBITMAP_NULL_CHECK( byte )

    mameBitmap_memory_callback_param *callback_param = (mameBitmap_memory_callback_param*)param;
    MAMEBITMAP_CHECK( callback_param->buffer_pos < callback_param->buffer_length )

    *byte = ((uint8_t*)callback_param->buffer_ptr)[ callback_param->buffer_pos ];
    callback_param->buffer_pos++;

    return true;
}

bool mameBitmap_output_to_memory_callback( void* param, uint8_t byte )
{
    MAMEBITMAP_NULL_CHECK( param )

    mameBitmap_memory_callback_param *callback_param = (mameBitmap_memory_callback_param*)param;
    MAMEBITMAP_CHECK( callback_param->buffer_pos < callback_param->buffer_length )

    ((uint8_t*)callback_param->buffer_ptr)[ callback_param->buffer_pos ] = byte;
    (( mameBitmap_memory_callback_param* )param)->buffer_pos++;

    return true;
}

bool mameBitmap_input_from_file_callback( void* param, uint8_t* byte )
{
    MAMEBITMAP_NULL_CHECK( param )
    MAMEBITMAP_NULL_CHECK( byte )

    FILE* fp = (FILE*)param;
    size_t n = fread( byte, 1, 1, fp );

    return ( n == 1 );
}

bool mameBitmap_output_to_file_callback( void* param, uint8_t byte )
{
    MAMEBITMAP_NULL_CHECK( param )

    FILE* fp = (FILE*)param;
    size_t n = fwrite( &byte, 1, 1, fp );

    return ( n == 1 );
}

bool mameBitmap_getInfoFromMemory( void* input_buffer,
                                   size_t input_buffer_size,
                                   uint16_t* width,
                                   uint16_t* height,
                                   mameBitmap_format* format,
                                   uint16_t* max_value )
{
    MAMEBITMAP_NULL_CHECK( input_buffer );
    MAMEBITMAP_CHECK( 0 < input_buffer_size );
    MAMEBITMAP_NULL_CHECK( width );
    MAMEBITMAP_NULL_CHECK( height );
    MAMEBITMAP_NULL_CHECK( format );
    MAMEBITMAP_NULL_CHECK( max_value );

    mameBitmap_memory_callback_param input_param = {
        .buffer_ptr = input_buffer,
        .buffer_pos = 0,
        .buffer_length = input_buffer_size
    };

    MAMEBITMAP_CHECK( mameBitmap_getInfo( mameBitmap_input_from_memory_callback, &input_param, width, height, format, max_value ) );
    return  true;
}

bool mameBitmap_decodeFromMemory( void* input_buffer,
                                  size_t input_buffer_size,
                                  void* output_buffer,
                                  size_t output_buffer_size,
                                  uint16_t* width,
                                  uint16_t* height,
                                  mameBitmap_format* format,
                                  uint16_t* max_value,
                                  size_t* used_buffer_size )
{
    MAMEBITMAP_NULL_CHECK( input_buffer );
    MAMEBITMAP_CHECK( 0 < input_buffer_size );
    MAMEBITMAP_NULL_CHECK( output_buffer );
    MAMEBITMAP_CHECK( 0 < output_buffer_size );
    MAMEBITMAP_NULL_CHECK( width );
    MAMEBITMAP_NULL_CHECK( height );
    MAMEBITMAP_NULL_CHECK( format );
    MAMEBITMAP_NULL_CHECK( max_value );

    mameBitmap_context context[1];
    mameBitmap_memory_callback_param input_param = {
        .buffer_ptr = input_buffer,
        .buffer_pos = 0,
        .buffer_length = input_buffer_size
    };
    mameBitmap_memory_callback_param output_param = {
        .buffer_ptr = output_buffer,
        .buffer_pos = 0,
        .buffer_length = output_buffer_size
    };
    MAMEBITMAP_CHECK( mameBitmap_initialize( context,
                                             mameBitmap_input_from_memory_callback,
                                             &input_param,
                                             mameBitmap_output_to_memory_callback,
                                             &output_param ) );

    MAMEBITMAP_CHECK( mameBitmap_decode( context ) );
    *width = context->info.width;
    *height = context->info.height;
    *format = context->info.format;
    *max_value = context->info.max_value;
    *used_buffer_size = context->used_buffer_size;

    return true;
}

bool mameBitmap_encodeToMemory( void* input_buffer,
                                size_t input_buffer_size,
                                void* output_buffer,
                                size_t output_buffer_size,
                                uint16_t width,
                                uint16_t height,
                                mameBitmap_format format,
                                uint16_t max_value,
                                size_t* used_buffer_size )
{
    MAMEBITMAP_NULL_CHECK( input_buffer );
    MAMEBITMAP_CHECK( 0 < input_buffer_size );
    MAMEBITMAP_NULL_CHECK( output_buffer );
    MAMEBITMAP_CHECK( 0 < output_buffer_size );
    MAMEBITMAP_CHECK( 0 < width );
    MAMEBITMAP_CHECK( 0 < height );
    MAMEBITMAP_CHECK( MAMEBITMAP_FORMAT_PBM_ASCII <= format && format <= MAMEBITMAP_FORMAT_PPM_RAW );
    MAMEBITMAP_CHECK( 0 < max_value );

    mameBitmap_context context[1];
    mameBitmap_memory_callback_param input_param = {
        .buffer_ptr = input_buffer,
        .buffer_pos = 0,
        .buffer_length = input_buffer_size
    };
    mameBitmap_memory_callback_param output_param = {
        .buffer_ptr = output_buffer,
        .buffer_pos = 0,
        .buffer_length = output_buffer_size
    };
    MAMEBITMAP_CHECK( mameBitmap_initialize( context,
                                             mameBitmap_input_from_memory_callback,
                                             &input_param,
                                             mameBitmap_output_to_memory_callback,
                                             &output_param ) );

    MAMEBITMAP_CHECK( mameBitmap_encode( context, width, height, format, max_value ) );
    *used_buffer_size = context->used_buffer_size;
    return true;
}

bool mameBitmap_getInfoFromFile( const char* file_name,
                                 uint16_t* width,
                                 uint16_t* height,
                                 mameBitmap_format* format,
                                 uint16_t* max_value )
{
    MAMEBITMAP_NULL_CHECK( file_name );
    MAMEBITMAP_NULL_CHECK( format );
    MAMEBITMAP_NULL_CHECK( width );
    MAMEBITMAP_NULL_CHECK( height );
    MAMEBITMAP_NULL_CHECK( max_value );

    FILE* fp = fopen(file_name, "r");
    MAMEBITMAP_NULL_CHECK( fp );

    bool ret = mameBitmap_getInfo( mameBitmap_input_from_file_callback, (void*)fp, width, height, format, max_value );
    fclose( fp );

    return  ret;
}

bool mameBitmap_decodeFromFile( const char* file_name,
                                void* output_buffer,
                                size_t output_buffer_size,
                                uint16_t* width,
                                uint16_t* height,
                                mameBitmap_format* format,
                                uint16_t* max_value,
                                size_t* used_buffer_size )
{
    MAMEBITMAP_NULL_CHECK( file_name );
    MAMEBITMAP_NULL_CHECK( output_buffer );
    MAMEBITMAP_CHECK( 0 < output_buffer_size );

    mameBitmap_context context[1];
    FILE* fp = fopen(file_name, "r");
    MAMEBITMAP_NULL_CHECK( fp );
    mameBitmap_memory_callback_param output_param = {
        .buffer_ptr = output_buffer,
        .buffer_pos = 0,
        .buffer_length = output_buffer_size
    };

    bool ret = mameBitmap_initialize( context,
                                      mameBitmap_input_from_file_callback,
                                      (void*)fp,
                                      mameBitmap_output_to_memory_callback,
                                      &output_param );
    if( !ret )
    {
        fclose(fp);
        return false;
    }

    ret = mameBitmap_decode( context );
    fclose(fp);

    *width = context->info.width;
    *height = context->info.height;
    *format = context->info.format;
    *max_value = context->info.max_value;
    *used_buffer_size = context->used_buffer_size;

    return ret;
}

bool mameBitmap_encodeToFile( void* input_buffer,
                              size_t input_buffer_size,
                              const char* file_name,
                              uint16_t width,
                              uint16_t height,
                              mameBitmap_format format,
                              uint16_t max_value,
                              size_t* used_buffer_size )
{
    MAMEBITMAP_NULL_CHECK( file_name );
    MAMEBITMAP_NULL_CHECK( input_buffer );
    MAMEBITMAP_CHECK( 0 < input_buffer_size );
    MAMEBITMAP_CHECK( 0 < width );
    MAMEBITMAP_CHECK( 0 < height );
    MAMEBITMAP_CHECK( MAMEBITMAP_FORMAT_PBM_ASCII <= format && format <= MAMEBITMAP_FORMAT_PPM_RAW );
    MAMEBITMAP_CHECK( 0 < max_value );

    mameBitmap_context context[1];
    FILE* fp = fopen(file_name, "w");
    MAMEBITMAP_NULL_CHECK( fp );
    mameBitmap_memory_callback_param input_param = {
        .buffer_ptr = input_buffer,
        .buffer_pos = 0,
        .buffer_length = input_buffer_size
    };

    bool ret = mameBitmap_initialize( context,
                                      mameBitmap_input_from_memory_callback,
                                      &input_param,
                                      mameBitmap_output_to_file_callback,
                                      (void*)fp );
    if( !ret )
    {
        fclose(fp);
        return false;
    }

    ret = mameBitmap_encode( context, width, height, format, max_value );

    fclose(fp);
    return ret;
}

# ifdef __cplusplus
}
# endif

#endif /* __MAME_BITMAP_HEADER__ */
