/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCore/log/SpyLogger.hpp>

#include <fwData/Array.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/Image.hpp>
#include <fwData/Float.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Boolean.hpp>
#include <fwData/String.hpp>
#include <fwData/Color.hpp>
#include <fwData/Vector.hpp>
#include <fwData/Material.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/Acquisition.hpp>
#include <fwData/Study.hpp>
#include <fwData/Patient.hpp>
#include <fwData/PatientDB.hpp>
#include <fwData/TransformationMatrix3D.hpp>
#include <fwData/Point.hpp>
#include <fwData/PointList.hpp>
#include <fwData/Composite.hpp>
#include <fwData/TransferFunction.hpp>

#include <fwDataTools/Patient.hpp>
#include <fwDataTools/MeshGenerator.hpp>
#include <fwDataTools/Image.hpp>

#include <fwAtomConversion/AtomHelper.hpp>
#include <fwAtomConversion/CampObjectVisitor.hpp>
#include <fwAtomConversion/CampObjectPrinterVisitor.hpp>
#include <fwAtomConversion/RetreiveObjectVisitor.hpp>

#include "AtomHelperTest.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION( ::fwAtomConversion::ut::AtomHelperTest );

namespace fwAtomConversion
{
namespace ut
{

//-----------------------------------------------------------------------------

// Set up context before running a test.
void AtomHelperTest::setUp()
{};

//-----------------------------------------------------------------------------

void AtomHelperTest::tearDown()
{
    // Clean up after the test run.
}

//-----------------------------------------------------------------------------

void AtomHelperTest::dataToMeta()
{
    ::fwData::Patient::sptr patient = ::fwData::Patient::New();
    ::fwDataTools::Patient::generatePatient(patient, 2, 2, 2);

    const ::fwData::Object::sptr VALUES[] = {
            ::fwData::Integer::New(1337),
            ::fwData::Float::New(),
            ::fwData::String::New(),
            ::fwData::Boolean::New(),
            ::fwData::String::New(),
            ::fwData::Vector::New(),
            ::fwData::Color::New(1.4, 0.9, 1.1, 1.67),
            ::fwData::Array::New(),
            ::fwData::Image::New(),
            ::fwData::Mesh::New(),
            ::fwData::Material::New(),
            ::fwData::Reconstruction::New(),
            ::fwData::Acquisition::New(),
            ::fwData::Study::New(),
            ::fwData::Composite::New(),
            ::fwData::Patient::New(),
            ::fwData::Point::New(),
            ::fwData::PointList::New(),
            ::fwData::TransformationMatrix3D::New(),
            ::fwData::TransferFunction::New(),
             patient,
    };

    ::fwAtoms::Object::sptr metaObject;

    BOOST_FOREACH ( fwData::Object::sptr  object, VALUES )
    {
        AtomHelper metaHelper;
        metaObject = metaHelper.dataToMeta( object);

        const ::camp::Class& metaClass = ::camp::classByName(object->getClassname());

        //Type test
        CPPUNIT_ASSERT_EQUAL(object->getClassname(), metaObject->getType());


        ::fwAtoms::Object::Attributes attributes = metaObject->getAttributes();
        ::fwAtoms::Object::Attributes::const_iterator cIt = attributes.begin();

         //Test attribute type
        for(; cIt != attributes.end(); ++cIt)
        {
                int type = metaClass.property(cIt->first).type();
                switch(type)
                {
                case camp::stringType:
                    CPPUNIT_ASSERT(cIt->second->isString());
                    CPPUNIT_ASSERT(cIt->second->isValue());
                    break;
                case camp::realType :
                case camp::intType :
                    CPPUNIT_ASSERT(cIt->second->isNumeric());
                    CPPUNIT_ASSERT(cIt->second->isValue());
                    break;
                case camp::boolType :
                    CPPUNIT_ASSERT(cIt->second->isBoolean());
                    CPPUNIT_ASSERT(cIt->second->isValue());
                    break;
                case camp::userType:
                    CPPUNIT_ASSERT(cIt->second->isObject() || cIt->second->isBlob());
                    break;
                case camp::mappingType:
                    CPPUNIT_ASSERT(cIt->second->isMapping());
                    break;
                case camp::enumType:
                    CPPUNIT_ASSERT(cIt->second->isString());
                    CPPUNIT_ASSERT(cIt->second->isValue());
                    break;
                case camp::arrayType:
                    CPPUNIT_ASSERT(cIt->second->isSequence());
                    break;
                }
        }
    }
}

//-----------------------------------------------------------------------------

void AtomHelperTest::metaToData()
{
    AtomHelper metaHelper;

    ::fwData::Color::sptr color = ::fwData::Color::New(0.2, 1.2, 1.3, 0.9);
    ::fwData::Material::sptr material = ::fwData::Material::New();
    material->setAmbient(color);

    ::fwAtoms::Object::sptr metaObject = metaHelper.dataToMeta( material);
    ::fwData::Object::sptr materialRes = metaHelper.metaToData(metaObject);
    ::fwData::Material::sptr materialResultat = ::fwData::Material::dynamicCast(materialRes);
    bool materialComparison = ::fwDataTools::Patient::compareMaterial(material, materialResultat, std::string("material"));

    CPPUNIT_ASSERT_MESSAGE("Material Not equal" , materialComparison);

    metaHelper.clearCache();
    metaHelper.clearMetaCache();

    ::fwData::Patient::sptr patient = ::fwData::Patient::New();
    ::fwDataTools::Patient::generatePatient(patient, 1, 1, 1);

    metaObject = metaHelper.dataToMeta( patient);
    ::fwData::Object::sptr patientObject = metaHelper.metaToData(metaObject);
    ::fwData::Patient::sptr patientResultat = ::fwData::Patient::dynamicCast(patientObject);
    bool patientComparison = ::fwDataTools::Patient::comparePatient(patient, patientResultat);

    CPPUNIT_ASSERT_MESSAGE("Patient Not equal" , patientComparison);
}

//-----------------------------------------------------------------------------

void AtomHelperTest::visitCampObject( const camp::UserObject & campObj )
{
    //::fwAtomConversion::CampObjectVisitor visitor( campObj );
    ::fwAtomConversion::CampObjectPrinterVisitor visitor( campObj );
    campObj.getClass().visit(visitor);
}

//-----------------------------------------------------------------------------

void AtomHelperTest::visitPatientDataTest()
{
    // Generate data
    ::fwData::Patient::sptr patient = ::fwData::Patient::New();
    ::fwDataTools::Patient::generatePatient(patient, 1, 1, 1);

    // Convert in camp object
    ::camp::UserObject campObj ( patient.get() );

    // visit camp object
    this->visitCampObject( campObj );
}

//-----------------------------------------------------------------------------

void AtomHelperTest::visitPatientMetaDataTest()
{
    // Generate data
    ::fwData::Patient::sptr patient = ::fwData::Patient::New();
    ::fwDataTools::Patient::generatePatient(patient, 1, 1, 1);

    // Convert in meta data
    fwAtomConversion::AtomHelper metaHelper;
    ::fwAtoms::Object::sptr metaObject = metaHelper.dataToMeta( patient );

    // Convert in camp object
    ::camp::UserObject campObj ( metaObject.get() );

    // visit camp object
    this->visitCampObject( campObj );
}

//-----------------------------------------------------------------------------

void AtomHelperTest::visitCompositeDataTest()
{
    // Generate data
    ::fwData::Image::NewSptr img1;
    ::fwDataTools::Image::generateRandomImage(img1, ::fwTools::Type::create("int16"));

    ::fwData::Image::NewSptr img2;
    ::fwDataTools::Image::generateRandomImage(img2, ::fwTools::Type::create("uint8"));

    ::fwData::Composite::NewSptr composite;
    composite->getContainer()["img1"]=img1;
    composite->getContainer()["img2"]=img2;

    // Convert in camp object
    ::camp::UserObject campObj ( composite.get() );

    // visit camp object
    this->visitCampObject( campObj );
}


//-----------------------------------------------------------------------------

void AtomHelperTest::retreiveCampObjectVisitorTest()
{
    // Visit 1
    ::fwData::Image::NewSptr img1;
    ::fwDataTools::Image::generateRandomImage(img1, ::fwTools::Type::create("int16"));
    ::fwData::Image::NewSptr img2;
    ::fwDataTools::Image::generateRandomImage(img2, ::fwTools::Type::create("uint8"));
    ::fwData::Composite::NewSptr composite;
    composite->getContainer()["img1"]=img1;
    composite->getContainer()["img2"]=img2;
    ::fwData::Object::sptr subObj1 = ::fwAtomConversion::getSubObject( composite, "@values.img2" );
    CPPUNIT_ASSERT_MESSAGE("Image must be equal" , subObj1 == img2);

    // Visit 2
    ::fwData::Float::sptr zspacing = ::fwAtomConversion::getSubObject< ::fwData::Float >( composite, "@values.img2.spacing.2" );
    CPPUNIT_ASSERT_MESSAGE("spacing must be equal" , img2->getSpacing()[2] - 0.001 < zspacing->value() && zspacing->value() < img2->getSpacing()[2] + 0.001 );

    // Visit 3
    ::fwData::Patient::NewSptr patient1;
    patient1->setFirstname( "toto" );
    ::fwData::String::sptr str= ::fwAtomConversion::getSubObject< ::fwData::String >( patient1, "@firstname" );
    CPPUNIT_ASSERT_MESSAGE("Firstname must be equal" , patient1->getFirstname() == str->value() );

    // Visit 4
    ::fwData::Patient::NewSptr patient2;
    ::fwDataTools::Patient::generatePatient(patient2, 2, 1, 2);
    ::fwData::Reconstruction::sptr rec = ::fwAtomConversion::getSubObject< ::fwData::Reconstruction >( patient2, "@studies.1.acquisitions.0.reconstructions.1" );
    ::fwData::Study::sptr study = patient2->getStudies()[1];
    ::fwData::Acquisition::sptr acq = study->getAcquisitions()[0];
    ::fwData::Reconstruction::sptr rec2 = acq->getReconstructions()[1];
    CPPUNIT_ASSERT_MESSAGE("Reconstruction must be equal" , rec ==  rec2 );

    // Visit 5
    composite->setField("toto",img1);
    img1->setField("titi",img2);
    ::fwData::Object::sptr subObj2 = ::fwAtomConversion::getSubObject( composite, "@fields.toto.fields.titi" );
    CPPUNIT_ASSERT_MESSAGE("Image must be equal" , subObj2 == img2 );
}

//-----------------------------------------------------------------------------

}  // namespace ut
}  // namespace fwAtomConversion