/*
* This file is part of radiooooo-qt
* GitHub: https://github.com/NikonP/radiooooo-qt
* radiooooo.h - header file for radiooooo.cpp
*/

#ifndef RADIOOOOO_H
#define RADIOOOOO_H

#include <QObject>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include "configurator.h"

/**
* @defgroup radiooooo_base Основа Radiooooo
* Базовые функции и переменные класса Radiooooo
*/

/**
* @class Radiooooo
*
* @brief Реализует взаимодействие с https://radiooooo.app/ и логику аудиоплеера
*
* @note Функции слоты класса Radiooooo доступны из QML
*
* @note Взаимодействие с объектом класса Configurator происходит при помощи сигналов
*
* @ingroup radiooooo_base
*/
class Radiooooo : public QObject
{
    Q_OBJECT
public:
    /**
    * @ingroup radiooooo_base
    *
    * @brief Radiooooo - дефолтный констуктор класса QObject
    *
    * @param parent - указатель на родительский QObject
    */
    explicit Radiooooo(QObject *parent = nullptr);

    /**
    * @ingroup radiooooo_base
    *
    * @brief Все состояния аудиоплеера
    */
    enum {IDLE, PLAYING, PAUSED, DOWNLOADING};
    /**
    * @ingroup radiooooo_base
    *
    * @brief Текущее состояние аудиоплеера
    */
    int state = IDLE;

    /**
    * @defgroup radiooooo_api API
    * Функции и переменные для работы с API
    */
    /**
    * @defgroup radiooooo_player Медиаплеер
    * Функции и переменные для работы с медиаплеером
    */
private:
    /**
    * @ingroup radiooooo_api
    * @brief Корневой эндпоинт для API Radiooooo
    */
    const QString baseAPI = "https://radiooooo.app";
    /**
    * @ingroup radiooooo_api
    * @brief Эндпоинт для получения исокодов
    */
    const QString getCodesUrl = baseAPI + "/country/mood?decade=";
    /**
    * @ingroup radiooooo_api
    * @brief Эндпоинт для получения аудиофайла
    */
    const QString getSongUrl = baseAPI + "/play";
    /**
    * @ingroup radiooooo_player
    * @brief Строка, которая хранит название трека и название группы
    */
    QString nowPlaying;
    /**
    * @ingroup radiooooo_player
    * @brief Продолжительность трека
    */
    qint64 audioDuration = 0;

    /**
    * @ingroup radiooooo_player
    * @brief Указатель на Configurator
    */
    Configurator *cfg;
    /**
    * @ingroup radiooooo_player
    * @brief Указатель на QMediaPlayer (класс Qt для работы с медиафайлами)
    */
    QMediaPlayer *mediaPlayer;
    /**
    * @ingroup radiooooo_api
    * @brief Указатель на QNetworkAccessManager (класс Qt для работы с сетью)
    */
    QNetworkAccessManager *netManager;

    /**
    * @ingroup radiooooo_api
    * @brief Получает данные через API Radiooooo
    * @return QJsonObject со всей информацией о треке
    */
    QJsonObject getSongInfo();
    /**
    * @ingroup radiooooo_api
    * @brief Получает доступные исокоды
    * @param Десятилетие (например 1970, 1980)
    * @return QJsonObject со списком кодов
    *
    * @note Не проверяется кратность
    */
    QJsonObject getCountries(QString decade);
    /**
    * @ingroup radiooooo_api
    * @brief Скачивает файл по URL
    * @param path - имя файла для сохранения
    * @param url - url откуда скачать файл
    * @return QString - полный путь к файлу
    */
    QString downloadFile(QString path, QString url);

    /**
    * @ingroup radiooooo_api
    * @brief Сохраняет файл
    * @param path - путь куда сохранить
    * @param data - QByteArray содержимое файла
    * @return true, если удалось сохранить
    */
    bool saveFile(QString path, QByteArray data);

    /**
    * @ingroup radiooooo_player
    * @brief Сохраняет файл и запускает его через QMediaPlayer
    */
    void playNext();

    /**
    * @defgroup radiooooo_signals Сигналы Radiooooo
    * Сигналы класса Radiooooo
    */
signals:
    /**
    * @ingroup radiooooo_signals
    * @brief Обновлет прогресс бар
    * @param progress - процент завершения
    */
    void updateProgressBar(double progress); // 0 - 0%, 1 - 100%

    /**
    * @ingroup radiooooo_signals
    * @brief Обновляет сообщение со статусом
    * @param message - строка с сообщением
    */
    void updateStatusMsg(QString message); // set status message

    /**
    * @ingroup radiooooo_signals
    * @brief Обновляет состояние кнопки "play/pause" в QML
    *
    * @todo Переписать без вызова QML из C++ (например при помощи проверки флага в QML)
    */
    void forcePause();

    /**
    * @defgroup radiooooo_slots Слоты Radiooooo
    * Слоты класса Radiooooo
    */
public slots:
    /**
    * @ingroup radiooooo_slots
    * @brief Слот для загрузки настроек
    * @return QString с настройками
    */
    QString loadConfig();

    /**
    * @ingroup radiooooo_slots
    * @brief Слот для обновления настроек
    * @param param - имя параметра
    * @param value - значение
    * @param enable - флаг, если true, то добавить настройку, иначе удалить
    */
    void updateConfig(QString param,
                      QString value,
                      bool enable);

    /**
        * @ingroup radiooooo_slots
        * @brief Изменяет состояние аудиоплеера
        * @param play - запустить (true) или остановить (false)
        */
    void playTrigger(bool play);
    /**
        * @ingroup radiooooo_slots
        * @brief Пропускает трек
        */
    void skipTrigger();
    /**
        * @ingroup radiooooo_slots
        * @brief Устанавливает громкость
        * @param volume - громкость (0-100)
        */
    void setVolume(int volume);

    /**
        * @ingroup radiooooo_slots
        * @brief Запускает следующий трек, когда текущий заканчивается
        * @param playerState - состояние плеера
        */
    void stateChanged(QMediaPlayer::State playerState);
    /**
        * @ingroup radiooooo_slots
        * @brief Обновляет длительность трека
        * @param newDuration - новая длительность трека
        */
    void durationChanged(qint64 newDuration);
    /**
        * @ingroup radiooooo_slots
        * @brief Считает прогресс трека и вызывает updateProgressBar
        * @param pos - прошедшее время трека
        */
    void updateProgress(qint64 pos);

    /**
        * @ingroup radiooooo_slots
        * @brief Получение стран для быстрой настройки
        * @return QString - строка со списком
        */
    QString getQuickCountries();
    /**
        * @ingroup radiooooo_slots
        * @brief Получает список настроений
        * @return QList строк
        */
    QList<QString> getMoods();
    /**
        * @ingroup radiooooo_slots
        * @brief геттер для minDecade
        * @return minDecade из Configurator
        */
    int getMinDecade();
    /**
        * @ingroup radiooooo_slots
        * @brief геттер для maxDecade
        * @return maxDecade из Configurator
        */
    int getMaxDeacde();

};

#endif // RADIOOOOO_H
