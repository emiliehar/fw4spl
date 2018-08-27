/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwServices/config.hpp"
#include "fwServices/IService.hpp"

#include <fwCom/SlotBase.hpp>

#include <fwData/Object.hpp>

namespace fwServices
{

/**
 * @brief Base class for application using qml
 *
 * This class allows to manage the service used by an application.
 * We can easily manage its inputs/inouts and the connections.It connect/disconnect the signals and slots when the
 * service is started/stopped and when an object is added/removed.
 *
 * @code{.cpp}
    m_appMgr =
            std::unique_ptr< ::fwServices::AppManager >(new ::fwServices::AppManager);

    // Initialize the manager
    m_appMgr->create();
    // Create and register a service
    // - readerService will be automatically started and updated
    // - mesherService will be automatically started but not updated
    auto readerService = m_appMgr->registerService("::uiIO::editor::SIOSelector", "", true, true);
    auto mesherService = m_appMgr->registerService("::opVTKMesh::SVTKMesher", "", true, false);

    // configure the services ...

    // Register the objects to associate with the services:
    // - readerService will generate an output image, it is registered as "loadedImage" in the application
    // - mesherService require an input image registered as "loadedImage" in the application
    // - mesherService will generate an output model series, it is registered as "generatedModel" in the application
    m_appMgr->registerObject(readerService, "loadedImage", "image", AccessType::OUTPUT, true);
    m_appMgr->registerObject(mesherService, "loadedImage", "image", AccessType::INPUT, true);
    m_appMgr->registerObject(mesherService, "generatedModel", "modelSeries", AccessType::OUTUT, true);

    // Start the reader service:
    // - readerService will be started because it does not require input or inout. It will also be updated.
    // - mesherService will not be started because it requires an input image.
    m_appMgr->startServices();

    // When readerService will be updated, it will generate the image required by the mesher service. As the image is
    // registered with the same identifier in the application, the mesherService will be automatically started.
   @endcode
 */
class FWSERVICES_CLASS_API AppManager : public ::fwCom::HasSlots
{
public:

    FWSERVICES_API AppManager();
    FWSERVICES_API ~AppManager();

    typedef ::fwServices::IService::AccessType AccessType;

    /// Initialize the Manager
    FWSERVICES_API virtual void create();

    /// Uninitialize the Manager
    FWSERVICES_API virtual void destroy();

    /**
     * @brief Create and register the service in the OSR
     *
     * This method does not start the service enven if autoStart=true, you must call startService(srv), startServices()
     * or registerObj() to start the service when all its required inputs are present.
     * To define the required inputs, use IService::preregisterInput() or IService::preregisterInOut().
     *
     * @param type service classname
     * @param uid service uid. If it is empty, it will be generated
     * @param autoStart if true, the service will be started when all its required inputs are present.
     * @param autoUpdate if true, the service will be updated after starting.
     * @return Return the created service
     */
    FWSERVICES_API ::fwServices::IService::sptr registerService(const std::string& type, const std::string& uid = "",
                                                                bool autoStart = false, bool autoUpdate = false);

    /**
     * @brief Create and register the service in the OSR
     *
     * This method does not start the service enven if autoStart=true, you must call startService(srv), startServices()
     * or registerObj() to start the service when all its required inputs are present.
     * To define the required inputs, use IService::preregisterInput() or IService::preregisterInOut().
     *
     * @param type service classname
     * @param uid service uid. If it is empty, it will be generated
     * @param autoStart if true, the service will be started when all its required inputs are present.
     * @return Return the created service
     */
    template<class SERVICE>
    SPTR(SERVICE) registerService( const std::string& type, const std::string& uid = "", bool autoStart = false,
                                   bool autoUpdate                                 = false);

    /**
     * @brief Register the service in the OSR.
     *
     * This method does not start the service enven if autoStart=true, you must call startService(srv), startServices()
     * or registerObj() to start the service when all its required inputs are present.
     * To define the required inputs, use IService::preregisterInput() or IService::preregisterInOut().
     *
     * @param srv service to register
     * @param autoStart if true, the service will be started when all its required inputs are present.
     */
    FWSERVICES_API void registerService(const ::fwServices::IService::sptr& srv, bool autoStart = false,
                                        bool autoUpdate = false);

    /**
     * @brief Start the service and register it in the started service container
     *
     * It also connect the service's signals and slots defined by connectSignal() and connectSlot().
     *
     * @pre All the required inputs/inout must be pesent
     * @throw raise a ::fwCore::Exception if all the required objects are not present
     */
    FWSERVICES_API void startService(const ::fwServices::IService::sptr& srv);

    /**
     * @brief Stop the service.
     *
     * It also disconnect the service's signals and slots defined by connectSignal() and connectSlot().
     */
    FWSERVICES_API void stopService(const ::fwServices::IService::sptr& srv);

    /// Start all the services with autoStart=true and with all the required objects
    FWSERVICES_API void startServices();

    /// Stop all the started service and unregister all the registered service
    FWSERVICES_API void stopAndUnregisterServices();

    /**
     * @brief Define the object required by a service.
     *
     * It the object is already registered in the AppManager, it will be registered into the service.
     * If all the service objects are present and the service is registered with autoConnect=yes, it will be started.
     *
     * @param srv the service requiring the object
     * @param objId the identifier of the object, this identifier is only used to retrieve the object inside this
     *        AppManager, it is different from the uid.
     * @param key key of the object used into the service
     * @param access access of the object into the service INPUT, INOUT or OUTPUT
     * @param autoConnect true if the service will be connected to the object signals
     * @param optional true if the object is optional (i.e. the service can be started even if the object is not
     *        present).
     */
    FWSERVICES_API void registerObject(const ::fwServices::IService::sptr& srv, const std::string& objId,
                                       const ::fwServices::IService::KeyType& key,
                                       const ::fwServices::IService::AccessType access,
                                       const bool autoConnect = false, const bool optional = false);

