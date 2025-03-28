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


# ЗАВДАННЯ 3:

## Умова:

Напишіть програму, що імітує кидання шестигранного кубика. Імітуйте кидки, результати записуйте у файл, для якого попередньо встановлено обмеження на його максимальний розмір (max file size). Коректно обробіть ситуацію перевищення ліміту.

## Код програми:

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_FILE_SIZE 128
#define FILE_NAME "dice_results.txt"

int check_file_size(const char *file_name, size_t max_size) {
    struct stat st;
    if (stat(file_name, &st) == 0) {
        return (st.st_size > max_size);
    }
    return 0;
}

void clear_file(const char *file_name) {
    FILE *file = fopen(file_name, "w");
    if (file != NULL) {
        fclose(file);
        printf("File cleared successfully.\n");
    } else {
        perror("Error clearing file");
    }
}

int main() {
    FILE *file;
    int roll;
    srand(time(NULL));

    file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    printf("Starting dice simulation. Press Ctrl+C to stop.\n");

    while (1) {
        if (check_file_size(FILE_NAME, MAX_FILE_SIZE)) {
            printf("File size exceeded %d bytes.\n", MAX_FILE_SIZE);
            printf("Do you want to clear the file? (y/n): ");

            char response[10];
            fgets(response, sizeof(response), stdin);

            if (strncmp(response, "y", 1) == 0 || strncmp(response, "Y", 1) == 0) {
                clear_file(FILE_NAME);
                file = freopen(FILE_NAME, "a", file);
                if (file == NULL) {
                    perror("Error reopening file");
                    return EXIT_FAILURE;
                }
            } else {
                printf("Exiting simulation.\n");
                break;
            }
        }

        roll = (rand() % 6) + 1;

        printf("Rolling: %d\n", roll);
        fprintf(file, "Rolling: %d\n", roll);
        fflush(file);

        usleep(100000);  // 100 мс
    }

    fclose(file);
    printf("Simulation finished.\n");
    return EXIT_SUCCESS;
}

```

## Виконання:

Для початку запустимо контейнер та встановимо `gcc` та `nano` за допомогою команд
```bash
apt update 
apt install gcc && apt install nano
```

та перевіримо встановлення за допомогою `--version`

![](task3_1.png)

Далі скомпілюємо та запустимо програму. Після запуску програма починає імітувати кидання кубика та записувати дані у файл `results.txt` поки користувач не зупинить програму за допомогою Ctrl + C або поки файл не заповниться. Якщо файл заповниться буде запропоновано очистити його або зупинити програму.

![](task3_2.png)

Можемо також переглянути вміст файлу `results.txt` за допомогою команди `cat results.txt`

![](task3_3.png)

та перевірити розмір файлу за допомогою `ls -lh results.txt` 

![](task3_4.png)

**Примітка**
*"Rolling:" займає 8 байтів, " цифра" займає 2 байти і ще один байт займає символ нового рядка. В результаті 99 байтів займає файл: 11 байтів на рядок і 9 рядків.*
