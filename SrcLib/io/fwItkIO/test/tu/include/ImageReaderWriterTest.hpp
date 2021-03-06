/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWITKIO_UT_IMAGEREADERWRITERTEST_HPP__
#define __FWITKIO_UT_IMAGEREADERWRITERTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>
#include <fwServices/macros.hpp>
#include <fwRuntime/EConfigurationElement.hpp>

#include <fwData/Image.hpp>

namespace fwItkIO
{
namespace ut
{

class ImageReaderWriterTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( ImageReaderWriterTest );
CPPUNIT_TEST( testSaveLoadInr );
CPPUNIT_TEST( stressTestInr );
CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();
    // fonctions de tests
    void testSaveLoadInr();
    void stressTestInr();

private:


    void stressTestInrWithType(::fwTools::Type type, int nbTest);
    void checkSaveLoadInr( ::fwData::Image::sptr image );
};

} //namespace ut
} //namespace fwItkIO

#endif // __FWITKIO_UT_IMAGEREADERWRITERTEST_HPP__
