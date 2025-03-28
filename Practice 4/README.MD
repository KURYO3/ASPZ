# ЗАВДАННЯ 1:

## Умова:

Скільки пам’яті може виділити malloc(3) за один виклик? Параметр malloc(3) є цілим числом типу даних size_t, тому логічно максимальне число, яке можна передати як параметр malloc(3), — це максимальне значення size_t на платформі (sizeof(size_t)). У 64-бітній Linux size_t становить 8 байтів, тобто 8 * 8 = 64 біти. Відповідно, максимальний обсяг пам’яті, який може бути виділений за один виклик malloc(3), дорівнює 2^64. Спробуйте запустити код на x86_64 та x86. Чому теоретично максимальний обсяг складає 8 ексабайт, а не 16?

## Код задачі:
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
  printf ("Size of size_t: %zu bytes\n", sizeof (size_t));

  size_t max_size = (size_t) - 1;
  printf ("Max size, which can be transfer to malloc: %zu bytes (%.2f GB)\n", max_size, (double) max_size / (1024.0 * 1024.0 * 1024.0));

  void *ptr = malloc(max_size);
  if (ptr == NULL) {
    perror ("malloc can't allocated mamory");
  } else {
    printf ("malloc succesfully allocated memory\n");
    free(ptr);
  }
  return 0;
}
```

## Пояснення програми:



## Результат роботи:

![](task1.png)


# ЗАВДАННЯ 2:

## Умова:

Що станеться, якщо передати malloc(3) від’ємний аргумент? Напишіть тестовий випадок, який обчислює кількість виділених байтів за формулою num = xa * xb. Що буде, якщо num оголошене як цілочисельна змінна зі знаком, а результат множення призведе до переповнення? Як себе поведе malloc(3)? Запустіть програму на x86_64 і x86.

## Код задачі:

```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

int main() {
  int x = -1;
  void *neg_ptr = malloc(x) ;
  if (neg_ptr == NULL) {
    perror ("malloc can't allocate memory for negative size\n");
  } else {
    printf ("malloc succesfully allocate memory for negative size\n");
    free(neg_ptr);
  }

  int xa = INT_MAX;
  int xb = 2;
  int num = xa * xb;
  printf ("xa: %d, xb: %d, num: %d\n", xa, xb, num) ;

  void *overf low_ptr = malloc (num);
  if (overf low_ptr == NULL) {
    perror ("malloc can't allocate memory durind overf low\n");
  } else {
    printf ("malloc succesfully allocate memory during overf low\n");
    free(overflow_ptr);
  }

  size_t xa1 = SIZE_MAX / 2;
  size_t xb1 = 3;
  size_t num1 = xa1 * xb1;
  printf ("xa1: %zu, xb1: %zu, num1: %zu\n", xa1, xb1, num1) ;

  void *overflow_ptr1 = malloc (num1);
  if (overf low_ptr1 == NULL) {
    perror ("malloc can't allocate memory during overflow\n");
  } else {
    printf ("malloc succesfully allocate memory during overf low\n");
    free(overf low_ptr1);
  }

  return 0;
}
```
## Пояснення програми:

У цій програмі тестується поведінка `malloc()` при передачі від’ємного значення та при переповненні при множенні двох цілих чисел. Перший тест перевіряє, що станеться, якщо передати -1 як аргумент для `malloc()`. Як видно з результатів, `malloc()` не може виділити пам’ять і повертає `NULL`, що очікувано, оскільки параметр повинен бути беззнаковим числом. Далі програма тестує ситуацію, коли результат множення двох цілих чисел зі знаком викликає переповнення. Від’ємне значення `num` після множення `xa` та `xb` свідчить про переповнення, і спроба виділення такого обсягу пам’яті також завершується помилкою. Аналогічний тест проводиться з беззнаковими змінними, і в 32-бітній системі також виникає переповнення, яке спричиняє помилку при виділенні пам’яті.

## Результат роботи:

![](task2.png)


# ЗАВДАННЯ 3:

## Умова:

Що станеться, якщо використати malloc(0)? Напишіть тестовий випадок, у якому malloc(3) повертає NULL або вказівник, що не є NULL, і який можна передати у free(). Відкомпілюйте та запустіть через ltrace. Поясніть поведінку програми.

## Код задачі:

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
  void *ptr = malloc (0);

  if (ptr == NULL) {
    printf ("malloc(0) return NULL\n");
  } else {
    printf ("malloc(0) return pointer: %p\n", ptr);
    free(ptr);
  }

  return 0;
}
```

## Пояснення програми:

Ця програма досліджує поведінку `malloc(0)`. Поведінка `malloc(0)` залежить від реалізації, він може повернути `NULL` або ненульовий вказівник, який можна передати `free()`. У результатах роботи програми видно, що `malloc(0)` повернув певний вказівник, який можна було звільнити за допомогою `free()`, що підтверджує можливу поведінку цього виклику.

## Результат роботи:

![](task3.png)


# ЗАВДАННЯ 4:

## Умова:

