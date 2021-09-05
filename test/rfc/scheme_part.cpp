//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/rfc/scheme_part.hpp>

#include <boost/url/bnf/parse.hpp>
#include "test_suite.hpp"
#include "test_bnf.hpp"

namespace boost {
namespace urls {
namespace rfc {

class scheme_part_test
{
public:
    void
    check(
        string_view s,
        scheme id)
    {
        error_code ec;
        scheme_part p;
        using bnf::parse;
        if(! BOOST_TEST(
            parse(s, ec, p)))
            return;
        BOOST_TEST(
            ! ec.failed());
        BOOST_TEST(
            p.id == id);
    }

    void
    run()
    {
        using T = scheme_part;

        bad<T>({
            "",
            "1",
            " ",
            " http"
            "http "
            "nope:"
            });
        check ("http", scheme::http);
        check ("HTTP", scheme::http);
        check ("HtTp", scheme::http);
        check ("a1steak", scheme::unknown);
    }
};

TEST_SUITE(
    scheme_part_test,
    "boost.url.scheme_part");

} // rfc
} // urls
} // boost
