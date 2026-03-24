#ifndef PLATFORMHELPERS_H
#define PLATFORMHELPERS_H

#include <QObject>

/// Namespace object to host platform helper functions.
class PlatformHelpers : public QObject
{
    Q_OBJECT

public:
    explicit PlatformHelpers(QObject *parent = nullptr);

    Q_INVOKABLE void notifyGuiReady();
};

#endif // PLATFORMHELPERS_H
