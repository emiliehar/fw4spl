/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "uiReconstructionQml/config.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>

#include <fwData/Material.hpp>

#include <fwQml/IQmlEditor.hpp>

#include <QObject>

namespace uiReconstructionQml
{

/**
 * @brief Display a widget to change the reconstruction representation (surface, point, edge, ...).
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::uiReconstructionQml::SRepresentationEditor">
       <inout key="reconstruction" uid="..." />
   </service>
   @endcode
 * @subsection In-Out In-Out
 * - \b reconstruction [::fwData::Reconstruction]: reconstruction that will be updated
 */
class UIRECONSTRUCTIONQML_CLASS_API SRepresentationEditor : public ::fwQml::IQmlEditor
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro( (SRepresentationEditor)(::fwQml::IQmlEditor) );

    /// Constructor. Do nothing.
    UIRECONSTRUCTIONQML_API SRepresentationEditor() noexcept;

    /// Destructor. Do nothing.
    UIRECONSTRUCTIONQML_API virtual ~SRepresentationEditor() noexcept;

Q_SIGNALS:
    void materialChanged(int representationMode, int shadingMode, int optionMode);

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /// Initialize the UI
    virtual void starting() override;

    /// Clean the UI
    virtual void stopping() override;

    /// Update the UI according to the reconstruction
    virtual void updating() override;

    /// Do nothing.
    virtual void configuring() override;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Reconstruction::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     */
    virtual KeyConnectionsMap getAutoConnections() const override;

    /// Notify the changes
    void notifyMaterial();

protected Q_SLOTS:

    void onChangeRepresentation( int id );
    void onChangeShading( int id );
    void onShowNormals(int state );

private:

    ::fwData::Material::sptr m_material;

};

} // uiReconstructionQml
