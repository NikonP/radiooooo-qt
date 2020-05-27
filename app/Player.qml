/*
 * This file is part of radiooooo-qt
 * GitHub: https://github.com/NikonP/radiooooo-qt
 */

import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.3

ColumnLayout {
    id: root
    spacing: 5

    property string buttonColor: "#000000"
    property bool playing: false
    property string playPause: playing ? "assets/pause.svg" : "assets/play.svg"
    property real playbackProgress: 0

    signal pauseChanged(bool state)
    signal nextSong()
    signal volumeChanged(int volume)

    function setStatus(newStatus) {
        statusText.text = qsTr(newStatus)
    }

    function setPlaybackProgress(progress) {
        if(progress <= 1) {
            playbackProgress = progress
        }
    }

    RowLayout {
        id: rl
        spacing: 5
        Frame {
            id: status
            Layout.fillWidth: true
            Label {
                id: statusText
                text: qsTr("")
                clip: true
                fontSizeMode: Text.HorizontalFit
                width: parent.width
                font.pointSize: 20
            }
        }

        RoundButton {
            id: playPauseBtn
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            highlighted: true

            icon.color: buttonColor
            icon.source: playPause

            onClicked: {
                playing = !playing
                pauseChanged(playing)
            }
        }

        RoundButton {
            id: skipBtn
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            highlighted: true
            icon.color: buttonColor
            icon.source: "assets/next.svg"

            onClicked: {
                nextSong()
            }
        }
    }

    Slider {
        id: volume
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        stepSize: 1
        wheelEnabled: true
        from: 0
        to: 100
        value: 70
        orientation: Qt.Horizontal

        onMoved: {
            volumeChanged(value)
        }
    }

    ProgressBar {
        id: progressBar
        value: playbackProgress
        Layout.fillWidth: true
    }
}
