#ifndef AUTOVIEWMODEL_H
#define AUTOVIEWMODEL_H

#include "viewmodelbase.h"

class AutoViewModel : public ViewModelBase
{
    Q_OBJECT
public:
    explicit AutoViewModel(ResourceManager *resource, CommunicationThread * comm);

signals:

};

#endif // AUTOVIEWMODEL_H
