# Документация Radiooooo-Qt

## Классы:
- Configurator - класс для работы с настройками приложения
- Radiooooo - класс для работы с API и аудиофайлами

## Заголовочные файлы:
- configurator.h - заголовочный файл [класса Configurator](@ref Configurator)
- radiooooo.h - заголовочный файл [класса Radiooooo](@ref Radiooooo)
- defaults.h - заголовочный файл со значениями по умолчанию

## Сигналы и слоты
Ассинхронные взаимодействия (в основном с UI) происходят при помощи сигналов и слотов.
Например, у [класса Radiooooo](@ref Radiooooo) есть [слот updateConfig](@ref Radiooooo.updateConfig) для получения сигнала из QML интерфейса.

Объявление слота в C++:

    public slots:
        void updateConfig(QString param,
                          QString value,
                          bool enable);

В QML выполняется такая конструкция:

    radio.updateConfig(param, value, enable);

Объект *radio* добавляется в контекс QML в main.cpp:

    Radiooooo radio;
    engine.rootContext()->setContextProperty("radio", &radio);
