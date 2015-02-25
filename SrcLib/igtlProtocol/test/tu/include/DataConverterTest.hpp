/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IGTLPROTOCOL_TU_DATACONVERTER_HPP__
#define __IGTLPROTOCOL_TU_DATACONVERTER_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace igtlProtocol
{
namespace ut
{

class DataConverterTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE(DataConverterTest);
CPPUNIT_TEST(meshConverterTest);
CPPUNIT_TEST(imageConverterTest);
CPPUNIT_TEST(matrixConverterTest);
CPPUNIT_TEST(pointListConverterTest);
CPPUNIT_TEST(stringConverterTest);
CPPUNIT_TEST(lineConverterTest);
CPPUNIT_TEST(scalarConverterTest);
CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void imageConverterTest();
    void matrixConverterTest();
    void pointListConverterTest();
    void stringConverterTest();
    void lineConverterTest();
    void scalarConverterTest();
    void meshConverterTest();

};

}
}

#endif // __IGTLPROTOCOL_TU_DATACONVERTER_HPP__
