# Отчет по лабораторной работе №8

**Студент:** MAX-shadow
**Тема:** Изучение систем автоматизации развёртывания на примере Docker

---

## 1. Цель работы

Научиться упаковывать приложение в Docker-образ, собирать его и запускать в
контейнере, а также работать с томами (volume) для проброса данных наружу.

Используется проект из лабораторной работы №7 — библиотека `banking` и
приложение `demo`, которое пишет результат переводов в файл `LOG_PATH`.

> Примечание: в методичке используется базовый образ `ubuntu:18.04`, но он
> снят с поддержки (apt-репозитории недоступны), поэтому взят `ubuntu:22.04`.

---

## 2. Ход выполнения работы

### 2.1. Подготовка проекта

Проект скопирован из лабораторной работы №7:

```sh
git clone https://github.com/MAX-shadow/lab07 lab08
cd lab08
git remote set-url origin https://github.com/MAX-shadow/lab08
```

### 2.2. Dockerfile

Создан `Dockerfile`. Он ставит компилятор и cmake, копирует проект,
собирает и устанавливает его, задаёт переменную `LOG_PATH`, объявляет том
для логов и запускает приложение `demo`:

```dockerfile
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y gcc g++ cmake git ca-certificates && \
    rm -rf /var/lib/apt/lists/*

COPY . /app/
WORKDIR /app

RUN cmake -H. -B_build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=_install
RUN cmake --build _build
RUN cmake --build _build --target install

ENV LOG_PATH=/home/logs/log.txt
VOLUME /home/logs
WORKDIR /app/_install/bin
ENTRYPOINT ["./demo"]
```

### 2.3. Сборка образа

```sh
$ docker build -t logger .
$ docker images
REPOSITORY   TAG       IMAGE ID       SIZE
logger       latest    41c73baed4c1   ...
```

### 2.4. Запуск контейнера с томом

```sh
$ mkdir logs
$ printf "1000\n50\n3000\n-5\n" | docker run -i --rm -v "$(pwd)/logs:/home/logs" logger
```

Приложение получает суммы переводов со стандартного ввода, прогоняет их
через библиотеку `banking` и пишет результат в `/home/logs/log.txt`, который
проброшен на хост через том. Содержимое `logs/log.txt`:

```
transfer 1000 : ok
transfer 50 : error (too small)
transfer 3000 : ok
transfer -5 : error (sum can't be negative)
```

То есть данные из контейнера успешно сохранились на хост-машину.

### 2.5. CI

Вместо `.travis.yml` (методичка) сборка образа вынесена в GitHub Actions
(`.github/workflows/ci.yml`): на каждый push собирается образ `logger` и
запускается контейнер для проверки.

```yaml
  docker:
    runs-on: ubuntu-22.04
    steps:
      - uses: actions/checkout@v4
      - name: Build docker image
        run: docker build -t logger .
      - name: Run logger container
        run: |
          mkdir -p logs
          printf "1000\n50\n3000\n" | docker run -i --rm -v "$(pwd)/logs:/home/logs" logger
          cat logs/log.txt
```

---

## 3. Результаты

- Создан Dockerfile, собран образ `logger`.
- Контейнер запускается, читает данные со stdin и пишет лог в проброшенный том.
- Сборка образа автоматизирована через GitHub Actions.
