/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __SCENE2D_ADAPTOR_VIEWPORTUPDATER_HPP__
#define __SCENE2D_ADAPTOR_VIEWPORTUPDATER_HPP__

#include "scene2D/adaptor/IAdaptor.hpp"

namespace scene2D
{
namespace adaptor
{


/**
 * @brief Adaptor implementation that manages the camera on the view (by updating the viewport object).
 *
 * The ViewportUpdater updates the viewport of the scene it is attached to, by picking values from the
 * managed ::scene2D::data::Viewport object.
 *
 * Configuration example:
   @code{.xml}
   <adaptor id="viewportRangeUpdater" class="::scene2D::adaptor::ViewportUpdater" objectId="myViewport">
       <config xAxis="xAxis" yAxis="yAxis" zValue="7" />
   </adaptor>
   @endcode
 *
 * \b objectID  : the viewport object this updater takes values from.
 *
 * \b xAxis     : see ::scene2D::adaptor::IAdaptor
 *
 * \b yAxis     : see ::scene2D::adaptor::IAdaptor
 *
 * \b zValue    : see ::scene2D::adaptor::IAdaptor
 *
 * This adaptor is intended to be used with a ::scene2D::adaptor::ViewportRangeSelector adaptor.
 */
class ViewportUpdater : public ::scene2D::adaptor::IAdaptor
{

public:
    fwCoreServiceClassDefinitionsMacro ( (ViewportUpdater)(::scene2D::adaptor::IAdaptor) );
    ViewportUpdater() noexcept;
    ~ViewportUpdater() noexcept;

protected:
    SCENE2D_API void configuring();
    SCENE2D_API void doStart();
    SCENE2D_API void doUpdate();
    SCENE2D_API void doSwap();
    SCENE2D_API void doStop();

    SCENE2D_API void processInteraction( ::scene2D::data::Event::sptr _event );
};


}   // namespace adaptor
}   // namespace scene2D

#endif  // __SCENE2D_ADAPTOR_VIEWPORTUPDATER_HPP__

