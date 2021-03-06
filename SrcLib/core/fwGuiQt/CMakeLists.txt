fwLoadProperties()

if(WIN32)
    set(FW_QT5_PLUGINS_PATH "bin/qt5/plugins")
else()
    set(FW_QT5_PLUGINS_PATH "lib/qt5/plugins")
endif()

configure_file("${CMAKE_CURRENT_LIST_DIR}/bin/qt.conf" "${CMAKE_BINARY_DIR}/bin/qt.conf")

target_compile_definitions(${FWPROJECT_NAME} PUBLIC "-DQT_NO_KEYWORDS")

find_package(Qt5 QUIET COMPONENTS Core Gui Widgets Concurrent REQUIRED)

fwForwardInclude(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Concurrent_INCLUDE_DIRS}
)

fwLink(
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
)

set_target_properties(${FWPROJECT_NAME} PROPERTIES AUTOMOC TRUE)
