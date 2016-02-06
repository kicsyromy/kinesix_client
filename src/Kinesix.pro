QT       += core gui widgets dbus network

CONFIG += c++11

TARGET = Kinesix
TEMPLATE = app

CONFIG(debug, debug|release) {
    DEFINES += DEBUG_BUILD
}
CONFIG(release, debug|release) {
    DEFINES += RELEASE_BUILD
}

CONFIG(x11_input_emulation) {
    DEFINES += X11_INPUT_EMULATION
    QT += x11extras
    LIBS += -lX11 -lXtst
} else {
    DEFINES += UINPUT_INPUT_EMULATION
}

CONFIG += c++11
INCLUDEPATH += \
    3rdparty \
    ui \
    input

SOURCES += \
    main.cpp \
    device.cpp \
    kinesixd_proxy.cpp \
    configuration_client.cpp \
    input/input_handler.cpp \
    input/uinput_delegate.cpp \
    input/x11_delegate.cpp \
    ui/gesture_list_delegate.cpp \
    ui/gesture_list_model.cpp \
    ui/main_window.cpp \
    ui/valid_device_list_model.cpp \
    ui/device_list_delegate.cpp \
    3rdparty/single_application.cpp

HEADERS  += \
    device.h \
    gesture.h \
    logger.h \
    input/x11delegate.h \
    kinesixd_proxy.h \
    action_base.h \
    configuration_client.h \
    input/input_handler.h \
    input/key_combination.h \
    input/uinput_delegate.h \
    ui/gesture_config_panel.h \
    ui/gesture_list_delegate.h \
    ui/gesture_list_model.h \
    ui/main_window.h \
    ui/main_window_layout.h \
    ui/active_device_selection_dialog.h \
    ui/valid_device_list_model.h \
    ui/device_list_delegate.h \
    3rdparty/single_application.h \
    ui/status_notifier_layout.h

RESOURCES += \
    ui/resources.qrc

DISTFILES +=
