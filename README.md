# Qt Quick Music Player

This is a music player that can play songs from a playlist, and it also supports the addition of songs from Jamendo Music. I mainly followed [Scythe Studio's tutorial](https://github.com/scytheStudio/qt-qml-tutorial) through the course of developing this project. In later sections, I explain the functionalities of each component of the app.

## Player Controller

The player controller implements QAbstractListModel and functions as a model for the playlist. It also provides functionalities such as play/pause, switch to next song, and seek to position for the user interface.

## Audio Search Model

The audio search model also implements QAbstractListModel. It sends out HTTP get requests to retrive songs from Jamendo Music.

## Audio Info

Stores song information such as name, author name, and cover image
