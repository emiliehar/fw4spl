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
    m_enableHideAll(true)
{
    m_sigReconstructionSelected = newSignal< ReconstructionSelectedSignalType >( s_RECONSTRUCTION_SELECTED_SIG );
    m_sigEmptiedSelection       = newSignal< EmptiedSelectionSignalType >( s_EMPTIED_SELECTION_SIG );

    newSlot(s_SHOW_RECONSTRUCTIONS_SLOT, &SModelSeriesList::showReconstructions, this);
}

//------------------------------------------------------------------------------

SModelSeriesList::~SModelSeriesList() noexcept
{

}

//------------------------------------------------------------------------------

void SModelSeriesList::starting()
{
    this->updating();
}

//------------------------------------------------------------------------------

void SModelSeriesList::stopping()
{
    // TODO: clean the table view
}

//------------------------------------------------------------------------------

void SModelSeriesList::configuring()
{

}

//------------------------------------------------------------------------------

void SModelSeriesList::updating()
{
//    this->blockSignals(true);

    this->updateReconstructions();
    this->refreshVisibility();

//    this->blockSignals(false);
}

//------------------------------------------------------------------------------

void SModelSeriesList::updateReconstructions()
{
    this->fillTree();
}

//------------------------------------------------------------------------------

void SModelSeriesList::fillTree()
{
    ::fwMedData::ModelSeries::sptr modelSeries = this->getInOut< ::fwMedData::ModelSeries >(s_MODEL_SERIES_INOUT);
    SLM_ASSERT("inout 'modelSeries' is missing", modelSeries);

    for (auto& rec : modelSeries->getReconstructionDB())
    {
        Q_EMIT this->addOrgan(QString("{}"));
    }
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
//    const bool visible = static_cast<bool>(state);

//    m_checkAllButton->setEnabled(!visible);
//    m_unCheckAllButton->setEnabled(!visible);
//    m_tree->setEnabled(!visible);

//    ::fwMedData::ModelSeries::sptr modelSeries = this->getInOut< ::fwMedData::ModelSeries >(s_MODEL_SERIES_INOUT);
//    if (!modelSeries)
//    {
//        FW_DEPRECATED_KEY(s_MODEL_SERIES_INOUT, "inout", "18.0");
//        modelSeries = this->getObject< ::fwMedData::ModelSeries >();
//    }
//    {
//        ::fwDataTools::helper::Field helper( modelSeries );
//        helper.addOrSwap("ShowReconstructions", ::fwData::Boolean::New(state == Qt::Unchecked));
//    }
}

////------------------------------------------------------------------------------

void SModelSeriesList::refreshVisibility()
{
//    for( int i = 0; i < m_tree->topLevelItemCount(); ++i )
//    {
//        QTreeWidgetItem* item = m_tree->topLevelItem( i );
//        std::string id        = item->data(0, Qt::UserRole).toString().toStdString();
//        ::fwData::Reconstruction::sptr rec = ::fwData::Reconstruction::dynamicCast(::fwTools::fwID::getObject(id));
//        item->setCheckState(0, rec->getIsVisible() ? Qt::Checked : Qt::Unchecked );
//    }
}

////------------------------------------------------------------------------------

void SModelSeriesList::showReconstructions(bool show)
{
//    if(m_showCheckBox)
//    {
//        m_showCheckBox->setCheckState(show ? Qt::Unchecked : Qt::Checked );
//    }
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

void SModelSeriesList::onCheckAllCheckBox()
{
    this->onCheckAllBoxes(true);
}

//------------------------------------------------------------------------------

void SModelSeriesList::onUnCheckAllCheckBox()
{
    this->onCheckAllBoxes(false);
}

//------------------------------------------------------------------------------

void SModelSeriesList::onCheckAllBoxes( bool visible )
{
//    for( int i = 0; i < m_tree->topLevelItemCount(); ++i )
//    {
//        QTreeWidgetItem* item = m_tree->topLevelItem( i );
//        item->setCheckState(0, visible ? Qt::Checked : Qt::Unchecked );
//    }
}

//------------------------------------------------------------------------------

} // namespace uiMedDataQml
