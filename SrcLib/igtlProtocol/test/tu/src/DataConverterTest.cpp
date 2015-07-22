/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "DataConverterTest.hpp"

#include <igtlProtocol/DataConverter.hpp>
#include <igtlProtocol/RawMessage.hpp>

#include <fwData/Float.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Image.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/Line.hpp>
#include <fwData/Object.hpp>
#include <fwData/PointList.hpp>
#include <fwData/String.hpp>
#include <fwData/TransformationMatrix3D.hpp>
#include <fwDataCamp/visitor/CompareObjects.hpp>

#include <fwComEd/helper/Mesh.hpp>

#include <fwTools/Type.hpp>
#include <fwTest/generator/Image.hpp>
#include <fwTest/generator/Mesh.hpp>
#include <fwTest/helper/compare.hpp>

#include <igtl/igtlPointMessage.h>
#include <igtl/igtlPositionMessage.h>
#include <igtl/igtlStringMessage.h>
#include <igtl/igtlTransformMessage.h>
#include <igtl/igtlImageMessage.h>

#include <algorithm>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( ::igtlProtocol::ut::DataConverterTest );

namespace igtlProtocol
{
namespace ut
{
static const double epsilon = 0.00001;

//------------------------------------------------------------------------------

void DataConverterTest::setUp()
{
}

//------------------------------------------------------------------------------

void DataConverterTest::tearDown()
{
}

//------------------------------------------------------------------------------

void DataConverterTest::meshConverterTest()
{
    DataConverter::sptr converter = DataConverter::getInstance();
    ::fwData::Mesh::sptr mesh = ::fwData::Mesh::New();

    ::fwData::Object::sptr obj;

    ::fwTest::generator::Mesh::generateMesh(mesh);

    ::igtl::MessageBase::Pointer msg = converter->fromFwObject(mesh);

    CPPUNIT_ASSERT_MESSAGE("Message is null", msg);

    obj = ::fwData::Mesh::New();
    converter->fromIgtlMessage(msg, obj);

    ::fwData::Mesh::sptr mesh2 = ::fwData::Mesh::dynamicCast(obj);
    CPPUNIT_ASSERT_MESSAGE("Mesh is null", mesh2);

    ::fwComEd::helper::Mesh meshHelper(mesh);
    ::fwComEd::helper::Mesh mesh2Helper(mesh2);

    CPPUNIT_ASSERT_EQUAL(mesh->getNumberOfPoints(), mesh2->getNumberOfPoints());
    CPPUNIT_ASSERT_EQUAL(mesh->getNumberOfCells(), mesh2->getNumberOfCells());
    CPPUNIT_ASSERT_EQUAL(mesh->getCellDataSize(), mesh2->getCellDataSize());

    CPPUNIT_ASSERT(mesh->getPointsArray()->getSize()          == mesh2->getPointsArray()->getSize());
    CPPUNIT_ASSERT(mesh->getCellTypesArray()->getSize()       == mesh2->getCellTypesArray()->getSize());
    CPPUNIT_ASSERT(mesh->getCellDataOffsetsArray()->getSize() == mesh2->getCellDataOffsetsArray()->getSize());

    CPPUNIT_ASSERT(mesh->getCellDataArray()->getSize()        == mesh2->getCellDataArray()->getSize());
    CPPUNIT_ASSERT(mesh->getPointColorsArray()->getSize()     == mesh2->getPointColorsArray()->getSize());
    CPPUNIT_ASSERT(mesh->getCellColorsArray()->getSize()      == mesh2->getCellColorsArray()->getSize());

    CPPUNIT_ASSERT(mesh->getPointNormalsArray()->getSize()    == mesh2->getPointNormalsArray()->getSize());
    CPPUNIT_ASSERT(mesh->getCellNormalsArray()->getSize()     == mesh2->getCellNormalsArray()->getSize());

    ::fwData::Mesh::PointsMultiArrayType meshPointArray  = meshHelper.getPoints();
    ::fwData::Mesh::PointsMultiArrayType mesh2PointArray = mesh2Helper.getPoints();

    ::fwData::Mesh::CellTypesMultiArrayType meshCellArray  = meshHelper.getCellTypes();
    ::fwData::Mesh::CellTypesMultiArrayType mesh2CellArray = meshHelper.getCellTypes();


    ::fwData::Mesh::PointColorsMultiArrayType meshPointColorArray  = meshHelper.getPointColors();
    ::fwData::Mesh::PointColorsMultiArrayType mesh2PointColorArray = mesh2Helper.getPointColors();

    ::fwData::Mesh::PointNormalsMultiArrayType meshPointNormalsArray  = meshHelper.getPointNormals();
    ::fwData::Mesh::PointNormalsMultiArrayType mesh2PointNormalsArray = mesh2Helper.getPointNormals();

    ::fwData::Mesh::CellColorsMultiArrayType meshCellColorArray  = meshHelper.getCellColors();
    ::fwData::Mesh::CellColorsMultiArrayType mesh2CellColorArray = mesh2Helper.getCellColors();

    ::fwData::Mesh::CellNormalsMultiArrayType meshCellNormalsArray  = meshHelper.getCellNormals();
    ::fwData::Mesh::CellNormalsMultiArrayType mesh2CellNormalsArray = mesh2Helper.getCellNormals();


    for (unsigned int i = 0; i< mesh->getNumberOfPoints(); i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            CPPUNIT_ASSERT_EQUAL(meshPointArray[i][j], mesh2PointArray[i][j]);

        }
    }

