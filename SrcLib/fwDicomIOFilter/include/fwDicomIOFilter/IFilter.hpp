/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTER_IFILTER_HPP__
#define __FWDICOMIOFILTER_IFILTER_HPP__

#include <fwDicomData/DicomSeries.hpp>
#include <fwTools/Object.hpp>

#include "fwDicomIOFilter/exceptions/FilterFailure.hpp"
#include "fwDicomIOFilter/factory/new.hpp"
#include "fwDicomIOFilter/registry/detail.hpp"

#include "fwDicomIOFilter/config.hpp"

namespace fwDicomIOFilter
{

/**
 * @class IFilter
 * @brief Base class for Dicom instance filter.
 * @date  2014.
 */
class FWDICOMIOFILTER_CLASS_API IFilter :   public ::fwTools::Object,
                                            public ::fwTools::DynamicAttributes< ::fwDicomIOFilter::IFilter >
{
public:

    typedef ::fwDicomIOFilter::factory::Key Key;

    /**
     * @brief Class used to register a class factory in factory registry.
     * This class defines also the object factory ( 'create' )
     *
     * @tparam T Factory product type
     */
    template <typename T>
    class Registrar
    {
    public:
        Registrar()
        {
            ::fwDicomIOFilter::registry::get()->addFactory(T::classname(), &::fwDicomIOFilter::factory::New<T>);
        }
    };

    fwCoreNonInstanciableClassDefinitionsMacro( (IFilter)(::fwTools::Object) );

    typedef std::vector< ::fwDicomData::DicomSeries::sptr > DicomSeriesContainerType;

    /**
     * @brief Filter types
     */
    typedef enum
    {
        MODIFIER  = 0,  /*! Modifier */
        SORTER    = 1,  /*! Sorter */
        SPLITTER  = 2,  /*! Splitter */
        COMPOSITE = 3,  /*! Composite */
        CUSTOM    = 4,  /*! Custom */
    } FilterType;

    /// Destructor
    FWDICOMIOFILTER_API virtual ~IFilter();

    /**
     * @brief Apply the filter
     * @return Returns one or more Dicom Instance Group
     */
    FWDICOMIOFILTER_API virtual DicomSeriesContainerType apply(::fwDicomData::DicomSeries::sptr series) const
    throw(::fwDicomIOFilter::exceptions::FilterFailure) = 0;

    /// Return the name of the filter
    FWDICOMIOFILTER_API virtual std::string getName() const = 0;

    /// Return the description of the filter
    FWDICOMIOFILTER_API virtual std::string getDescription() const = 0;

    /// Return filter type
    FWDICOMIOFILTER_API virtual FilterType getFilterType() const = 0;

    /// Return true if a configuration is required
    FWDICOMIOFILTER_API virtual bool isConfigurationRequired();

    /// Return true if the filter is configurable using GUI
    FWDICOMIOFILTER_API virtual bool isConfigurableWithGUI();

    /// Configure the filter using GUI dialog
    FWDICOMIOFILTER_API virtual void configureWithGUI();

protected:

    /// Constructor
    FWDICOMIOFILTER_API IFilter();

};

} // namespace fwDicomIOFilter


#endif /* __FWDICOMIOFILTER_IFILTER_HPP__ */
