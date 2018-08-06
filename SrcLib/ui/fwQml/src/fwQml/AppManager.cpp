/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQml/AppManager.hpp"

#include <fwServices/registry/ObjectService.hpp>

#include <boost/foreach.hpp>

namespace fwQml
{

AppManager::AppManager()
{

}

//------------------------------------------------------------------------------

AppManager::~AppManager()
{

}

//------------------------------------------------------------------------------

void AppManager::registerService(const ::fwServices::IService::sptr& srv)
{
    m_createdService.emplace_back(srv);
}

//------------------------------------------------------------------------------

void AppManager::startService(const ::fwServices::IService::sptr& srv)
{
    srv->start();
    m_startedService.emplace_back(srv);
}

//------------------------------------------------------------------------------

void AppManager::stopAndUnregisterServices()
{
    std::vector< ::fwServices::IService::SharedFutureType > futures;

    // stop the started services
    BOOST_REVERSE_FOREACH(auto& srv, m_startedService)
    {
        futures.emplace_back(srv->stop());
    }
    std::for_each(futures.begin(), futures.end(), std::mem_fn(&::std::shared_future<void>::wait));

    // unregister the services
    for(auto& srv : m_createdService)
    {
        ::fwServices::OSR::unregisterService(srv);
    }
    m_startedService.clear();
    m_createdService.clear();
}

//------------------------------------------------------------------------------

void AppManager::registerObj(const ::fwServices::IService::sptr& srv, const ::fwData::Object::sptr& obj,
                             const ::fwServices::IService::KeyType& key,
                             const ::fwServices::IService::AccessType access,
                             bool autoStart, bool autoConnect, bool optional)
{
    bool isStarted = srv->isStarted();

    if (isStarted && !optional)
    {
        srv->stop();
    }
    if (::fwServices::OSR::isRegistered(key, access, srv))
    {
        srv->unregisterObject(key, access);
    }
    srv->registerObject(obj, key, access, autoConnect, optional);
    if (optional)
    {
        srv->swapKey(key, obj);
    }
    else if (isStarted)
    {
        srv->start();
    }
    else if (autoStart) // TODO check if it has all the required inputs
    {
        this->startService(srv);
    }

    m_registeredObject.emplace(obj);
}

} // namespace fwQml
