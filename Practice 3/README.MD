# ЗАВДАННЯ 1:

## Умова:

Запустіть Docker-контейнер і поекспериментуйте з максимальним лімітом ресурсів відкритих файлів. Для цього виконайте команди у вказаному порядку:

$ ulimit -n

$ ulimit -aS | grep "open files"

$ ulimit -aH | grep "open files"

$ ulimit -n 3000

$ ulimit -aS | grep "open files"

$ ulimit -aH | grep "open files"

$ ulimit -n 3001

$ ulimit -n 2000

$ ulimit -n

$ ulimit -aS | grep "open files"

$ ulimit -aH | grep "open files"

$ ulimit -n 3000

Як наступне вправу, повторіть перераховані команди з root-правами.

## Код задачі:

```bash
$ ulimit -n

$ ulimit -aS | grep "open files"

$ ulimit -aH | grep "open files"

$ ulimit -n 3000

$ ulimit -aS | grep "open files"

$ ulimit -aH | grep "open files"

$ ulimit -n 3001

$ ulimit -n 2000

$ ulimit -n

$ ulimit -aS | grep "open files"

$ ulimit -aH | grep "open files"

$ ulimit -n 3000
```

## Виконання:

Для виконання цього завдання було встановлено Ubuntu Server з Docker-ом на Virtual Box. Далі запускаємо контейнер і вводимо команди з умови:

```bash
docker run -it --rm ubuntu bash
```

![](task1.png)

Як результат отримуємо, що змінювати ліміти ми можемо лише вниз. Для підвищення не вистачає прав.

Після цього виходимо з контейнера та запускаємо його повторно, але з параметром `--privileged`, щоб отримати root-права.

```bash
docker run -it --rm --privileged ubuntu bash
```

![](task1_1.png)

І як результат, за допомогою root-прав можемо як понижувати так і підвищувати ліміти.


# ЗАВДАННЯ 2:

## Умова:

У Docker-контейнері встановіть утиліту `perf(1)`. Поекспериментуйте з досягненням процесом встановленого ліміту.

## Виконання:

Для початку запускаємо контейнер та встановлюємо `perf` за допомогою 
```bash
apt update && apt install -y linux-tools-common linux-tools-generic linux-tools-$(uname -r)
```

Після цього перевіряємо встановлення `perf` за допомогою `perf --version` та встановлюємо ліміт на кількість запущених процесів:

![](task2_1.png)


**Примітка:** 

*Пізніше, дослідницьким шляхом, було з'ясвоно, що встановлення лімітів за допомогою команди `ulimit -u` впливає на кількість запущених процесів користувачем, а не системою загалом, тож дана команда виявиться марною.*

Далі за допомогою команди `nano test.sh` створюємо та редагуємо скрипт:

```sh
#!/bin/bash

counter=0
while true; do
  sleep 2 &
  counter=$((counter + 1))
  echo "Created $counter processes'

done
```

Після збереження файлу, запускаємо його за допомогою `nohup ./test.sh &`. `./test.sh` запускає сам скрипт, `&` - робить запуск фоновим, а `nohup` ігнорує ввід та перенаправляє вивід скрипта у файл `nohup.out` замість виводу в `bash`.

![](task2_3.png)

Після цього запускаємо
```bash
perf top -p $(pgrep -d',' -f test.sh)
```
для того, щоб побачити скільки ресурсів займає скрипт `test.sh`. `perf top` відображає всі активні процес та скільки ресурсів займає кожен з них наживо, `-p` фільтрує вивід за конкретним процесом, а `pgrep -d',' -f test.sh` знаходить всі процеси, що мають `test.sh` в імені та виводить їх PID.

![](task2_4.png)

Додатково можна запустити

```bash
ps aux | grep test.sh
```

щоб побачити процеси, які відповідають `test.sh` та їх PID.

![](task2_5.png)
