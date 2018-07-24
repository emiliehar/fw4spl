/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQml/IQmlRegistry.hpp"

namespace fwQml
{

IQmlRegistry::ClassList IQmlRegistry::m_classList;

IQmlRegistry::IQmlRegistry()
{
    m_classList.push_back(this);
}

IQmlRegistry::~IQmlRegistry()
{
}

//------------------------------------------------------------------------------

void IQmlRegistry::registarAllClasses()
{
    auto iterator = m_classList.begin();

    while (iterator != m_classList.end())
    {
        (*iterator)->registar();
        iterator = m_classList.erase(iterator);
    }
}

}
