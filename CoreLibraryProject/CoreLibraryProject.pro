TEMPLATE = subdirs

contains(QT_ARCH, arm.*) {
    SUBDIRS += \
        ExternalModules/qtftp \
        CoreLib \
        SampleApp \
        SystemSettingApp

    CoreLib.depends = ExternalModules/qtftp
    SampleApp.depends = CoreLib
    SystemSettingApp.depends = CoreLib
} else {
    win32 {
        SUBDIRS += \
            ExternalModules/qtftp \
            CoreLib \
            SampleApp \
            testApp/ModBus/ModBusTestApp \
            SystemSettingApp

        CoreLib.depends = ExternalModules/qtftp
        SampleApp.depends = CoreLib
        ModBusTestApp.depends = CoreLib
        SystemSettingApp.depends = CoreLib
    } else:win32 {
        SUBDIRS += \
            ExternalModules/qtftp \
            CoreLib \
            SampleApp \
            testApp/ModBus/ModBusTestApp \
            SystemSettingApp

        CoreLib.depends = ExternalModules/qtftp
        SampleApp.depends = CoreLib
        ModBusTestApp.depends = CoreLib
        SystemSettingApp.depends = CoreLib
    } else:unix {
        SUBDIRS += \
            ExternalModules/qtftp \
            CoreLib \
            CoreLibUnitTest \
            SampleApp \
            testApp/ModBus/ModBusTestApp \
            SystemSettingApp

        CoreLib.depends = ExternalModules/qtftp
        CoreLibUnitTest.depends = CoreLib
        SampleApp.depends = CoreLib
        ModBusTestApp.depends = CoreLib
        SystemSettingApp.depends = CoreLib
    }
}
CONFIG += ordered

SUBDIRS += \
    WatchDogTimerAlive
