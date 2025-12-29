#include <stdio.h>

#define M 100 //макс. абонентов
#define K 10 //макс. строк инфорации про абонента 

int main()
{
    struct abonent 
    {
        char name[K];
        char second_name[K];
        char tel[K];
    };

    struct abonent ab_book[M]; // массив абонентов
    int choice;//хранит выбор, введеный пользователем
    int count = 0; //для отслеживания количества абонентов 

    for (int i = 0; i < M; i++) // Инициализация массива пустыми строками
    {
        for (int j = 0; j < K; j++) 
            {
                ab_book[i].name[j] = '\0';
                ab_book[i].second_name[j] = '\0';
                ab_book[i].tel[j] = '\0';
            }
    }

     while (1) 
     {
        printf("\nАбонентский справочник\n");
        printf("1 - Добавить абонента\n");
        printf("2 - Удалить абонента\n");
        printf("3 - Поиск абонентов по имени\n");
        printf("4 - Вывод всех записей\n");
        printf("5 - Выход\n");
        printf("Введите пункт из меню: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1: // Добавление абонента
                if (count >= M) 
                {
                    printf("Справочник заполнен. Добавление невозможно.\n");
                } 
                else 
                {
                    printf("Введите имя: ");
                    scanf("%s", ab_book[count].name);
                    printf("Введите фамилию: ");
                    scanf("%s", ab_book[count].second_name);
                    printf("Введите телефон: ");
                    scanf("%s", ab_book[count].tel);
                    count++;
                    printf("Абонент добавлен.\n");
                }
                break;

            case 2: // Удаление абонента
            {
                int index; // какой номер из справочника хотим удалить
                printf("Введите номер абонента для удаления (1-%d): ", count);
                scanf("%d", &index);
                if (index < 1 || index > count) // проверка допустимости вводимого числа
                {
                    printf("Некорректный номер.\n");
                } 
                else 
                {
                    index--; // так как счет начинается с 0 в программе 
                    for (int j = 0; j < K; j++) //удаление(обнуление)
                    {
                        ab_book[index].name[j] = '\0';
                        ab_book[index].second_name[j] = '\0';
                        ab_book[index].tel[j] = '\0';
                    }
                    printf("Абонент удален.\n");
                }
                break;
            }

            case 3: // Поиск по имени
            {
                char search_name[K]; //хранит имя, введеное пользователем
                int found = 0; // показывает найден ли абонент
                printf("Введите имя для поиска: ");
                scanf("%s", search_name);
                for (int i = 0; i < count; i++) //перебор абонентов
                {
                    int equal = 1; 
                    for (int j = 0; j < K; j++) //сравниваем каждый символ двух строк:
                    {
                        if (ab_book[i].name[j] != search_name[j]) // не нашли совпадений
                        {
                            equal = 0;
                            break;
                        }
                        if (search_name[j] == '\0') break; //дошли до конца - выходим
                    }
                    if (equal && ab_book[i].name[0] != '\0') //имена совпадают и запись не пустая
                    {
                        printf("Абонент %d: %s %s, Тел: %s\n", i + 1, ab_book[i].name, ab_book[i].second_name, ab_book[i].tel);
                        found = 1;//хотя бы один абонент найден
                    }
                }
                if (!found) 
                {
                    printf("Абонентов с именем '%s' не найдено.\n", search_name);
                }
                break;
            }

            case 4: // Вывод всех абонентов
            {
                int empty = 1;
                for (int i = 0; i < count; i++) 
                {
                    if (ab_book[i].name[0] != '\0')//Проверяем, не пустая ли запись
                    {
                        printf("%d) %s %s, Тел: %s\n", i + 1, ab_book[i].name, ab_book[i].second_name, ab_book[i].tel);
                        empty = 0;
                    }
                }
                if (empty) 
                {
                    printf("Справочник пуст.\n");
                }
                break;
            }

            case 5: // Выход
                printf("Выход из программы.\n");
                return 0;

            default: // Любой другой ввод
                printf("Некорректный выбор, попробуйте снова.\n");
                break;
        }
    }

    return 0;


}
