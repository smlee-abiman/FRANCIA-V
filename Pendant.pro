TEMPLATE = subdirs

SUBDIRS += \
    CoreLibraryProject \
    CartesianModel

CONFIG += ordered
Phase2.depends = CoreLibraryProject
