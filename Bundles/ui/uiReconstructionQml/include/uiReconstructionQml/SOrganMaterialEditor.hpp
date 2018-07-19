/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "uiReconstructionQml/config.hpp"

#include <fwQml/IQmlEditor.hpp>

#include <QColor>
#include <QObject>

namespace uiReconstructionQml
{

/**
 * @brief Display a widget to change the reconstruction material (color and transparency).
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::uiReconstructionQml::OrganMaterialEditor">
       <inout key="reconstruction" uid="..." />
   </service>
   @endcode
 * @subsection In-Out In-Out
 * - \b reconstruction [::fwData::Reconstruction]: reconstruction containing the material to update.
 */
class UIRECONSTRUCTIONQML_CLASS_API SOrganMaterialEditor : public ::fwQml::IQmlEditor
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro( (SOrganMaterialEditor)(::fwQml::IQmlEditor) );

    /// Constructor. Do nothing.
    UIRECONSTRUCTIONQML_API SOrganMaterialEditor() noexcept;

    /// Destructor. Do nothing.
    UIRECONSTRUCTIONQML_API virtual ~SOrganMaterialEditor() noexcept;

Q_SIGNALS:

    void materialChanged(QColor color, int opacity);

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /// Initialize the UI
    virtual void starting() override;

    /// Clean the UI
    virtual void stopping() override;

    /// Update the UI according to the material (color and transparency widgets)
    virtual void updating() override;

    /// Do nothing
    virtual void configuring() override;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Reconstruction::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     */
    virtual KeyConnectionsMap getAutoConnections() const override;

protected Q_SLOTS:

    void onOpacitySlider( int value);
    void onColor(QColor color);

private:

    /// Notify the material changes
    void materialNotification();
};

} // uiReconstructionQml
