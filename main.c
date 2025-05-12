#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ADMIN_ID "Admin"
#define ADMIN_PASS "Ad.123"


typedef struct user_information{
    char name[30];
    char city[20];
}info;

typedef struct weather_information{
    int temp, humidity, aqi, uv, wspeed;
}winfo;


int user_login(info *user);
int admin_login(void);
void weather(winfo *report);
int save(info *user, winfo *report);
int random_range(int min, int max);
void display(info *user, winfo *report);
void w_history(void );
void u_history(void);
void entries(void);


int main(){
    srand(time(NULL));
    info user;
    winfo report;
    int check, sign, choice, save_check;

    while(1){
        int  flag1 = 1, flag2 = 1;
        printf("Press... 0 if you are Admin, 1 otherwise: ");
        scanf("%d",&sign);

        if(sign == 0){
        check = admin_login(); 
        if (check == 0){
            while(flag1 == 1){
                printf("\nPress any one option to continue...\n1. View Total Entries\n2. View Weather History\n3. View User's History\n4. Exit\nEnter your choice: ");
                scanf("%d",&choice);
                switch(choice){
                    case 1: entries(); break;
                    case 2: w_history(); break;
                    case 3: u_history(); break;
                    default: flag1 = 0 ; break;
                }
            }
        }
        }

        else if(sign == 1){
            check = user_login(&user);
            if (check == 0){

                weather(&report);

                save_check = save(&user, &report);
                if(save_check == 1){
                    printf("Error saving file\n\n");
                    return 1;
                }
                else{
                    printf("File saved successfully\n\n");
                }

                while(flag2 == 1){
                    printf("\nPress any one option to continue...\n1. Display Weather Report\n2. View Weather History\n3. Exit\nEnter your choice: ");
                    scanf("%d",&choice);
                    switch(choice){
                        case 1: display(&user, &report); break;
                        case 2: w_history(); break;
                        default: flag2 = 0; break;
                    }
                }
            }
        }
        else{
            printf("Invalid Option\n");
        }
    }
    return 0;
}


int user_login(info *user){
    int pass;
    printf("Enter the password: ");
    scanf("%d",&pass);
    if(pass != 12345){
        printf("Invalid Password. Access Denied\n");
        return 1;
    }
    printf("Access granted.\n\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Enter your name: ");
    fgets(user->name,sizeof(user->name),stdin);
    user->name[strcspn(user->name, "\n")] = 0;

    printf("Enter the name of the city: ");
    scanf("%s",user->city);
    printf("\n\n");
    return 0;
}

int random_range(int min, int max){
    return (rand() % (max - min + 1) + min);
}

void weather(winfo *report){
    report->temp = random_range(10,35);
    report->humidity = random_range(25,50);
    report->aqi = random_range(30,115);
    report->uv = random_range(1,10);
    report->wspeed = random_range(10,25);
}


int save(info *user, winfo *report){
    time_t t = time(NULL);
    struct tm date = *localtime(&t);

    FILE *ufile, *wfile, *idfile;
    int lastid = 1; 
    idfile = fopen("id.txt","r");
    if(idfile == NULL){
        lastid = 1;
    }
    else{
        fscanf(idfile,"%d",&lastid);
        fclose(idfile);
    }

    ufile =  fopen("user.txt","a");
    if(ufile == NULL){
        printf("Error opening the file\n\n");
        return 1;
    }
    fprintf(ufile,"User ID: %d | Name: %s | City: %s | Date: %d-%02d-%02d | Time: %02d: %02d\n"
    , lastid, user->name, user->city, date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min);
    fclose(ufile);

    wfile = fopen("weather.txt","a");
    if(wfile == NULL){
        printf("Error opening the file\n\n");
        return 1;
    }
    fprintf(wfile,"User ID: %d | City: %s | Temperature: %d deg C | Humidity: %d%% | AQI: %d | UV: %d | Wind Speed: %d km/h | Date: %d-%02d-%02d | Time: %02d: %02d\n",
    lastid, user->city, report->temp, report->humidity, report->aqi, report->uv, report->wspeed, date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min);
    fclose(wfile);

    idfile = fopen("id.txt","w");
    if(idfile == NULL){
        printf("Error opening the file\n\n");
        return 1;
    }
    fprintf(idfile,"%d", lastid + 1);
    fclose(idfile);
    return 0;
}


void display(info *user, winfo *report){
    printf("User Name: %s | City: %s | Temperature: %d deg C | Humidity: %d%% | AQI: %d | UV: %d | Wind Speed: %d km/h \n\n",
    user->name, user->city, report->temp, report->humidity, report->aqi, report->uv, report->wspeed);
    return;
}


void w_history(void){
    FILE *file;
    file = fopen("weather.txt","r");
    char line[250];
    if(file == NULL){
        printf("Error opening file\n\n");
        return;
    }

    printf("\n\n------------------------------Weather History-----------------------------\n\n");
    while(fgets(line, sizeof(line), file)){
        printf("%s",line);
    }
    fclose(file);
    printf("\n");
    return; 
}


void u_history(void){
    FILE *file;
    file = fopen("user.txt","r");
    char line[250];
    if(file == NULL){
        printf("Error opening file\n");
        return;
    }

    printf("\n\n------------------------------User's History-----------------------------\n\n");
    while(fgets(line, sizeof(line), file)){
        printf("%s",line);
    }
    fclose(file);
    printf("\n");
    return;
}

int admin_login(void){
    char a_id[10];
    char a_pass[10];
    printf("Enter admin id: ");
    scanf("%s",a_id);
    printf("Enter password: ");
    scanf("%s",a_pass);

    if(strcspn(a_id, ADMIN_ID) == 0 && strcspn(a_pass, ADMIN_PASS) == 0){
        printf("Admin access granted\n\n");
        return 0;
    }
    printf("Admin access denied\n\n");
    return 1;
}


void entries(void){
    FILE *fp;
    int u_lines = 0;
    int w_lines = 0;
    char c;

    fp = fopen("user.txt","r");
    if (fp == NULL){
        printf("Error opening the file\n\n");
        return;
    }
    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            u_lines++;
        }
    }
    fclose(fp);

    fp = fopen("weather.txt","r");
    if (fp == NULL){
        printf("Error opening the file\n\n");
        return;
    }
    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            w_lines++;
        }
    }
    fclose(fp);

    printf("Total User Enteries are: %d\nTotal Weather Entries are:%d\n\n\n",u_lines,w_lines);
    return;
}