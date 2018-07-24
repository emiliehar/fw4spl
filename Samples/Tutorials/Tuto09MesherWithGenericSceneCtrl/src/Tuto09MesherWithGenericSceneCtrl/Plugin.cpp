/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD,2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "Tuto09MesherWithGenericSceneCtrl/Plugin.hpp"

#include "Tuto09MesherWithGenericSceneCtrl/Tuto09Manager.hpp"

#include <fwQml/IQmlRegistry.hpp>

#include <fwRuntime/operations.hpp>
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include <fwVTKQml/FrameBufferItem.hpp>

#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickWindow>

namespace Tuto09MesherWithGenericSceneCtrl
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar(
    "::Tuto09MesherWithGenericSceneCtrl::Plugin");

//------------------------------------------------------------------------------

Plugin::Plugin() noexcept
{
}

//------------------------------------------------------------------------------

Plugin::~Plugin() noexcept
{
}

//------------------------------------------------------------------------------

void Plugin::start()
{

}

//------------------------------------------------------------------------------

void Plugin::initialize()
{
    ::fwQml::IQmlRegistry::registarAllClasses();

    m_engine = new QQmlApplicationEngine();

    auto path = ::fwRuntime::getBundleResourceFilePath("Tuto09MesherWithGenericSceneCtrl-0.1/ui.qml");

    QQmlComponent component(m_engine);

    component.loadUrl(QUrl::fromLocalFile(QString::fromStdString(path.string())));
    SLM_ASSERT(qPrintable(component.errorString()), component.isReady());

    QObject* topLevel        = component.create();
    QQuickWindow* rootWindow = qobject_cast<QQuickWindow*>(topLevel);
    rootWindow->show();
}

//------------------------------------------------------------------------------

void Plugin::stop() noexcept
{
}

//------------------------------------------------------------------------------

void Plugin::uninitialize() noexcept
{

}

//------------------------------------------------------------------------------

} // namespace Tuto09MesherWithGenericSceneCtrl
