/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwTools/Failed.hpp"

namespace fwTools
{

Failed::Failed(const std::string &message) noexcept
    : ::fwCore::Exception(message)
{
}


Failed::~Failed() noexcept
{
}



} // namespace fwTools
