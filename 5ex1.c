#include <stdio.h>

#define M 100
#define K 10

struct abonent
{
    char name[K];
    char second_name[K];
    char tel[K];
};
//Прототипы функций,чтобы компилятор 
//знал о функциях до их использования
void init_book(struct abonent ab_book[]);
void add_abonent(struct abonent ab_book[], int *count);
void delete_abonent(struct abonent ab_book[], int count);
void search_abonent(struct abonent ab_book[], int count);
void print_all(struct abonent ab_book[], int count);
void menu();

int main()
{
    struct abonent ab_book[M];
    int choice;
    int count = 0;

    init_book(ab_book);

    while (1)
    {
        menu();
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                add_abonent(ab_book, &count);
                break;

            case 2:
                delete_abonent(ab_book, count);
                break;

            case 3:
                search_abonent(ab_book, count);
                break;

            case 4:
                print_all(ab_book, count);
                break;

            case 5:
                printf("Выход из программы.\n");
                return 0;

            default:
                printf("Некорректный выбор.\n");
        }
    }

    return 0;
}

void menu()
{
    printf("\nАбонентский справочник\n");
    printf("1 - Добавить абонента\n");
    printf("2 - Удалить абонента\n");
    printf("3 - Поиск абонентов по имени\n");
    printf("4 - Вывод всех записей\n");
    printf("5 - Выход\n");
    printf("Введите пункт из меню: ");
}

void init_book(struct abonent ab_book[])//Очищает массив
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < K; j++)
        {
            ab_book[i].name[j] = '\0';
            ab_book[i].second_name[j] = '\0';
            ab_book[i].tel[j] = '\0';
        }
    }
}

void add_abonent(struct abonent ab_book[], int *count)//заполнение массивов
{
    if (*count >= M)
    {
        printf("Справочник заполнен.\n");
        return;
    }

    printf("Введите имя: ");
    scanf("%s", ab_book[*count].name);

    printf("Введите фамилию: ");
    scanf("%s", ab_book[*count].second_name);

    printf("Введите телефон: ");
    scanf("%s", ab_book[*count].tel);

    (*count)++;

    printf("Абонент добавлен.\n");
}

void delete_abonent(struct abonent ab_book[], int count)//удаление
{
    int index;

    printf("Введите номер абонента для удаления (1-%d): ", count);
    scanf("%d", &index);

    if (index < 1 || index > count)
    {
        printf("Некорректный номер.\n");
        return;
    }

    index--;

    for (int j = 0; j < K; j++)
    {
        ab_book[index].name[j] = '\0';
        ab_book[index].second_name[j] = '\0';
        ab_book[index].tel[j] = '\0';
    }

    printf("Абонент удален.\n");
}

void search_abonent(struct abonent ab_book[], int count)// поиск абонентов
{
    char search_name[K];
    int found = 0;

    printf("Введите имя для поиска: ");
    scanf("%s", search_name);

    for (int i = 0; i < count; i++)
    {
        int equal = 1;

        for (int j = 0; j < K; j++)
        {
            if (ab_book[i].name[j] != search_name[j])
            {
                equal = 0;
                break;
            }

            if (search_name[j] == '\0')
                break;
        }

        if (equal && ab_book[i].name[0] != '\0')
        {
            printf("Абонент %d: %s %s, Тел: %s\n",
                   i + 1,
                   ab_book[i].name,
                   ab_book[i].second_name,
                   ab_book[i].tel);

            found = 1;
        }
    }

    if (!found)
    {
        printf("Абонентов с таким именем не найдено.\n");
    }
}

void print_all(struct abonent ab_book[], int count)//вывод справочника 
{
    int empty = 1;

    for (int i = 0; i < count; i++)
    {
        if (ab_book[i].name[0] != '\0')
        {
            printf("%d) %s %s, Тел: %s\n",
                   i + 1,
                   ab_book[i].name,
                   ab_book[i].second_name,
                   ab_book[i].tel);

            empty = 0;
        }
    }

    if (empty)
    {
        printf("Справочник пуст.\n");
    }
}