Чи є помилки у такому коді?
```c
void *ptr = NULL;
while (<some-condition-is-true>) {
    if (!ptr)
        ptr = malloc(n);
    [... <використання 'ptr'> ...]
    free(ptr);
}
```

Напишіть тестовий випадок, який продемонструє проблему та правильний варіант коду.

## Код задачі:

*Початковий код:*
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
  void *ptr = NULL;
  for (int i = 0; i < 3; i++) {
    if (!ptr)
      ptr = malloc (10);
    if (!ptr) {
      perror ("malloc failed");
      return 1;
    }

    printf ("Iteration %d: ptr = %p\n", i, ptr);
    free(ptr);
  }
  return 0;
}
```

*Приклад проблеми:*
```c
#include <stdio.h>
#include <stdlib.h>
#include <string .h>

int main() {
  void *ptr = malloc(16);
  if (!ptr) {
    perror ("malloc failed");
    return 1;
  }

  strcpy(ptr, "Hello!");
  printf ("Before free: %s (ptr = %p)\n", (char *)ptr, ptr);

  free(ptr);

  printf ("After free, ptr still holds: %p\n", ptr);
  printf ("After free: %s\n", (char *)ptr);

  return 0;
}
```

*Правильний варіант коду:*
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
  void *ptr = NULL;
  for (int i = 0; i < 3; i++) {
    if (!ptr)
      ptr = malloc(10);
    if (!ptr) {
      perror ("malloc failed");
      return 1;
    }

    printf ("Iteration %d: ptr = %p\n", i, ptr);
    free(ptr);
    ptr = NULL;
  }

  return 0;
}
```

## Пояснення програми:

Ця програма демонструє проблему використання вказівника після його звільнення. У початковому варіанті коду після `free()` вказівник `ptr` не оновлюється, тому в наступній ітерації він може залишатися невизначеним, що може спричинити використання звільненої пам’яті. У тестовому прикладі показано, як вказівник все ще містить старі дані після виклику `free()`, що є потенційною помилкою. Правильний варіант коду вирішує проблему, встановлюючи `ptr` у `NULL` після виклику `free()`, що унеможливлює повторне використання звільненої пам’яті.

## Результат роботи:

![](task4.png)


# ЗАВДАННЯ 5:

## Умова:

Що станеться, якщо realloc(3) не зможе виділити пам’ять? Напишіть тестовий випадок, що демонструє цей сценарій.

## Код задачі:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  size_t big_size = 1024L * 1024L * 1024L * 1024L;
  void *ptr = malloc(1024);

  if (!ptr) {
    perror ("Initial malloc failed");
    return 1;
  }

  printf ("Initial allocation successful, ptr = %p\n", ptr);

  void *new_ptr = realloc(ptr, big_size);

  if (!new_ptr) {
    perror ("realloc failed");
    printf ("ptr is still valid: %p\n", ptr);
  } else {
    ptr = new_ptr;
    printf ("Realloc successful, new ptr = %p\n", ptr);

    memset (ptr, 0, big_size);
    printf ("Memory initialized successfully\n");
  }

  free(ptr);
  return 0;
}
```

## Пояснення програми:

Коли `realloc(3)` не може виділити пам’ять, вона повертає `NULL`, а початковий вказівник залишається дійсним і повинен бути звільнений вручну. У коді виділяється спочатку 1024 байти, що вдається зробити, після чого виконується `realloc(ptr, big_size)`, де `big_size` встановлений у 1 ТБ. Система не може виділити такий великий обсяг пам’яті, що призводить до помилки. У виводі програми спочатку вказано, що первинне виділення пам’яті пройшло успішно, проте після `realloc` система вичерпала простір підкачки (`swap_pager: out of swap space`), а ядро FreeBSD зупинило процес через неможливість звільнити пам’ять (`pid 1158 (task5) was killed: failed to reclaim memory`). Це означає, що операційна система намагалася зарезервувати запитану пам’ять, але, коли її виявилося недостатньо, процес було завершено, щоб запобігти загальному зависанню системи.

## Результат роботи:

![](task5.png)


# ЗАВДАННЯ 6:

## Умова:

Якщо realloc(3) викликати з NULL або розміром 0, що станеться? Напишіть тестовий випадок.

## Код задачі:

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
  void *ptr1 = realloc (NULL, 16);
  if (ptr1) {
    printf ("realloc (NULL, 16) successful: ptr1 = %p\n", ptr1);
  } else {
    printf ("realloc (NULL, 16) failed\n");
  }

  void *ptr2 = malloc(32);
  if (!ptr2) {
    perror ("malloc failde");
    return 1;
  }

  printf ("Initial allocation: ptr2 = %p\n", ptr2);

  void *ptr3 = realloc(ptr2, 0);
  printf ("realloc(ptr2, 0) returned: ptr3 = %p\n", ptr3) ;

  if (ptr3) {
    free(ptr3);
  }

  return 0;
}
```

## Пояснення програми:

Ця програма тестує два випадки використання `realloc`:

- Якщо `realloc(NULL, size)` викликається, він працює так само, як `malloc(size)`, тобто виділяє новий блок пам’яті.

