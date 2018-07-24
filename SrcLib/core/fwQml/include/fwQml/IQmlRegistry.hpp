/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwQml/config.hpp"

#include <vector>

namespace fwQml
{
class FWQML_CLASS_API IQmlRegistry
{
public:
    using ClassList = std::vector<IQmlRegistry*>;

    FWQML_API IQmlRegistry();
    FWQML_API ~IQmlRegistry();

    static FWQML_API void registarAllClasses();

protected:
    virtual FWQML_API void registar() const = 0;

private:
    static FWQML_API ClassList m_classList;
};

} // fwQml
