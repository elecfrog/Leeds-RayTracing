cmake_minimum_required(VERSION 3.25)
project(RaytraceRenderWindow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C://Qt//6.5.1//msvc2019_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGLWidgets
        REQUIRED)

find_package(OpenGL REQUIRED)



add_executable(RaytraceRenderWindow
        # Input
        src/ArcBall/ArcBall.h
        src/ArcBall/ArcBallWidget.h
        src/Math/Vector4.h
        src/Render/Light.h
        src/Render/Material.h
        src/Math/Matrix4.h
        src/Math/Quaternion.h
        src/Math/Ray.h
        src/RayTrace/RaytraceRenderWidget.h
        src/RenderController.h
        src/RenderParameters.h
        src/OpenGL/RenderWidget.h
        src/RenderWindow.h
        src/Render/RGBAImage.h
        src/Render/RGBAValue.h
        src/Render/Scene.h
        src/Render/ThreeDModel.h
        src/Render/Triangle.h

        src/ArcBall/ArcBall.cpp
        src/ArcBall/ArcBallWidget.cpp
        src/Math/Vector3.cpp
        src/Math/Vector4.cpp
        src/Render/Light.cpp
        src/Render/Material.cpp
        src/Math/Ray.cpp
        src/Renderparameters.cpp
        src/Render/Scene.cpp
        src/Render/ThreeDModel.cpp
        src/Render/Triangle.cpp
        src/main.cpp
        src/Math/Matrix4.cpp
        src/Math/Quaternion.cpp
        src/RayTrace/RaytraceRenderWidget.cpp
        src/RenderController.cpp
        src/OpenGL/RenderWidget.cpp
        src/RenderWindow.cpp
        src/Render/RGBAImage.cpp
        src/Render/RGBAValue.cpp
)

target_link_libraries(RaytraceRenderWindow
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGLWidgets

        OpenGL::GL
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets Sql)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
