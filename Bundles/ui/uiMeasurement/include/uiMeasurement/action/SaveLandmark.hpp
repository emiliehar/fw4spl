/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "uiMeasurement/config.hpp"

#include <fwGui/IActionSrv.hpp>

#include <boost/filesystem/path.hpp>

namespace uiMeasurement
{
namespace action
{

/**
 * @brief This action allows to save the landmarks stored in image.
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
    <service uid="..." type="::uiMeasurement::action::SaveLandmark" >
        <in key="image" uid="..." />
    </service>
   @endcode
 * @subsection Input Input
 * - \b image [::fwData::Image]: Image containig the landmarks field.
 *
 * @deprecated This service is deprecated and will be removed, use SWriter to write the landmarks
 */
class UIMEASUREMENT_CLASS_API SaveLandmark : public ::fwGui::IActionSrv
{
public:
    fwCoreServiceClassDefinitionsMacro( (SaveLandmark)( ::fwGui::IActionSrv) );

    UIMEASUREMENT_API SaveLandmark() noexcept;

    UIMEASUREMENT_API virtual ~SaveLandmark() noexcept;

protected:

    void configuring() override;

    void starting() override;

    void updating() override;

    void stopping() override;

    void info(std::ostream& _sstream ) override;

    void save(const ::boost::filesystem::path& path);

};

} // namespace action

} // namespace uiMeasurement
