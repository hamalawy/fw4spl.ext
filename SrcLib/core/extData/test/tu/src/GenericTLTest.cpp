/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "GenericTLTest.hpp"

#include <extData/timeline/GenericObject.hpp>
#include <extData/timeline/GenericObject.hxx>
#include <extData/GenericTL.hpp>
#include <extData/GenericTL.hxx>

#include <fwTest/Exception.hpp>
#include <fwData/registry/macros.hpp>


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::extData::ut::GenericTLTest );

namespace extData
{

//------------------------------------------------------------------------------

class Float3TL : public GenericTL< float[3] >
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (Float3TL)(::extData::TimeLine),(()), ::fwData::factory::New< Float3TL >)
    Float3TL( ::fwData::Object::Key key ) : GenericTL< float[3] >(key)
    {
    }
};
fwDataRegisterMacro( ::extData::Float3TL )

typedef float float4[4];

//------------------------------------------------------------------------------

class Float4TL : public GenericTL< float4 >
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (Float4TL)(::extData::TimeLine),(()), ::fwData::factory::New< Float4TL >)
    Float4TL( ::fwData::Object::Key key ) : GenericTL< float4 >(key)
    {
    }
};
fwDataRegisterMacro( ::extData::Float4TL )

//------------------------------------------------------------------------------

class TestContained
{
public:
    int m_int;
    double m_floats[2];
    std::vector<int>    m_vector;
};

class TestClassTL : public GenericTL< TestContained >
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (TestClassTL)(::extData::TimeLine),(()),
                                            ::fwData::factory::New< TestClassTL >)
    TestClassTL( ::fwData::Object::Key key ) : GenericTL< TestContained >(key)
    {
    }
};
fwDataRegisterMacro( ::extData::TestClassTL )

