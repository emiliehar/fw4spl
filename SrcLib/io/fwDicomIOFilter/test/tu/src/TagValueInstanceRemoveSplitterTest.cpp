/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "TagValueInstanceRemoveSplitterTest.hpp"

#include <fwDicomIOFilter/factory/new.hpp>
#include <fwDicomIOFilter/helper/Filter.hpp>
#include <fwDicomIOFilter/IFilter.hpp>
#include <fwDicomIOFilter/splitter/TagValueInstanceRemoveSplitter.hpp>

#include <fwGdcmIO/reader/SeriesDB.hpp>

#include <fwTest/Data.hpp>

#include <boost/filesystem/path.hpp>

#include <dcmtk/dcmdata/dcdeftag.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwDicomIOFilter::ut::TagValueInstanceRemoveSplitterTest );

namespace fwDicomIOFilter
{
namespace ut
{

//------------------------------------------------------------------------------

void TagValueInstanceRemoveSplitterTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void TagValueInstanceRemoveSplitterTest::tearDown()
{
    // Clean up after the test run.
}

//-----------------------------------------------------------------------------

void TagValueInstanceRemoveSplitterTest::simpleApplication()
{
    ::fwMedData::SeriesDB::sptr seriesDB = ::fwMedData::SeriesDB::New();

    const std::string filename           = "08-CT-PACS";
    const ::boost::filesystem::path path = ::fwTest::Data::dir() / "fw4spl/Patient/Dicom/DicomDB" / filename;

    CPPUNIT_ASSERT_MESSAGE("The dicom directory '" + path.string() + "' does not exist",
                           ::boost::filesystem::exists(path));

    // Read DicomSeries
    ::fwGdcmIO::reader::SeriesDB::sptr reader = ::fwGdcmIO::reader::SeriesDB::New();
    reader->setObject(seriesDB);
    reader->setFolder(path);
    CPPUNIT_ASSERT_NO_THROW(reader->readDicomSeries());
    CPPUNIT_ASSERT_EQUAL(size_t(1), seriesDB->size());

    // Retrieve DicomSeries
    ::fwMedData::DicomSeries::sptr dicomSeries = ::fwMedData::DicomSeries::dynamicCast((*seriesDB)[0]);
    CPPUNIT_ASSERT(dicomSeries);
    std::vector< ::fwMedData::DicomSeries::sptr > dicomSeriesContainer;
    dicomSeriesContainer.push_back(dicomSeries);

    // Apply filter
    ::fwDicomIOFilter::splitter::TagValueInstanceRemoveSplitter::sptr filter =
        ::fwDicomIOFilter::splitter::TagValueInstanceRemoveSplitter::dynamicCast(
            ::fwDicomIOFilter::factory::New("::fwDicomIOFilter::splitter::TagValueInstanceRemoveSplitter"));
    CPPUNIT_ASSERT(filter);
    filter->setTag(DCM_AcquisitionNumber);
    filter->setTagValue("1");
    ::fwDicomIOFilter::helper::Filter::applyFilter(dicomSeriesContainer, filter, true);
    CPPUNIT_ASSERT_EQUAL(size_t(1), dicomSeriesContainer.size());
    dicomSeries = dicomSeriesContainer[0];

    // Check number of instances in series
    CPPUNIT_ASSERT_EQUAL(size_t(275), dicomSeries->getDicomContainer().size());
}

//------------------------------------------------------------------------------

} // namespace ut
} // namespace fwDicomIOFilter
