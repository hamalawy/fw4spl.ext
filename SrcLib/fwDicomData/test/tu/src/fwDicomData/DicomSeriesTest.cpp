/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fstream>
#include <boost/filesystem/operations.hpp>

#include <fwTools/Type.hpp>
#include <fwTools/System.hpp>
#include <fwData/Array.hpp>

#include "fwDicomData/DicomSeriesTest.hpp"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwDicomData::ut::DicomSeriesTest );

namespace fwDicomData
{
namespace ut
{

//------------------------------------------------------------------------------

void DicomSeriesTest::setUp()
{
    // Set up context before running a test.
    m_series = ::fwDicomData::DicomSeries::New();
}

//------------------------------------------------------------------------------

void DicomSeriesTest::tearDown()
{
    // Clean up after the test run.
    m_series.reset();
}

//------------------------------------------------------------------------------

void DicomSeriesTest::dicomTest()
{
    CPPUNIT_ASSERT(m_series);

    //Create Path
    const ::boost::filesystem::path path = ::fwTools::System::getTemporaryFolder() / "dicomtest";
    ::boost::filesystem::create_directories(path);
    const std::string filename = path.string()+"/"+"file";
    std::ofstream file;
    file.open(filename.c_str(), std::ofstream::out);
    file << "42";
    file.close();

    //None
    CPPUNIT_ASSERT_EQUAL(::fwDicomData::DicomSeries::NONE, m_series->getDicomAvailability());
    CPPUNIT_ASSERT(!m_series->isInstanceAvailable(42));
    m_series->setNumberOfInstances(100);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100,m_series->getNumberOfInstances(),0);

    //Paths
    m_series->setDicomAvailability(::fwDicomData::DicomSeries::PATHS);
    CPPUNIT_ASSERT_EQUAL(::fwDicomData::DicomSeries::PATHS, m_series->getDicomAvailability());
    m_series->addDicomPath(42, filename);
    CPPUNIT_ASSERT(m_series->isInstanceAvailable(42));

    //Binaries
    m_series->setDicomAvailability(::fwDicomData::DicomSeries::BINARIES);
    CPPUNIT_ASSERT_EQUAL(::fwDicomData::DicomSeries::BINARIES, m_series->getDicomAvailability());
    ::fwData::Array::sptr array = ::fwData::Array::New();
    m_series->addBinary(filename, array);
    CPPUNIT_ASSERT_EQUAL(array, m_series->getDicomBinaries().at(filename));

}

//------------------------------------------------------------------------------

} //namespace ut
} //namespace fwDicomData
