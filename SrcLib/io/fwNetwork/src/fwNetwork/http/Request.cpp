/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/make_shared.hpp>

#include "fwNetwork/http/Request.hpp"

namespace fwNetwork {

namespace http {

Request::Request()
{
}

Request::Request(const std::string & url) : m_url(url)
{
}

Request::sptr Request::New(const std::string& url)
{
    return ::boost::make_shared<Request>(url);
}

void Request::addHeader(const std::string & key, const std::string & value)
{
    m_headers[key] = value;
}

void Request::setHeaders(const HeadersType & headers)
{
    m_headers = headers;
}

const Request::HeadersType& Request::getHeaders() const
{
    return m_headers;
}

const std::string& Request::getUrl()const
{
    return m_url;
}

void Request::setUrl(const std::string& url)
{
    m_url = url;
}

} // namespace http
} // namespace fwNetwork

