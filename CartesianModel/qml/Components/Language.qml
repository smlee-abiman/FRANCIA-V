import QtQuick 2.12

Item {
    id: root
    property int value: 0

    Binding {
        target: root
        property: "value"
        value: resourceManager.language
        when: visible
    }
}
