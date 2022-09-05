//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_ENCODED_REF_IPP
#define BOOST_URL_IMPL_SEGMENTS_ENCODED_REF_IPP

#include <boost/url/segments_encoded_ref.hpp>
#include <boost/url/url.hpp>
#include <boost/url/detail/path.hpp>

namespace boost {
namespace urls {

auto
segments_encoded_ref::
insert(
    iterator before,
    pct_string_view s0) ->
        iterator
{
    string_view s = s0;
    u_->edit_segments(
        before.it_,
        before.it_,
        detail::make_segments_encoded_iter(
            &s, &s + 1));
    return std::next(begin(), before.it_.index);
}

auto
segments_encoded_ref::
erase(
    iterator first,
    iterator last) noexcept ->
        iterator
{
    string_view s;
    u_->edit_segments(
        first.it_,
        last.it_,
        detail::make_segments_encoded_iter(
            &s, &s));
    return std::next(begin(), first.it_.index);
}

} // urls
} // boost

#endif
