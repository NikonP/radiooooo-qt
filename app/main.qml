/*
 * This file is part of radiooooo-qt
 *
 */

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: w

    Material.theme: Material.Dark
    Material.accent: Material.Amber
    property string materialAmber: "#FFE082"
    property real qwe: 0

    visible: true
    width: 640
    minimumWidth: 640
    maximumWidth: 640
    height: 480
    minimumHeight: 480
    maximumHeight: 480
    title: qsTr("Radiooooo")

    Connections {
        target: radio

        onUpdateProgressBar: {
            audioPlayer.setPlaybackProgress(progress)
        }

        onUpdateStatusMsg: {
            audioPlayer.setStatus(message)
        }
    }

    Frame {
        id: base
        anchors.fill: parent

        Player {
            id: audioPlayer

            anchors.topMargin: 1
            anchors.rightMargin: 1
            anchors.leftMargin: 1
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            Component.onCompleted: {
                audioPlayer.setStatus("Radio is turned off")
            }

            onPauseChanged: {
                radio.playPause(state)
            }

            onNextSong: {
                radio.nextSong()
            }

            onVolumeChanged: {
                radio.setVolume(volume)
            }
        }

        Settings {
            id: settings

            anchors.left: parent.left
            anchors.leftMargin: 1
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 1
            anchors.top: audioPlayer.bottom
            anchors.topMargin: 5

            onUpdateConfig: {
                radio.updateConfig(param, value, enable);
            }
        }
    }
}


/*##^##
Designer {
    D{i:5;anchors_height:100;anchors_width:100}D{i:4;anchors_height:200;anchors_width:200}
D{i:3;anchors_x:0;anchors_y:83}
}
##^##*/
