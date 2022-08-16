//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PCT_ENCODING_IPP
#define BOOST_URL_IMPL_PCT_ENCODING_IPP

#include <boost/url/pct_encoding.hpp>
#include <boost/url/grammar/charset.hpp>
#include <memory>

namespace boost {
namespace urls {

std::size_t
pct_decode_bytes_unchecked(
    string_view s) noexcept
{
    auto it = s.data();
    auto const end =
        it + s.size();
    std::size_t n = 0;
    while(it < end)
    {
        if(*it != '%')
        {
            // unescaped
            ++it;
            ++n;
            continue;
        }
        if(end - it < 3)
            return n;
        it += 3;
        ++n;
    }
    return n;
}

std::size_t
pct_decode_unchecked(
    char* const dest0,
    char const* end,
    string_view s,
    pct_decode_opts const& opt) noexcept
{
    auto const decode_hex = [](
        char const* it)
    {
        auto d0 = grammar::hexdig_value(it[0]);
        auto d1 = grammar::hexdig_value(it[1]);
        return static_cast<char>(
            ((static_cast<
                unsigned char>(d0) << 4) +
            (static_cast<
                unsigned char>(d1))));
    };
    auto it = s.data();
    auto const last = it + s.size();
    auto dest = dest0;

    if(opt.plus_to_space)
    {
        while(it != last)
        {
            if(dest == end)
            {
                // dest too small
                return dest - dest0;
            }
            if(*it == '+')
            {
                // plus to space
                *dest++ = ' ';
                ++it;
                continue;
            }
            if(*it == '%')
            {
                // escaped
                ++it;
                if(last - it < 2)
                {
                    // missing input,
                    // initialize output
                    std::memset(dest,
                        0, end - dest);
                    return dest - dest0;
                }
                *dest++ = decode_hex(it);
                it += 2;
                continue;
            }
            // unescaped
            *dest++ = *it++;
        }
        return dest - dest0;
    }

    while(it != last)
    {
        if(dest == end)
        {
            // dest too small
            return dest - dest0;
        }
        if(*it == '%')
        {
            // escaped
            ++it;
            if(last - it < 2)
            {
                // missing input,
                // initialize output
                std::memset(dest,
                    0, end - dest);
                return dest - dest0;
            }
            *dest++ = decode_hex(it);
            it += 2;
            continue;
        }
        // unescaped
        *dest++ = *it++;
    }
    return dest - dest0;
}

namespace detail
{
result<std::size_t>
validate_pct_encoding(
    string_view s,
    std::true_type) noexcept
{
    const auto is_safe = [](char c)
    {
        return c != '%';
    };

    std::size_t pcts = 0;
    char const* it = s.data();
    char const* end = it + s.size();
    it = grammar::find_if_not(it, end, is_safe);
    while (it != end)
    {
        if (end - it < 2)
        {
            // missing HEXDIG
            BOOST_URL_RETURN_EC(
                error::missing_pct_hexdig);
        }
        if (!grammar::hexdig_chars(it[1]) ||
            !grammar::hexdig_chars(it[2]))
        {
            // expected HEXDIG
            BOOST_URL_RETURN_EC(
                error::bad_pct_hexdig);
        }
        it += 3;
        ++pcts;
        it = grammar::find_if_not(it, end, is_safe);
    }
    return s.size() - pcts * 2;
}

result<std::size_t>
validate_pct_encoding(
    string_view s,
    std::false_type) noexcept
{
    const auto is_safe = [](char c)
    {
        return c != '%' && c != '\0';
    };

    std::size_t pcts = 0;
    char const* it = s.data();
    char const* end = it + s.size();
    it = grammar::find_if_not(it, end, is_safe);
    while (it != end)
    {
        if (*it == '\0')
        {
            // null in input
            BOOST_URL_RETURN_EC(
                error::illegal_null);
        }
        if (end - it < 2)
        {
            // missing HEXDIG
            BOOST_URL_RETURN_EC(
                error::missing_pct_hexdig);
        }
        if (!grammar::hexdig_chars(it[1]) ||
            !grammar::hexdig_chars(it[2]))
        {
            // expected HEXDIG
            BOOST_URL_RETURN_EC(
                error::bad_pct_hexdig);
        }
        if (it[1] == '0' &&
            it[2] == '0')
        {
            // null in input
            BOOST_URL_RETURN_EC(
                error::illegal_null);
        }
        it += 3;
        ++pcts;
        it = grammar::find_if_not(it, end, is_safe);
    }
    return s.size() - pcts * 2;
}
}

result<std::size_t>
validate_pct_encoding(
    string_view s,
    pct_decode_opts const& opt) noexcept
{
    if (opt.allow_null)
        return detail::validate_pct_encoding(
            s, std::true_type{});
    else
       return detail::validate_pct_encoding(
            s, std::false_type{});
}

result<std::size_t>
pct_decode(
    char* dest,
    char const* end,
    string_view s,
    pct_decode_opts const& opt) noexcept
{
    auto const rn =
        validate_pct_encoding(s, opt);
    if( !rn )
        return rn;
    auto const n1 =
        pct_decode_unchecked(
            dest, end, s, opt);
    if(n1 < *rn)
    {
        return error::no_space;
    }
    return n1;
}


} // urls
} // boost

#endif
