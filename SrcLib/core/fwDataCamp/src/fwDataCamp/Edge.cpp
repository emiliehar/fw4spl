/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwData/Edge.hpp>

#include <fwCamp/UserObject.hpp>

fwCampImplementDataMacro((fwData)(Edge))
{
    builder
    .tag("object_version", "1")
    .tag("lib_name", "fwData")
    .base< ::fwData::Object>()
    .property("nature", &::fwData::Edge::m_nature)
    .property("to_identifier", &::fwData::Edge::m_toPortIdentifier)
    .property("from_identifier", &::fwData::Edge::m_fromPortIdentifier)
    ;
}
