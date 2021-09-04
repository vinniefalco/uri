//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_BNF_IMPL_IPV_FUTURE_IPP
#define BOOST_URL_BNF_IMPL_IPV_FUTURE_IPP

#include <boost/url/rfc/ipv_future.hpp>
#include <boost/url/bnf/char_set.hpp>
#include <boost/url/bnf/parse.hpp>
#include <boost/url/rfc/char_sets.hpp>

namespace boost {
namespace urls {
namespace rfc {

char const*
parse(
    char const* const start,
    char const* const end,
    error_code& ec,
    ipv_future& t)
{
    using namespace bnf;
    string_view v0;
    string_view v1;
    auto it = parse(start, end, ec,
        'v',
        token<hexdig_chars>{v0},
        '.',
        token<masked_char_set<
            unsub_char_mask |
            colon_char_mask>>{v1});
    if(ec)
        return start;
    if(v0.empty())
    {
        // can't be empty
        ec = error::syntax;
        return start;
    }
    if(v1.empty())
    {
        // can't be empty
        ec = error::syntax;
        return start;
    }
    t.s_ = string_view(
        start, it - start);
    return it;
}

} // rfc
} // urls
} // boost

#endif