- Якщо `realloc(ptr, 0)` викликається, зазвичай він або повертає `NULL`, або діє так само, як `free(ptr)`.

Перше повідомлення свідчить про те, що `realloc(NULL, 16)` виділив нову область пам’яті за адресою `0x3b60f9209000`, тобто працював аналогічно `malloc(16)`. Далі `ptr2` був успішно виділений через `malloc(32)`, отримавши адресу `0x3b60f9214000`. Останній рядок показує, що `realloc(ptr2, 0)` повернув нову адресу `0x3b60f9208008`. Це означає, що в реалізації `realloc` у FreeBSD функція не лише звільняє пам’ять, а й повертає певну службову адресу.

## Результат роботи:

![](task6.png)


# ЗАВДАННЯ 7:

## Умова:

Перепишіть наступний код, використовуючи reallocarray(3):
```c
struct sbar *ptr, *newptr;
ptr = calloc(1000, sizeof(struct sbar));
newptr = realloc(ptr, 500*sizeof(struct sbar));
```

Порівняйте результати виконання з використанням ltrace.

## Код задачі:

```c
#include <stdio.h>
#include <stdlib.h>

struct sbar {
  int data;
}

int main() {
  struct sbar *ptr, *newptr;

  ptr = calloc(1000, sizeof (struct sbar));
  if (!ptr) {
    perror ("calloc failed");
    return 1;
  }

  printf ("calloc successful: ptr = %p\n", ptr);

  newptr = reallocarray (ptr, 500, sizeof (struct sbar));
  if (!newptr) {
    perror ("reallocarray failed");
    free (ptr);
    return 1;
  }

  printf ("reallocarray successful: newptr = %p\n", newptr);

  free (newptr);
  return 0;
}
```

## Пояснення програми:

Тут потрібно переписати код із використанням `reallocarray(3)`, який працює аналогічно `realloc`, але безпечно обчислює розмір виділеної пам’яті, запобігаючи переповненню. Спочатку `calloc(1000, sizeof(struct sbar))` успішно виділяє область пам’яті, про що свідчить повідомлення `calloc successful: ptr = 0x205518609000`. Потім виконується `reallocarray(ptr, 500, sizeof(struct sbar))`, який зменшує розмір області, і повертається новий вказівник `0x205518613000`. Вивід `ltrace` показує, що спочатку виконується `calloc(1000, 4)`, потім `reallocarray(0x204f49009000, 500, 4)`, і новий вказівник успішно змінюється. Це підтверджує, що `reallocarray` коректно переобчислює розмір і працює так само, як `realloc`, але із додатковою безпекою від переповнення.

## Результат роботи:

![](task7.png)


# ЗАВДАННЯ ПО ВАРІАНТАХ (7 ВАРІАНТ):

## Умова:

Напишіть програму, яка перевіряє ефективність cache locality для розподіленої пам’яті.

## Код задачі:

```c
#include <stdio.h>
#include <stdlib.h>
#include <time. h>

#def int SIZE 8192
#def int REPEATS 10

double measure_row_access (int arr [SIZE][SIZE]) {
  clock_t start = clock();
  volatile int sum = 0;

  for (int r = 0; r < REPEATS; r++) {
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        sum += arr[i][j];
      }
    }
  }
  return (double) (clock() - start) / CLOCKS_PER_SEC;
}

double measure_col_access (int arr[SIZE][SIZE]) {
  clock_t start = clock();
  volatile int sum = 0;

  for (int r = 0; r < REPEATS; r++) {
    for (int j = 0: j < SIZE; j++) {
      for (int i = 0; i < SIZE; i++) {
        sum += arr[i][j];
      }
    }
  }
  return (double) (clock() - start) / CLOCKS_PER_SEC;
}

int main() {
  int (*arr ) [SIZE] = malloc (SIZE * SIZE * sizeof (int));
  if (!arr) {
    perror ("Memory allocation failed");
    return 1;
  }

  for (int i = 0; i < SIZE; i++)
    for (int j = 0: j < SIZE; j++)
      arr[i][j] = i + j;

  double row_time = measure_row_access (arr);
  double col_time = measure_col_access (arr);

  printf ("Row-wise access time: %f seconds\n", row_time);
  printf ("Column-wise access time: %f seconds\n", col_time);

  free (arr);
  return 0;
}
```

## Пояснення програми:

Програма тестує ефективність доступу до пам’яті при построковому та постовпчиковому обході. В результатах виконання:

- Для `SIZE = 1024` час доступу по рядках та по стовпцях приблизно однаковий.

- Для `SIZE = 8192` построчний доступ займає ~1.3 секунди, тоді як постовпчиковий — понад 8 секунд.

Це відбувається через кешування пам’яті: процесор кешує дані по рядках, тому построчний доступ є значно ефективнішим. При проходженні по стовпцях доступ до пам’яті відбувається не послідовно, що призводить до частих кеш-промахів і значного сповільнення виконання.

## Результат роботи:

*`#def int SIZE 1024`*

![](task8(1024).png)

*`#def int SIZE 8192`*

![](task8(8192).png)
