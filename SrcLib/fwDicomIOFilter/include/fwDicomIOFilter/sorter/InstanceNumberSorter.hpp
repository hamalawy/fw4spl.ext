/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTER_SORTER_INSTANCENUMBERSORTER_HPP__
#define __FWDICOMIOFILTER_SORTER_INSTANCENUMBERSORTER_HPP__

#include <fwDicomData/DicomSeries.hpp>

#include "fwDicomIOFilter/sorter/TagValueSorter.hpp"
#include "fwDicomIOFilter/config.hpp"

namespace fwDicomIOFilter
{
namespace sorter
{

/**
 * @class InstanceNumberSorter
 * @brief Filter that uses the InstanceNumber tag to sort the instances.
 * @date  2014.
 */
class FWDICOMIOFILTER_CLASS_API InstanceNumberSorter : public ::fwDicomIOFilter::sorter::TagValueSorter
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (InstanceNumberSorter)(::fwDicomIOFilter::sorter::TagValueSorter),
            (()), ::fwDicomIOFilter::factory::New< InstanceNumberSorter > );

    /// Constructor
    FWDICOMIOFILTER_API InstanceNumberSorter(::fwDicomIOFilter::IFilter::Key key);

    /// Destructor
    FWDICOMIOFILTER_API virtual ~InstanceNumberSorter();

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

} // namespace sorter
} // namespace fwDicomIOFilter


#endif /* __FWDICOMIOFILTER_SORTER_INSTANCENUMBERSORTER_HPP__ */
