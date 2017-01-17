/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWATOMS_FACTORY_NEW_HPP__
#define __FWATOMS_FACTORY_NEW_HPP__

#include "fwAtoms/config.hpp"
#include "fwAtoms/registry/detail.hpp"

#include <string>

namespace fwAtoms
{

class Base;

namespace factory
{

template<class CLASSNAME > SPTR( CLASSNAME )  New();

/**
 * @brief Key class used to restrict access to Object construction.
 * See http://www.drdobbs.com/184402053
 */
class Key
{
template<typename CLASSNAME>
friend SPTR( CLASSNAME ) fwAtoms::factory::New();

Key()
{
}
};

FWATOMS_API SPTR( ::fwAtoms::Base ) New( const ::fwAtoms::registry::KeyType & classname );

template<class CLASSNAME > SPTR( CLASSNAME )  New()
{
    SPTR(CLASSNAME) obj = std::make_shared< CLASSNAME >( Key() );
    return obj;
}

} // namespace factory

} // namespace fwAtoms

#endif /* __FWATOMS_FACTORY_NEW_HPP__ */

