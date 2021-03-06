/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSERVICES_UT_SLOTSSIGNALSTEST_HPP__
#define __FWSERVICES_UT_SLOTSSIGNALSTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwServices
{
namespace ut
{

class SlotsSignalsTest : public CPPUNIT_NS::TestFixture
{
private:
    CPPUNIT_TEST_SUITE( SlotsSignalsTest );
    CPPUNIT_TEST( basicTest );
    CPPUNIT_TEST( comObjectServiceTest );
    CPPUNIT_TEST( comServiceToServiceTest );
    CPPUNIT_TEST( blockConnectionTest );
    CPPUNIT_TEST_SUITE_END();


public:

    void setUp();
    void tearDown();

    void basicTest();
    void comObjectServiceTest();
    void comServiceToServiceTest();
    void blockConnectionTest();

};

} //namespace ut
} //namespace fwServices

#endif // __FWSERVICES_UT_SLOTSSIGNALSTEST_HPP__


