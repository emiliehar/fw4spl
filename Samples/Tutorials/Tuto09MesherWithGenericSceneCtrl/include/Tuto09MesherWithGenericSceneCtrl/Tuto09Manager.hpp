/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "Tuto09MesherWithGenericSceneCtrl/config.hpp"

#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/ModelSeries.hpp>

#include <fwQml/AppManager.hpp>

#include <fwServices/IService.hpp>

#include <fwVTKQml/FrameBufferItem.hpp>

#include <QObject>

namespace Tuto09MesherWithGenericSceneCtrl
{

/**
 * @brief   This class is started when the bundles is loaded.
 */
class TUTO09MESHERWITHGENERICSCENECTRL_CLASS_API Tuto09Manager : public ::fwQml::AppManager
{

typedef ::fwVTKQml::FrameBufferItem FrameBufferItem;
Q_OBJECT;
Q_PROPERTY(FrameBufferItem* frameBuffer MEMBER m_frameBuffer)
public:
    /// Constructor.
    TUTO09MESHERWITHGENERICSCENECTRL_API Tuto09Manager() noexcept;

    /// Destructor. Do nothing.
    TUTO09MESHERWITHGENERICSCENECTRL_API ~Tuto09Manager() noexcept;

public Q_SLOTS:
    void initialize();
    void uninitialize();
    void onOpenImage();
    void onSaveMesh();
    void onSnap(const QUrl& url);
    void applyMesher(unsigned int reduction);
    void onShowScan(bool isShown);
    void onServiceCreated(const QVariant& obj);

Q_SIGNALS:

    void imageLoaded();

private:

    bool m_isInitialized {false};
    ::fwVTKQml::FrameBufferItem* m_frameBuffer;
    ::fwMedData::ImageSeries::sptr m_loadedImageSeries;
    ::fwMedData::ModelSeries::sptr m_modelSeries;

    ::fwServices::IService::sptr m_imageAdaptor;
    ::fwServices::IService::sptr m_modelSeriesAdaptor;
    ::fwServices::IService::sptr m_snapshotAdaptor;
    ::fwServices::IService::sptr m_sliceIndexEditor;

};

} // namespace Tuto02DataServiceBasicCtrl
