/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTER_SPLITTER_TAGVALUEINSTANCEREMOVESPLITTER_HPP__
#define __FWDICOMIOFILTER_SPLITTER_TAGVALUEINSTANCEREMOVESPLITTER_HPP__

#include <fwDicomData/DicomSeries.hpp>
#include "fwDicomIOFilter/splitter/ISplitter.hpp"
#include "fwDicomIOFilter/config.hpp"

namespace fwDicomIOFilter
{
namespace splitter
{

/**
 * @class TagValueInstanceRemoveSplitter
 * @brief Filter that remove instances according to tag value
 * @date  2014.
 */
class FWDICOMIOFILTER_CLASS_API TagValueInstanceRemoveSplitter : public ISplitter
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (TagValueInstanceRemoveSplitter)(ISplitter),
            (()), ::fwDicomIOFilter::factory::New< TagValueInstanceRemoveSplitter > );

    /// Constructor
    FWDICOMIOFILTER_API TagValueInstanceRemoveSplitter(::fwDicomIOFilter::IFilter::Key key);

    /// Destructor
    FWDICOMIOFILTER_API virtual ~TagValueInstanceRemoveSplitter();

    /// Override
    FWDICOMIOFILTER_API virtual DicomSeriesContainerType apply(::fwDicomData::DicomSeries::sptr series) const
        throw(::fwDicomIOFilter::exceptions::FilterFailure);

    /// Return the name of the filter
    FWDICOMIOFILTER_API virtual std::string getName() const;

    /// Return the description of the filter
    FWDICOMIOFILTER_API virtual std::string getDescription() const;

    /// Return true if a configuration is required
    FWDICOMIOFILTER_API virtual bool isConfigurationRequired();

    fwGettersSettersDocMacro(Tag, tag, DcmTagKey, Tag used to sort instances);
    fwGettersSettersDocMacro(TagValue, tagValue, std::string,
            Tag value used to determine if an instance must be removed);

protected:
    /// Filter name
    static const std::string s_FILTER_NAME;

    /// Filter description
    static const std::string s_FILTER_DESCRIPTION;

    /// Tag used to sort instances
    DcmTagKey m_tag;

    /// Tag value used to determine if an instance must be removed
    std::string m_tagValue;

};

} // namespace splitter
} // namespace fwDicomIOFilter


#endif /* __FWDICOMIOFILTER_SPLITTER_TAGVALUEINSTANCEREMOVESPLITTER_HPP__ */
