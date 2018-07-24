/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwVTKQml/config.hpp"
#include "fwVTKQml/vtkInternalOpenGLRenderWindow.hpp"

#include <vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

#include <QVTKInteractorAdapter.h>

#include <QtQuick/QQuickFramebufferObject>
// Use the OpenGL API abstraction from Qt instead of from VTK because vtkgl.h
// and other Qt OpenGL-related headers do not play nice when included in the
// same compilation unit
#include <QOpenGLFunctions>
#include <qqmlapplicationengine.h>
#include <QPointer>
#include <QMutex>

namespace fwVTKQml
{

class FrameBufferItem;

class FWVTKQML_CLASS_API FrameBufferRenderer : public QObject,
                                               public QQuickFramebufferObject::Renderer
{
Q_OBJECT
public:
    FWVTKQML_API FrameBufferRenderer(vtkInternalOpenGLRenderWindow*, FrameBufferItem*);
    FWVTKQML_API ~FrameBufferRenderer();
    /**
     * @brief createFramebufferObject: initialize a framebuffer
     * @param size: size of the framebuffer
     * @return QOpenGLFrameBufferObject: View where will be perform openGL command (VTK)
     */
    QOpenGLFramebufferObject FWVTKQML_API* createFramebufferObject(const QSize& size);

    /**
     * @brief render: perform open GL command
     */
    void FWVTKQML_API   render();

    /**
     * @brief: synchronize m_item when frame is ready
     */
    virtual void FWVTKQML_API synchronize(QQuickFramebufferObject*);

    FrameBufferItem const* getItem() const;

Q_SIGNALS:
    /**
     * @brief ready
     * Emit when frame is ready
     */
    void    ready();

private:
    vtkInternalOpenGLRenderWindow* m_vtkRenderWindow;
    QOpenGLFramebufferObject* m_framebufferObject;
    FrameBufferItem* m_item;
    bool m_readyToRender;

    friend class vtkInternalOpenGLRenderWindow;
};

class FWVTKQML_CLASS_API FrameBufferItem : public QQuickFramebufferObject
{
Q_OBJECT

public:
    FWVTKQML_API FrameBufferItem(QQuickItem* parent = nullptr);
    FWVTKQML_API ~FrameBufferItem();

    /**
     *  @brief: create a FrameBufferRenderer instance
     */
    FWVTKQML_API QQuickFramebufferObject::Renderer* createRenderer() const override;
    /**
     *  @brief: return m_win
     */
    FWVTKQML_API vtkInternalOpenGLRenderWindow* getRenderWindow() const;

    /**
     * @brief getRenderer
     * @return current renderer
     */
    FWVTKQML_API vtkSmartPointer<vtkRenderer>    getRenderer() const;

    FWVTKQML_API void    lockRenderer();
    FWVTKQML_API void    unlockRenderer();

public Q_SLOTS:
    /**
     *  @brief: initialize variables
     */
    FWVTKQML_API void  initialize();

protected:
    /**
     * Event management
     * Theses events are transmit to VTK by QVTKAdapter
     * @{
     */
    FWVTKQML_API bool event(QEvent* evt) Q_DECL_OVERRIDE;

    FWVTKQML_API void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    FWVTKQML_API void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    FWVTKQML_API void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    FWVTKQML_API void mouseDoubleClickEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    /**
     * }@
     */

Q_SIGNALS:
    /**
     * @brief ready: emit when FrameBufferItem is ready
     */
    void    ready();

private:
    vtkSmartPointer<vtkInternalOpenGLRenderWindow> m_win;
    QPointer<QVTKInteractorAdapter> m_interactorAdapter;
    vtkSmartPointer<vtkRenderer>    m_renderer;
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
    QMutex m_viewLock;

    friend class FrameBufferRenderer;
};

} // fwVTKQml
