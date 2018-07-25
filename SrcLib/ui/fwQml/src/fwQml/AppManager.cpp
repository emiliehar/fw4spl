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

AppManager::~AppManager()
{

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
    auto services = ::fwServices::OSR::getServices("::fwServices::IService");
    for(auto& srv : services)
    {
        ::fwServices::OSR::unregisterService(srv);
    }
    m_startedService.clear();
}

//------------------------------------------------------------------------------

void AppManager::registerObj(const ::fwServices::IService::sptr& srv, const ::fwData::Object::sptr& obj,
                             const ::fwServices::IService::KeyType& key,
                             const ::fwServices::IService::AccessType access,
                             bool autoConnect, bool optional)
{
    if (srv->isStarted())
    {
        srv->stop();
    }
    if (::fwServices::OSR::isRegistered(key, access, srv))
    {
        srv->unregisterObject(key, access);
    }
    srv->registerObject(obj, key, access, autoConnect, optional);
    srv->start();
    m_startedService.emplace(srv);
    m_registeredObject.emplace(obj);
}

}
