/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTER_SPLITTER_SOPCLASSUIDSPLITTER_HPP__
#define __FWDICOMIOFILTER_SPLITTER_SOPCLASSUIDSPLITTER_HPP__

#include <fwDicomData/DicomSeries.hpp>
#include "fwDicomIOFilter/splitter/TagValueSplitter.hpp"
#include "fwDicomIOFilter/config.hpp"

namespace fwDicomIOFilter
{
namespace splitter
{

/**
 * @class SOPClassUIDSplitter
 * @brief Filter that uses the SOPClassUID tag to split the instances
 * @date  2014.
 */
class FWDICOMIOFILTER_CLASS_API SOPClassUIDSplitter : public ::fwDicomIOFilter::splitter::TagValueSplitter
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (SOPClassUIDSplitter)(::fwDicomIOFilter::splitter::TagValueSplitter),
            (()), ::fwDicomIOFilter::factory::New< SOPClassUIDSplitter > );

    /// Constructor
    FWDICOMIOFILTER_API SOPClassUIDSplitter(::fwDicomIOFilter::IFilter::Key key);

    /// Destructor
    FWDICOMIOFILTER_API virtual ~SOPClassUIDSplitter();

    /// Override
    FWDICOMIOFILTER_API virtual DicomSeriesContainerType apply(::fwDicomData::DicomSeries::sptr series) const
        throw(::fwDicomIOFilter::exceptions::FilterFailure);

    /// Return the name of the filter
    FWDICOMIOFILTER_API virtual std::string getName() const;

    /// Return the description of the filter
    FWDICOMIOFILTER_API virtual std::string getDescription() const;

    /// Return true if a configuration is required
    FWDICOMIOFILTER_API virtual bool isConfigurationRequired();

protected:
    /// Filter name
    static const std::string s_FILTER_NAME;

    /// Filter description
    static const std::string s_FILTER_DESCRIPTION;

};

} // namespace splitter
} // namespace fwDicomIOFilter


#endif /* __FWDICOMIOFILTER_SPLITTER_SOPCLASSUIDSPLITTER_HPP__ */
