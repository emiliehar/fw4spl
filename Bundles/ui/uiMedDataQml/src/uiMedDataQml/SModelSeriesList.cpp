/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiMedDataQml/SModelSeriesList.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hxx>

#include <fwCore/base.hpp>

#include <fwData/Boolean.hpp>
#include <fwData/Float.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/String.hpp>

#include <fwDataTools/helper/Field.hpp>

#include <fwMedData/ModelSeries.hpp>

#include <fwQml/QmlRegistry.hpp>

namespace uiMedDataQml
{

const ::fwCom::Signals::SignalKeyType SModelSeriesList::s_RECONSTRUCTION_SELECTED_SIG = "reconstructionSelected";
const ::fwCom::Signals::SignalKeyType SModelSeriesList::s_EMPTIED_SELECTION_SIG       = "emptiedSelection";
const ::fwCom::Slots::SlotKeyType SModelSeriesList::s_SHOW_RECONSTRUCTIONS_SLOT       = "showReconstructions";

const ::fwServices::IService::KeyType s_MODEL_SERIES_INOUT = "modelSeries";

static ::fwQml::QmlRegistry<SModelSeriesList> registrar("uiMedDataQml", 1, 0, "SModelSeriesList");

SModelSeriesList::SModelSeriesList() noexcept :
    m_enableHideAll(true),
    m_listModel(nullptr)
{
    m_sigReconstructionSelected = newSignal< ReconstructionSelectedSignalType >( s_RECONSTRUCTION_SELECTED_SIG );
}

//------------------------------------------------------------------------------

SModelSeriesList::~SModelSeriesList() noexcept
{

}

//------------------------------------------------------------------------------

void SModelSeriesList::starting()
{
    ::fwQml::IQmlEditor::starting();
}

//------------------------------------------------------------------------------

void SModelSeriesList::stopping()
{
    m_listModel->updateModelSeries(nullptr);

    ::fwQml::IQmlEditor::stopping();
}

//------------------------------------------------------------------------------

void SModelSeriesList::configuring()
{

}

//------------------------------------------------------------------------------

void SModelSeriesList::updating()
{
    ::fwMedData::ModelSeries::sptr modelSeries = this->getInOut< ::fwMedData::ModelSeries >(s_MODEL_SERIES_INOUT);
    SLM_ASSERT("inout 'modelSeries' is missing", modelSeries);

    SLM_ASSERT("list model is not defined.", m_listModel);
    m_listModel->updateModelSeries(modelSeries);
}

//------------------------------------------------------------------------------

//void SModelSeriesList::onCurrentItemChanged( QTreeWidgetItem* current, QTreeWidgetItem* )
//{
//    SLM_ASSERT( "Current selected item is null", current );
//    std::string id = current->data(0, Qt::UserRole).toString().toStdString();

//    ::fwData::Reconstruction::sptr rec = ::fwData::Reconstruction::dynamicCast(::fwTools::fwID::getObject(id));

//    m_sigReconstructionSelected->asyncEmit(rec);
//}

////------------------------------------------------------------------------------

//void SModelSeriesList::onCurrentItemChanged ( QTreeWidgetItem* current, int column )
//{
//    this->onOrganChoiceVisibility(current, column);
//}

////------------------------------------------------------------------------------

//void SModelSeriesList::onOrganChoiceVisibility(QTreeWidgetItem* item, int )
//{
//    std::string id = item->data(0, Qt::UserRole).toString().toStdString();
//    ::fwData::Reconstruction::sptr rec = ::fwData::Reconstruction::dynamicCast(::fwTools::fwID::getObject(id));
//    SLM_ASSERT("rec not instanced", rec);

//    const bool itemIsChecked = (item->checkState(0) == Qt::Checked);

//    if (rec->getIsVisible() != itemIsChecked)
//    {
//        rec->setIsVisible(itemIsChecked);

//        ::fwData::Reconstruction::VisibilityModifiedSignalType::sptr sig;
//        sig = rec->signal< ::fwData::Reconstruction::VisibilityModifiedSignalType >(
//            ::fwData::Reconstruction::s_VISIBILITY_MODIFIED_SIG);
//        sig->asyncEmit(itemIsChecked);
//    }
//}

////------------------------------------------------------------------------------

void SModelSeriesList::onShowReconstructions(int state )
{
    ::fwMedData::ModelSeries::sptr modelSeries = this->getInOut< ::fwMedData::ModelSeries >(s_MODEL_SERIES_INOUT);
    SLM_ASSERT("'" + s_MODEL_SERIES_INOUT+ "' must be defined as 'inout'", modelSeries);
    ::fwDataTools::helper::Field helper( modelSeries );
    helper.addOrSwap("ShowReconstructions", ::fwData::Boolean::New(state == Qt::Unchecked));
    helper.notify();
}

//------------------------------------------------------------------------------

void SModelSeriesList::onCheckAllBoxes( bool checked )
{
    ::fwMedData::ModelSeries::sptr modelSeries = this->getInOut< ::fwMedData::ModelSeries >(s_MODEL_SERIES_INOUT);
    SLM_ASSERT("'" + s_MODEL_SERIES_INOUT+ "' must be defined as 'inout'", modelSeries);

    for (const auto& rec : modelSeries->getReconstructionDB())
    {
        if (rec->getIsVisible() != checked)
        {
            rec->setIsVisible(checked);

            ::fwData::Reconstruction::VisibilityModifiedSignalType::sptr sig;
            sig = rec->signal< ::fwData::Reconstruction::VisibilityModifiedSignalType >(
                ::fwData::Reconstruction::s_VISIBILITY_MODIFIED_SIG);
            sig->asyncEmit(checked);
        }
    }
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsMap SModelSeriesList::getAutoConnections() const
{
    KeyConnectionsMap connections;

    // FIXME hack to support deprecated getObject() with any key
    if (this->getInOut< ::fwMedData::ModelSeries >(s_MODEL_SERIES_INOUT))
    {
        connections.push(s_MODEL_SERIES_INOUT, ::fwMedData::ModelSeries::s_MODIFIED_SIG, s_UPDATE_SLOT);
        connections.push(s_MODEL_SERIES_INOUT, ::fwMedData::ModelSeries::s_RECONSTRUCTIONS_ADDED_SIG, s_UPDATE_SLOT);
        connections.push(s_MODEL_SERIES_INOUT, ::fwMedData::ModelSeries::s_RECONSTRUCTIONS_REMOVED_SIG, s_UPDATE_SLOT);
    }

    return connections;
}

//------------------------------------------------------------------------------

} // namespace uiMedDataQml
