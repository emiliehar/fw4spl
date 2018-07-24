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

#include "Tuto09MesherWithGenericSceneCtrl/Tuto09Manager.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hxx>

#include <fwCore/spyLog.hpp>

#include <fwData/Image.hpp>
#include <fwData/Reconstruction.hpp>

#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>
#include <fwDataTools/helper/Image.hpp>

#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/ModelSeries.hpp>

#include <fwQml/QmlRegistry.hpp>

#include <fwRenderVTK/factory/new.hpp>
#include <fwRenderVTK/SRender.hpp>

#include <fwServices/op/Add.hpp>
#include <fwServices/op/Get.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwVTKQml/VtkRenderWindowInteractorManager.hpp>

namespace Tuto09MesherWithGenericSceneCtrl
{

static ::fwQml::QmlRegistry<Tuto09Manager> registrar("fw4spl.tuto.Tuto09Manager", 1, 0, "Tuto09Manager");

//------------------------------------------------------------------------------

Tuto09Manager::Tuto09Manager() noexcept :
    m_frameBuffer(nullptr)
{
}

//------------------------------------------------------------------------------

Tuto09Manager::~Tuto09Manager() noexcept
{
    this->uninitialize();
    delete m_frameBuffer;
}

//------------------------------------------------------------------------------

void Tuto09Manager::initialize()
{
    // generic scene
    auto renderSrv = ::fwServices::add< ::fwRenderVTK::SRender >("::fwRenderVTK::SRender", "genericScene");
    m_imageAdaptor       = ::fwServices::add("::visuVTKAdaptor::SNegatoMPR", "imageAdaptor");
    m_modelSeriesAdaptor = ::fwServices::add("::visuVTKAdaptor::SModelSeries", "modelSeriesAdaptor");
    auto snapshotAdaptor = ::fwServices::add("::visuVTKAdaptor::SSnapshot", "snapshotAdaptor");

    /* **************************************************************************************
    *              genericScene configuration
    ****************************************************************************************/

    // create and register the render service
    ::fwServices::IService::ConfigType renderConfig;
    ::fwServices::IService::ConfigType pickerConfig;
    pickerConfig.add("<xmlattr>.vtkclass", "fwVtkCellPicker");
    pickerConfig.add("<xmlattr>.id", "picker");
    renderConfig.add_child("scene.picker", pickerConfig);
    renderConfig.add("scene.renderer.<xmlattr>.id", "default");
    ::fwServices::IService::ConfigType adpt1Config;
    adpt1Config.put("<xmlattr>.uid", "modelSeriesAdaptor");
    renderConfig.add_child("scene.adaptor", adpt1Config);
    ::fwServices::IService::ConfigType adpt2Config;
    adpt2Config.put("<xmlattr>.uid", "imageAdaptor");
    renderConfig.add_child("scene.adaptor", adpt2Config);
    ::fwServices::IService::ConfigType adpt3Config;
    adpt3Config.put("<xmlattr>.uid", "snapshotAdaptor");
    renderConfig.add_child("scene.adaptor", adpt3Config);
    renderSrv->setConfiguration(renderConfig);
    renderSrv->useContainer(false);

    auto interactorManager = ::fwRenderVTK::factory::New< ::fwVTKQml::VtkRenderWindowInteractorManager >();
    SLM_ASSERT("Frame Buffer is not yet defined", m_frameBuffer);
    interactorManager->setFrameBuffer(m_frameBuffer);
    renderSrv->setInteractorManager(interactorManager);
    renderSrv->configure();

    ::fwServices::IService::ConfigType imageAdaptorConfig;
    imageAdaptorConfig.add("config.<xmlattr>.renderer", "default");
    imageAdaptorConfig.add("config.<xmlattr>.picker", "picker");
    imageAdaptorConfig.add("config.<xmlattr>.mode", "3d");
    imageAdaptorConfig.add("config.<xmlattr>.slice", "3");
    imageAdaptorConfig.add("config.<xmlattr>.sliceIndex", "axial");
    m_imageAdaptor->setConfiguration(imageAdaptorConfig);
    m_imageAdaptor->configure();

    ::fwServices::IService::ConfigType modelSeriesAdaptorConfig;
    modelSeriesAdaptorConfig.add("config.<xmlattr>.renderer", "default");
    modelSeriesAdaptorConfig.add("config.<xmlattr>.picker", "");
    m_modelSeriesAdaptor->setConfiguration(modelSeriesAdaptorConfig);
    m_modelSeriesAdaptor->configure();

    ::fwServices::IService::ConfigType snapshotAdaptorConfig;
    snapshotAdaptorConfig.add("config.<xmlattr>.renderer", "default");
    snapshotAdaptor->setConfiguration(snapshotAdaptorConfig);
    snapshotAdaptor->configure();

    /* **************************************************************************************
    *              start the services
    ****************************************************************************************/

    renderSrv->start();
    m_startedService.emplace(renderSrv);
}

//------------------------------------------------------------------------------

void Tuto09Manager::uninitialize()
{
    this->stopAndUnregisterServices();
}

//------------------------------------------------------------------------------

void Tuto09Manager::onOpenImage()
{
    auto imageSeriesReader = ::fwServices::add("::uiIO::editor::SIOSelector");
    ::fwServices::IService::ConfigType imageSeriesReaderConfig;
    imageSeriesReaderConfig.put("type.<xmlattr>.mode", "reader");
    imageSeriesReaderConfig.put("type.<xmlattr>.class", "::fwMedData::ImageSeries");
    imageSeriesReader->setConfiguration(imageSeriesReaderConfig);
    imageSeriesReader->configure();
    imageSeriesReader->start();
    imageSeriesReader->update();
    ::fwMedData::ImageSeries::sptr imageSeries = imageSeriesReader->getOutput< ::fwMedData::ImageSeries >("data");
    imageSeriesReader->stop();
    ::fwServices::OSR::unregisterService(imageSeriesReader);

    if (imageSeries)
    {
        ::fwData::Image::sptr image = imageSeries->getImage();

        if(::fwDataTools::fieldHelper::MedicalImageHelpers::
           checkImageValidity(image))
        {
            ::fwDataTools::helper::Image helper( image );

            helper.createLandmarks();
            helper.createTransferFunctionPool();
            helper.createImageSliceIndex();

            this->registerObj(m_imageAdaptor, image, "image",  ::fwServices::IService::AccessType::INOUT, true);
            m_loadedImageSeries = imageSeries;
        }
    }
}

//------------------------------------------------------------------------------

void Tuto09Manager::onSaveMesh()
{
    if (m_modelSeries)
    {
        auto modelSeriesWriter = ::fwServices::add("::uiIO::editor::SIOSelector");
        modelSeriesWriter->registerInput(m_modelSeries, "data");
        ::fwServices::IService::ConfigType modelSeriesWriterConfig;
        modelSeriesWriterConfig.put("type.<xmlattr>.mode", "writer");
        modelSeriesWriter->setConfiguration(modelSeriesWriterConfig);
        modelSeriesWriter->configure();
        modelSeriesWriter->start();
        modelSeriesWriter->update();
        modelSeriesWriter->stop();
        ::fwServices::OSR::unregisterService(modelSeriesWriter);
    }
}

//------------------------------------------------------------------------------

void Tuto09Manager::applyMesher(unsigned int reduction)
{
    if (m_loadedImageSeries)
    {
        auto mesher = ::fwServices::add("::opVTKMesh::SVTKMesher");
        mesher->registerInput(m_loadedImageSeries, "imageSeries");

        ::fwServices::IService::ConfigType mesherConfig;
        mesherConfig.put("config.percentReduction", reduction);
        mesher->setConfiguration(mesherConfig);
        mesher->configure();
        mesher->start();
        mesher->update();
        m_modelSeries = mesher->getOutput< ::fwMedData::ModelSeries >("modelSeries");
        mesher->stop();
        ::fwServices::OSR::unregisterService(mesher);

        this->registerObj(m_modelSeriesAdaptor, m_modelSeries, "model", ::fwServices::IService::AccessType::INPUT,
                          true);
    }
}

//------------------------------------------------------------------------------

} // namespace Tuto09MesherWithGenericSceneCtrl
