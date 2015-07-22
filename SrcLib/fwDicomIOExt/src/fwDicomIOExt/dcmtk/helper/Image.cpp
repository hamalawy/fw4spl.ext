/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/spyLog.hpp>

#include "fwDicomIOExt/dcmtk/helper/Image.hpp"

namespace fwDicomIOExt
{
namespace dcmtk
{
namespace helper
{

Image::Image(
    unsigned short samplesPerPixel,
    unsigned short bitsAllocated,
    unsigned short bitsStored,
    unsigned short highBit,
    unsigned short pixelRepresentation,
    double rescaleSlope,
    double rescaleIntercept) :
    m_samplesPerPixel(samplesPerPixel),
    m_bitsAllocated(bitsAllocated),
    m_bitsStored(bitsStored),
    m_highBit(highBit),
    m_pixelRepresentation(pixelRepresentation),
    m_rescaleSlope(rescaleSlope),
    m_rescaleIntercept(rescaleIntercept)
{
}

//-----------------------------------------------------------------------------

Image::~Image()
{
}

//-----------------------------------------------------------------------------

::fwTools::Type Image::findImageTypeFromMinMaxValues() const
{
    ::fwTools::Type result = ::fwTools::Type::s_UNSPECIFIED_TYPE;

    // Bool & Monochrome values
    if(m_bitsAllocated == 1 && m_pixelRepresentation == 0)
    {
        result = ::fwTools::Type::s_INT8;
    }
    else
    {
        // Double
        if(m_rescaleSlope != (int) m_rescaleSlope || m_rescaleIntercept != (int) m_rescaleIntercept)
        {
            result = ::fwTools::Type::s_DOUBLE;
        }
        else
        {
            const double min = m_rescaleSlope * (double)this->getPixelMin() + m_rescaleIntercept;
            const double max = m_rescaleSlope * (double)this->getPixelMax() + m_rescaleIntercept;

            SLM_ASSERT("Min must be lower than max.", min <= max );
            SLM_ASSERT("Min and max values must be of type int64_t.", min == (int64_t)min && max == (int64_t)max );

            // Unsigned values
            if(min >= 0)
            {
                if( max <= std::numeric_limits<uint8_t>::max() )
                {
                    result = ::fwTools::Type::s_UINT8;
                }
                else if( max <= std::numeric_limits<uint16_t>::max() )
                {
                    result = ::fwTools::Type::s_UINT16;
                }
                else if( max <= std::numeric_limits<uint32_t>::max() )
                {
                    result = ::fwTools::Type::s_UINT32;
                }
                else
                {
                    SLM_ASSERT("Unable to determine the pixel format.", 0 );
                }
            }
            // Signed values
            else
            {

                if( max <= std::numeric_limits<int8_t>::max() && min >= std::numeric_limits<int8_t>::min() )
                {
                    result = ::fwTools::Type::s_INT8;
                }
                else if( max <= std::numeric_limits<int16_t>::max() && min >= std::numeric_limits<int16_t>::min() )
                {
                    result = ::fwTools::Type::s_INT16;
                }
                else if( max <= std::numeric_limits<int32_t>::max() && min >= std::numeric_limits<int32_t>::min() )
                {
                    result = ::fwTools::Type::s_INT32;
                }
                else
                {
                    SLM_ASSERT("Unable to determine the pixel format.", 0 );
                }
            }

        }

    }

    return result;
}

//-----------------------------------------------------------------------------

int64_t Image::getPixelMin() const
{
    SLM_ASSERT("The number of bits allocated must be known", m_bitsAllocated);
    SLM_ASSERT("The number of bits stored must be less than 33.", m_bitsStored <= 32);

    if(m_pixelRepresentation == 1)
    {
        return (int64_t)(~(((1ull << m_bitsStored) - 1) >> 1));
    }
    else if(m_pixelRepresentation == 0)
    {
        return 0;
    }

    SLM_ASSERT("Unable to determine minimum value of pixel", 0);
    return 0;
}

//-----------------------------------------------------------------------------

int64_t Image::getPixelMax() const
{
    SLM_ASSERT("The number of bits allocated must be known", m_bitsAllocated);
    SLM_ASSERT("The number of bits stored must be less than 33.", m_bitsStored <= 32);

    if(m_pixelRepresentation == 1)
    {
        return (int64_t)(((1ull << m_bitsStored) - 1) >> 1);
    }
    else if(m_pixelRepresentation == 0)
    {
        return (int64_t)((1ull << m_bitsStored) - 1);
    }

    SLM_ASSERT("Unable to determine maximum value of pixel", 0);
    return 0;
}


} //dcmtk
} //helper
} //fwDicomIOExt
