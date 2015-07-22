/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTER_CUSTOM_NOFILTER_HPP__
#define __FWDICOMIOFILTER_CUSTOM_NOFILTER_HPP__

#include <fwDicomData/DicomSeries.hpp>
#include "fwDicomIOFilter/custom/ICustom.hpp"
#include "fwDicomIOFilter/config.hpp"

namespace fwDicomIOFilter
{
namespace custom
{

/**
 * @class NoFilter
 * @brief Filter that doesn't apply any modification.
 * @date  2014.
 */
class FWDICOMIOFILTER_CLASS_API NoFilter : public ICustom
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (NoFilter)(ICustom), (()), ::fwDicomIOFilter::factory::New< NoFilter > );

    /// Constructor
    FWDICOMIOFILTER_API NoFilter(::fwDicomIOFilter::IFilter::Key key);

    /// Destructor
    FWDICOMIOFILTER_API virtual ~NoFilter();

    /// Override
    FWDICOMIOFILTER_API virtual DicomSeriesContainerType apply(::fwDicomData::DicomSeries::sptr series) const
    throw(::fwDicomIOFilter::exceptions::FilterFailure);

    /// Return the name of the filter
    FWDICOMIOFILTER_API virtual std::string getName() const;

    /// Return the description of the filter
    FWDICOMIOFILTER_API virtual std::string getDescription() const;

protected:

    /// Filter name
    static const std::string s_FILTER_NAME;

    /// Filter description
    static const std::string s_FILTER_DESCRIPTION;
};

} // namespace custom
} // namespace fwDicomIOFilter


#endif /* __FWDICOMIOFILTER_CUSTOM_NOFILTER_HPP__ */
