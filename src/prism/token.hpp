#pragma once
#include <string>

namespace ts_extra_utilities::prism
{
    constexpr int i_num_letters = 38;
    constexpr char psz_letters[ i_num_letters ] = {
        '\0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b',
        'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
        'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_'
    };

    struct ulldiv_t
    {
        unsigned long long int quot;
        unsigned long long int rem;

        ulldiv_t() : quot( 0ull ), rem( 0ull )
        {
        }
    };

    inline ulldiv_t div( unsigned long long int num, unsigned long long int divider )
    {
        ulldiv_t result;
        result.rem = num % divider;
        result.quot = num / divider;
        return result;
    }


    class token_t
    {
    public:
        uint64_t m_token;

        explicit token_t( const uint64_t token ) : m_token( token )
        {
        }

        [[nodiscard]] std::string to_string() const
        {
            uint64_t token_copy = m_token;
            std::string dest;

            ulldiv_t result;
            int i = 0;
            for ( ; token_copy != 0; ++i )
            {
                result = div( token_copy, 38ull );
                token_copy = result.quot;
                dest.push_back( psz_letters[ result.rem ] );
            }
            dest.push_back( '\0' );
            return dest;
        }
    };

    inline int get_id_char( const char letter )
    {
        for ( int i = 0; i < i_num_letters; ++i )
            if ( letter == psz_letters[ i ] )
                return i;
        return 0;
    }

    inline unsigned long long int powull( const unsigned long long int base, const long num )
    {
        unsigned long long int result = 1;
        for ( long i = 0; num > i; ++i )
            result = result * base;
        return result;
    }

    inline token_t string_to_token( const char* str )
    {
        token_t result( 0 );
        const unsigned len = strlen( str );
        for ( unsigned i = 0; i < len; i++ )
            result.m_token += powull( 38, i ) * static_cast< unsigned long long >( get_id_char( tolower( str[ i ] ) ) );
        return result;
    }
}
