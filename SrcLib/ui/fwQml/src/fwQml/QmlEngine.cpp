/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQml/QmlEngine.hpp"

#include <fwCore/util/LazyInstantiator.hpp>

#include <QQmlComponent>
#include <QQuickWindow>

namespace fwQml
{

//-----------------------------------------------------------------------------

QmlEngine::QmlEngine()
{
    m_engine = new QQmlApplicationEngine();
}

//-----------------------------------------------------------------------------

QmlEngine::~QmlEngine()
{

}

//-----------------------------------------------------------------------------

SPTR(QmlEngine) QmlEngine::getDefault()
{
    auto engineInstance = ::fwCore::util::LazyInstantiator< QmlEngine >::getInstance();
    return engineInstance;
}

//-----------------------------------------------------------------------------

FWQML_API void QmlEngine::loadMainComponent(const ::boost::filesystem::path& file)
{
    m_engine->load(QUrl::fromLocalFile(QString::fromStdString(file.string())));
}

//-----------------------------------------------------------------------------

FWQML_API void QmlEngine::loadModule(const ::boost::filesystem::path& path)
{
    m_engine->addImportPath(QString::fromStdString(path.parent_path().string()));
}

//-----------------------------------------------------------------------------

FWQML_API QList<QObject*> QmlEngine::getRootObjects()
{
    return m_engine->rootObjects();
}

//-----------------------------------------------------------------------------

}
