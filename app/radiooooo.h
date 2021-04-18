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
 * @class Radiooooo
 *
 * @brief Реализует взаимодействие с https://radiooooo.app/ и логику аудиоплеера
 *
 * @note Функции слоты класса Radiooooo доступны из QML
 *
 * @note Взаимодействие с объектом класса Configurator происходит при помощи сигналов
 */
class Radiooooo : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Radiooooo - дефолтный констуктор класса QObject
     *
     * @param parent - указатель на родительский QObject
     */
    explicit Radiooooo(QObject *parent = nullptr);

    /**
     * @brief Все состояния аудиоплеера
     */
    enum {IDLE, PLAYING, PAUSED, DOWNLOADING};
    /**
     * @brief Текущее состояние аудиоплеера
     */
    int state = IDLE;

private:
    /**
     * @brief Корневой эндпоинт для API Radiooooo
     */
    const QString baseAPI = "https://radiooooo.app";
    /**
     * @brief Эндпоинт для получения исокодов
     */
    const QString getCodesUrl = baseAPI + "/country/mood?decade=";
    /**
     * @brief Эндпоинт для получения аудиофайла
     */
    const QString getSongUrl = baseAPI + "/play";
    /**
     * @brief Строка, которая хранит название трека и название группы
     */
    QString nowPlaying;
    /**
     * @brief Продолжительность трека
     */
    qint64 audioDuration = 0;

    /**
     * @brief Указатель на Configurator
     */
    Configurator *cfg;
    /**
     * @brief Указатель на QMediaPlayer (класс Qt для работы с медиафайлами)
     */
    QMediaPlayer *mediaPlayer;
    /**
     * @brief Указатель на QNetworkAccessManager (класс Qt для работы с сетью)
     */
    QNetworkAccessManager *netManager;

    /**
     * @brief Получает данные через API Radiooooo
     * @return QJsonObject со всей информацией о треке
     */
    QJsonObject getSongInfo();
    /**
     * @brief Получает доступные исокоды
     * @param Десятилетие (например 1970, 1980)
     * @return QJsonObject со списком кодов
     *
     * @note Не проверяется кратность
     */
    QJsonObject getCountries(QString decade);
    /**
     * @brief Скачивает файл по URL
     * @param path - имя файла для сохранения
     * @param url - url откуда скачать файл
     * @return QString - полный путь к файлу
     */
    QString downloadFile(QString path, QString url);

    /**
     * @brief Сохраняет файл
     * @param path - путь куда сохранить
     * @param data - QByteArray содержимое файла
     * @return true, если удалось сохранить
     */
    bool saveFile(QString path, QByteArray data);

    /**
     * @brief Сохраняет файл и запускает его через QMediaPlayer
     */
    void playNext();

signals:
    /**
     * @brief Обновлет прогресс бар
     * @param progress - процент завершения
     */
    void updateProgressBar(double progress); // 0 - 0%, 1 - 100%

    /**
     * @brief Обновляет сообщение со статусом
     * @param message - строка с сообщением
     */
    void updateStatusMsg(QString message); // set status message

    /**
     * @brief Обнолвет состояние кнопки "play/pause" в QML
     *
     * @todo Переписать без вызова QML из C++ (например при помощи проверки флага в QML)
     */
    void forcePause();

public slots:
    /**
     * @brief Слот для загрузки настроек
     * @return QString с настройками
     */
    QString loadConfig();

    // updates param in config, adds value if enable==true, removes otherwise
    /**
     * @brief Слот для обновления настроек
     * @param param - имя параметра
     * @param value - значение
     * @param enable - флаг, если true, то добавить настройку, иначе удалить
     */
    void updateConfig(QString param,
                      QString value,
                      bool enable);

    /**
     * @brief Изменяет состояние аудиоплеера
     * @param play - запустить (true) или отсановить (false)
     */
    void playTrigger(bool play);
    /**
     * @brief Пропускает трек
     */
    void skipTrigger();
    /**
     * @brief Устанавливает громкость
     * @param volume - громкость (0-100)
     */
    void setVolume(int volume);

    /**
     * @brief Запускает следующий трек когда текущий заканчивается
     * @param playerState - состояние плеера
     */
    void stateChanged(QMediaPlayer::State playerState);
    /**
     * @brief Обновляет длительность трека
     * @param newDuration - новая длительность трека
     */
    void durationChanged(qint64 newDuration);
    /**
     * @brief Считает прогресс трека и вызывает updateProgressBar
     * @param pos - прошедшее время трека
     */
    void updateProgress(qint64 pos);

    /**
     * @brief Получение стран для быстрой настройки
     * @return QString - строка со списком
     */
    QString getQuickCountries();
    /**
     * @brief Получает список настроений
     * @return QList строк
     */
    QList<QString> getMoods();
    /**
     * @brief геттер для minDecade
     * @return minDecade из Configurator
     */
    int getMinDecade();
    /**
     * @brief геттер для maxDecade
     * @return maxDecade из Configurator
     */
    int getMaxDeacde();

};

#endif // RADIOOOOO_H
