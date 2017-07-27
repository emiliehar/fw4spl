/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWRENDERVTK_VTK_FWHANDLEREPRESENTATION3D_HPP__
#define __FWRENDERVTK_VTK_FWHANDLEREPRESENTATION3D_HPP__

#include "fwRenderVTK/config.hpp"

#include <vtkPolygonalHandleRepresentation3D.h>
#include <vtkSmartPointer.h>

class vtkCubeSource;
class vtkSphereSource;
class vtkCleanPolyData;
class vtkPolyDataAlgorithm;

namespace fwRenderVTK
{

namespace vtk
{

/**
 * @brief 3D Representation to be used with vtkHandleWidget (represents a cube or a sphere).
 */
class FWRENDERVTK_CLASS_API fwHandleRepresentation3D : public vtkPolygonalHandleRepresentation3D
{
public:

    enum Shape
    {
        SPHERE,
        CUBE
    };

    // Description:
    // Instantiate this class.
    FWRENDERVTK_API static fwHandleRepresentation3D* New();

    // Description:
    // Standard vtk methods
    vtkTypeMacro(fwHandleRepresentation3D,
                 vtkPolygonalHandleRepresentation3D);
    FWRENDERVTK_API void PrintSelf(ostream& os, vtkIndent indent) override;

    // Description:
    // Set the position of the point in world and display coordinates.
    FWRENDERVTK_API virtual void SetWorldPosition(double p[3]) override;

    // Description:
    // Set/Get the radius factor of the marker (must be > 1 to be visible)
    FWRENDERVTK_API vtkSetMacro( MarkerRadiusFactor, double );
    FWRENDERVTK_API vtkGetMacro( MarkerRadiusFactor, double );

    // Description:
    // Define the shape of the representation (SPHERE or CUBE)
    FWRENDERVTK_API void SetShapeRepresentation( Shape shape);
    FWRENDERVTK_API vtkGetMacro( ShapeRepresentation, Shape );

    // Description:
    // Set/Get the handle properties for marker.
    FWRENDERVTK_API void SetMarkerProperty(vtkProperty*);
    FWRENDERVTK_API vtkGetObjectMacro(MarkerProperty, vtkProperty);

    // Description:
    // For some exporters and other other operations we must be
    // able to collect all the actors or volumes. These methods
    // are used in that process.
    FWRENDERVTK_API virtual void GetActors(vtkPropCollection*) override;

    // Description:
    // Release any graphics resources that are being consumed by this actor.
    // The parameter window could be used to determine which graphic
    // resources to release.
    FWRENDERVTK_API virtual void ReleaseGraphicsResources(vtkWindow*) override;

    // Description:
    // Support the standard render methods.
    FWRENDERVTK_API virtual int RenderOpaqueGeometry(vtkViewport* viewport) override;
    FWRENDERVTK_API virtual int RenderTranslucentPolygonalGeometry(vtkViewport* viewport) override;

    // Description:
    // Does this prop have some translucent polygonal geometry?
    FWRENDERVTK_API virtual int HasTranslucentPolygonalGeometry() override;

protected:
    fwHandleRepresentation3D();
    ~fwHandleRepresentation3D();

    // Description:
    // Recomputes the handle world size based on the set display size.
    virtual void BuildRepresentation() override;

    // Create default properties for markers
    void CreateDefaultProperties();

    vtkSmartPointer<vtkCubeSource> CubeSource;
    vtkSmartPointer<vtkSphereSource> SphereSource;
    Shape ShapeRepresentation;

    // the cursor3D
    vtkSmartPointer<vtkFollower>          Follower;
    vtkSmartPointer<vtkCleanPolyData>     CleanPolyData;
    vtkSmartPointer<vtkPolyDataMapper>    MarkerMapper;
    vtkSmartPointer<vtkPolyDataAlgorithm> Marker;

    double MarkerRadiusFactor;

    // Properties used to control the appearance of selected objects and
    // the manipulator in general.
    vtkProperty* MarkerProperty;

private:
    fwHandleRepresentation3D(const fwHandleRepresentation3D&); //Not implemented
    void operator=(const fwHandleRepresentation3D&); //Not implemented
};

} // namespace vtk

} // namespace fwRenderVTK

#endif //__FWRENDERVTK_VTK_FWHANDLEREPRESENTATION3D_HPP__

