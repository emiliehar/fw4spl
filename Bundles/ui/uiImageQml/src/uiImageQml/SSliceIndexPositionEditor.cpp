/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiImageQml/SSliceIndexPositionEditor.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwCore/base.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwQml/QmlRegistry.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

#include <functional>

namespace uiImageQml
{

static ::fwQml::QmlRegistry<SSliceIndexPositionEditor> registrar("uiImageQml", 1, 0, "SSliceIndexPositionEditor");

const std::string* SSliceIndexPositionEditor::SLICE_INDEX_FIELDID[ 3 ] =
{
    &fwDataTools::fieldHelper::Image::m_sagittalSliceIndexId,
    &fwDataTools::fieldHelper::Image::m_frontalSliceIndexId,
    &fwDataTools::fieldHelper::Image::m_axialSliceIndexId
};

static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_INDEX_SLOT = "updateSliceIndex";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_TYPE_SLOT  = "updateSliceType";

static const ::fwServices::IService::KeyType s_IMAGE_INOUT = "image";

//------------------------------------------------------------------------------

SSliceIndexPositionEditor::SSliceIndexPositionEditor() noexcept
{
    newSlot(s_UPDATE_SLICE_INDEX_SLOT, &SSliceIndexPositionEditor::updateSliceIndex, this);
    newSlot(s_UPDATE_SLICE_TYPE_SLOT, &SSliceIndexPositionEditor::updateSliceType, this);
}

//------------------------------------------------------------------------------

SSliceIndexPositionEditor::~SSliceIndexPositionEditor() noexcept
{
}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::starting()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);

    this->updateImageInfos(image);
    this->updateSliceTypeFromImg(m_orientation);

    this->updating();
}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::stopping()
{

}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::configuring()
{
    if( this->m_configuration->size() > 0 )
    {
        std::vector< ::fwRuntime::ConfigurationElement::sptr > slideIndexCfg = m_configuration->find("sliceIndex");
        SLM_ASSERT("Only one xml element \"sliceIndex\" is accepted.", slideIndexCfg.size() == 1 );
        SLM_ASSERT("The xml element \"sliceIndex\" is empty.", !(*slideIndexCfg.begin())->getValue().empty() );
        std::string orientation = (*slideIndexCfg.begin())->getValue();
        ::boost::algorithm::trim(orientation);
        ::boost::algorithm::to_lower(orientation);

        if(orientation == "axial" )
        {
            m_orientation = Z_AXIS;
        }
        else if(orientation == "frontal" )
        {
            m_orientation = Y_AXIS;
        }
        else if(orientation == "sagittal" )
        {
            m_orientation = X_AXIS;
        }
        else
        {
            SLM_FATAL("The value for the xml element \"sliceIndex\" can only be axial, frontal or sagittal.");
        }
    }
}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::updating()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);

    this->updateImageInfos(image);
    this->updateSliceIndexFromImg();
}

//-----------------------------------------------------------------------------

void SSliceIndexPositionEditor::updateSliceIndex(int axial, int frontal, int sagittal)
{
    m_axialIndex->value()    = axial;
    m_frontalIndex->value()  = frontal;
    m_sagittalIndex->value() = sagittal;

    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);

    image->setField( fwDataTools::fieldHelper::Image::m_axialSliceIndexId, m_axialIndex);
    image->setField( fwDataTools::fieldHelper::Image::m_frontalSliceIndexId, m_frontalIndex);
    image->setField( fwDataTools::fieldHelper::Image::m_sagittalSliceIndexId, m_sagittalIndex);
    this->updateSliceIndexFromImg();
}

//-----------------------------------------------------------------------------

void SSliceIndexPositionEditor::updateSliceType(int from, int to)
{
    if( to == static_cast< int > ( m_orientation ) )
    {
        m_orientation = static_cast< Orientation > ( from );
    }
    else if(from == static_cast<int>(m_orientation))
    {
        m_orientation = static_cast< Orientation >( to );
    }
    this->updateSliceTypeFromImg(m_orientation);
}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::info( std::ostream& _sstream )
{
}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::updateSliceIndexFromImg()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);

    if (::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity(image))
    {
        // Get Index
        std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
        OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getField( fieldID ) );
        int index = image->getField< ::fwData::Integer >( fieldID )->value();

        // Update QSlider
        int max = 0;
        if(image->getNumberOfDimensions() > m_orientation)
        {
            max = static_cast<int>(image->getSize()[m_orientation]-1);
        }
        this->setSliceRange( 0, max );
        this->setSliceValue( index );
    }
}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::updateSliceTypeFromImg(Orientation type )
{
    // Update Type Choice
    this->setSliceType(static_cast< int >( type ));

    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);

    this->updateSliceIndexFromImg();
}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::onSliceIndex(int index)
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);

    std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
    OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getField( fieldID ));
    image->getField< ::fwData::Integer >( fieldID )->value() = index;

    auto sig = image->signal< ::fwData::Image::SliceIndexModifiedSignalType >(
        ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG);
    ::fwCom::Connection::Blocker block(sig->getConnection(this->slot(s_UPDATE_SLICE_INDEX_SLOT)));
    sig->asyncEmit(m_axialIndex->value(), m_frontalIndex->value(), m_sagittalIndex->value());
}

//------------------------------------------------------------------------------

void SSliceIndexPositionEditor::onSliceType( int _type )
{
    Orientation type = static_cast< Orientation >( _type );
    OSLM_ASSERT("Bad slice type "<<type, type == X_AXIS ||
                type == Y_AXIS ||
                type == Z_AXIS );

    int oldType = static_cast< int > ( m_orientation );
    // Change slice type
    m_orientation = type;

    // Fire the signal
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);

    auto sig = image->signal< ::fwData::Image::SliceTypeModifiedSignalType >(
        ::fwData::Image::s_SLICE_TYPE_MODIFIED_SIG);
    {
        ::fwCom::Connection::Blocker block(sig->getConnection(this->slot(s_UPDATE_SLICE_TYPE_SLOT)));
        sig->asyncEmit(oldType, _type);
    }
    this->updateSliceIndexFromImg();
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsMap SSliceIndexPositionEditor::getAutoConnections() const
{
    KeyConnectionsMap connections;

    //FIXME hack to support deprecated getObject()
    if (this->getInOut< ::fwData::Image >(s_IMAGE_INOUT))
    {
        connections.push(s_IMAGE_INOUT, ::fwData::Image::s_MODIFIED_SIG, s_UPDATE_SLOT);
        connections.push(s_IMAGE_INOUT, ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG, s_UPDATE_SLICE_INDEX_SLOT);
        connections.push(s_IMAGE_INOUT, ::fwData::Image::s_SLICE_TYPE_MODIFIED_SIG, s_UPDATE_SLICE_TYPE_SLOT);
        connections.push(s_IMAGE_INOUT, ::fwData::Image::s_BUFFER_MODIFIED_SIG, s_UPDATE_SLOT);
    }

    return connections;
}

//------------------------------------------------------------------------------

}
