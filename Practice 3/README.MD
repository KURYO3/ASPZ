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

Для початку запускаємо контейнер та за допомогою ```bashapt update && apt install -y linux-tools-common linux-tools-generic linux-tools-$(uname -r)``` встановлюємо `perf`

![](task2_1.png)

![](task2_2.png)

![](task2_3.png)

![](task2_4.png)

![](task2_5.png)
