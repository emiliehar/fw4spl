/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwQml/config.hpp"

#include <QQmlEngine>

namespace fwQml
{

/// This call allows to register the Qml modules
template<typename Type>
class QmlRegistry
{
public:
    /**
     *  @brief: constructor, call qmlRegisterType<Type> that precise to QML which class is used for this typename
     */
    QmlRegistry(std::string const& packageName, int versionMajor, int versionMinor, std::string const& objectName)
    {
        qmlRegisterType<Type>(packageName.c_str(), versionMajor, versionMinor, objectName.c_str());
    }
};

} // fwQml
