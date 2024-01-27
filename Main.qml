import QtQuick
import QtQuick.Controls
import com.company.PlayerController
import com.company.AudioSearchModel

Window {
    id: root

    width: 640
    height: 480
    visible: true
    title: qsTr("Music Player")

    Rectangle {
        id: topBar

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: 50
        color: "#5F8575"

        SearchField {
            anchors {
                left: parent.left
                right: closeSearchButton.left
                verticalCenter: parent.verticalCenter
                margins: 10
            }

            height: 30

            visible: !searchPanel.hidden

            onAccepted: (value) => {
                            AudioSearchModel.searchSong(value)
                            topBar.forceActiveFocus()
                        }
        }

        ImageButton {
            id: playlistButton

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: 20
            }

            width: 32
            height: 32
            source: "assets/icons/menu_icon.png"

            visible: searchPanel.hidden

            onClicked: {
                playlistPanel.hidden = !playlistPanel.hidden
            }
        }

        ImageButton {
            id: closeSearchButton

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: 20
            }

            width: 32
            height: 32
            source: "assets/icons/close_icon.png"
            visible: !searchPanel.hidden

            onClicked: {
                searchPanel.hidden = true
            }
        }
    }

    PlaylistPanel {
        id: playlistPanel

        anchors.top: topBar.bottom

        x: hidden ? parent.width : parent.width - width
        z: 1

        onSearchRequested: {
            searchPanel.hidden = false
        }
    }

    SearchPanel {
        id: searchPanel

        anchors {
            left: parent.left
            right: parent.right
        }

        height: mainSection.height + bottomBar.height
        y: hidden ? parent.height : topBar.height
        z: 1
    }

    Rectangle {
        id: mainSection

        anchors {
            top: topBar.bottom
            bottom: bottomBar.top
            left: parent.left
            right: parent.right
        }

        color: "#1e1e1e"

        AudioInfoBox {
            id: songInfo

            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                margins: 20
            }
        }
    }

    Rectangle {
        id: bottomBar

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        height: 100
        color: "#333333"

        Row {
            id: playerControls

            anchors.centerIn: parent

            spacing: 20
            enabled: !!PlayerController.currentSong
            opacity: enabled ? 1 : 0.3

            ImageButton {
                id: previousButton

                width: 64
                height: 64

                source: "assets/icons/previous_icon.png"

                onClicked: PlayerController.switchToPrevSong()
            }

            ImageButton {
                id: playPauseButton

                width: 64
                height: 64

                source: PlayerController.playing ? "assets/icons/pause_icon.png" : "assets/icons/play_icon.png"

                onClicked: PlayerController.playPause()
            }

            ImageButton {
                id: nextButton

                width: 64
                height: 64

                source: "assets/icons/next_icon.png"

                onClicked: PlayerController.switchToNextSong()
            }
        }

        Slider {
            id: audioSlider

            anchors {
                verticalCenter: parent.verticalCenter
                left: playerControls.right
                leftMargin: 20
            }

            from: 0
            value: !!PlayerController.currentSong ? PlayerController.currentSong.position : 0

            onMoved: {
                PlayerController.slideToPosition(valueAt(position))
            }

            Connections {
                target: PlayerController
                function onDurationChanged(newDuration) {
                    audioSlider.to = newDuration
                }
            }
        }
    }
}
