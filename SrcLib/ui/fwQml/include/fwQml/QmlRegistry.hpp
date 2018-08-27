/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwQml/config.hpp"

#include <QQmlEngine>

namespace fwQml
{

/**
 * @brief This class allows to register the Qml modules.
 *
 * It should be used as a global static variable declared on the top of your cpp file.
 *
 * @code{.cpp}
        static ::fwQml::QmlRegistry< SServiceName> registrar("bundleName", versionMajor, versionMinor,
                "SServiceName");
   @endcode
 */
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
