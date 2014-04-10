/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_CONTAINER_DICOMSURFACE_HPP__
#define __GDCMIO_CONTAINER_DICOMSURFACE_HPP__

#include <stdint.h> // for uint16_t on Windows
#include <fwData/Mesh.hpp>
#include <fwData/Reconstruction.hpp>

#include "gdcmIO/config.hpp"

namespace gdcmIO
{
namespace container
{

/**
 * @class DicomSurface
 * @brief This class defines one surface mesh item in order to transform into DICOM/FW4SPL form.
 */
class GDCMIO_CLASS_API DicomSurface
{

public:

    /// Constructor
    GDCMIO_API DicomSurface();

    /// Destructor
    GDCMIO_API ~DicomSurface();

    /**
     * @brief Set members from a reconstruction.
     * @param[in] reconstruction Source reconstruction
     */
    GDCMIO_API void setFromData(fwData::Reconstruction::csptr reconstruction);

    /**
     * @brief Set a reconstruction from DicomSurface data.
     * @note All members have to be set before.
     * @param[in] reconstruction Destination reconstruction
     */
    GDCMIO_API void convertToData(fwData::Reconstruction::sptr reconstruction);

    /**
     * @brief Convert Surface to ::fwData::Mesh
     * @param[in] coord Coordinates
     * @param[in] coordSize Number of coordinates
     * @param[in] index Indices
     * @param[in] indexSize Number of indices
     * @param[in] normalCoord Normal coordinates
     */
    GDCMIO_API ::fwData::Mesh::sptr convertToData(const float *coord, const unsigned long coordSize,
            const uint32_t *index, const unsigned long indexSize, const float * normalCoord);

    /// Return point coordinates
    GDCMIO_API const ::boost::shared_ptr< float > getPointCoordData() const;

    /// Return number of points
    GDCMIO_API unsigned long getPointCoordSize() const;

    /// Return index list
    GDCMIO_API const ::boost::shared_ptr< uint32_t > getPointIndexList() const;

    /// Return number of index
    GDCMIO_API unsigned long getPointIndexSize() const;

    /// Return normal list
    GDCMIO_API const ::boost::shared_ptr< float > getNormalCoordData() const;

    /// Return number of normals
    GDCMIO_API unsigned long getNormalCoordSize() const;

    /**
     * @brief Set point coordinates
     * @param[in] array Point coordinates
     */
    GDCMIO_API void setPointCoordData(const ::boost::shared_ptr< float > array);

    /**
     * @brief Set number of points
     * @param[in] size Number of points
     */
    GDCMIO_API void setPointCoordSize(const unsigned long size);

    /**
     * @brief Set index list
     * @param[in] array Index list
     */
    GDCMIO_API void setPointIndexList(const ::boost::shared_ptr< uint32_t > array);

    /**
     * @brief Set number of index
     * @param[in] size Number of index
     */
    GDCMIO_API void setPointIndexSize(const unsigned long size);

    /**
     * @brief Set normals list
     * @param[in] array Normals list
     */
    GDCMIO_API void setNormalCoordData(const ::boost::shared_ptr< float > array);

    /**
     * @brief Set number of normals
     * @param[in] size Number of normals
     */
    GDCMIO_API void setNormalCoordSize(const unsigned long size);

private:

    /// Surface Points Coordinates (List of points coordinates for one surface) (eg : x1,y1,z1, x2,y2,z2, ...).
    ::boost::shared_ptr< float > m_pointCoordData;

    /// Number of points.
    unsigned long m_pointCoordSize;

    /// Surface Mesh Primitives (List of points index for all triangle of one surface) (eg : p1,p3,p2, p2,p3,p4, ...).
    /// Primitives will be written in Triangle Point Index List ( Tag(0066,0023) ) which has a VR equal to OW
    /// VR::OW is a string of 16-bit words where the encoding of the contents is specified by the negotiated Transfer Syntax
    /// Here, we try to force VR::OW to have a length of 32 bits for each primitive
    ::boost::shared_ptr< uint32_t > m_pointIndexList;

    /// Number of primitives/cells.
    unsigned long m_pointIndexSize;

    /// Surface Point Normal Coordinates (List of point normal coordinates for one surface) (eg : x1,y1,z1, x2,y2,z2, ...).
    ::boost::shared_ptr< float > m_normalCoordData;

    /// Number of normals.
    unsigned long m_normalCoordSize;
};

} //namespace container
} //namespace gdcmIO

#endif /* __GDCMIO_CONTAINER_DICOMSURFACE_HPP__ */
