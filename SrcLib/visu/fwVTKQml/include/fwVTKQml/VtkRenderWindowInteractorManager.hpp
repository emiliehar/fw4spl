/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwVTKQml/config.hpp"
#include "fwVTKQml/FrameBufferItem.hpp"

#include <fwRenderVTK/IVtkRenderWindowInteractorManager.hpp>

#include <QPointer>

class vtkRenderWindowInteractor;

namespace fwGui
{
namespace container
{
class fwContainer;
}
}

namespace fwVTKQml
{

/**
 * @brief   Defines a class to manage vtkRenderWindowInteractor in a window.
 */
class FWVTKQML_CLASS_API VtkRenderWindowInteractorManager : public ::fwRenderVTK::IVtkRenderWindowInteractorManager
{

public:

    fwCoreNonInstanciableClassDefinitionsMacro( (VtkRenderWindowInteractorManager)
                                                (::fwRenderVTK::IVtkRenderWindowInteractorManager) )

    FWVTKQML_API VtkRenderWindowInteractorManager(::fwRenderVTK::IVtkRenderWindowInteractorManager::Key key);

    FWVTKQML_API virtual ~VtkRenderWindowInteractorManager();

    /// Creates an interactor and installs it in window.
    FWVTKQML_API virtual void installInteractor( ::fwGui::container::fwContainer::sptr _parent ) override;

    /// Deletes interactor and manage correctly the window (removing layout).
    FWVTKQML_API virtual void uninstallInteractor() override;

    /// Return a pointer on interactor
    FWVTKQML_API virtual ::vtkRenderWindowInteractor* getInteractor() override;

    FWVTKQML_API void setFrameBuffer(QPointer< ::fwVTKQml::FrameBufferItem > frameBuffer );

private:

    QPointer< ::fwVTKQml::FrameBufferItem > m_frameBufferItem;

};

} // namespace fwVTKQml
