#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Date
{
    char day;
    char month;
    int year;
} Date;
typedef struct Person
{
    long long id;
    char* name;
    char* family;
    Date dateOfBirth;
    long long partnerId;
    long long motherId;
    long long fatherId;
    char numOfChildren;
    long long* childrenPtr;
} Person;
typedef struct db_mgr
{
    Person* people;
    int sizeOfPeople;
    int sizeOfPeopleIRT;
} db_mgr;
// כדי לדעת מה כל פונקציה עושה אנא הניחו את העכבר על שמה של הפונקציה בשורת ההכרזה כאן למטה ויופיע תיאור קצר

void print_person(const Person *p);
int init_db(db_mgr* db);
int menu();
int add_person(db_mgr* db);
void orderById(db_mgr* db , Person* p);
Person* search_id(const db_mgr* db, long long id);
void search_person(const db_mgr* db);
void search_parents(const db_mgr *db);
int delete_person(db_mgr* db);
int Delete_ChildrenFromParent(db_mgr* db ,const long long* parent ,const long long* id);
void Delete_Parent(db_mgr* db, Person* p);
void orderById2(db_mgr* db);
void print_db(const db_mgr* db);
void search_by_name(const db_mgr* db);
void quit(db_mgr* db);
int get_gen(const db_mgr* db);
int generation(const db_mgr* db, Person* p);
Person* brotherOrSister(const db_mgr*db ,const Person* p ,const long long* id);
void idCheck(long long* id , int x);
Person *FatherOrMother(long long id, Person* parents, int size);
void main()
{
    db_mgr admin;
    admin.people = NULL;
    admin.sizeOfPeopleIRT = 0;
    printf("---Welcome To Our Database---\n");
    init_db(&admin);
    int choose = menu();
     while(choose != 8)
     {
        if (choose == 1)
        {
            if(add_person(&admin) == 0)
            {
                printf("Error - out of memory");
                break;
            }
        }
        if (choose == 2)
            search_person(&admin);
        if (choose == 3)
            search_parents(&admin);
        if (choose == 4)
        {
            if(delete_person(&admin) ==0)
            {
                printf("Error - out of memory");
                break;
            }
        }
        if (choose == 5)
        {

            if (get_gen(&admin) == 0)
            {
                printf("Error - out of memory");
                break;
            }
        }
        if (choose == 6)
            print_db(&admin);
        if (choose == 7)
            search_by_name(&admin);
        choose = menu();
     }
     quit(&admin);
}
//פונקציה שנועדה להקצאת זיכרון
int init_db(db_mgr* db)
{
    Person* tmp = NULL;
    if (db->sizeOfPeopleIRT == 0)
    {
        printf("Please enter how many people you want to Add:\n");
        scanf("%d", &(db->sizeOfPeople));
        while (db->sizeOfPeople < 0)
        {
            printf("Error-please enter again:\n");
            scanf("%d", &(db->sizeOfPeople));
        }
        db->people = (Person*)calloc(db->sizeOfPeople , sizeof(Person));
        if (!db->people)
        {
            printf("Error - out of memory");
            return 0;
        }
        return 1;
    }
    else
    {
        int x = 0;
        if (db->sizeOfPeople == db->sizeOfPeopleIRT)
            x = 1;
        else
            db->sizeOfPeopleIRT--;
        tmp = (Person*)calloc(db->sizeOfPeople , sizeof(Person));
        if (!tmp)
        {
            printf("Error - out of memory");
            return 0;
        }
        int k = 0;
        for (int i = 0; i < db->sizeOfPeople - x; i++)
        {
            if (db->people[i].id != 0)
                tmp[k++] = db->people[i];
        }
        free(db->people);
        db->people = tmp;
        tmp = NULL;
        return 1;
    }
}
//פונקציה שנועדה להדפיס את כל פרטי אותו אדם
void print_person(const Person* p)
{
    printf("The id number is: %lld\n", p->id);
    printf("The name is: %s\n", p->name);
    printf("The last name is: %s\n", p->family);
    printf("The date of birth is: %d/%d/%d\n", p->dateOfBirth.day, p->dateOfBirth.month, p->dateOfBirth.year);
    if (p->partnerId != 0)
        printf("The partner id is: %lld\n", p->partnerId);
    if (p->motherId != 0)
        printf("The mother id is: %lld\n", p->motherId);
    if (p->fatherId != 0)
        printf("The father id is: %lld\n", p->fatherId);
    printf("The number of children is: %d\n", p->numOfChildren);
    for (int i = 0; i < p->numOfChildren; i++)
        printf("the number id for children %d is: %lld\n", (i + 1), p->childrenPtr[i]);
    printf("---------------------------------------\n");
}
//פונקציה שנועדה להצגת תפריט פעולות
int menu()
{
    int num;
    do
    {
        printf("Database system menu:\n");
        printf("1. Add person\n");
        printf("2. Search a person\n");
        printf("3. Search parents\n");
        printf("4. Delete a person\n");
        printf("5. Get generation\n");
        printf("6. Print database\n");
        printf("7. Search by name\n");
        printf("8. Quit\n");
        printf("Please choose your operation\n");
        scanf("%d", &num);
        if(num > 8 || num < 1)
            printf("Error : please enter again\n");
    }while (num > 8 || num < 1);
    return num;
}
//פונקציה להוספת אדם למערך התושבים
int add_person(db_mgr* db)
{
    long long id;
    printf("Please enter id number : \n");
    scanf("%lld", &id);
    Person* p = search_id(db, id);
    while (id <= 0 || p != NULL) // לולאה הבודקת תקינות הקלט ועם כבר התעודת זהות קיימת במערכת
    {
        if(p)
            printf("Error - this id has already taken, please enter again : \n");
        else
            printf("Error - please enter again : \n");
        scanf("%lld", &id);
        p = search_id(db, id);
    }
    char str[20];
    db->sizeOfPeopleIRT++;
    if(db->sizeOfPeopleIRT > db->sizeOfPeople)
    {
        db->sizeOfPeople++;
        int control = init_db(db);
        if (control == 0)
        {
            db->sizeOfPeople--;
            db->sizeOfPeopleIRT--;
            printf("Error - out of memory\n");
            return 0;
        }
    }
    int i = (db->sizeOfPeopleIRT)-1, childrens;
    db->people[i].id = id;
    printf("Please enter the first name\n");
    scanf("%s", str);
    db->people[i].name=(char*)malloc((strlen(str)+1) * sizeof(char));
    if (db->people[i].name == 0)
    {
        db->sizeOfPeopleIRT--;
        return 0;
    }
    strcpy(db->people[i].name, str);
    printf("Please enter the last name\n");
    scanf("%s", str);
    db->people[i].family=(char*)malloc((strlen(str) + 1) * sizeof(char));
    if (db->people[i].family == 0)
    {
        db->sizeOfPeopleIRT--;
        free(db->people[i].name);
        db->people[i].name = NULL;
        return 0;
    }
    strcpy(db->people[i].family, str);
    printf("Please enter the date of birth (example:1/1/2000)\n");
    scanf("%d/%d/%d",&(db->people[i].dateOfBirth.day), &(db->people[i].dateOfBirth.month), &(db->people[i].dateOfBirth.year));
    while (db->people[i].dateOfBirth.day > 30 || db->people[i].dateOfBirth.day <= 0 || db->people[i].dateOfBirth.month > 12 || db->people[i].dateOfBirth.month <= 0|| db->people[i].dateOfBirth.year <= 0 ||db->people[i].dateOfBirth.year > 10000)
    {
        printf("Wrong date - please enter again :");
        scanf("%d/%d/%d", &(db->people[i].dateOfBirth.day), &(db->people[i].dateOfBirth.month), &(db->people[i].dateOfBirth.year));
    }
    printf("Please enter your parnter id (if you don't have press 0):\n");
    idCheck(&(db->people[i].partnerId), -1);
    printf("Please enter your mother id (if you don't have press 0):\n");
    idCheck(&(db->people[i].motherId), -1);
    printf("Please enter your father id (if you don't have press 0):\n");
    idCheck(&(db->people[i].fatherId), -1);
    printf("How many children do you have? (if you don't have press 0) \n");
    scanf("%d", &childrens);
    while (childrens < 0)
    {
        printf("Error - please enter again : \n");
        scanf("%d", &childrens);
    }
    db->people[i].numOfChildren = childrens;
    if (childrens > 0)
        db->people[i].childrenPtr = (long long*)malloc(childrens * sizeof( long long));
    if (db->people[i].childrenPtr == NULL && childrens > 0)
    {
        db->sizeOfPeopleIRT--;
        free(db->people[i].name);
        db->people[i].name = NULL;
        free(db->people[i].family);
        db->people[i].family = NULL;
        return 0;
    }
    for (int n = 0; n < childrens; n++)
    {
        printf("Please enter id number for child %d : \n", n+1);
        idCheck(&(db->people[i].childrenPtr[n]) , 0);
    }
    orderById(db,&(db->people[i]));
    printf("---------------------------------------\n");
    printf("The addition succeed :) \n");
    printf("---------------------------------------\n");
    return 1;
}
// פונקציה שמכניסה את הבן אדם למקומו המתאים למערך שכבר ממוין
void orderById(db_mgr* db , Person* p)
{
   Person tmp;
   for (int k = 0; k < db->sizeOfPeopleIRT ; k++)
        {
            if (p->id < db->people[k].id)
            {
                tmp = db->people[k];
                db->people[k] = *p;
                *p = tmp;
            }
            if (k == db->sizeOfPeopleIRT - 1)
                db->people[k] = *p;
        }
}
//פונקציה שמחפשת ברשומות, אדם לפי תעודת הזהות שלו ומחזירה מצביע לאדם המבוקש
Person* search_id(const db_mgr* db,  long long id)
{
    for (int i = 0; i < db->sizeOfPeopleIRT; i++)
    {
        if (db->people[i].id ==  id)
            return &(db->people[i]);
    }
    return NULL;
}
// פונקציה שבודקת אם קיים אדם כזה
void search_person(const db_mgr* db)
{
    Person* p;
    long long id;
    printf("Please enter id number:\n");
    idCheck(&id ,0);
    p = search_id(db, id);
    printf("---------------------------------------\n");
    if (p)
    {
        printf("We found the person with the id you entered, and the details is : \n");
        print_person(p);
    }
    else
    {
        printf("There is no person with this id in our data-base\n");
        printf("---------------------------------------\n");
    }
    
}
//פונקציה שנועדה לחפש האם קיימים לאדם במערכת הורים,ואם כן להדפיס את פרטיהם
void search_parents(const db_mgr* db)
{
    long long id;
    printf("Please enter id number of the child : \n");
    idCheck(&id , 0);
    Person *p = search_id(db , id);
    printf("---------------------------------------\n");
    if (!p)
    {
        printf("There is no person with this id in our data-base\n");
        printf("---------------------------------------\n");
        return;
    }
    if (p->fatherId == 0 && p->motherId == 0)
    {
        printf("Both parents are not registred in our data-base\n");
        printf("---------------------------------------\n");
    }
    else
    {
        if(search_id(db, p->fatherId) != NULL)
        {
            printf("The details of the father is : \n");
            print_person(search_id(db, p->fatherId));
        }
        else
        {
            printf("There is no detalis on the father in our data-base\n");
            printf("---------------------------------------\n");
        }
        if (search_id(db, p->motherId) != NULL)
        {
            printf("The details of the mother is : \n");
            print_person(search_id(db, p->motherId));
        }
        else
        {
            printf("There is no detalis on the mother in our data-base\n");
            printf("---------------------------------------\n");
        }
    }
}
//פונקציה שנועדה למחוק אדם שקיים במערכת
int delete_person(db_mgr* db)
{
    long long id;
    Person temp;
    printf("Please enter number the id person you want to delete : \n");
    idCheck(&id , 0);
    Person* p = search_id(db, id);
    printf("---------------------------------------\n");
    if (!p)
    {
        printf("There is no person with this id in our data-base\n");
        printf("---------------------------------------\n");
        return 1;
    }
    free(p->name);
    p->name = NULL;
    free(p->family);
    p->family = NULL;
    if (p->partnerId != 0)
    {
        Person *p1 = search_id(db, p->partnerId);
        if (p1)
            p1->partnerId = 0;
    }
    if (p->fatherId != 0)
    {
        if (Delete_ChildrenFromParent(db ,&(p->fatherId), &(p->id)) == 0)
            return 0;
    }
    if (p->motherId != 0)
    {
        if (Delete_ChildrenFromParent(db , &(p->motherId) , &(p->id)) == 0)
            return 0;
    }
    if (p->numOfChildren > 0)
        Delete_Parent(db,p);
    db->sizeOfPeople--;
    temp = db->people[db->sizeOfPeople];
    db->people[db->sizeOfPeople] = *p;
    *p = temp;
    int control1 = init_db(db);
    if(control1 == 0)
        return 0;
    orderById2(db);
    printf("The erasure succeed :)\n");
    printf("---------------------------------------\n");
    return 1;
}
//פונקציה שמוחקת את הילד מההורה שלו. כך שבהדפסת ההורה הילד אינו יופיע.
int Delete_ChildrenFromParent(db_mgr* db ,const long long* parent ,const long long* id)
{
    Person* p = search_id(db, *parent);
    if (!p)
        return 1;
    for (int i = 0; i < p->numOfChildren; i++)
    {
        if (p->childrenPtr[i] == *id)
        {
            p->childrenPtr[i] = 0;
            break;
        }
    }
    p->numOfChildren--;
    long long* temp = (long long*)malloc(p->numOfChildren * sizeof(long long));
    if (!temp)
        return 0;
    int k = 0;
    for (int i = 0; i < p->numOfChildren+1; i++)
    {
        if (p->childrenPtr[i] != 0)
            temp[k++] = p->childrenPtr[i];
    }
    free(p->childrenPtr);
    p->childrenPtr = temp;
    temp = NULL;
    return 1;
}
//פונקציה שמוחקת הורה מהרשומה של הילד שלו. ההורה אינו יופיע בהדפסת הילד כי ההורה נמחק מהרשומות
void Delete_Parent(db_mgr* db,Person* p)
{
    for(int i = 0; i < p->numOfChildren; i++)
    {
        Person *p1=search_id(db, p->childrenPtr[i]);
        if (p1!= 0)
        {
            if (p1->fatherId == p->id)
                p1->fatherId = 0;
            else
                p1->motherId = 0;
        }
    }
    free(p->childrenPtr);
    p->childrenPtr = NULL;
}
//פונקציה הממיינת את מערך האנשים לפי סדר עולה של התעודות זהות שלהם
void orderById2(db_mgr* db)
{
    long long min = db->people[0].id;
    Person tmp;
    for (int i = 0; i < db->sizeOfPeopleIRT; i++)
    {
        for (int j = 0; j+i < db->sizeOfPeopleIRT; j++)
        {
          if (db->people[j+i].id < min)
            {
               min = db->people[j+i].id;
               tmp = db->people[i];
               db->people[i] = db->people[j+i];
               db->people[j+i] = tmp;
            }
        }
        min = db->people[i+1].id;
    }
}
//פונקציה המדפיסה את כל הרשומות שקיימות
void print_db(const db_mgr* db)
{
    printf("There is %d people in our data-base : \n",db->sizeOfPeopleIRT);
    printf("---------------------------------------\n");
    for (int i = 0; i < db->sizeOfPeopleIRT; i++)
    {
        printf("Person number %d :\n",i+1);
        print_person(&(db->people[i]));
    }
}
//פונקציה המחפשת שם מלא ומדפיסה את כל האנשים עם השם המלא שנקלט
void search_by_name(const db_mgr* db)
{
    char str1[20];
    printf("Please enter the first name\n");
    scanf("%s", str1);
    char str2[20];
    printf("Please enter me the last name\n");
    scanf("%s", str2);
    int counter = 0;
    printf("---------------------------------------\n");
    for (int i = 0; i < db->sizeOfPeopleIRT; i++)
    {
        if(strcmp(str1, db->people[i].name) == 0 && strcmp(str2, db->people[i].family) == 0)
        {
            print_person(&(db->people[i]));
            counter++;
        }
    }
    if (counter == 0)
        printf("No people with full name like you entered in our data-base\n");
    else printf("We found %d people with the full name you entered\n", counter);
    printf("---------------------------------------\n");
}
// פוקנציית שחרור - פונקציה המשחררת את כל הזיכרון הדינאמי
void quit(db_mgr* db)
{
    printf("The running is over, thank you :) \n");
    for (int i = 0; i < db->sizeOfPeopleIRT; i++)
    {
        free(db->people[i].name);
        db->people[i].name = NULL;
        free(db->people[i].family);
        db->people[i].family = NULL;
        if (db->people[i].numOfChildren > 0)
        {
            free(db->people[i].childrenPtr);
            db->people[i].childrenPtr = NULL;
        }
    }
    free(db->people);
    db->people = NULL;
}
// מחזירה את מספר הדורות של הבן אדם המבוקש
int get_gen(const db_mgr* db)
{
    Person* p;
    long long  id;
    printf("Please enter id number : \n");
    idCheck(&id , 0);
    p = search_id(db, id);
    printf("---------------------------------------\n");
    if (!p)
    {
        printf("There is no person with this id in our data-base\n");
        return 1;
    }
    if (p->numOfChildren > 0)
    {
        int gen = generation(db, p);
        if (!gen)
            return 0;
        printf("The generation of %s %s is %d\n", p->name, p->family ,gen);
    }
    else
        printf("The generation of %s %s is 1\n", p->name, p->family);
    printf("---------------------------------------\n");
    return 1;
}
// פונקציית עזר המקבלת מצביע לבן אדם ומצביע למבנה של המנהל ומחזירה את מספר הדור של הבן אדם שקיבלנו
int generation(const db_mgr* db, Person* p)
{
    int counter = 1, maxCounter = 1, k = 0, check = 0, size = 1;
    Person* parents = NULL;
    long long id = 0;
    Person* p1 = p;
    Person parent;
    int i = 0;
        do
        {
            for (int j = 0; j < size - 1; j++) // לולאת בדיקה אם האדם הנוכחי נמצא במערך ההורים שלנו
            {
                if (parents[j].id == p1->id)
                {
                    check = 1;       // אם האדם הנוכחי נמצא אז צ׳ק שווה ל1
                    break;           // כשמצאנו אין טעם להמשיך
                }
            }
            if(p1->numOfChildren > 0 && check == 0)// אם לאדם הנוכחי יש ילדים וצ׳ק שווה ל0 ניכנס לתנאי
            {
                counter++;
                parents = (Person*)realloc(parents, size * sizeof(Person));
                if (!parents)
                    return 0;
                parents[size - 1] = *p1; // השמה של ההורה במערך ההורים במקום האחרון-החדש
                k = size - 1;
                size++;
                k++;
                i = 0;
                id = p1->childrenPtr[i];
                p1 = search_id(db ,id); // נמשיך לילד הראשון של האדם הנוכחי
                i=-1;
                if(maxCounter < counter)
                    maxCounter = counter;
            }
            else
            {
                if(parents[k-1].numOfChildren-1 <= i) // בדיקה אם האדם הנוכחי הוא הילד האחרון(אם כן נכנסים לתנאי)
                {
                    if(k-2 < 0)  // בדיקה אם יש חריגה למשתנה - אם יש חריגה אז הגענו לבן אדם שששאלנו על הדור שלו
                        p1 = p;
                    else
                    {
                        for (int j = 0; j < size - 1; j++) // חיפוש סבא/סבתא לאדם הנוכחי
                        {
                            if(parents[k-1].fatherId == parents[j].id) // חיפוש אמא במערך parents
                            {
                                parent = parents[j];
                                break;
                            }
                            if(parents[k-1].motherId == parents[j].id) // חיפוש אבא במערך parents
                            {
                                parent = parents[j];
                                break;
                            }
                        }
                        p1 = brotherOrSister(db, &parent, &parents[k - 1].id); //בודקים אם קיים אח של ההורה של האדם הנוכח
                        if(!p1) // בדיקה אם קיבלנו אח של ההורה של האדם הנוכחי
                        {
                            p1 = &parent; // לא מצאנו בן אדם כזה לכן נלך עוד דור אחורה ועכשיו נהיה בסבא/סבתא של הנוכחי
                            counter--; // יורדים דור
                        }
                        k--;
                    }
                }
                else
                    p1 = search_id(db, parents[k-1].childrenPtr[i+1]); //אין לאדם הנוכחי ילדים לכן נמשיך לאחיו/אחותו
            }
            if (!p1) // אם המצביע שלנו הוא נאל נמשיך לאח או להורה שלו
            {
                Person* parent1 = FatherOrMother(id, parents, size);
                Person* p3 = brotherOrSister(db , parent1 ,&id);  // מצביע כדי לבדוק האם קיים אח
                if (!p3) // אם לא קיים עוד אח במאגר נמשיך להורה
                {
                    p1 = parent1; // נשווה להורה
                    counter--; // נרד דור
                }
                else p1 = p3; // אם האח במאגר אז נשווה לאח
            }
            check = 0;
            i++;
        }while (p1->id != p->id);
        free(parents);
        parents = NULL;
        return maxCounter;
}
// פונקציה הבודקת אם קיים עוד אח. היא מקבלת תעודת זהות של הילד ומצביע פרסון של ההורה ואם מוצאת אח רשום במאגר היא מחזירה מצביע אליו אחרת מחזירה נאל
Person* brotherOrSister(const db_mgr*db ,const Person* p ,const long long* id)
{
    for(int i = 0; i < p->numOfChildren; i++)
    {
        if ( (p->childrenPtr[i] == *id) && (i+1 < p->numOfChildren) )
        {
            for (int j = i; j < p->numOfChildren; j++)
            {
                Person* tmp = search_id(db, p->childrenPtr[j + 1]); // מצביע לבדוק האם קיים אח גדול
                if (!tmp) // אם לא קיים אח
                    continue; // אז נמשיך לאח הבא
               return tmp; // אם מצאנו אח אז נחזיר אותו
            }
            return NULL; // לא מצאנו עוד אחים הרשומים במאגר לכן נחזיר נאל
        }
    }
    return NULL;
}
// פונקציה הבודקת את תקינות הקלט ומקבלת ארגומנט איקס שמחליט אם 0 זה תקין או לא. אם האיקס שווה ל0 אז ללא אופציית האפס. אם האיקס שווה ל1- אז אופציית ה0 מתקבלת.
void idCheck(long long* id ,int x)
{
    scanf("%lld", id);
    while(*id <= x)
    {
        printf("Error - please enter id number again : \n");
        scanf("%lld", id);
    }
}
// פונקציה המוצאת הורה לפי מספר תעודת זהות של הילד. כאשר ההורה נמצא במערך של ההורים בפונקצית דור ומחזירה כתובת של ההורה
Person* FatherOrMother(long long id ,Person* parents , int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < parents[i].numOfChildren; j++)
        {
            if (parents[i].childrenPtr[j] == id)
                return parents + i;
        }
    }
    return NULL;
}