    /**
     * @brief Register the connection of the service's signal to the channel
     *
     * The signal will be connected just before the service starting, and disconnected just after the service stopping.
     *
     * @param channel channel to connect
     * @param srv service containig the signal
     * @param sigName name of the signal
     */
    FWSERVICES_API void connectSignal(const std::string& channel, const ::fwServices::IService::sptr& srv,
                                      const std::string sigName);

    /**
     * @brief Register the connection of the service's slot to the channel
     *
     * The slot will be connected just before the service starting, and disconnected just after the service stopping.
     *
     * @param channel channel to connect
     * @param srv service containig the slot
     * @param sigName name of the slot
     */
    FWSERVICES_API void connectSlot(const std::string& channel, const ::fwServices::IService::sptr& srv,
                                    const std::string& slotName);

    /**
     * @brief Register the connection of the object's signal to the channel
     *
     * The signal will be connected just before the object is added, and disconnected just after the object is removed.
     *
     * @param channel channel to connect
     * @param objId identifier of the object in the AppManager
     * @param sigName name of the signal
     */
    FWSERVICES_API void connectObjectSignal(const std::string& channel, const std::string& objId,
                                            const std::string& sigName);

    /**
     * @brief Slot: register the added object into all the services that require it
     *
     * This slot is connected to the OSR to listen the created object, it could also be called by the AppManager
     * inherited classes to register an object.
     *
     * @param obj the new object to register
     * @param id the identifier of the object, this identifier is only used to retrieve the object inside this
     *        AppManager, it is different from the uid.
     */
    FWSERVICES_API void addObject(::fwData::Object::sptr obj, const std::string& id);

    /**
     * @brief Slot: unregister the object from all the services that require it
     *
     * This slot is connected to the OSR to listen the removed object, it could also be called by the AppManager
     * inherited classes to unregister an object.
     *
     * @param obj the object to unregister
     * @param id the identifier of the object, this identifier is only used to retrieve the object inside this
     *        AppManager, it is different from the uid.
     */
    FWSERVICES_API void removeObject(::fwData::Object::sptr obj, const std::string& id);

protected:

    /// Information about connection <channel, sig/slot name>
    typedef std::unordered_map< std::string, std::string > ConnectionInfo;

    /// Register the service information
    struct ServiceInfo
    {
        /// constructor
        FWSERVICES_API ServiceInfo(const ::fwServices::IService::sptr& srv, const bool autoStart,
                                   const bool autoUpdate);

        /// add the object in the service requirement
        FWSERVICES_API void addObject(const std::string& objId, const ::fwServices::IService::KeyType& key,
                                      const ::fwServices::IService::AccessType access, const bool autoConnect = false,
                                      const bool optional = false);

        /// Return true if the service contain this object into its requirement
        FWSERVICES_API bool requireObject(const std::string& objId) const;

        /// Return true if all the non-optional object required by the service are present
        FWSERVICES_API bool hasAllRequiredObjects() const;

        /// Return the information about the required object
        FWSERVICES_API const ::fwServices::IService::ObjectServiceConfig& getObjInfo(const std::string& objId) const;

        /// Identifier
        std::string m_id;

        /// service
        ::fwServices::IService::wptr m_service;

        /// required objects
        std::vector < ::fwServices::IService::ObjectServiceConfig > m_objects;

        /// signal connection information <channel, signal_name>
        ConnectionInfo m_signalConnection;

        /// slot connection information <channel, slot_name>
        ConnectionInfo m_slotConnection;

        /// True if the service will be automatically started when all its required objects are present
        bool m_autoStart;

        /// True if the service will be automatically updated after starting
        bool m_autoUpdate;
    };

private:

    /// Return the service information
    ServiceInfo& getServiceInfo(const ::fwServices::IService::sptr& srv);

    /// Return the service information
    const ServiceInfo& getServiceInfo(const ::fwServices::IService::sptr& srv) const;

    /// Start the service and connect its slots and signals
    ::fwServices::IService::SharedFutureType start(const ServiceInfo& info);

    /// Stop the service and connect its slots and signals
    ::fwServices::IService::SharedFutureType stop(const ServiceInfo& info);

    /// Store the information of the services (objects, autoStart, autoUpdate)
    std::vector< ServiceInfo > m_services;

    /// Store started services
    std::vector< ::fwServices::IService::sptr > m_startedService;

    /// Store registered objects <id, obj>
    std::unordered_map< std::string, ::fwData::Object::sptr > m_registeredObject;

    /// Store the object connections <objId, connection >
    std::unordered_map< std::string, ConnectionInfo > m_objectConnection;

    /// Connections to the OSR
    ::fwCom::Connection m_addObjectConnection;
    ::fwCom::Connection m_removeObjectConnection;
};

//------------------------------------------------------------------------------

template<class SERVICE>
SPTR(SERVICE) AppManager::registerService( const std::string& type, const std::string& uid, bool autoStart,
                                           bool autoUpdate)
{
    auto srv = this->registerService(type, uid, autoStart, autoUpdate);

    auto castedSrv = std::dynamic_pointer_cast< SERVICE >(srv);
    FW_RAISE_IF("Failed to cast service from factory type '" + type + "' into '" +
                ::fwCore::TypeDemangler<SERVICE>().getClassname() + "'", !srv );

    return castedSrv;
}

} // fwServices
