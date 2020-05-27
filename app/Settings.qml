/*
 * This file is part of radiooooo-qt
 * GitHub: https://github.com/NikonP/radiooooo-qt
 */

import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.3

Frame {
    id: root
    anchors.right: parent.right
    anchors.rightMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 0
    anchors.top: parent.top
    anchors.topMargin: 0

    signal updateConfig(string param, string value, bool enable)

    // some countries for quick setup
    // <ISO-3166-1 Alpha-3>: <name of the country>
    property var quickCountries: JSON.parse(radio.getQuickCountries())
    property var moodsArr: radio.getMoods()
    property var minDecade: radio.getMinDecade()
    property var maxDeacde: radio.getMaxDeacde()

    property var config

    function loadConfig() {
        config = JSON.parse(radio.loadConfig());
    }

    GroupBox {
        id: groupBox
        anchors.fill: parent
        font.pointSize: 12
        title: qsTr("Settings")

        ListModel {
            id: decadesModel
            Component.onCompleted: {
                for(var i = minDecade; i <= maxDeacde; i+=10){
                    var isSelected = false;
                    if("decades" in config) {
                        isSelected = config["decades"].includes(i.toString());
                    }

                    decadesModel.append({"name": i.toString(), "checked": isSelected});
                }
            }
        }

        ListModel {
            id: countriesModel

            Component.onCompleted: {
                for(var code in quickCountries) {
                    var name = quickCountries[code];

                    var isSelected = false;
                    if("isocodes" in config) {
                        isSelected = (config["isocodes"].includes(code));
                    }

                    countriesModel.append({"name": name, "code": code, "checked": isSelected});
                }
            }
        }

        ListModel {
            id: moodsModel

            Component.onCompleted: {
                for(var i = 0; i < moodsArr.length; i++) {
                    var mood = moodsArr[i];

                    var isSelected = false;
                    if("moods" in config) {
                        isSelected = (config["moods"].includes(mood));
                    }

                    moodsModel.append({"name": mood, "checked": isSelected});
                }
            }
        }

        Column {
            id: cl

            ComboBox {
                id: decades
                width: 200
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                model: decadesModel
                displayText: "Select decades"

                delegate: Item {
                    width: parent.width
                    implicitHeight: decadesCD.implicitHeight

                    CheckDelegate {
                        id: decadesCD
                        width: parent.width
                        text: model.name
                        highlighted: decades.highlightedIndex === index
                        checked: model.checked
                        onCheckedChanged: {
                            model.checked = checked
                            updateConfig("decades", model.name, checked)
                        }
                    }
                }
            }

            ComboBox {
                id: countries
                width: 200
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                model: countriesModel
                displayText: "Select Countries"

                delegate: Item {
                    width: parent.width
                    implicitHeight: countriesCD.implicitHeight

                    CheckDelegate {
                        id: countriesCD
                        width: parent.width
                        text: model.name
                        highlighted: countries.highlightedIndex === index
                        checked: model.checked
                        onCheckedChanged: {
                            model.checked = checked
                            updateConfig("isocodes", model.code, checked)
                        }
                    }
                }
            }

            ComboBox {
                id: moods
                width: 200
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                model: moodsModel
                displayText: "Select moods"

                delegate: Item {
                    width: parent.width
                    implicitHeight: moodsCD.implicitHeight

                    CheckDelegate {
                        id: moodsCD
                        width: parent.width
                        text: model.name
                        highlighted: moods.highlightedIndex === index
                        checked: model.checked
                        onCheckedChanged: {
                            model.checked = checked
                            updateConfig("moods", model.name, checked)
                        }
                    }
                }
            }

        }


        Component.onCompleted: {
            loadConfig()
        }
    }
}
