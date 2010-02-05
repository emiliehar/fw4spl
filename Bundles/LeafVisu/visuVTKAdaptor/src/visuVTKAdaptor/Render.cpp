/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/macros.hpp>
#include <fwServices/Factory.hpp>

#include <fwTools/UUID.hpp>
#include <fwData/String.hpp>

#include <fwServices/ObjectServiceRegistry.hpp>


#include <vtkRenderWindowInteractor.h>

#include "visuVTKAdaptor/Render.hpp"



REGISTER_SERVICE( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::Render, ::fwTools::Object ) ;

namespace visuVTKAdaptor
{

//------------------------------------------------------------------------------

Render::Render() throw()
{
    m_vtkPipelineModified = false;
    m_comChannelPriority = 0.1;
    addNewHandlingEvent("SCENE_RENDER_REQUEST");
}

//------------------------------------------------------------------------------

Render::~Render() throw()
{
}

//------------------------------------------------------------------------------

void Render::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    assert(m_configuration->getName() == "config");
    // TODO allow to add other object
}

//------------------------------------------------------------------------------

void Render::doStart() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    addNewHandlingEvent( "SCENE_RENDER_REQUEST" );
    this->getRenderService()->setPendingRenderRequest(false);
}

//------------------------------------------------------------------------------

void Render::doStop() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void Render::doUpdate() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void Render::doSwap() throw(fwTools::Failed)
{
    this->doUpdate();
}

//------------------------------------------------------------------------------

void Render::doUpdate( ::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    if ( msg->hasEvent("SCENE_RENDER_REQUEST") )
    {
        ::fwData::Object::csptr dataInfo = msg->getDataInfo("SCENE_RENDER_REQUEST");
        SLM_ASSERT("dataInfo is missing", dataInfo);
        SLM_ASSERT("dataInfo must contain a string", ::fwData::String::dynamicConstCast( dataInfo ));
        std::string requestSceneID = ::fwData::String::dynamicConstCast( dataInfo )->value();
        if( requestSceneID == ::fwTools::UUID::get( this->getRenderService() ) )
        {
            this->getInteractor()->Render() ;
            this->getRenderService()->setPendingRenderRequest(false);
        }
    }
}

} //namespace visuVTKAdaptor
