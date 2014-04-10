/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <string>
#include <sstream>

#include <boost/foreach.hpp>

#include <fwComEd/helper/Composite.hpp>
#include <fwComEd/StringMsg.hpp>
#include <fwCore/base.hpp>
#include <fwData/Composite.hpp>
#include <fwData/String.hpp>
#include <fwDicomIOFilter/IFilter.hpp>
#include <fwGui/dialog/SelectorDialog.hpp>
#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/helper.hpp>
#include <fwServices/IEditionService.hpp>
#include <fwServices/macros.hpp>


#include "ioDicomExt/dcmtk/editor/SFilterSelectorDialog.hpp"

namespace ioDicomExt
{

namespace dcmtk
{

namespace editor
{

//------------------------------------------------------------------------------

fwServicesRegisterMacro( ::gui::editor::IDialogEditor , ::ioDicomExt::dcmtk::editor::SFilterSelectorDialog , ::fwData::String );

//------------------------------------------------------------------------------

SFilterSelectorDialog::SFilterSelectorDialog() :
    m_filtersAreExcluded   ( true )
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

SFilterSelectorDialog::~SFilterSelectorDialog()  throw()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SFilterSelectorDialog::configuring() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    m_selectedFilters.clear();

    //  Config Elem
    //  <selection mode="exclude">
    //  <addSelection filter="::fwDicomIOFilter::composite::CTImageStorageDefaultComposite" />
    //  <addSelection filter="::fwDicomIOFilter::composite::CTImageStorageDefaultComposite" />

    ::fwRuntime::ConfigurationElementContainer::Iterator iter = this->m_configuration->begin() ;
    for( ; iter != this->m_configuration->end() ; ++iter )
    {
        SLM_INFO( "SFilterSelectorDialog "  + (*iter)->getName());

        if( (*iter)->getName() == "selection" )
        {
            SLM_ASSERT( "Sorry, xml element <selection> must have attribute 'mode'.", (*iter)->hasAttribute("mode")) ;
            const std::string mode = (*iter)->getExistingAttributeValue("mode") ;
            m_filtersAreExcluded = ( mode == "exclude" );
            SLM_ASSERT( "Sorry, xml attribute <mode> must be 'exclude' or 'include'.", mode == "exclude" ||
                    mode == "include" );
            SLM_DEBUG( "mode => " + mode );
        }

        if( (*iter)->getName() == "addSelection" )
        {
            SLM_ASSERT( "Sorry, xml element <addSelection> must have attribute 'filter'.",
                    (*iter)->hasAttribute("filter")) ;
            m_selectedFilters.push_back( (*iter)->getExistingAttributeValue("filter") ) ;
            SLM_DEBUG( "add selection => " + (*iter)->getExistingAttributeValue("filter") );
        }

    }

}

//------------------------------------------------------------------------------

void SFilterSelectorDialog::starting() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SFilterSelectorDialog::stopping() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SFilterSelectorDialog::updating() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    // Retrieve available filters
    std::vector< ::fwDicomIOFilter::IFilter::sptr > registredFilters;
    BOOST_FOREACH(std::string key, ::fwDicomIOFilter::registry::get()->getFactoryKeys())
    {
        ::fwDicomIOFilter::IFilter::sptr filter = ::fwDicomIOFilter::factory::New(key);
        registredFilters.push_back(filter);
    }

    // Filter available extensions and replace id by service description
    std::map< std::string, ::fwDicomIOFilter::IFilter::sptr > availableFiltersMap;
    std::vector< std::string > availableFilterNames;

    BOOST_FOREACH( ::fwDicomIOFilter::IFilter::sptr  filter, registredFilters )
    {
        const bool filterIsSelectedByUser = std::find( m_selectedFilters.begin(), m_selectedFilters.end(),
                filter->getClassname() ) != m_selectedFilters.end();

        // Test if the filter is considered here as available by users
        // excluded mode => add filters that are not selected by users
        // included mode => add filters selected by users
        if( (m_filtersAreExcluded && ! filterIsSelectedByUser) ||
            (! m_filtersAreExcluded && filterIsSelectedByUser) )
        {
            // Add this filter
            std::string filterName = filter->getName();
            filterName = (filterName.empty())?filter->getClassname():filterName;
            availableFiltersMap[filterName] = filter;
            availableFilterNames.push_back( filterName );
        }
    }

    // Sort available services (lexical string sort)
    std::sort( availableFilterNames.begin(), availableFilterNames.end() );

    // Test if we have an extension
    if ( ! availableFilterNames.empty() )
    {
        std::string filterName = *availableFilterNames.begin() ;
        bool filterSelectionIsCanceled = false;

        // Selection of extension when availableFilterNames.size() > 1
        if ( availableFilterNames.size() > 1 )
        {
            ::fwGui::dialog::SelectorDialog::sptr selector = ::fwGui::dialog::SelectorDialog::New();

            selector->setTitle("Filter to use");
            selector->setSelections(availableFilterNames);
            filterName = selector->show();
            filterSelectionIsCanceled = filterName.empty();

            SLM_ASSERT("Unable to find the selected filter name in the filter map.",
                    filterSelectionIsCanceled || availableFiltersMap.find(filterName) != availableFiltersMap.end() );
        }

        if ( ! filterSelectionIsCanceled )
        {

            ::fwDicomIOFilter::IFilter::sptr filter = availableFiltersMap[filterName];

            ::fwData::String::sptr obj = this->getObject< ::fwData::String >();
            SLM_ASSERT("The filter selector service must work on a ::fwData::String object.", obj);
            obj->setValue(filter->getClassname());

            ::fwComEd::StringMsg::sptr msg = ::fwComEd::StringMsg::New();
            msg->addEvent(::fwComEd::StringMsg::VALUE_IS_MODIFIED);
            ::fwServices::IEditionService::notify(this->getSptr(),  obj, msg);

        }
    }
    else
    {
        SLM_WARN("SFilterSelectorDialog::load : availableFilters is empty.");
        ::fwGui::dialog::MessageDialog messageBox;
        messageBox.setTitle("Filter not found");
        messageBox.setMessage( "Sorry, there is no available filter for this reader." );
        messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
        messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
        messageBox.show();
    }
}

//------------------------------------------------------------------------------

void SFilterSelectorDialog::receiving( ::fwServices::ObjectMsg::csptr ) throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SFilterSelectorDialog::info( std::ostream &_sstream )
{
    // Update message
    _sstream << "SFilterSelectorDialog";
}

//------------------------------------------------------------------------------

} // namespace editor
} // namespace dcmtk
} // namespace ioDicomExt
