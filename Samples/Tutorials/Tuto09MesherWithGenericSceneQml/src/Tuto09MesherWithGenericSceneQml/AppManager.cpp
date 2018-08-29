/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "Tuto09MesherWithGenericSceneQml/AppManager.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hxx>

#include <fwCore/spyLog.hpp>

#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>
#include <fwDataTools/helper/Image.hpp>
#include <fwDataTools/helper/MedicalImageAdaptor.hpp>

#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/ModelSeries.hpp>

#include <fwQml/IQmlEditor.hpp>

#include <fwRenderVTK/factory/new.hpp>
#include <fwRenderVTK/SRender.hpp>

#include <fwServices/op/Add.hpp>
#include <fwServices/op/Get.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/registry/Proxy.hpp>

#include <fwVTKQml/VtkRenderWindowInteractorManager.hpp>

namespace Tuto09MesherWithGenericSceneQml
{

static const std::string s_IMAGE_ID          = "image";
static const std::string s_MODELSERIES_ID    = "modelSeries";
static const std::string s_RECONSTRUCTION_ID = "reconstruction";

static const std::string s_recSelectedChannel    = "recSelected";
static const std::string s_emptySelectionChannel = "emptySelection";

//------------------------------------------------------------------------------

AppManager::AppManager() noexcept :
    m_frameBuffer(nullptr)
{

}

//------------------------------------------------------------------------------

AppManager::~AppManager() noexcept
{
    delete m_frameBuffer;
}

//------------------------------------------------------------------------------

void AppManager::initialize()
{
    this->create();

    ::fwThread::Worker::sptr worker = ::fwThread::Worker::New();
    auto workerRegistry = ::fwServices::registry::ActiveWorkers::getDefault();
    workerRegistry->addWorker("AppManager-slots", worker);

    auto proxy = ::fwServices::registry::Proxy::getDefault();

    std::function<void(::fwData::Object::sptr)>  recSelectedFct =
        [ = ] (::fwData::Object::sptr rec)
        {
            this->addObject(rec, s_RECONSTRUCTION_ID);
        };
    m_slotRecSelected = ::fwCom::newSlot(recSelectedFct);
    m_slotRecSelected->setWorker(worker);
    proxy->connect(s_recSelectedChannel, m_slotRecSelected);

    std::function<void()>  emptySelectionFct =
        [ = ] ()
        {
            this->removeObject(nullptr, s_RECONSTRUCTION_ID);
        };
    m_slotEmptySelection = ::fwCom::newSlot(emptySelectionFct);
    m_slotEmptySelection->setWorker(worker);
    proxy->connect(s_emptySelectionChannel, m_slotEmptySelection);
}

//------------------------------------------------------------------------------

void AppManager::uninitialize()
{
    this->stopAndUnregisterServices();

    auto proxy = ::fwServices::registry::Proxy::getDefault();
    proxy->disconnect(s_recSelectedChannel, m_slotRecSelected);
    proxy->disconnect(s_emptySelectionChannel, m_slotEmptySelection);
}

//------------------------------------------------------------------------------

void AppManager::createVtkScene()
{
    if (!m_vtkSceneCreated)
    {
        // generic scene
        auto renderSrv = this->registerService< ::fwRenderVTK::SRender >("::fwRenderVTK::SRender", "", true);
        m_imageAdaptor       = this->registerService("::visuVTKAdaptor::SNegatoMPR", "", true);
        m_modelSeriesAdaptor = this->registerService("::visuVTKAdaptor::SModelSeries", "", true);
        m_snapshotAdaptor    = this->registerService("::visuVTKAdaptor::SSnapshot", "", true);

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

        this->registerObject(m_imageAdaptor, s_IMAGE_ID, "image", AccessType::INOUT, true);
        this->registerObject(m_modelSeriesAdaptor, s_MODELSERIES_ID, "model", AccessType::INPUT, true);

        this->startServices();
        m_vtkSceneCreated = true;
    }
}

//------------------------------------------------------------------------------

void AppManager::onOpenImage()
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

            this->addObject(image, s_IMAGE_ID);

            m_loadedImageSeries = imageSeries;
            Q_EMIT imageLoaded();
        }
    }
}

//------------------------------------------------------------------------------

void AppManager::onSaveMesh()
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

void AppManager::applyMesher(unsigned int reduction)
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
        this->addObject(m_modelSeries, s_MODELSERIES_ID);
    }
}

//------------------------------------------------------------------------------

void AppManager::onSnap(const QUrl& url)
{
    m_snapshotAdaptor->slot("snap")->asyncRun(url.path().toStdString());
}

//------------------------------------------------------------------------------

void AppManager::onUpdateSliceMode(int mode)
{
    m_imageAdaptor->slot("updateSliceMode")->asyncRun(mode);
}

//------------------------------------------------------------------------------

void AppManager::onShowScan(bool isShown)
{
    m_imageAdaptor->slot("showSlice")->asyncRun(isShown);
}

//------------------------------------------------------------------------------

void AppManager::onServiceCreated(const QVariant& obj)
{
    ::fwQml::IQmlEditor::sptr srv(obj.value< ::fwQml::IQmlEditor* >());
    if (srv)
    {
        if (srv->isA("::uiImageQml::SSliceIndexPositionEditor"))
        {
            this->registerService(srv, true);
            this->registerObject(srv, s_IMAGE_ID, "image",  ::fwServices::IService::AccessType::INOUT, true);
        }
        else if (srv->isA("::uiMedDataQml::SModelSeriesList"))
        {
            this->registerService(srv, true);
            this->registerObject(srv, s_MODELSERIES_ID, "modelSeries",  ::fwServices::IService::AccessType::INOUT,
                                 true);

            this->connectSignal(s_recSelectedChannel, srv, "reconstructionSelected");
            this->connectSignal(s_emptySelectionChannel, srv, "emptiedSelection");
        }
        else if (srv->isA("::uiReconstructionQml::SOrganMaterialEditor"))
        {
            this->registerService(srv, true);
            this->registerObject(srv, s_RECONSTRUCTION_ID, "reconstruction",  ::fwServices::IService::AccessType::INOUT,
                                 true);
        }
        else if (srv->isA("::uiReconstructionQml::SRepresentationEditor"))
        {
            this->registerService(srv, true);
            this->registerObject(srv, s_RECONSTRUCTION_ID, "reconstruction",  ::fwServices::IService::AccessType::INOUT,
                                 true);
        }
    }
}

//------------------------------------------------------------------------------

} // namespace Tuto09MesherWithGenericSceneQml
