// xodimlarni boshqarish tizimi
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

char *amallar[] = {
    "1. Xodim qo'shish;",
    "2. Xodimlar ro'yxati;",
    "3. Qidirish",
    "4. Arxivlash",
    "5. Chiqish"};

struct Foydalanuvchi
{
    char foydalanuvchi_ismi[100];
    char foydalanuvchi_paroli[100];
};

typedef struct
{
    int ID;
    char ism[40];
    char familiya[40];
    char jinsi[10];
    char tugilgan_sana[20];
    bool arxivlangan;
} Xodim;

bool foydalanuvchini_tasdiqlash(struct Foydalanuvchi *);
void ekranni_tozalash();
void xodim_qoshish();
void xodimlar_royxati();

int main()
{
    struct Foydalanuvchi foydalanuvchi;
    puts("Dasturga xush kelibsiz!");
start:
    printf("Foydalanuvchi ismingizni kiriting: ");
    scanf(" %[^\n]s", foydalanuvchi.foydalanuvchi_ismi);
    printf("Foydalanuvchi parolingizni kiriting: ");
    scanf(" %[^\n]s", foydalanuvchi.foydalanuvchi_paroli);
    bool foydalanuvchi_tasdiqlandi = foydalanuvchini_tasdiqlash(&foydalanuvchi);

    if (foydalanuvchi_tasdiqlandi)
    {
        ekranni_tozalash();
        printf("Xush kelibsiz, %s!\n", foydalanuvchi.foydalanuvchi_ismi);
        int tanlov = -1;
        while (tanlov)
        {
            printf("Menudan nima qilishni tanlang!\n");
            for (int i = 0; i < 5; i++)
            {
                printf("%s\n", amallar[i]);
            }
            scanf(" %d", &tanlov);
            switch (tanlov)
            {
            case 1:
                xodim_qoshish();
                break;
            case 2:
                xodimlar_royxati();
                break;
            case 5:
                ekranni_tozalash();
                goto start;
            default:
                break;
            }
        }
    }
    else
    {
        printf("Foydalanuvchi ismingiz va/yoki parolingiz noto'g'ri\n"
               "Qaytadan urinib ko'ring!\n");
        goto start;
    }

    return 0;
}

