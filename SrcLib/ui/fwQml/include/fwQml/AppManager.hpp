/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwQml/config.hpp"

#include <fwCom/SlotBase.hpp>

#include <fwData/Object.hpp>

#include <fwServices/IService.hpp>

#include <QObject>

namespace fwQml
{

class FWQML_CLASS_API AppManager : public QObject
{
public:

    FWQML_API AppManager();
    FWQML_API ~AppManager();

protected:

    /// Register the service to be able to unregister it
    void registerService(const ::fwServices::IService::sptr& srv);

    /// Start the service and register it in the started service container
    void startService(const ::fwServices::IService::sptr& srv);

    /// Stop all the started service and unregister all the created service
    void stopAndUnregisterServices();

    /// Register the object to the service, and eventually unregister the previous object and start it
    void registerObj(const ::fwServices::IService::sptr& srv, const ::fwData::Object::sptr& obj,
                     const ::fwServices::IService::KeyType& key, const ::fwServices::IService::AccessType access,
                     bool autoStart = false, bool autoConnect = false, bool optional = false);

private:
    std::vector< ::fwServices::IService::sptr > m_startedService;
    std::vector< ::fwServices::IService::sptr > m_createdService;
    std::set< ::fwData::Object::sptr > m_registeredObject;

    std::vector< ::fwCom::SlotBase::sptr > m_slots;
};

} // fwQml
