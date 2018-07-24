/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

# include "fwVTKQml/config.hpp"

#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_2_0>
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
 * @brief The vtkInternalOpenGLRenderWindow class
 *
 * This class is used to render a offscreen VTK window in a FrameBuffer
 */
class FWVTKQML_CLASS_API vtkInternalOpenGLRenderWindow : public vtkExternalOpenGLRenderWindow,
                                                         protected QOpenGLFunctions_2_0
{
public:
    friend class FrameBufferRenderer;

    static vtkInternalOpenGLRenderWindow* New();
    vtkTypeMacro(vtkInternalOpenGLRenderWindow, vtkGenericOpenGLRenderWindow);

protected:
    FWVTKQML_API vtkInternalOpenGLRenderWindow();
    FWVTKQML_API ~vtkInternalOpenGLRenderWindow();

public:
    /**
     *  @brief: initialize openGL context/functions
     */
    virtual FWVTKQML_API void OpenGLInitState() override;

    virtual FWVTKQML_API void OpenGLEndState();

    /**
     * @brief: Call render method of the window
     */
    virtual FWVTKQML_API void Render() override;
    /**
     *  @brief: render into the framebuffer
     */
    FWVTKQML_API void  internalRender();
    /**
     *  @brief: set frame buffer to the window
     */
    FWVTKQML_API void setFrameBufferObject(QOpenGLFramebufferObject*);
    /**
     * @brief: configure renderer
     */
    FWVTKQML_API void setRenderer(FrameBufferRenderer*);
    /**
     * @brief: return renderer
     */
    FWVTKQML_API FrameBufferRenderer* getRenderer() const;

private:
    FrameBufferRenderer* m_qtParentRenderer;
};

} //namespace fwVTKQml
