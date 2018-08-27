/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwQml/config.hpp"

#include <fwServices/IService.hpp>

#include <QObject>

namespace fwQml
{

/**
 * @brief Defines the base class for service editing an object associated to a qml UI.
 */
class FWQML_CLASS_API IQmlEditor : public QObject,
                                   public ::fwServices::IService
{

Q_OBJECT
public:

    fwCoreServiceClassDefinitionsMacro( (IQmlEditor)(::fwServices::IService) );

    /// Constructor. Do nothing.
    IQmlEditor() noexcept;

    /// Destructor. Do nothing.
    virtual ~IQmlEditor() noexcept;

Q_SIGNALS:

    /// Signal emitted when the service is started
    void started();

    /// Signal emitted when the service is stopped
    void stopped();

protected:

    /// call updating and emit 'started' qt signal
    FWQML_API virtual void starting() override;

    /// emit 'stopped' qt signal
    FWQML_API virtual void stopping() override;

};

} // namespace fwQml
