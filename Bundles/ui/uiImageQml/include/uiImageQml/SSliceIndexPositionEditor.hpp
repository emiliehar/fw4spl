/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "uiImageQml/config.hpp"

#include <fwDataTools/helper/MedicalImageAdaptor.hpp>

#include <fwQml/IQmlEditor.hpp>

#include <fwTools/Failed.hpp>

#include <QObject>

namespace uiImageQml
{

/**
 * @brief   SSliceIndexPositionEditor service allows to change the slice index of an image.
 *
 * This is represented by
 *  - a slider to select the slice index
 *  - a choice list to select the slice orientation (axial, frontal, sagittal)
 *
 * @section Slots Slots
 * - \b updateSliceIndex(int, int, int): update image slice index
 * - \b updateSliceType(int, int): update image slice type
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service uid="..." type="::uiImageQml::SSliceIndexPositionEditor" autoConnect="yes">
      <inout key="image" uid="..."/>
      <sliceIndex>${orientationValue}</sliceIndex>
   </service>
   @endcode
 * @subsection In-Out In-Out
 * - \b image [::fwData::Image]: image on which the slice index will be changed
 *
 * @subsection Configuration Configuration
 * - \b sliceIndex : Axis on which the index will be changed, must be "axial", "frontal" or "sagittal".
 */
class UIIMAGEQML_CLASS_API SSliceIndexPositionEditor : public ::fwQml::IQmlEditor,
                                                      public ::fwDataTools::helper::MedicalImageAdaptor
{
Q_OBJECT
Q_PROPERTY(int sliceIndex MEMBER m_sliceIndex)
public:

    fwCoreServiceClassDefinitionsMacro( (SSliceIndexPositionEditor)(::fwQml::IQmlEditor) );

    /// Constructor. Do nothing.
    UIIMAGEQML_API SSliceIndexPositionEditor() noexcept;

    /// Destructor. Do nothing.
    UIIMAGEQML_API virtual ~SSliceIndexPositionEditor() noexcept;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Image::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     * Connect Image::s_SLICE_INDEX_MODIFIED_SIG to this::s_UPDATE_SLICE_INDEX_SLOT
     * Connect Image::s_SLICE_TYPE_MODIFIED_SIG to this::s_UPDATE_SLICE_TYPE_SLOT
     * Connect Image::s_BUFFER_MODIFIED_SIG to this::s_UPDATE_BUFFER_SLOT
     */
    UIIMAGEQML_API virtual KeyConnectionsType getObjSrvConnections() const override;

Q_SIGNALS:
    void setSliceRange(int min, int max);
    void setSliceValue(int value);
    void setSliceType(int type);

public Q_SLOTS:
    /// This method is called when the slider is move. Notify the slice index is modified.
    UIIMAGEQML_API void onSliceIndex(int index);

    /// This method is called when the slice type selected change. Notify the slice type is modified.
    UIIMAGEQML_API void onSliceType( int type );

protected:

    /// @brief The slice type: axial, frontal, sagittal.
    using ::fwDataTools::helper::MedicalImageAdaptor::Orientation;

    /**
     * @brief Install the layout.
     */
    virtual void starting() override;

    /**
     * @brief Destroy the layout.
     */
    virtual void stopping() override;

    /// Update editor information from the image
    virtual void updating() override;

    /**
     * @brief Configure the editor.
     */
    virtual void configuring() override;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Image::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     * Connect Image::s_SLICE_INDEX_MODIFIED_SIG to this::s_UPDATE_SLICE_INDEX_SLOT
     * Connect Image::s_SLICE_TYPE_MODIFIED_SIG to this::s_UPDATE_SLICE_TYPE_SLOT
     * Connect Image::s_BUFFER_MODIFIED_SIG to this::s_UPDATE_BUFFER_SLOT
     */
    UIIMAGEQML_API virtual KeyConnectionsMap getAutoConnections() const override;

    /// Overrides
    UIIMAGEQML_API virtual void info( std::ostream& _sstream ) override;

    /// Update the editor slider from the image slice index.
    UIIMAGEQML_API void updateSliceIndexFromImg();

    /// Update the editor slice type choice from the image slice type.
    UIIMAGEQML_API void updateSliceTypeFromImg(Orientation type );

private:

    /**
     * @name Slots
     * @{
     */
    /// Slot: update image slice index
    void updateSliceIndex(int axial, int frontal, int sagittal);

    /// Slot: update image slice type
    void updateSliceType(int from, int to);
    /**
     * @}
     */

    /// @brief The field IDs for the slice index.
    static const std::string* SLICE_INDEX_FIELDID[ 3 ];

    int m_sliceIndex;

};

} // uiImageQml
