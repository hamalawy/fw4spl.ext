/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTERQT_SPLITTER_TAGVALUEINSTANCEREMOVECONFIGURABLESPLITTER_HPP__
#define __FWDICOMIOFILTERQT_SPLITTER_TAGVALUEINSTANCEREMOVECONFIGURABLESPLITTER_HPP__

#include <fwDicomData/DicomSeries.hpp>
#include <fwDicomIOFilter/splitter/TagValueInstanceRemoveSplitter.hpp>

#include "fwDicomIOFilterQt/config.hpp"

namespace fwDicomIOFilterQt
{
namespace splitter
{

/**
 * @class TagValueInstanceRemoveConfigurableSplitter
 * @brief Filter that remove instances if a tag has a certain value.
 * @date  2014.
 */
class FWDICOMIOFILTERQT_CLASS_API TagValueInstanceRemoveConfigurableSplitter :
public ::fwDicomIOFilter::splitter::TagValueInstanceRemoveSplitter
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (TagValueInstanceRemoveConfigurableSplitter)
            (::fwDicomIOFilter::splitter::TagValueInstanceRemoveSplitter),
            (()), ::fwDicomIOFilter::factory::New< TagValueInstanceRemoveConfigurableSplitter > );

    /// Constructor
    FWDICOMIOFILTERQT_API TagValueInstanceRemoveConfigurableSplitter(::fwDicomIOFilter::IFilter::Key key);

    /// Destructor
    FWDICOMIOFILTERQT_API virtual ~TagValueInstanceRemoveConfigurableSplitter();

    /// Return the name of the filter
    FWDICOMIOFILTERQT_API virtual std::string getName() const;

    /// Return the description of the filter
    FWDICOMIOFILTERQT_API virtual std::string getDescription() const;

    /// Return true if a configuration is required
    FWDICOMIOFILTERQT_API virtual bool isConfigurationRequired();

    /// Return true if the filter is configurable using GUI
    FWDICOMIOFILTERQT_API virtual bool isConfigurableWithGUI();

    /// Configure the filter using GUI dialog
    FWDICOMIOFILTERQT_API virtual void configureWithGUI();

protected:
    /// Filter name
    FWDICOMIOFILTERQT_API static const std::string s_FILTER_NAME;

    /// Filter description
    FWDICOMIOFILTERQT_API static const std::string s_FILTER_DESCRIPTION;

};

} // namespace splitter
} // namespace fwDicomIOFilterQt


#endif /* __FWDICOMIOFILTERQT_SPLITTER_TAGVALUEINSTANCEREMOVECONFIGURABLESPLITTER_HPP__ */
