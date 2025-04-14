#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define SIZE 16

// Функция для варианта А4.1 (байтовые массивы)
void run_A41(void) {
    // Исходные массивы: знаковые байты
    signed char arrA[SIZE] = { 1, 2, 3, 4, -5, -6, 7, -8, 9,  10, -11,  12, -13, 14, -15, 16 };
    signed char arrB[SIZE] = { 127, 64, -64, 32, 16, -16, 8, -8, 4, -4, 2, -2, 1, -1, 0, 0 };
    // Массив для адресов найденных элементов; для каждой пары сохраняем адрес из arrA и arrB
    int arrC[2 * SIZE] = { 0 };
    int pairCount = 0;  // Количество найденных пар
    int i;

    // Вывод исходных массивов (в десятичном формате)
    printf("Вариант А4.1 (байтовые массивы)\n");
    printf("Исходный массив A:\n");
    for (i = 0; i < SIZE; i++)
        printf("%4d ", arrA[i]);
    printf("\n\nИсходный массив B:\n");
    for (i = 0; i < SIZE; i++)
        printf("%4d ", arrB[i]);
    printf("\n");

    // Ассемблерная вставка: перебор элементов и проверка произведения
    __asm {
            xor ecx, ecx; i = 0 (ECX - счетчик цикла)
            xor ebx, ebx; EBX = 0 (счетчик найденных пар)
        loop_A41 :
            cmp ecx, SIZE; Если i == SIZE, выходим из цикла
            jae end_A41

            ; Загрузка arrA[i] с знаковым расширением
            movsx eax, byte ptr[arrA + ecx]
            ; Загрузка arrB[i] с знаковым расширением
            movsx edx, byte ptr[arrB + ecx]
            imul eax, edx; Вычисление произведения arrA[i] * arrB[i]

            ; Проверка диапазона произведения: [-128;127]
            cmp eax, -128
            jl skip_A41
            cmp eax, 127
            jg skip_A41

           ; Если произведение удовлетворяет условию, сохраняем адреса элементов:
            lea edx, arrA[ecx]; Адрес arrA[i]
            mov[arrC + ebx * 4], edx
            lea edx, arrB[ecx]; Адрес arrB[i]
            mov[arrC + ebx * 4 + 4], edx
            inc ebx; Увеличиваем счетчик пар
        skip_A41 :
            inc ecx; Переход к следующему элементу
            jmp loop_A41
        end_A41 :
            mov pairCount, ebx; Сохранить число найденных пар
    }

    // Вывод результатов (адреса выводятся в шестнадцатеричном формате)
    printf("\nНайдено пар: %d\n", pairCount);
    printf("Массив адресов (в 16-ричном формате):\n");
    for (i = 0; i < pairCount * 2; i++) {
        printf("%p ", (void*)arrC[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
    printf("\n");
}

// Функция для варианта А4.2 (расширенные описания: массивы слов)
void run_A42(void) {
    // Исходные массивы: знаковые слова (short int)
    short int arrA_ext[SIZE] = { 1, 2, 3, 4, -5, -6, 7, -8, 9, 10, -11, 12, -13, 14, -15, 16 };
    short int arrB_ext[SIZE] = { 127, 64, -64, 32, 16, -16, 8, -8, 4, -4, 2, -2, 1, -1, 0, 0 };
    // Массив для адресов найденных элементов (храним два адреса на пару)
    int arrC_ext[2 * SIZE] = { 0 };
    int pairCount_ext = 0;  // Количество найденных пар
    int i;

    // Вывод исходных массивов (в десятичном формате)
    printf("Вариант А4.2 (массивы слов)\n");
    printf("Исходный массив A (short):\n");
    for (i = 0; i < SIZE; i++)
        printf("%6d ", arrA_ext[i]);
    printf("\n\nИсходный массив B (short):\n");
    for (i = 0; i < SIZE; i++)
        printf("%6d ", arrB_ext[i]);
    printf("\n");

    // Ассемблерная вставка для обработки массива слов:
    __asm {
            xor ecx, ecx; i = 0, ECX - индекс
            xor ebx, ebx; EBX = 0, счетчик найденных пар
        loop_A42 :
            cmp ecx, SIZE; Если i == SIZE, завершаем цикл
            jae end_A42

            ; Загрузка элемента arrA_ext[i](тип word = 2 байта)
            movsx eax, word ptr[arrA_ext + ecx * 2]
            ; Загрузка элемента arrB_ext[i]
            movsx edx, word ptr[arrB_ext + ecx * 2]
            imul eax, edx; Вычисляем произведение

            ; Проверка, входит ли произведение в диапазон[-128; 127]
            cmp eax, -128
            jl skip_A42
            cmp eax, 127
            jg skip_A42

            ; Если условие выполнено – сохраняем адреса элементов
            lea edx, arrA_ext[ecx * 2]; Адрес элемента arrA_ext[i]
            mov[arrC_ext + ebx * 4], edx
            lea edx, arrB_ext[ecx * 2]; Адрес элемента arrB_ext[i]
            mov[arrC_ext + ebx * 4 + 4], edx
            inc ebx; Увеличиваем счетчик найденных пар
        skip_A42 :
            inc ecx; Переход к следующему индексу
            jmp loop_A42
        end_A42 :
            mov pairCount_ext, ebx; Сохраняем число найденных пар
    }

    // Вывод результатов (адреса выводятся в шестнадцатеричном формате)
    printf("\nНайдено пар: %d\n", pairCount_ext);
    printf("Массив адресов (в 16-ричном формате):\n");
    for (i = 0; i < pairCount_ext * 2; i++) {
        printf("%p ", (void*)arrC_ext[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
    printf("\n");
}

int main(void) {
	setlocale(LC_ALL, "Russian");
    int choice = 0;
    printf("Выберите вариант задания А4:\n");
    printf("1. Вариант А4.1 (байтовые массивы)\n");
    printf("2. Вариант А4.2 (массивы слов)\n");
    printf("Введите 1 или 2: ");
    while (1) {
        printf("Введите 1 или 2: ");
        if (scanf_s("%d", &choice) == 1 && (choice == 1 || choice == 2)) {
            break;
        }
        printf("Ошибка ввода! Попробуйте снова.\n");
        while (getchar() != '\n');
    }
	if (choice == 1) {
		run_A41();
	}
	else if (choice == 2) {
		run_A42();
	}
    return 0;
}
