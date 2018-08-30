/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwRuntime/Plugin.hpp"

#include "fwRuntime/ConfigurationElement.hpp"

namespace fwRuntime
{

//------------------------------------------------------------------------------

std::shared_ptr<Bundle> Plugin::getBundle() const noexcept
{
    return m_bundle.lock();
}

//------------------------------------------------------------------------------

void Plugin::setInitializationData(const ConfigurationElement::sptr) noexcept
{
}

//------------------------------------------------------------------------------

void Plugin::setBundle( std::shared_ptr<Bundle> bundle) noexcept
{
    m_bundle = std::weak_ptr<Bundle>(bundle);
}

//------------------------------------------------------------------------------

} // namespace fwRuntime
