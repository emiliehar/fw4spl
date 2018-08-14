/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "Tuto09MesherWithGenericSceneCtrl/Tuto09Manager.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hxx>

#include <fwCore/spyLog.hpp>

#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Reconstruction.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>
#include <fwDataTools/helper/Image.hpp>
#include <fwDataTools/helper/MedicalImageAdaptor.hpp>

#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/ModelSeries.hpp>

#include <fwQml/IQmlEditor.hpp>
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

static ::fwQml::QmlRegistry<Tuto09Manager> registrar("tuto.Tuto09Manager", 1, 0, "Tuto09Manager");

//------------------------------------------------------------------------------

Tuto09Manager::Tuto09Manager() noexcept :
    m_frameBuffer(nullptr)
{
}

//------------------------------------------------------------------------------

Tuto09Manager::~Tuto09Manager() noexcept
{
    delete m_frameBuffer;
}

//------------------------------------------------------------------------------

void Tuto09Manager::initialize()
{
    if (!m_isInitialized)
    {
        // generic scene
        auto renderSrv = ::fwServices::add< ::fwRenderVTK::SRender >("::fwRenderVTK::SRender");
        m_imageAdaptor       = ::fwServices::add("::visuVTKAdaptor::SNegatoMPR");
        m_modelSeriesAdaptor = ::fwServices::add("::visuVTKAdaptor::SModelSeries");
        m_snapshotAdaptor    = ::fwServices::add("::visuVTKAdaptor::SSnapshot");
        this->registerService(renderSrv);
        this->registerService(m_imageAdaptor);
        this->registerService(m_modelSeriesAdaptor);
        this->registerService(m_snapshotAdaptor);

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
        renderSrv->setConfiguration(renderConfig);
        renderSrv->useContainer(false);
        renderSrv->displayAdaptor(m_modelSeriesAdaptor->getID());
        renderSrv->displayAdaptor(m_imageAdaptor->getID());
        renderSrv->displayAdaptor(m_snapshotAdaptor->getID());

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
        m_snapshotAdaptor->setConfiguration(snapshotAdaptorConfig);
        m_snapshotAdaptor->configure();

        /* **************************************************************************************
        *              start the services
        ****************************************************************************************/

        this->startService(renderSrv);
        this->startService(m_snapshotAdaptor);
        m_isInitialized = true;
    }
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

            this->registerObj(m_imageAdaptor, image, "image",  ::fwServices::IService::AccessType::INOUT, true, true);
            if (m_sliceIndexEditor)
            {
                this->registerObj(m_sliceIndexEditor, image, "image",  ::fwServices::IService::AccessType::INOUT, true,
                                  true);
            }
            m_loadedImageSeries = imageSeries;
            Q_EMIT imageLoaded();
        }
    }
}

//------------------------------------------------------------------------------

void Tuto09Manager::onSaveMesh()
{
    if (m_modelSeries)
    {
        auto modelSeriesWriter = ::fwServices::add("::uiIO::editor::SIOSelector");
        modelSeriesWriter->registerInOut(m_modelSeries, "data");
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

        SLM_ASSERT("modelSeries is not created", m_modelSeries);
        this->registerObj(m_modelSeriesAdaptor, m_modelSeries, "model", ::fwServices::IService::AccessType::INPUT,
                          true, true);

        if (m_modelSeriesListEditor)
        {
            this->registerObj(m_modelSeriesListEditor, m_modelSeries, "modelSeries",
                              ::fwServices::IService::AccessType::INOUT, true, true);
        }
    }
}

//------------------------------------------------------------------------------

void Tuto09Manager::onSnap(const QUrl& url)
{
    m_snapshotAdaptor->slot("snap")->asyncRun(url.path().toStdString());
}

//------------------------------------------------------------------------------

void Tuto09Manager::onUpdateSliceMode(int mode)
{
    m_imageAdaptor->slot("updateSliceMode")->asyncRun(mode);
}

//------------------------------------------------------------------------------

void Tuto09Manager::onShowScan(bool isShown)
{
    m_imageAdaptor->slot("showSlice")->asyncRun(isShown);
}

//------------------------------------------------------------------------------

void Tuto09Manager::onServiceCreated(const QVariant& obj)
{
    ::fwQml::IQmlEditor::sptr srv(obj.value< ::fwQml::IQmlEditor* >());
    if (srv)
    {
        if (srv->isA("::uiImageQml::SSliceIndexPositionEditor") && m_loadedImageSeries)
        {
            this->registerObj(srv, m_loadedImageSeries->getImage(), "image",
                              ::fwServices::IService::AccessType::INOUT, true, true);
            m_sliceIndexEditor = srv;
        }
        else if (srv->isA("::uiMedDataQml::SModelSeriesList") && m_modelSeries)
        {
            this->registerObj(srv, m_modelSeries, "modelSeries",
                              ::fwServices::IService::AccessType::INOUT, true, true);
            m_modelSeriesListEditor = srv;
        }
    }
}

//------------------------------------------------------------------------------

} // namespace Tuto09MesherWithGenericSceneCtrl
