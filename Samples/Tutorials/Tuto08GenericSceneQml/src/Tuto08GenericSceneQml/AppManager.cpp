/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "Tuto08GenericSceneQml/AppManager.hpp"

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/ModelSeries.hpp>

#include <fwQml/IQmlEditor.hpp>

#include <fwRenderVTK/factory/new.hpp>
#include <fwRenderVTK/SRender.hpp>

#include <fwServices/op/Add.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwVTKQml/VtkRenderWindowInteractorManager.hpp>

namespace Tuto08GenericSceneQml
{

static const std::string s_IMAGE_ID       = "image";
static const std::string s_MODELSERIES_ID = "modelSeries";
static const std::string s_TEXTURE_ID     = "texture";

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
}

//------------------------------------------------------------------------------

void AppManager::uninitialize()
{
    this->stopAndUnregisterServices();
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
        m_textureAdaptor     = this->registerService("::visuVTKAdaptor::STexture", "", true);
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
        renderSrv->displayAdaptor(m_textureAdaptor->getID());
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
        ::fwServices::IService::ConfigType textureAdaptorConfig;
        textureAdaptorConfig.add("config.<xmlattr>.renderer", "default");
        textureAdaptorConfig.add("config.<xmlattr>.picker", "");
        textureAdaptorConfig.add("config.<xmlattr>.filtering", "linear");
        textureAdaptorConfig.add("config.<xmlattr>.wrapping", "repeat");
        m_textureAdaptor->setConfiguration(textureAdaptorConfig);
        m_textureAdaptor->configure();

        ::fwServices::IService::ConfigType snapshotAdaptorConfig;
        snapshotAdaptorConfig.add("config.<xmlattr>.renderer", "default");
        m_snapshotAdaptor->setConfiguration(snapshotAdaptorConfig);
        m_snapshotAdaptor->configure();

        /* **************************************************************************************
        *              register and star the services
        ****************************************************************************************/

        this->registerObject(m_imageAdaptor, s_IMAGE_ID, "image", AccessType::INOUT, true);
        this->registerObject(m_modelSeriesAdaptor, s_MODELSERIES_ID, "model", AccessType::INPUT, true);
        this->registerObject(m_textureAdaptor, s_TEXTURE_ID, "texture", AccessType::INOUT, true);

        this->connectSignal("textureChannel", m_modelSeriesAdaptor, "textureApplied");
        this->connectSlot("textureChannel", m_textureAdaptor, "applyTexture");

        this->startServices();
        m_vtkSceneCreated = true;
    }
}

//------------------------------------------------------------------------------

void AppManager::onOpenImage()
{
    auto imageReader = ::fwServices::add("::uiIO::editor::SIOSelector");
    ::fwServices::IService::ConfigType imageReaderConfig;
    imageReaderConfig.put("type.<xmlattr>.mode", "reader");
    imageReaderConfig.put("type.<xmlattr>.class", "::fwData::Image");
    imageReader->setConfiguration(imageReaderConfig);
    imageReader->configure();
    imageReader->start();
    imageReader->update();
    ::fwData::Image::sptr image = imageReader->getOutput< ::fwData::Image >("data");

    if (image && ::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity(image))
    {
        ::fwDataTools::helper::Image helper( image );

        helper.createLandmarks();
        helper.createTransferFunctionPool();
        helper.createImageSliceIndex();

        this->addObject(image, s_IMAGE_ID);
        Q_EMIT imageLoaded();
    }

    imageReader->stop();
    ::fwServices::OSR::unregisterService(imageReader);
}

//------------------------------------------------------------------------------

void AppManager::onOpenModel()
{
    auto modelSeriesReader = ::fwServices::add("::uiIO::editor::SIOSelector");
    ::fwServices::IService::ConfigType modelSeriesReaderConfig;
    modelSeriesReaderConfig.put("type.<xmlattr>.mode", "reader");
    modelSeriesReaderConfig.put("type.<xmlattr>.class", "::fwMedData::ModelSeries");
    modelSeriesReader->setConfiguration(modelSeriesReaderConfig);
    modelSeriesReader->configure();
    modelSeriesReader->start();
    modelSeriesReader->update();
    auto modelSeries = modelSeriesReader->getOutput< ::fwMedData::ModelSeries >("data");
    this->addObject(modelSeries, s_MODELSERIES_ID);

    modelSeriesReader->stop();
    ::fwServices::OSR::unregisterService(modelSeriesReader);
}

//------------------------------------------------------------------------------

void AppManager::onOpenTexture()
{
    auto textureReader = ::fwServices::add("::uiIO::editor::SIOSelector");
    ::fwServices::IService::ConfigType textureReaderConfig;
    textureReaderConfig.put("type.<xmlattr>.mode", "reader");
    textureReaderConfig.put("type.<xmlattr>.class", "::fwData::Image");
    textureReader->setConfiguration(textureReaderConfig);
    textureReader->configure();
    textureReader->start();
    textureReader->update();
    auto texture = textureReader->getOutput< ::fwData::Image >("data");

    this->addObject(texture, s_TEXTURE_ID);

    if (m_textureAdaptor->isStarted() && m_modelSeriesAdaptor->isStarted())
    {
        m_modelSeriesAdaptor->update();
    }
    textureReader->stop();
    ::fwServices::OSR::unregisterService(textureReader);
}

//------------------------------------------------------------------------------

void AppManager::onSnap(QUrl url)
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
    }
}

//------------------------------------------------------------------------------

} // namespace Tuto08GenericSceneQml