    for (unsigned int i = 0; i< mesh->getNumberOfCells(); i++)
    {
        CPPUNIT_ASSERT_EQUAL(meshCellArray[i], mesh2CellArray[i]);
    }


}

//------------------------------------------------------------------------------

void DataConverterTest::imageConverterTest()
{
    DataConverter::sptr converter = DataConverter::getInstance();
    ::fwData::Image::sptr image  = ::fwData::Image::New();
    ::fwData::Image::sptr image2 = ::fwData::Image::New();
    ::fwData::Object::sptr obj;
    ::fwTools::Type type = ::fwTools::Type::create< ::boost::int32_t >();
    ::fwTest::generator::Image::generateRandomImage(image, type);

    ::igtl::MessageBase::Pointer msg = converter->fromFwObject(image);

    CPPUNIT_ASSERT_MESSAGE("Message is null", msg);

    obj = ::fwData::Image::New();
    converter->fromIgtlMessage(msg, obj);

    image2 = ::fwData::Image::dynamicCast(obj);

    CPPUNIT_ASSERT_MESSAGE("Image is null", image2);

    ::fwTest::helper::ExcludeSetType exclude;
    exclude.insert("array.isOwner");
    exclude.insert("window_center");
    exclude.insert("window_width");
    exclude.insert("spacing.0");
    exclude.insert("origin.0");
    exclude.insert("spacing.1");
    exclude.insert("origin.1");
    exclude.insert("spacing.2");
    exclude.insert("origin.2");

    CPPUNIT_ASSERT_DOUBLES_EQUAL(image->getSpacing()[0],image2->getSpacing()[0],epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(image->getSpacing()[1],image2->getSpacing()[1],epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(image->getSpacing()[2],image2->getSpacing()[2],epsilon);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(image->getOrigin()[0],image2->getOrigin()[0],epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(image->getOrigin()[1],image2->getOrigin()[1],epsilon);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(image->getOrigin()[2],image2->getOrigin()[2],epsilon);

    CPPUNIT_ASSERT(::fwTest::helper::compare(image, image2, exclude));

}

//------------------------------------------------------------------------------

void DataConverterTest::matrixConverterTest()
{
    DataConverter::sptr converter = DataConverter::getInstance();
    ::igtl::TransformMessage::Pointer convertedMatrix;
    ::fwData::TransformationMatrix3D::sptr matrix;
    ::igtl::Matrix4x4 igtlMatrix;

    matrix = ::fwData::TransformationMatrix3D::New();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrix->setCoefficient(i, j, i+j);
        }
    }
    convertedMatrix =
        ::igtl::TransformMessage::Pointer(dynamic_cast< ::igtl::TransformMessage*>(converter->fromFwObject(matrix).
                                                                                   GetPointer()));
    CPPUNIT_ASSERT(convertedMatrix);

    convertedMatrix->GetMatrix(igtlMatrix);
    for (int i = 0; i < 4; ++i)
    {
        CPPUNIT_ASSERT(std::equal(igtlMatrix[i], igtlMatrix[i] + 4, matrix->getCoefficients().begin() + i * 4));
    }
    ::fwData::Object::sptr destObj = matrix;
    converter->fromIgtlMessage(::igtl::MessageBase::Pointer(convertedMatrix.GetPointer()), destObj);
    for (int i = 0; i < 4; ++i)
    {
        CPPUNIT_ASSERT(std::equal(igtlMatrix[i], igtlMatrix[i] + 4, matrix->getCoefficients().begin() + i * 4));
    }
}

//------------------------------------------------------------------------------

void DataConverterTest::pointListConverterTest()
{
    const float points[2][3] = {
        {0.1f, 1.4f, 2.3f},
        {0.4f, 0.5f, 2.0f}
    };
    igtl::PointElement::Pointer igtlPointElement[2];
    float igtlPoints[2][3];
    DataConverter::sptr converter = DataConverter::getInstance();
    ::fwData::PointList::sptr pointList;
    ::igtl::PointMessage::Pointer msg;
    ::fwData::Point::sptr point;

    pointList = ::fwData::PointList::New();
    for (int i = 0; i < 2; ++i)
    {
        point = ::fwData::Point::New();
        std::copy(points[i], points[i] + 3, point->getRefCoord().begin());
        pointList->getRefPoints().push_back(point);
    }
    msg =
        ::igtl::PointMessage::Pointer(dynamic_cast< ::igtl::PointMessage*>(converter->fromFwObject(
                                                                               pointList).GetPointer()));
    for (int i = 0; i < 2; ++i)
    {
        msg->GetPointElement(i, igtlPointElement[i]);
        igtlPointElement[i]->GetPosition(igtlPoints[i]);
        CPPUNIT_ASSERT(std::equal(points[i], points[i] + 3, igtlPoints[i]));
    }
    ::fwData::Object::sptr destObj = pointList;
    converter->fromIgtlMessage(::igtl::MessageBase::Pointer(msg.GetPointer()), destObj);
    for (int i = 0; i < 2; ++i)
    {
        CPPUNIT_ASSERT(std::equal(pointList->getCRefPoints()[i]->getCRefCoord().begin(),
                                  pointList->getCRefPoints()[i]->getCRefCoord().end(),
                                  points[i]));
    }
}

//------------------------------------------------------------------------------

void DataConverterTest::stringConverterTest()
{
    std::string const &sample     = "Hello world";
    DataConverter::sptr converter = DataConverter::getInstance();
    ::fwData::String::sptr str;
    ::igtl::StringMessage::Pointer strMsg;

    str = ::fwData::String::New();
    str->setValue(sample);
    strMsg =
        ::igtl::StringMessage::Pointer(dynamic_cast< ::igtl::StringMessage*>(converter->fromFwObject(
                                                                                 str).GetPointer()));
    CPPUNIT_ASSERT(std::string(strMsg->GetString()) == sample);
    ::fwData::Object::sptr destObj = str;
    converter->fromIgtlMessage(::igtl::MessageBase::Pointer(strMsg.GetPointer()), destObj);
    CPPUNIT_ASSERT(str->getValue() == sample);
}

//------------------------------------------------------------------------------

void DataConverterTest::lineConverterTest()
{
    float position[3]  = {0.0, 1.0, 2.0};
    float direction[3] = {0.0, 1.0, 2.0};

    ::fwData::Point::sptr point;
    ::fwData::Line::sptr line;
    DataConverter::sptr converter = DataConverter::getInstance();
    ::igtl::PositionMessage::Pointer lineMsg;

    line = ::fwData::Line::New();
    line->setPosition(::fwData::Point::New());
    line->setDirection(::fwData::Point::New());
    std::copy(direction, direction + 3, line->getDirection()->getRefCoord().begin());
    std::copy(position, position + 3, line->getPosition()->getRefCoord().begin());
    lineMsg =
        ::igtl::PositionMessage::Pointer(dynamic_cast< ::igtl::PositionMessage*>(converter->fromFwObject(
                                                                                     line).GetPointer()));
    lineMsg->GetPosition(position);
    lineMsg->GetQuaternion(direction);
    CPPUNIT_ASSERT(std::equal(line->getPosition()->getCRefCoord().begin(),
                              line->getPosition()->getCRefCoord().end(), position));
    CPPUNIT_ASSERT(std::equal(line->getDirection()->getCRefCoord().begin(),
                              line->getDirection()->getCRefCoord().end(), direction));
    ::fwData::Object::sptr destObj = line;
    converter->fromIgtlMessage(::igtl::MessageBase::Pointer(lineMsg.GetPointer()), destObj);
    CPPUNIT_ASSERT(std::equal(line->getPosition()->getCRefCoord().begin(),
                              line->getPosition()->getCRefCoord().end(), position));
    CPPUNIT_ASSERT(std::equal(line->getDirection()->getCRefCoord().begin(),
                              line->getDirection()->getCRefCoord().end(), direction));

}

//------------------------------------------------------------------------------

void DataConverterTest::scalarConverterTest()
{
    DataConverter::sptr converter = DataConverter::getInstance();
    RawMessage::Pointer msg;
    ::fwData::Object::sptr destObj;

    const int valueInt = 10;
    ::fwData::Integer::sptr dataInteger = ::fwData::Integer::New(valueInt);

    msg = RawMessage::Pointer(dynamic_cast< RawMessage*>(converter->fromFwObject(dataInteger).GetPointer()));
    CPPUNIT_ASSERT(msg->getMessage().size() == sizeof(int));

    ::fwData::Integer::sptr newDataInteger = ::fwData::Integer::New();
    destObj                                = newDataInteger;
    converter->fromIgtlMessage(::igtl::MessageBase::Pointer(msg.GetPointer()), destObj);
    CPPUNIT_ASSERT(newDataInteger->getValue() == valueInt);


    const float valueFloat = 16.64f;
    ::fwData::Float::sptr dataFloat = ::fwData::Float::New(valueFloat);

    msg = RawMessage::Pointer(dynamic_cast< RawMessage*>(converter->fromFwObject(dataFloat).GetPointer()));
    CPPUNIT_ASSERT(msg->getMessage().size() == sizeof(float));

    ::fwData::Float::sptr newDataFloat = ::fwData::Float::New(valueFloat);
    destObj                            = newDataFloat;
    converter->fromIgtlMessage(::igtl::MessageBase::Pointer(msg.GetPointer()), destObj);
    CPPUNIT_ASSERT(newDataFloat->getValue() == valueFloat);
}

} //namespace ut
} //namespace OpenIGTLinkProtocol
