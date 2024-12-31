// Ripped from an old GTA project I used to use
// https://bitbucket.org/gir489/m0d-s0beit-v-redux/src/master/m0d-s0beit-v/Pattern.h
#pragma once
#include <vector>
#include <sstream>

namespace ts_extra_utilities
{
    struct pattern_byte
    {
        pattern_byte() : ignore( true )
        {
            //
        }

        explicit pattern_byte( const std::string& byte_string, const bool ignore_this = false ) : ignore( ignore_this ),
                                                                                                  data( string_to_uint8( byte_string ) )
        {
        }

        bool ignore;
        uint8_t data;

    private:
        static uint8_t string_to_uint8( const std::string& str )
        {
            std::istringstream iss( str );

            uint32_t ret;

            if ( iss >> std::hex >> ret )
            {
                return static_cast< uint8_t >( ret );
            }

            return 0;
        }
    };

    struct pattern
    {
        static uint64_t scan( const std::string s, const uint64_t start, const uint64_t length )
        {
            std::vector< pattern_byte > p;
            std::istringstream iss( s );
            std::string w;

            while ( iss >> w )
            {
                if ( w[ 0 ] == '?' )
                {
                    // Wildcard
                    p.emplace_back();
                }
                else if ( w.length() == 2 && isxdigit( w[ 0 ] ) && isxdigit( w[ 1 ] ) )
                {
                    // Hex
                    p.emplace_back( w );
                }
                else
                {
                    return NULL;
                }
            }

            for ( uint64_t i = 0; i < length; i++ )
            {
                auto current_byte = reinterpret_cast< uint8_t* >( start + i );

                auto found = true;

                for ( size_t ps = 0; ps < p.size(); ps++ )
                {
                    if ( p[ ps ].ignore == false && current_byte[ ps ] != p[ ps ].data )
                    {
                        found = false;
                        break;
                    }
                }

                if ( found )
                {
                    return reinterpret_cast< uint64_t >( current_byte );
                }
            }

            return NULL;
        }
    };
}