bool foydalanuvchini_tasdiqlash(struct Foydalanuvchi *foydalanuvchi)
{
    FILE *foydalanuvchi_fayli = fopen("xbt_adminlar_file.bin", "rb");
    struct Foydalanuvchi file_foydalanuvchi;
    fread(file_foydalanuvchi.foydalanuvchi_ismi, 100, 1, foydalanuvchi_fayli);
    fread(file_foydalanuvchi.foydalanuvchi_paroli, 100, 1, foydalanuvchi_fayli);
    fclose(foydalanuvchi_fayli);
    int fi_natija = strcmp(foydalanuvchi->foydalanuvchi_ismi, file_foydalanuvchi.foydalanuvchi_ismi);
    int fp_natija = strcmp(foydalanuvchi->foydalanuvchi_paroli, file_foydalanuvchi.foydalanuvchi_paroli);
    if (!fi_natija && !fp_natija)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ekranni_tozalash()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void xodim_qoshish()
{
    Xodim xodim;

    printf("Yangi xodim ismini kiriting: ");
    scanf(" %[^\n]s", xodim.ism);
    printf("Yangi xodim familiyasini kiriting: ");
    scanf(" %[^\n]s", xodim.familiya);
    printf("Yangi xodim jinsini kiriting: ");
    scanf(" %[^\n]s", xodim.jinsi);
    printf("Yangi xodim tug'ilgan sanasini kiriting: ");
    scanf(" %[^\n]s", xodim.tugilgan_sana);

    FILE *xodimlar = fopen("xodimlar.bin", "ab+");
    xodim.ID = -1;
    if (xodimlar == NULL)
    {
        printf("xodimlar.bin file ni ochishda xatolik yuz berdi\n");
        return;
    }
    if (fgetc(xodimlar) == EOF)
    {
        xodim.ID = 0;
    }
    else
    {
        char data[120];
        xodim.arxivlangan = false;
        fseek(xodimlar, -1, SEEK_CUR);
        while (fgetc(xodimlar) != EOF)
        {
            fseek(xodimlar, -1, SEEK_CUR);
            fread(data, 120, 1, xodimlar);
            char *token = strtok(data, ",");
            xodim.ID = atoi(token) + 1;
        }
        sprintf(data,
                "%d,%s,%s,%s,%s,%d\n",
                xodim.ID,
                xodim.ism,
                xodim.familiya,
                xodim.jinsi,
                xodim.tugilgan_sana,
                xodim.arxivlangan);
        fwrite(data, 120, 1, xodimlar);
        fclose(xodimlar);
        ekranni_tozalash();
        return;
    }
    char data[120];
    xodim.arxivlangan = false;
    sprintf(data,
            "%d,%s,%s,%s,%s,%d\n",
            xodim.ID,
            xodim.ism,
            xodim.familiya,
            xodim.jinsi,
            xodim.tugilgan_sana,
            xodim.arxivlangan);
    fwrite(data, 120, 1, xodimlar);
    fclose(xodimlar);
}

void xodimlar_royxati()
{
    Xodim xodim;

    FILE *xodimlar_file = fopen("xodimlar.bin", "rb");
    if (xodimlar_file == NULL)
    {
        puts("xodimlar.bin file ni ochish amalga oshmadi");
        return;
    }
    Xodim **xodimlar = (Xodim **)malloc(0 * sizeof(Xodim));
    char data[120];
    int xodimlar_soni = 0;
    puts("xodimlar:");
    while (fgetc(xodimlar_file) != EOF)
    {
        xodimlar = (Xodim **)realloc(xodimlar, ++xodimlar_soni * sizeof(Xodim));
        fseek(xodimlar_file, -1, SEEK_CUR);
        fread(data, 120, 1, xodimlar_file);
        xodimlar[xodimlar_soni - 1] = (Xodim *)malloc(sizeof(Xodim));
        char *ID = strtok(data, ",");
        xodimlar[xodimlar_soni - 1]->ID = atoi(ID);
        char *ism = strtok(NULL, ",");
        strcpy(xodimlar[xodimlar_soni - 1]->ism, ism);
        char *familiya = strtok(NULL, ",");
        strcpy(xodimlar[xodimlar_soni - 1]->familiya, familiya);
        char *jinsi = strtok(NULL, ",");
        strcpy(xodimlar[xodimlar_soni - 1]->jinsi, jinsi);
        char *tugilgan_sana = strtok(NULL, ",");
        strcpy(xodimlar[xodimlar_soni - 1]->tugilgan_sana, tugilgan_sana);
        char *arxivlangan = strtok(NULL, ",");
        xodimlar[xodimlar_soni - 1]->arxivlangan = (bool)atoi(arxivlangan);
    }
    fclose(xodimlar_file);

    puts("----------------------------------------------------------------------------------------------------"
         "-------------------------------------------------");
    puts("ID:\t|\tIsm:\t|\tFamiliya:\t|\tJinsi:\t|\tTug'ilgan sana:\t|\tArxivlangan:");
    puts("----------------------------------------------------------------------------------------------------"
         "-------------------------------------------------");
    for (int i = 0; i < xodimlar_soni; i++)
    {
        printf("%d\t|\t%s\t|\t%s\t|\t%s\t|\t%s\t|\t%s\n",
               xodimlar[i]->ID,
               xodimlar[i]->ism,
               xodimlar[i]->familiya,
               xodimlar[i]->jinsi,
               xodimlar[i]->tugilgan_sana,
               xodimlar[i]->arxivlangan ? "arxivlangan" : "arxivlanmagan");
        puts("----------------------------------------------------------------------------------------------------"
             "-------------------------------------------------");
    }

    for (int i = 0; i < xodimlar_soni; i++)
    {
        free(xodimlar[i]);
    }

    free(xodimlar);
}