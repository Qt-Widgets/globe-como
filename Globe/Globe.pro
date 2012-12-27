
TEMPLATE = app
TARGET = Globe
DESTDIR = ..
QT += core gui network
CONFIG += console

RESOURCES = resources.qrc

HEADERS += application_cfg.hpp \
           channel_attributes.hpp \
           channels.hpp \
           channels_cfg.hpp \
           channels_list.hpp \
           channel_timeout_widget.hpp \
           channels_to_show.hpp \
           channel_widget.hpp \
           condition.hpp \
           condition_cfg.hpp \
           condition_widget.hpp \
           configuration.hpp \
           db.hpp \
           mainwindow.hpp \
           mainwindow_cfg.hpp \
           otherwise_condition_widget.hpp \
           properties.hpp \
           properties_cfg.hpp \
           properties_cfg_filename_dialog.hpp \
           properties_dialog.hpp \
           properties_key_type_dialog.hpp \
           properties_model.hpp \
           properties_widget.hpp \
           source_manual_dialog.hpp \
           sources.hpp \
           sources_dialog.hpp \
           sources_mainwindow.hpp \
           sources_mainwindow_cfg.hpp \
           sources_model.hpp \
           sources_widget.hpp \
           window_state_cfg.hpp \
           tool_window_object.hpp \
           utils.hpp

SOURCES += application_cfg.cpp \
           channel_attributes.cpp \
           channels.cpp \
           channels_cfg.cpp \
           channels_list.cpp \
           channel_timeout_widget.cpp \
           channels_to_show.cpp \
           channel_widget.cpp \
           condition.cpp \
           condition_cfg.cpp \
           condition_widget.cpp \
           configuration.cpp \
           db.cpp \
           mainwindow.cpp \
           mainwindow_cfg.cpp \
           main.cpp \
           otherwise_condition_widget.cpp \
           properties.cpp \
           properties_cfg.cpp \
           properties_cfg_filename_dialog.cpp \
           properties_dialog.cpp \
           properties_key_type_dialog.cpp \
           properties_model.cpp \
           properties_widget.cpp \
           source_manual_dialog.cpp \
           sources.cpp \
           sources_dialog.cpp \
           sources_mainwindow.cpp \
           sources_mainwindow_cfg.cpp \
           sources_model.cpp \
           sources_widget.cpp \
           window_state_cfg.cpp \
           tool_window_object.cpp \
           utils.cpp

FORMS	 = channels_to_show.ui \
           channel_attributes.ui \
           condition_widget.ui \
           otherwise_condition_widget.ui \
           properties_mainwindow.ui \
           properties_cfg_filename_dialog.ui \
           properties_key_type_dialog.ui \
           source_manual_dialog.ui \
           sources_widget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib -lQtConfFile -lComo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib -lQtConfFile -lComo
else:symbian: LIBS += -lQtConfFile -lComo
else:unix: LIBS += -L$$OUT_PWD/../lib -lQtConfFile -lComo

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/QtConfFile.lib $$OUT_PWD/../lib/Como.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/QtConfFile.lib $$OUT_PWD/../lib/Como.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../lib/libQtConfFile.a $$OUT_PWD/../lib/libComo.a
