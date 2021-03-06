/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "Tuto02DataServiceBasicCtrl/Plugin.hpp"

#include <fwRuntime/operations.hpp>
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include <fwServices/op/Add.hpp>
#include <fwServices/registry/ObjectService.hpp>

namespace Tuto02DataServiceBasicCtrl
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::Tuto02DataServiceBasicCtrl::Plugin");

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
    // create an empty image
    m_image = ::fwData::Image::New();

    // create and register the reader service
    m_readerSrv = ::fwServices::add("::ioVTK::SImageReader");
    m_readerSrv->registerInOut(m_image, "data"); // add the in-out image
    // create the reader configuration
    ::fwServices::IService::ConfigType readerCfg;
    readerCfg.put("file", "../../data/patient1.vtk");
    m_readerSrv->setConfiguration( readerCfg );
    m_readerSrv->configure();

    // create and register the render service
    m_renderSrv = ::fwServices::add("::vtkSimpleNegato::SRenderer");
    m_renderSrv->registerInput(m_image, "image"); // add the input image
    m_renderSrv->setID( "myRenderingTuto" ); // set an identifier
    m_renderSrv->configure();

    // create and register frame service
    m_frameSrv = ::fwServices::add("::gui::frame::SDefaultFrame");

    // create the frame configuration
    ::fwServices::IService::ConfigType frameConfig;
    frameConfig.put("gui.frame.name", "tutoDataServiceBasicCtrl");
    frameConfig.put("gui.frame.icon", "Tuto02DataServiceBasicCtrl-0.1/tuto.ico");
    frameConfig.put("gui.frame.minSize.<xmlattr>.width", "800");
    frameConfig.put("gui.frame.minSize.<xmlattr>.height", "600");
    // use the render identifier to display it in the frame
    frameConfig.put("registry.view.<xmlattr>.sid", "myRenderingTuto");

    m_frameSrv->setConfiguration( frameConfig );
    m_frameSrv->configure();

    // start the services
    m_frameSrv->start();
    m_readerSrv->start();
    m_renderSrv->start();

    // update the services
    m_readerSrv->update();
    m_renderSrv->update();
}

//------------------------------------------------------------------------------

void Plugin::stop() noexcept
{
}

//------------------------------------------------------------------------------

void Plugin::uninitialize() noexcept
{
    // stop the services
    m_renderSrv->stop();
    m_readerSrv->stop();
    m_frameSrv->stop();

    // unregister the services
    ::fwServices::OSR::unregisterService( m_readerSrv );
    ::fwServices::OSR::unregisterService( m_frameSrv );
    ::fwServices::OSR::unregisterService( m_renderSrv );
    m_image.reset();
}

//------------------------------------------------------------------------------

} // namespace Tuto02DataServiceBasicCtrl
