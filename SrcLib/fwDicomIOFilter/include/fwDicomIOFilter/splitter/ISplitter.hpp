/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTER_SPLITTER_ISPLITTER_HPP__
#define __FWDICOMIOFILTER_SPLITTER_ISPLITTER_HPP__

#include <fwDicomData/DicomSeries.hpp>

#include "fwDicomIOFilter/IFilter.hpp"
#include "fwDicomIOFilter/config.hpp"

namespace fwDicomIOFilter
{
namespace splitter
{

/**
 * @class ISplitter
 * @brief Base class for Dicom instance splitter.
 * @date  2014.
 */
class FWDICOMIOFILTER_CLASS_API ISplitter : public ::fwDicomIOFilter::IFilter
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro( (ISplitter)(::fwDicomIOFilter::IFilter) );

    /// Destructor
    FWDICOMIOFILTER_API virtual ~ISplitter();

    /// Return filter type
    FWDICOMIOFILTER_API FilterType getFilterType() const;

protected:
    /// Constructor
    FWDICOMIOFILTER_API ISplitter();
};

} // namespace splitter
} // namespace fwDicomIOFilter


#endif /* __FWDICOMIOFILTER_SPLITTER_ISPLITTER_HPP__ */
