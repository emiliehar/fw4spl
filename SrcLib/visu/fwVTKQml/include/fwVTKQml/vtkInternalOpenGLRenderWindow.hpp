/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

# include "fwVTKQml/config.hpp"

#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_3_1>
#include <QQuickFramebufferObject>
#include <vtkCamera.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkObjectFactory.h>
#include <vtkRendererCollection.h>

namespace fwVTKQml
{

class FrameBufferRenderer;

/**
 * @brief This class is used to render a offscreen VTK window in a FrameBuffer
 */
class FWVTKQML_CLASS_API vtkInternalOpenGLRenderWindow : public vtkExternalOpenGLRenderWindow,
                                                         protected QOpenGLFunctions_3_1
{
public:
    friend class FrameBufferRenderer;

    static vtkInternalOpenGLRenderWindow* New();
    vtkTypeMacro(vtkInternalOpenGLRenderWindow, vtkExternalOpenGLRenderWindow);

protected:
    /// Constructor
    FWVTKQML_API vtkInternalOpenGLRenderWindow();
    /// Destructor
    FWVTKQML_API ~vtkInternalOpenGLRenderWindow();

public:
    /**
     *  @brief: initialize openGL context/functions
     */
    virtual FWVTKQML_API void OpenGLInitState() override;

    virtual FWVTKQML_API void OpenGLEndState();

    /// Call render method of the renderer
    virtual FWVTKQML_API void Render() override;

    /// Render into the framebuffer
    FWVTKQML_API void  internalRender();

    /// Set frame buffer to the window
    FWVTKQML_API void setFrameBufferObject(QOpenGLFramebufferObject*);

    /// Set the renderer
    FWVTKQML_API void setRenderer(FrameBufferRenderer*);

    /// Return the renderer
    FWVTKQML_API FrameBufferRenderer* getRenderer() const;

private:

    /// Frame renderer
    FrameBufferRenderer* m_qtParentRenderer;
};

} //namespace fwVTKQml
