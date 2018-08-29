/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "uiMedDataQml/config.hpp"
#include "uiMedDataQml/OrganListModel.hpp"

#include <fwQml/IQmlEditor.hpp>

#include <QIdentityProxyModel>
#include <QObject>

namespace uiMedDataQml
{

/**
 * @brief   SModelSeriesList service allows to manage the ModelSeries organs.
 *
 *  @section Signals Signals
 * - \b reconstructionSelected(::fwData::Object::sptr): this signal emits the selected reconstruction
 * - \b emptiedSelection(): this signal is emitted when no reconstruction is selected
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service uid="..." type="::uiMedDataQml::SModelSeriesList" autoConnect="yes">
       <inout key="modelSeries" uid="..." autoConnect="yes" />
   </service>
   @endcode
 * @subsection In-Out In-Out
 * - \b modelSeries [::fwMedData::ModelSeries]: model series containing the organs to list
 */
class UIMEDDATAQML_CLASS_API SModelSeriesList : public ::fwQml::IQmlEditor
{
Q_OBJECT
Q_PROPERTY(OrganListModel* listModel MEMBER m_listModel)
public:

    fwCoreServiceClassDefinitionsMacro( (SModelSeriesList)(::fwQml::IQmlEditor) )

    /// Constructor. Do nothing.
    UIMEDDATAQML_API SModelSeriesList() noexcept;

    /// Destructor. Do nothing.
    UIMEDDATAQML_API virtual ~SModelSeriesList() noexcept;

Q_SIGNALS:

public Q_SLOTS:

protected:

    ///This method launches the IEditor::starting method.
    virtual void starting() override;

    ///This method launches the IEditor::stopping method.
    virtual void stopping() override;

    virtual void updating() override;

    /// Configures the editor.
    virtual void configuring() override;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect ModelSeries::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     * Connect ModelSeries::s_RECONSTRUCTIONS_ADDED_SIG to this::s_UPDATE_SLOT
     * Connect ModelSeries::s_RECONSTRUCTIONS_REMOVED_SIG to this::s_UPDATE_SLOT
     */
    virtual KeyConnectionsMap getAutoConnections() const override;

    static const ::fwCom::Signals::SignalKeyType s_RECONSTRUCTION_SELECTED_SIG;
    typedef ::fwCom::Signal< void (::fwData::Object::sptr) > ReconstructionSelectedSignalType;

    static const ::fwCom::Signals::SignalKeyType s_EMPTIED_SELECTION_SIG;
    typedef ::fwCom::Signal< void () > EmptiedSelectionSignalType;

protected Q_SLOTS:

    void onOrganSelected(int index);

    void onShowReconstructions(int state);

    void onOrganVisibilityChanged(int index, bool visible);

    void onCheckAllBoxes(bool checked);

private:

    /// Signal emitted when a reconstruction is selected
    ReconstructionSelectedSignalType::sptr m_sigReconstructionSelected;

    /// Signal emitted when we clean the list
    EmptiedSelectionSignalType::sptr m_sigEmptiedSelection;

    OrganListModel* m_listModel;
};

} // uiMedDataQml