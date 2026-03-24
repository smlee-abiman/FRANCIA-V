TEMPLATE = subdirs
SUBDIRS += \
    pendant-gui.pro

!isEmpty(BUILD_TESTS): SUBDIRS += tests
!isEmpty(BUILD_TESTS): SUBDIRS += tests_model/UnitTest.pro
