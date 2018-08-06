/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwQml/config.hpp"

#include <fwCore/macros.hpp>

#include <boost/filesystem/path.hpp>

#include <QList>
#include <QPointer>
#include <QQmlApplicationEngine>

namespace fwQml
{

/// This call allows to manage the QQmlEngine
class QmlEngine
{
public:

    QmlEngine();

    ~QmlEngine();

    FWQML_API static SPTR(QmlEngine) getDefault();

    FWQML_API void loadMainComponent(const ::boost::filesystem::path& file);

    FWQML_API void importModulePath(const ::boost::filesystem::path& path);

    FWQML_API QList<QObject*> getRootObjects();

private:
    static SPTR(QmlEngine) s_current;

    QPointer<QQmlApplicationEngine> m_engine;

};

} // fwQml
