/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <iconv.h>
#include <zlib.h>

#if !defined(_WIN32) && !defined(__MACOSX__)
#include <localcharset.h>
#endif

#include "fwDicomIOExt/Plugin.hpp"

namespace fwDicomIOExt
{

Plugin::Plugin()
{
    // Hack to force link with libiconv
    iconv_t i = iconv_open(0,0);

    // Hack to force link with zlib
    int j = deflateEnd(0);

    // Hack to force link with locale charset
#if !defined(_WIN32) && !defined(__MACOSX__)
    locale_charset();
#endif
}

Plugin::~Plugin()
{
}

} //namespace fwDicomIOExt


