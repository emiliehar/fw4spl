/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "ZipTest.hpp"

#include <fwZip/ReadZipArchive.hpp>
#include <fwZip/WriteZipArchive.hpp>

#include <fwTest/Data.hpp>

#include <fwTools/System.hpp>

#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/path.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwZip::ut::ZipTest );

namespace fwZip
{
namespace ut
{

//------------------------------------------------------------------------------

void ZipTest::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void ZipTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void ZipTest::commentTest()
{
    const std::string writerComment = "Example of comment";

    const ::boost::filesystem::path dirPath = ::fwTools::System::getTemporaryFolder() / "fwZipTest";
    ::boost::filesystem::create_directories( dirPath );
    const ::boost::filesystem::path path        = dirPath / "jambon.zip";
    const ::boost::filesystem::path sourceFile  = ::fwTest::Data::dir() / "fw4spl/image/jpg/makao01.jpg";
    const ::boost::filesystem::path archiveFile = "makao.jpg";

    CPPUNIT_ASSERT_MESSAGE("The file '" + sourceFile.string() + "' does not exist",
                           ::boost::filesystem::exists(sourceFile));

    SPTR(WriteZipArchive) writer = std::make_shared<WriteZipArchive>(path, writerComment);
    writer->putFile(sourceFile, archiveFile);

    SPTR(ReadZipArchive) reader = std::make_shared<ReadZipArchive>(path);
    std::string readerComment = reader->getComment();

    ::boost::filesystem::remove_all( path );

    CPPUNIT_ASSERT_EQUAL(writerComment, readerComment);

}

} // namespace ut
} // namespace fwZip
