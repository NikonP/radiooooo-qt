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
* @defgroup configurator_base Основа Configurator
* Базовые функции и переменные класса Configurator
*/

/**
* @defgroup configurator_fs Файловая система
* Функции и переменные для работы с файловой системой
*/

/**
* @defgroup configurator_settings Настройки
* Функции и переменные для работы с настройками приложения
*/

/**
* @class Configurator
*
* @brief Используется для загрузки и сохранения настроек приложения.
*
* @todo Переписать взаимодействие с QML придерживаясь MVP
*
* @ingroup configurator_base
*/
class Configurator : public QObject {
    Q_OBJECT
public:
    /**
    * @ingroup configurator_base
    *
    * @brief Configurator - дефолтный констуктор класса QObject
    *
    * @param parent - указатель на родительский QObject
    */
    explicit Configurator(QObject *parent = nullptr);

    /**
    * @ingroup configurator_base
    *
    * @brief Тип данных для хранения настроек
    */
    typedef QMap<QString, QList<QString>> ConfigStorage;

    /**
    * @ingroup configurator_fs
    *
    * @brief Создаёт папки в домашней дирректории приложения
    */
    void initDirs();
    /**
    * @ingroup configurator_settings
    *
    * @brief Заполняет настройки со значениями по умолчанию
    */
    void initConfig();
    /**
    * @ingroup configurator_fs
    *
    * @brief Сохраняет настройки в json файл
    */
    void saveConfig();
    /**
    * @ingroup configurator_fs
    *
    * @brief Загружает настройки из json файла
    */
    void loadConfig();

    /**
    * @ingroup configurator_settings
    *
    * @brief Обновляет настройки приложения
    * @param param - имя параметра
    * @param value - значение
    * @param enable - флаг, если true, то добавить настройку, иначе удалить
    */
    void updateConfig(QString param, QString value, bool enable);

    /**
    * @ingroup configurator_settings
    *
    * @brief Все доступные "настроения"
    */
    const QList<QString> moods = defMoods;
    /**
    * @ingroup configurator_settings
    *
    * @brief Все доступные страны
    */
    const QList<QString> allCountries = defCountries;
    /**
    * @ingroup configurator_settings
    *
    * @brief Настройки по умолчанию
    */
    const QMap<QString, QString> defaultConfig = defConfig;
    /**
    * @ingroup configurator_settings
    *
    * @brief Минимальный год
    */
    const int minDecade = 1910;
    /**
    * @ingroup configurator_settings
    *
    * @brief Максимальный год
    */
    const int maxDecade = 2020;

    /**
    * @ingroup configurator_settings
    *
    * @brief Список стран для быстрой настройки
    */
    const QMap<QString, QString> quickCountries = quickSetupCountries;

    /**
    * @ingroup configurator_settings
    *
    * @brief Геттер для настроек приложения
    * @return настройки в виде ConfigStorage - key:value структуры с настройками
    * приложения
    */
    ConfigStorage getConfig();
    /**
    * @ingroup configurator_settings
    *
    * @brief Геттер для настроек приложения
    * @return настройки в виде json строки
    */
    QString getConfigStr();
    /**
    * @ingroup configurator_settings
    *
    * @brief Конвертирует настройки в json документ
    * @param c - список настроек
    * @return QJsonDocument - json документ с настройками
    */
    QJsonDocument configToJson(ConfigStorage c);

    /**
    * @ingroup configurator_fs
    *
    * @brief Имя для домашней дирректории приложения
    */
    const QString appDir = ".radiooooo-qt";
    /**
    * @ingroup configurator_fs
    *
    * @brief Путь к домашней дирректории приложения
    */
    const QString appDirPath = QDir::homePath() + "/" + appDir;
    /**
    * @ingroup configurator_fs
    *
    * @brief Путь к json файлу для настроек
    */
    const QString configFilePath = appDirPath + "/config.json";
    /**
    * @ingroup configurator_fs
    *
    * @brief Путь к папке для хранения аудиофайлов
    */
    const QString audioDirPath = appDirPath + "/" + "audio-files";

    /**
    * @ingroup configurator_settings
    *
    * @brief Возвращает список стран для быстрой настройки в виде строки (для QML)
    * @return QString - json строка со списком стран
    */
    QString getQuickCountries();

private:
    /**
    * @ingroup configurator_settings
    *
    * @brief Переменная для хранения настроек
    */
    ConfigStorage config;
};

#endif // CONFIGURATOR_H
