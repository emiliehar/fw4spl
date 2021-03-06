/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwIO/config.hpp"
#include "fwIO/ioTypes.hpp"

#include <fwServices/IService.hpp>

#include <boost/filesystem/path.hpp>

namespace fwIO
{

/**
 * @brief Writer service API. It manages extension points definition and extension configuration
 *
 * @section Slots Slots
 * - \b setFileFolder(const ::boost::filesystem::path&) : Sets the folder when a path is configured in FILE or
 * FILES mode
 *
 * This class represents the base interface for writer services.
 * Use the base service methods :
 * @li The service is configured with methods setConfiguration(cfg) and configure()
 * @li The method start() initialize the service
 * @li To write the object use update() method
 * @li Finally we must call stop() before deleting the service
 */
class FWIO_CLASS_API IWriter : public ::fwServices::IService
{

public:
    fwCoreServiceClassDefinitionsMacro( (IWriter)(::fwServices::IService) )

    /**
     * @name Slots API
     * @{
     */
    FWIO_API static const ::fwCom::Slots::SlotKeyType s_SET_FILE_FOLDER;
    /// @}

    /**
     * @brief Configure the image path (by default does nothing).
     *
     * This method is used to find
     * the file path  using a file selector.
     */
    FWIO_API virtual void configureWithIHM() = 0;

    /**
     * @brief This method must be implemented by concrete service writers
     * that use path file system to write data.
     *
     * This method returns supported path format (file, files or folder).
     * A reader can support file and folder, or files and folder, but not
     * file and files ( because files include file concept ).
     */
    FWIO_API virtual ::fwIO::IOPathType getIOPathType() const;

    /**
     * @brief Returns the file path set by the user or set during service configuration
     * @pre exception if a file path is not defined  ( m_locations.empty() )
     * @pre exception if service does not support FILE mode
     */
    FWIO_API const ::boost::filesystem::path& getFile() const;

    /**
     * @brief Sets file path
     * @pre exception if service does not support FILE mode
     */
    FWIO_API void setFile(const ::boost::filesystem::path& file);

    /**
     * @brief Returns file paths set by the user or set during service configuration
     * @pre exception if a file path is not defined ( m_locations.empty() )
     * @pre exception if service does not support FILES mode
     */
    FWIO_API const ::fwIO::LocationsType& getFiles() const;

    /**
     * @brief Sets file paths
     * @pre exception if service does not support FILES mode
     */
    FWIO_API void setFiles(const ::fwIO::LocationsType& files);

    /**
     * @brief Returns folder path set by the user or set during service configuration
     * @pre exception if a folder path is not defined ( m_locations.empty() )
     * @pre exception if service does not support FOLDER mode
     */
    FWIO_API const ::boost::filesystem::path& getFolder() const;

    /**
     * @brief Clear any location set by the setFile/setFiles/setFolder setter
     */
    FWIO_API void clearLocations();

    /**
     * @brief Returns file/files/folder paths set by the user or set during service configuration
     * @pre exception if a file path is not defined ( m_locations.empty() )
     */
    FWIO_API const ::fwIO::LocationsType& getLocations() const;

    /**
     * @brief Sets folder path
     * @pre exception if service does not support FOLDER mode
     */
    FWIO_API void setFolder(const ::boost::filesystem::path& folder);

    /**
     * @brief Slot: Sets the folder when a path is configured in FILE or FILES mode
     * This is ignored if a path is not configured
     *
     * @pre exception if service does not support FILE or FILES mode
     */
    FWIO_API void setFileFolder(::boost::filesystem::path folder);

    /// Returns if a location has been defined ( by the configuration process or directly by user )
    FWIO_API bool hasLocationDefined() const;

protected:

    FWIO_API IWriter() noexcept;

    FWIO_API virtual ~IWriter() noexcept;

    /**
     * @brief This method proposes to parse xml configuration to retrieve
     * file/files/folder paths.
     *
     * You can implement your configuring method if you wan check another
     * information or information not correspond to a path on filesystem,
     * else you must use this generic approach.
     *
     * Sample configuration for a file:
     * @code{.xml}
       <service ... >
        <file>/home/user/myFile.jpg<file/>
       </service>
       @endcode
     * Sample configuration for many files:
     * @code{.xml}
       <service ... >
        <file>/home/user/myFile01.jpg<file/>
        <file>/home/user/myFile02.jpg<file/>
        <file>/home/user/myFile03.jpg<file/>
       </service>
       @endcode
     * Sample configuration for a folder:
     * @code{.xml}
       <service ... >
        <folder>/home/user/myFolder<folder/>
       </service>
       @endcode
     * You may specify the title of the modal file selection window with the `windowTitle` config attribute as such:
     * @code{.xml}
       <service ... >
        <windowTitle>Example window title</windowTitle>
       </service>
       @endcode
     */
    FWIO_API virtual void configuring() override;

    /**
     * @brief Title of the window that will open when the `configureWithIHM` slot is called
     */
    std::string m_windowTitle;

private:

    /// Value to stock file or folder paths
    ::fwIO::LocationsType m_locations;
};

} //namespace fwIO