namespace ut
{

static ::fwTest::Exception fwTestException(""); // force link with fwTest

//------------------------------------------------------------------------------

void GenericTLTest::setUp()
{
    // Set up context before running a test.

}

//------------------------------------------------------------------------------

void GenericTLTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void GenericTLTest::pushTest()
{
    ::extData::Float4TL::sptr timeline = ::extData::Float4TL::New();
    timeline->initPoolSize(3);

    ::fwCore::HiResClock::HiResClockType time1 = ::fwCore::HiResClock::getTimeInMilliSec();
    ::fwCore::HiResClock::HiResClockType time2 = time1 + 42;

    float4 values1 = {1.0f, 5.2f, 7.5f, 1.f};
    float4 values2 = {4.0f, 5.5f, 1.5f, 2.f};
    float4 values3 = {1.0f, 3.2f, 2.5f, 0.f};

    float4 values4 = {-1.0f, 1.1f, 0.5f, -1.f};
    float4 values6 = {2.0f, 2.2f, -2.9f, 0.2f};

    // All elements set
    SPTR(::extData::Float4TL::BufferType) data1 = timeline->createBuffer(time1);
    data1->setElement(values1, 0);
    data1->setElement(values2, 1);
    data1->setElement(values3, 2);
    // Stupid but we need to test that
    data1->setElement(values3, 2);
    data1->setElement(values3, 2);
    data1->setElement(values1, 1);
    data1->setElement(values2, 1);

    // Second element missing
    SPTR(::extData::Float4TL::BufferType) data2 = timeline->createBuffer(time2);
    data2->setElement(values4, 0);

    // Another way to set element
    data2->addElement(2);
    float4* newValues = data2->addElement(2);
    memcpy(newValues, values6, sizeof(float4));

    timeline->pushObject(data1);
    timeline->pushObject(data2);

    CPPUNIT_ASSERT(data1->getTimestamp() == time1);
    CPPUNIT_ASSERT(data2->getTimestamp() == time2);

    ////////////////////////////////////////////////////////////////////////////////
    // Check first data with all elements set

    CSPTR(::extData::timeline::Object) dataPushed1 = timeline->getObject(time1);
    CPPUNIT_ASSERT(data1 == dataPushed1);
    CPPUNIT_ASSERT(dataPushed1->getTimestamp() == time1);

    CSPTR(::extData::timeline::Object) dataPushed2 = timeline->getObject(time2);
    CPPUNIT_ASSERT(data2 == dataPushed2);
    CPPUNIT_ASSERT(dataPushed2->getTimestamp() == time2);

    CSPTR(::extData::timeline::Object) dataPushed1Bis = timeline->getClosestObject(time1 + 1.5);
    CSPTR(::extData::Float4TL::BufferType) obj        =
        boost::dynamic_pointer_cast< const ::extData::Float4TL::BufferType >(dataPushed1Bis);
    CPPUNIT_ASSERT(obj);
    CPPUNIT_ASSERT_EQUAL(obj, timeline->getClosestBuffer(time1 + 1.5));

    CPPUNIT_ASSERT_EQUAL(3u, obj->getPresentElementNum());
    CPPUNIT_ASSERT_EQUAL(size_t(16), obj->getElementSize());
    CPPUNIT_ASSERT_EQUAL(3u, obj->getMaxElementNum());
    CPPUNIT_ASSERT_EQUAL(true, obj->isPresent(0));
    CPPUNIT_ASSERT_EQUAL(true, obj->isPresent(1));
    CPPUNIT_ASSERT_EQUAL(true, obj->isPresent(2));
    CPPUNIT_ASSERT_EQUAL(::boost::uint64_t(7), obj->getMask());

    const float* buffData = obj->getElement(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5, buffData[2], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, buffData[3], 0.00001);

    buffData = obj->getElement(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, buffData[2], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, buffData[3], 0.00001);

    buffData = obj->getElement(2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, buffData[2], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[3], 0.00001);

    ////////////////////////////////////////////////////////////////////////////////
    // Check second data with one missing element

    CSPTR(::extData::timeline::Object) dataPushed2Bis = timeline->getNewerObject();
    CPPUNIT_ASSERT(data2 == dataPushed2Bis);

    ::fwCore::HiResClock::HiResClockType time2Pushed = timeline->getNewerTimestamp();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(time2, time2Pushed, 0.00001);

    obj = boost::dynamic_pointer_cast< const ::extData::Float4TL::BufferType >(dataPushed2Bis);
    CPPUNIT_ASSERT(obj);

    CPPUNIT_ASSERT_EQUAL(2u, obj->getPresentElementNum());
    CPPUNIT_ASSERT_EQUAL(size_t(16), obj->getElementSize());
    CPPUNIT_ASSERT_EQUAL(3u, obj->getMaxElementNum());
    CPPUNIT_ASSERT_EQUAL(true, obj->isPresent(0));
    CPPUNIT_ASSERT_EQUAL(false, obj->isPresent(1));
    CPPUNIT_ASSERT_EQUAL(true, obj->isPresent(2));
    CPPUNIT_ASSERT_EQUAL(::boost::uint64_t(5), obj->getMask());

    buffData = obj->getElement(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1., buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, buffData[2], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1., buffData[3], 0.00001);

    buffData = obj->getElement(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[2], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[3], 0.00001);

    buffData = obj->getElement(2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.9, buffData[2], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.2, buffData[3], 0.00001);

    timeline->clearTimeline();
    CSPTR(::extData::timeline::Object) nullObj = timeline->getNewerObject();
    CPPUNIT_ASSERT(nullObj == NULL);
}

//------------------------------------------------------------------------------

void GenericTLTest::pushClassTest()
{
    ::extData::TestClassTL::sptr timeline = ::extData::TestClassTL::New();
    timeline->initPoolSize(3);

    ::fwCore::HiResClock::HiResClockType time1 = ::fwCore::HiResClock::getTimeInMilliSec();
    ::fwCore::HiResClock::HiResClockType time2 = time1 + 42;

    TestContained testData1;
    testData1.m_int       = 87;
    testData1.m_floats[0] = 12.79;
    testData1.m_floats[1] = -459.59f;
    testData1.m_vector.push_back(1);
    testData1.m_vector.push_back(2);
    testData1.m_vector.push_back(3);
    testData1.m_vector.push_back(4);
    testData1.m_vector.push_back(5);

    TestContained testData2;
    testData2.m_int       = -45471;
    testData2.m_floats[0] = 68.49;
    testData2.m_floats[1] = -9.994f;
    testData2.m_vector.push_back(5);
    testData2.m_vector.push_back(2);
    testData2.m_vector.push_back(-4);
    testData2.m_vector.push_back(-5);

    TestContained testData3;
    testData3.m_int       = 447471;
    testData3.m_floats[0] = 6822.49;
    testData3.m_floats[1] = -92194.47f;
    testData3.m_vector.push_back(9);
    testData3.m_vector.push_back(814);
    testData3.m_vector.push_back(9);

    // All elements set
    SPTR(::extData::TestClassTL::BufferType) data1 = timeline->createBuffer(time1);
    data1->setElement(testData1, 1);
    data1->setElement(testData2, 2);

    // Second element missing
    SPTR(::extData::TestClassTL::BufferType) data2 = timeline->createBuffer(time2);
    data2->setElement(testData3, 2);

    timeline->pushObject(data1);
    timeline->pushObject(data2);

    ////////////////////////////////////////////////////////////////////////////////
    // Check first data

    CSPTR(::extData::TestClassTL::BufferType) dataPushed1 = timeline->getBuffer(time1);
    CPPUNIT_ASSERT(dataPushed1);

    CPPUNIT_ASSERT_EQUAL(2u, dataPushed1->getPresentElementNum());
    CPPUNIT_ASSERT_EQUAL(sizeof(TestContained), dataPushed1->getElementSize());
    CPPUNIT_ASSERT_EQUAL(3u, dataPushed1->getMaxElementNum());
    CPPUNIT_ASSERT_EQUAL(false, dataPushed1->isPresent(0));
    CPPUNIT_ASSERT_EQUAL(true, dataPushed1->isPresent(1));
    CPPUNIT_ASSERT_EQUAL(true, dataPushed1->isPresent(2));
    CPPUNIT_ASSERT_EQUAL(::boost::uint64_t(6), dataPushed1->getMask());

    {
        const TestContained& testData = dataPushed1->getElement(1);
        CPPUNIT_ASSERT_EQUAL(testData1.m_int, testData.m_int);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(testData1.m_floats[0], testData.m_floats[0], 0.00001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(testData1.m_floats[1], testData.m_floats[1], 0.00001);
        CPPUNIT_ASSERT_EQUAL(testData1.m_vector.size(), testData.m_vector.size());
        for(size_t i = 0; i < testData1.m_vector.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(testData1.m_vector[i], testData.m_vector[i]);
        }
    }
    {
        const TestContained& testData = dataPushed1->getElement(2);
        CPPUNIT_ASSERT_EQUAL(testData2.m_int, testData.m_int);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(testData2.m_floats[0], testData.m_floats[0], 0.00001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(testData2.m_floats[1], testData.m_floats[1], 0.00001);
        CPPUNIT_ASSERT_EQUAL(testData2.m_vector.size(), testData.m_vector.size());
        for(size_t i = 0; i < testData2.m_vector.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(testData2.m_vector[i], testData.m_vector[i]);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Check second data

    CSPTR(::extData::TestClassTL::BufferType) dataPushed2 = timeline->getBuffer(time2);
    CPPUNIT_ASSERT(dataPushed2);

    CPPUNIT_ASSERT_EQUAL(1u, dataPushed2->getPresentElementNum());
    CPPUNIT_ASSERT_EQUAL(sizeof(TestContained), dataPushed2->getElementSize());
    CPPUNIT_ASSERT_EQUAL(3u, dataPushed2->getMaxElementNum());
    CPPUNIT_ASSERT_EQUAL(false, dataPushed2->isPresent(0));
    CPPUNIT_ASSERT_EQUAL(false, dataPushed2->isPresent(1));
    CPPUNIT_ASSERT_EQUAL(true, dataPushed2->isPresent(2));
    CPPUNIT_ASSERT_EQUAL(::boost::uint64_t(4), dataPushed2->getMask());

    {
        const TestContained& testData = dataPushed2->getElement(2);
        CPPUNIT_ASSERT_EQUAL(testData3.m_int, testData.m_int);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(testData3.m_floats[0], testData.m_floats[0], 0.00001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(testData3.m_floats[1], testData.m_floats[1], 0.00001);
        CPPUNIT_ASSERT_EQUAL(testData3.m_vector.size(), testData.m_vector.size());
        for(size_t i = 0; i < testData3.m_vector.size(); ++i)
        {
            CPPUNIT_ASSERT_EQUAL(testData3.m_vector[i], testData.m_vector[i]);
        }
    }
}

//------------------------------------------------------------------------------

void GenericTLTest::copyTest()
{
    ::extData::Float3TL::sptr timeline = ::extData::Float3TL::New();
    timeline->initPoolSize(3);

    ::fwCore::HiResClock::HiResClockType time1 = ::fwCore::HiResClock::getTimeInMilliSec();
    ::fwCore::HiResClock::HiResClockType time2 = time1 + 42;
    ::fwCore::HiResClock::HiResClockType time3 = time2 + 52;
    ::fwCore::HiResClock::HiResClockType time4 = time3 + 52;

    float values1[3] = {1.0f, 5.2f, 7.5f};
    float values2[3] = {4.0f, 5.5f, 1.5f};
    float values3[3] = {1.0f, 3.2f, 2.5f};
    float values4[3] = {2.0f, -.2f, 2.5f};
    float values5[3] = {8.0f, 9.0f, 66.f};
    float values6[3] = {2.0f, 1.2f, 11.f};

    SPTR(::extData::Float3TL::BufferType) data1 = timeline->createBuffer(time1);
    data1->setElement(values1, 0);
    CPPUNIT_ASSERT(data1->getTimestamp() == time1);

    SPTR(::extData::Float3TL::BufferType) data2 = timeline->createBuffer(time2);
    data2->setElement(values2, 0);
    data2->setElement(values3, 1);
    data2->setElement(values4, 2);
    CPPUNIT_ASSERT(data2->getTimestamp() == time2);

    SPTR(::extData::Float3TL::BufferType) data3 = timeline->createBuffer(time3);
    data3->setElement(values5, 1);
    data3->setElement(values6, 2);
    CPPUNIT_ASSERT(data3->getTimestamp() == time3);

    SPTR(::extData::Float3TL::BufferType) data4 = timeline->createBuffer(time4);
    timeline->pushObject(data1);
    timeline->pushObject(data2);
    timeline->pushObject(data3);
    timeline->pushObject(data4);
    CPPUNIT_ASSERT(data4->getTimestamp() == time4);

    // deepCopy test
    ::extData::Float3TL::sptr deepTimeline = ::extData::Float3TL::New();
    deepTimeline->deepCopy(timeline);
    timeline.reset();

    CSPTR(::extData::timeline::Object) deepDataPushed1 = deepTimeline->getObject(time1);
    CPPUNIT_ASSERT(deepDataPushed1);
    CPPUNIT_ASSERT(data1 != deepDataPushed1);
    CSPTR(::extData::Float3TL::BufferType) obj1 =
        ::boost::dynamic_pointer_cast< const ::extData::Float3TL::BufferType >(deepDataPushed1);
    CPPUNIT_ASSERT(obj1);
    CPPUNIT_ASSERT_EQUAL(obj1, deepTimeline->getBuffer(time1));

    /// First element
    CPPUNIT_ASSERT_EQUAL(1u, obj1->getPresentElementNum());
    CPPUNIT_ASSERT_EQUAL(size_t(12), obj1->getElementSize());
    CPPUNIT_ASSERT_EQUAL(3u, obj1->getMaxElementNum());
    CPPUNIT_ASSERT_EQUAL(true, obj1->isPresent(0));
    CPPUNIT_ASSERT_EQUAL(false, obj1->isPresent(1));
    CPPUNIT_ASSERT_EQUAL(false, obj1->isPresent(2));
    CPPUNIT_ASSERT_EQUAL(::boost::uint64_t(1), obj1->getMask());
    CPPUNIT_ASSERT(obj1->getTimestamp() == time1);

    const float* buffData = obj1->getElement(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5, buffData[2], 0.00001);

    buffData = obj1->getElement(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[2], 0.00001);

    buffData = obj1->getElement(2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[2], 0.00001);

    /// Second element
    CSPTR(::extData::Float3TL::BufferType) obj2 = deepTimeline->getBuffer(time2);

    CPPUNIT_ASSERT_EQUAL(3u, obj2->getPresentElementNum());
    CPPUNIT_ASSERT_EQUAL(size_t(12), obj2->getElementSize());
    CPPUNIT_ASSERT_EQUAL(3u, obj2->getMaxElementNum());
    CPPUNIT_ASSERT_EQUAL(true, obj2->isPresent(0));
    CPPUNIT_ASSERT_EQUAL(true, obj2->isPresent(1));
    CPPUNIT_ASSERT_EQUAL(true, obj2->isPresent(2));
    CPPUNIT_ASSERT_EQUAL(::boost::uint64_t(7), obj2->getMask());
    CPPUNIT_ASSERT(obj2->getTimestamp() == time2);

    buffData = obj2->getElement(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, buffData[2], 0.00001);

    buffData = obj2->getElement(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, buffData[2], 0.00001);

    buffData = obj2->getElement(2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, buffData[2], 0.00001);

    /// Third element
    CSPTR(::extData::Float3TL::BufferType) obj3 = deepTimeline->getBuffer(time3);

    CPPUNIT_ASSERT_EQUAL(2u, obj3->getPresentElementNum());
    CPPUNIT_ASSERT_EQUAL(size_t(12), obj3->getElementSize());
    CPPUNIT_ASSERT_EQUAL(3u, obj3->getMaxElementNum());
    CPPUNIT_ASSERT_EQUAL(false, obj3->isPresent(0));
    CPPUNIT_ASSERT_EQUAL(true, obj3->isPresent(1));
    CPPUNIT_ASSERT_EQUAL(true, obj3->isPresent(2));
    CPPUNIT_ASSERT_EQUAL(::boost::uint64_t(6), obj3->getMask());
    CPPUNIT_ASSERT(obj3->getTimestamp() == time3);

    buffData = obj3->getElement(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[2], 0.00001);

    buffData = obj3->getElement(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(66., buffData[2], 0.00001);

    buffData = obj3->getElement(2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11., buffData[2], 0.00001);

    /// Fourth element
    CSPTR(::extData::Float3TL::BufferType) obj4 = deepTimeline->getBuffer(time4);

    CPPUNIT_ASSERT_EQUAL(0u, obj4->getPresentElementNum());
    CPPUNIT_ASSERT_EQUAL(size_t(12), obj4->getElementSize());
    CPPUNIT_ASSERT_EQUAL(3u, obj4->getMaxElementNum());
    CPPUNIT_ASSERT_EQUAL(false, obj4->isPresent(0));
    CPPUNIT_ASSERT_EQUAL(false, obj4->isPresent(1));
    CPPUNIT_ASSERT_EQUAL(false, obj4->isPresent(2));
    CPPUNIT_ASSERT_EQUAL(::boost::uint64_t(0), obj4->getMask());
    CPPUNIT_ASSERT(obj4->getTimestamp() == time4);

    buffData = obj4->getElement(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[2], 0.00001);

    buffData = obj4->getElement(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[2], 0.00001);

    buffData = obj4->getElement(2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, buffData[2], 0.00001);
}

//------------------------------------------------------------------------------

void GenericTLTest::iteratorTest()
{
    ::extData::Float3TL::sptr timeline = ::extData::Float3TL::New();
    timeline->initPoolSize(5);

    ::fwCore::HiResClock::HiResClockType time1 = ::fwCore::HiResClock::getTimeInMilliSec();
    ::fwCore::HiResClock::HiResClockType time2 = time1 + 42;
    ::fwCore::HiResClock::HiResClockType time3 = time2 + 52;
    ::fwCore::HiResClock::HiResClockType time4 = time3 + 52;

    float values1[3] = {1.0f, 5.2f, 7.5f};
    float values2[3] = {4.0f, 5.5f, 1.5f};
    float values3[3] = {1.0f, 3.2f, 2.5f};
    float values4[3] = {2.0f, -.2f, 2.5f};
    float values5[3] = {8.0f, 9.0f, 66.f};
    float values6[3] = {2.0f, 1.2f, 11.f};

    SPTR(::extData::Float3TL::BufferType) data1 = timeline->createBuffer(time1);
    data1->setElement(values1, 0);

    SPTR(::extData::Float3TL::BufferType) data2 = timeline->createBuffer(time2);
    data2->setElement(values2, 0);
    data2->setElement(values3, 2);
    data2->setElement(values4, 4);

    SPTR(::extData::Float3TL::BufferType) data3 = timeline->createBuffer(time3);
    data3->setElement(values5, 1);
    data3->setElement(values6, 2);

    SPTR(::extData::Float3TL::BufferType) data4 = timeline->createBuffer(time4);

    timeline->pushObject(data1);
    timeline->pushObject(data2);
    timeline->pushObject(data3);
    timeline->pushObject(data4);

    CSPTR(::extData::Float3TL::BufferType) obj1 = timeline->getBuffer(time1);

    /// First element
    ::extData::Float3TL::BufferType::iterator it = obj1->getPresenceIterator();
    CPPUNIT_ASSERT_EQUAL(1u, obj1->getPresentElementNum());

    const float* buffData = *it;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5, buffData[2], 0.00001);

    ++it;
    CPPUNIT_ASSERT_EQUAL(false, it.isValid());

    /// Second element
    CSPTR(::extData::Float3TL::BufferType) obj2 = timeline->getBuffer(time2);
    it                                          = obj2->getPresenceIterator();
    CPPUNIT_ASSERT_EQUAL(3u, obj2->getPresentElementNum());

    buffData = *it;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, buffData[2], 0.00001);

    ++it;
    buffData = *it;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, buffData[2], 0.00001);

    ++it;
    buffData = *it;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, buffData[2], 0.00001);

    ++it;
    CPPUNIT_ASSERT_EQUAL(false, it.isValid());

    /// Third element
    CSPTR(::extData::Float3TL::BufferType) obj3 = timeline->getBuffer(time3);
    it                                          = obj3->getPresenceIterator();
    CPPUNIT_ASSERT_EQUAL(2u, obj3->getPresentElementNum());

    buffData = *it;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(66., buffData[2], 0.00001);

    ++it;
    buffData = *it;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, buffData[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, buffData[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11., buffData[2], 0.00001);

    ++it;
    CPPUNIT_ASSERT_EQUAL(false, it.isValid());

    /// Fourth element
    CSPTR(::extData::Float3TL::BufferType) obj4 = timeline->getBuffer(time4);
    it                                          = obj4->getPresenceIterator();
    CPPUNIT_ASSERT_EQUAL(0u, obj4->getPresentElementNum());

    CPPUNIT_ASSERT_EQUAL(false, it.isValid());
}

//------------------------------------------------------------------------------

void GenericTLTest::objectValid()
{
    ::extData::Float3TL::sptr timeline1 = ::extData::Float3TL::New();
    timeline1->initPoolSize(3);
    ::extData::Float4TL::sptr timeline2 = ::extData::Float4TL::New();
    timeline2->initPoolSize(3);

    ::fwCore::HiResClock::HiResClockType time1  = ::fwCore::HiResClock::getTimeInMilliSec();
    SPTR(::extData::Float3TL::BufferType) data1 = timeline1->createBuffer(time1);
    SPTR(::extData::Float4TL::BufferType) data2 = timeline2->createBuffer(time1);

    CPPUNIT_ASSERT_EQUAL(false, timeline1->isObjectValid(data2));
    CPPUNIT_ASSERT_EQUAL(true, timeline1->isObjectValid(data1));

    CPPUNIT_ASSERT_EQUAL(false, timeline2->isObjectValid(data1));
    CPPUNIT_ASSERT_EQUAL(true, timeline2->isObjectValid(data2));

}

} //namespace ut
} //namespace extData

