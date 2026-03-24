import QtQuick 2.12
import QtQml 2.12

Item {
    id: root
    property var loadComponentList: []
    property int interval: 10
    property bool guard: false
    property bool loadCompleted: false

    function unloadAllComponents()
    {
        for (let i = 0; i < loadComponentList.length; i++) {
            loadComponentList[i].active = false
        }
        loadCompleted = false
    }

    function loadComponent(index)
    {
        if (index < loadComponentList.length) {
            loadComponentList[index].active = true
        }
    }

    function loadNextComponent()
    {
        loadTimer.start()
    }

    function doLoadComponent()
    {
        if (guard)
            return

        let loadIndex = -1
        for (let i = 0; i < loadComponentList.length; i++) {
            let loadComponent = loadComponentList[i]
            if (loadComponent.status === Loader.Loading) {
                loadIndex = -1
                break
            } else if (loadComponent.status === Loader.Null) {
                if (loadIndex === -1) {
                    loadIndex = i
                }
            }
        }
        if ((loadIndex >= 0) && (loadIndex < loadComponentList.length)) {
            loadComponentList[loadIndex].active = true
        }
    }

    function isLoadCompleted()
    {
        let ret = true
        for (let i = 0; i < loadComponentList.length; i++) {
            let component = loadComponentList[i]
            if (component.status !== Loader.Ready) {
                ret = false
                break
            }
        }
        return ret
    }

    Timer {
        id: loadTimer
        interval: root.interval
        running: false
        repeat: false
        onTriggered: {
            loadTimer.running = false;
            doLoadComponent()
            loadCompleted = isLoadCompleted()
        }
    }
}
