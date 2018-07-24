/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwQml/config.hpp"
#include "fwQml/IQmlRegistry.hpp"

#include <QQmlEngine>

#include <iostream>

namespace fwQml
{
/**
 *  @brief: QtQmlType represent a Type in QML like a Button
 *      These type aren't define in .xml file.
 *
 */
template<typename Type>
class QmlRegistry : public IQmlRegistry
{
public:
    /**
     *  @brief: constructor, call qmlRegisterType<Type> that precise to QML which class is used for this typename
     */
    QmlRegistry(std::string const& packageName, int versionMajor, int versionMinor, std::string const& objectName)
    {
        m_packageName  = packageName;
        m_versionMajor = versionMajor;
        m_versionMinor = versionMinor;
        m_objectName   = objectName;
    }

    //------------------------------------------------------------------------------

    void    registar() const
    {
        qmlRegisterType<Type>(m_packageName.c_str(), m_versionMajor, m_versionMinor, m_objectName.c_str());
    }

    /**
     *  @brief: destructor, do nothing.
     */
    ~QmlRegistry()
    {
    }

private:
    std::string m_packageName = "";
    int m_versionMajor        = -1;
    int m_versionMinor        = -1;
    std::string m_objectName  = "";

};

} // fwQml
