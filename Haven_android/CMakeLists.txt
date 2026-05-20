cmake_minimum_required(VERSION 3.16)

project(Haven_android VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Haven_android
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ai1.h ai1.cpp ai1.ui
        ui2.h ui2.cpp ui2.ui
        ui3.h ui3.cpp ui3.ui
        ui4.h ui4.cpp ui4.ui
        ui5.h ui5.cpp ui5.ui
        ui6.h ui6.cpp ui6.ui
        termes.h termes.cpp termes.ui
        android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml
        sources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Haven_android APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Haven_android SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Haven_android
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Haven_android PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network)

if(ANDROID)
    # 1. مسار مجلد include لم يتغير
    set(OPENSSL_INCLUDE_PATH "C:/Qt/android_openssl-master/include")

    # 2. المسار الأساسي لمجلدات المكتبات (ssl_3)
    set(OPENSSL_LIBS_BASE_PATH "C:/Qt/android_openssl-master/ssl_3")

    # 3. إضافة مسار الـ headers
    target_include_directories(Haven_android PRIVATE ${OPENSSL_INCLUDE_PATH})

    # 4. **الحل النهائي:** نربط مباشرة بالمسار الكامل للملفات الحقيقية
    # هذا يخبر CMake بتجاهل الملفات الصغيرة واستخدام الملفات الكبيرة مباشرة
    target_link_libraries(Haven_android PRIVATE
        "${OPENSSL_LIBS_BASE_PATH}/${CMAKE_ANDROID_ARCH_ABI}/libssl_3.so"
        "${OPENSSL_LIBS_BASE_PATH}/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_3.so"
    )

    # رسالة للتأكيد
    message(STATUS "OpenSSL libraries explicitly linked for ABI: ${CMAKE_ANDROID_ARCH_ABI}")
endif()
# --- END: The Real Final Corrected OpenSSL section ---


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Haven_android)
endif()
set_target_properties(Haven_android PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Haven_android
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Haven_android)
endif()

if(ANDROID)
    # تأكد من أن هذا المسار هو نفس المسار الذي استخدمته لربط المكتبات
    set(OPENSSL_LIBS_DEST_PATH "C:/Qt/android_openssl-master/ssl_3")

    # قائمة الملفات التي يجب نسخها (ملفات .so)
    set(OPENSSL_DEPLOY_LIBS
        "${OPENSSL_LIBS_DEST_PATH}/${CMAKE_ANDROID_ARCH_ABI}/libssl_3.so"
        "${OPENSSL_LIBS_DEST_PATH}/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_3.so"
    )

    # نسخ المكتبات إلى مجلد lib الخاص بالتطبيق على الأندرويد
    # Qt Creator سيقوم بجمع هذه الملفات ووضعها في المكان الصحيح على الجهاز
    foreach(lib ${OPENSSL_DEPLOY_LIBS})
        get_filename_component(lib_name ${lib} NAME)
        add_custom_command(TARGET Haven_android POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${lib}
            "${CMAKE_CURRENT_BINARY_DIR}/android-build/libs/${CMAKE_ANDROID_ARCH_ABI}/${lib_name}"
            COMMENT "Copying OpenSSL library ${lib_name} for deployment"
        )
    endforeach()
endif()
# --- END: Added for OpenSSL deployment ---
