#include <stdio.h>
#include <stdlib.h>

#define K 50

struct abonent//структура абонента(Имя,Фамилия,телефон)
{
    char name[K];
    char second_name[K];
    char tel[K];
};

struct Node //узел двусвязного списка 
{
    struct abonent data;//сам абонент
    struct Node* next;//указатель на следующий элемент
    struct Node* prev;//указатель на предыдущий элемент
};

// прототипы ф.
void add_abonent(struct Node** head);
void delete_abonent(struct Node** head);
void search_abonent(struct Node* head);
void print_all(struct Node* head);
void free_list(struct Node* head);
void menu();
int my_strcmp(char *a, char *b);

int main()
{
    struct Node* head = NULL; // в начале списка список пуст
    int choice;//выбор пункта меню

    while (1)
    {
        menu();
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                add_abonent(&head);//добавить
                break;

            case 2:
                delete_abonent(&head);//удалить
                break;

            case 3:
                search_abonent(head);//поиск
                break;

            case 4:
                print_all(head);//вывемсти
                break;

            case 5:
                free_list(head);
                printf("Выход из программы.\n");
                return 0;

            default:
                printf("Некорректный выбор.\n");
        }
    }
}

// меню
void menu()
{
    printf("\nАбонентский справочник\n");
    printf("1 - Добавить абонента\n");
    printf("2 - Удалить абонента\n");
    printf("3 - Поиск\n");
    printf("4 - Показать всех\n");
    printf("5 - Выход\n");
    printf("Выбор: ");
}

// поиск(сравнение строк)
int my_strcmp(char *a, char *b)
{
    int i = 0;

    while (1)
    {
        if (a[i] != b[i])
            return 0;

        if (a[i] == '\0')
            return 1;

        i++;
    }
}

// добавление абон.
void add_abonent(struct Node** head)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));//новый элемент списка памяти

    printf("Введите имя: ");
    scanf("%s", newNode->data.name);

    printf("Введите фамилию: ");
    scanf("%s", newNode->data.second_name);

    printf("Введите телефон: ");
    scanf("%s", newNode->data.tel);

    newNode->next = NULL;//Обнуляем связи
    newNode->prev = NULL;

    if (*head == NULL)
    {
        *head = newNode;//если список пуст, то новый элемент становится первым
    {
    }
    else
    {
        struct Node* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode;
        newNode->prev = temp;//если не пуст, вставляем в конец
    }

    printf("Абонент добавлен.\n");
}

// удаление
void delete_abonent(struct Node** head)
{
    char name[K];
    printf("Введите имя для удаления: ");
    scanf("%s", name);

    struct Node* temp = *head;

    while (temp != NULL)
    {
        if (my_strcmp(temp->data.name, name))
        {
            if (temp->prev != NULL)
                temp->prev->next = temp->next;//если не первый элемент
            else
                *head = temp->next;//первый элемент

            if (temp->next != NULL)
                temp->next->prev = temp->prev;

            free(temp);//освобождаем память
            printf("Удалено.\n");
            return;
        }

        temp = temp->next;
    }

    printf("Не найдено.\n");
}

// поиск
void search_abonent(struct Node* head)
{
    char name[K];
    int found = 0;

    printf("Введите имя: ");
    scanf("%s", name);

    struct Node* temp = head;

    while (temp != NULL)
    {
        if (my_strcmp(temp->data.name, name))
        {
            printf("%s %s, Тел: %s\n",
                   temp->data.name,
                   temp->data.second_name,
                   temp->data.tel);

            found = 1;
        }

        temp = temp->next;
    }

    if (!found)
        printf("Не найдено.\n");
}

// вывод
void print_all(struct Node* head)
{
    if (head == NULL)
    {
        printf("Справочник пуст.\n");
        return;
    }

    struct Node* temp = head;
    int i = 1;

    while (temp != NULL)
    {
        printf("%d) %s %s, Тел: %s\n",
               i,
               temp->data.name,
               temp->data.second_name,
               temp->data.tel);

        temp = temp->next;
        i++;
    }
}

// очистка памяти
void free_list(struct Node* head)
{
    struct Node* temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
