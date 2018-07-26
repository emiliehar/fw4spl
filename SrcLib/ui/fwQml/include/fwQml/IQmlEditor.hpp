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
 * @brief Defines the service interface managing the editor service for object.
 */

class FWQML_CLASS_API IQmlEditor : public QObject,
                                   public ::fwServices::IService
{

Q_OBJECT
public:

    fwCoreServiceClassDefinitionsMacro( (IQmlEditor)(::fwServices::IService) );

Q_SIGNALS:
    void created(QString uid);

public Q_SLOTS:
    //------------------------------------------------------------------------------

    void initialize()
    {
        this->created(QString::fromStdString(this->getID()));
    }

protected:

    /**
     * @brief   Constructor. Do nothing (Just initialize parameters).
     */
    IQmlEditor() noexcept
    {
    }

    /// Destructor. Do nothing.
    virtual ~IQmlEditor() noexcept
    {
    }

};

} // namespace fwQml
