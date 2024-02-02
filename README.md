# `pcap-task`

# Задание 1. Программа на C++

## Задание

Должна реализовать функционал классификации сетевых пакетов, для этого она должна проделать следующие действия:

1. С помощью библиотеки libpcap (либо другой на выбор разработчика) прочитать пакеты из pcap файла. Необходимо классифицировать только IPv4 пакеты. Возможность захвата пакетов с сетевого интерфейса будет дополнительным преимуществом при оценивании. 
2. Выделить из них заголовки IP пакетов и заголовки TCP/UDP.
3. Из выделенных заголовков прочитать IP адреса и порты.
4. Каждый пакет классифицировать к потоку (совокупности пакетов от IP адреса №1 до IP адреса №2 с уникальной комбинацией портов).
5. В каждом потоке посчитать количество пакетов и количество переданных байт.
6. После завершения чтения всех пакетов информацию о всех выделенных потоках необходимо записать в CSV файл. Формат CSV файла: `<IP адрес источника>,<IP адрес получателя>,<порт источника>,<порт получателя>,<кол-во пакетов>,<кол-во байт>`

Примечания:

- Для сборки программа не должна требовать настроек системы или нахождения определенных файлов в специфичном месте. 
- Исходный код программы на С++ должен компилироваться средствами cmake или make с использованием gcc/g++ для работы в среде Linux. В папке с исходным кодом не должно быть мусора: неиспользуемых файлов исходных кодов или ресурсов, промежуточных файлов сборки и т.д.
- Код на С++ должен быть выполнен в ООП стиле.

## Сборка проекта

Прежде всего, нужно установить зависимости. Проект использует библиотеку Pcap++, которая требует `libpcap-dev`. Для установки можно выполнить команду:

```bash
sudo apt-get install libpcap-dev
```

Для сборки проекта используется CMake. Для выполнения сборки достаточно выполнить следующие команды:

```bash
cd cpp-packet-classifier
cmake -S . -B build
cmake --build build
```

Скомпилированная утилита лежит в папке `build`. Для ее запуска достаточно выполнить:

```bash
cd build
./packet-classifier
```

## Инструкция пользователя

Утилита работает в двух основных режимах:

- Режим чтения пакетов из .pcap файла
- Режим захвата пакетов с сетевого интерфейса

### Чтение из файла .pcap

#### Использование:
```bash
./packet-classifier file -i <input_file.pcap> -o <output_file.csv>
```

#### Параметры:
- `-i, --input` - входной файл .pcap
- `-o, --output` - выходной файл .csv

### Захват с сетевого интерфейса

#### Использование:
```bash
./packet-classifier live -i <input_interface> -o <output_file.csv>
```

#### Параметры:
- `-i, --input` - сетевой интерфейс
- `-o, --output` - выходной файл .csv

#### Пример:
```bash
./packet-classifier live -i eth0 -o output.csv
```

### Список доступных интерфейсов

#### Использование:
```bash
./packet-classifier live list
```

Эта команда выводит список всех доступных сетевых интерфейсов.

# Задание 2. Программа на Python

## Задание

Запускается после завершения работы первой программы комплекса и должна выполнить постобработку полученных статистических сведений, для этого необходимо выполнять следующие действия:

1. Прочитать CSV файл, сгенерированный Программой №1.
2. Для каждого, представленного в файле IP адреса подсчитать кол-во принятых и переданных байт и пакетов.
3. Полученные данные записать в CSV файл. Формат: `<IP адрес>,<кол-во принятых пакетов>,<кол-во принятых байт>,<кол-во переданных пакетов>,<кол-в переданных байт>`

## Установка зависимостей

Для управления зависимостями используется Poetry. Для установки зависимостей достаточно выполнить следующие команды:

```bash
cd py-stats-processor
poetry install
```

Для запуска утилиты достаточно выполнить:

```bash
poetry run python App.py
```

## Инструкция пользователя

Утилита принимает на вход всего 2 аргумента:

#### Параметры:
- `-i, --input` - входной файл .csv
- `-o, --output` - выходной файл .csv

#### Использование:
```bash
python script_name.py -i <input_file.csv> -o <output_file.csv>
```