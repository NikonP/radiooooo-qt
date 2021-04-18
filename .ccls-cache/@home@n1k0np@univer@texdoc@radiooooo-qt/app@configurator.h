/*
* This file is part of radiooooo-qt
* GitHub: https://github.com/NikonP/radiooooo-qt
* configurator.h - header file for configurator.cpp
*/

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <defaults.h>

/**
* @class Configurator
*
* @brief Используется для загрузки и сохранения настроек приложения.
*
* @todo Переписать взаимодействие с QML придерживаясь MVP
*/
class Configurator : public QObject {
    Q_OBJECT
public:
    /**
    * @brief Configurator - дефолтный констуктор класса QObject
    *
    * @param parent - указатель на родительский QObject
    */
    explicit Configurator(QObject *parent = nullptr);

    /**
    * @brief Тип данных для хранения настроек
    */
    typedef QMap<QString, QList<QString>> ConfigStorage;

    /**
    * @brief Создаёт папки в домашней дирректории приложения
    */
    void initDirs();
    /**
    * @brief Заполняет настройки со значениями по умолчанию
    */
    void initConfig();
    /**
    * @brief Сохраняет настройки в json файл
    */
    void saveConfig();
    /**
    * @brief Загружает настройки из json файла
    */
    void loadConfig();

    /**
    * @brief Обновляет настройки приложения
    * @param param - имя параметра
    * @param value - значение
    * @param enable - флаг, если true, то добавить настройку, иначе удалить
    */
    void updateConfig(QString param, QString value, bool enable);

    /**
    * @brief Все доступные "настроения"
    */
    const QList<QString> moods = defMoods;
    /**
    * @brief Все доступные страны
    */
    const QList<QString> allCountries = defCountries;
    /**
    * @brief Настройки по умолчанию
    */
    const QMap<QString, QString> defaultConfig = defConfig;
    /**
    * @brief Минимальный год
    */
    const int minDecade = 1910;
    /**
    * @brief Максимальный год
    */
    const int maxDecade = 2020;

    /**
    * @brief Список стран для быстрой настройки
    */
    const QMap<QString, QString> quickCountries = quickSetupCountries;

    /**
    * @brief Геттер для настроек приложения
    * @return настройки в виде ConfigStorage - key:value структуры с настройками
    * приложения
    */
    ConfigStorage getConfig();
    /**
    * @brief Геттер для настроек приложения
    * @return настройки в виде json строки
    */
    QString getConfigStr();
    /**
    * @brief Конвертирует настройки в json документ
    * @param c - список настроек
    * @return QJsonDocument - json документ с настройками
    */
    QJsonDocument configToJson(ConfigStorage c);

    /**
    * @brief Имя для домашней дирректории приложения
    */
    const QString appDir = ".radiooooo-qt";
    /**
    * @brief Путь к домашней дирректории приложения
    */
    const QString appDirPath = QDir::homePath() + "/" + appDir;
    /**
    * @brief Путь к json файлу для настроек
    */
    const QString configFilePath = appDirPath + "/config.json";
    /**
    * @brief Путь к папке для хранения аудиофайлов
    */
    const QString audioDirPath = appDirPath + "/" + "audio-files";

    /**
    * @brief Возвращает список стран для быстрой настройки в виде строки (для
    * QML)
    * @return QString - json строка со списком стран
    */
    QString getQuickCountries();

private:
    /**
    * @brief Переменная для хранения настроек
    */
    ConfigStorage config;
};

#endif // CONFIGURATOR_H
