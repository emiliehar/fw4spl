/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include "uiMeasurementQt/Plugin.hpp"

namespace uiMeasurementQt
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::uiMeasurementQt::Plugin");

Plugin::~Plugin() noexcept
{
}

void Plugin::start()
{
}

void Plugin::stop() noexcept
{
}

} // namespace uiMeasurementQt
