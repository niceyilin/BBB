#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//char LED_PATH[]="/sys/class/leds/beaglebone:green:usr3";
#define LED_PATH "/sys/class/leds/beaglebone:green:usr3"

void writeLedFile(char filename[], char value[]);
void removeTrigger();
void readLedFile(char filename[]);

int main (int argc, char *argv[]){
    
    // puts (argv[0]);  it's ./LED
    
    if (argc != 2){
        printf("Error : wrong usage\n"); 
        printf("./BashLED.sh followed by one of these cmd...\n-on\n-off\n-flash\n-status\ne.g. ./LED on\n");
        exit(-1);
    }
    
    if (strcmp(argv[1], "on") == 0){
        printf("turn on LED\n");
        removeTrigger();
        writeLedFile("brightness","1");
    }
    else if (strcmp(argv[1], "off") == 0){
        printf("turn off LED\n");
        removeTrigger();
        writeLedFile("brightness","0");
    }
    else if (strcmp(argv[1], "flash") == 0){
        printf("flash LED\n");
        writeLedFile("trigger","timer");
        writeLedFile("delay_on","50");
        writeLedFile("delay_off","50");
    }
    else if (strcmp(argv[1], "status") == 0){
        printf("return status of LED\n");
        readLedFile("trigger");
    }
    else{
        printf("Error:invalid cmd! argv[1]=%s\n", argv[1]);
        exit(2);
    }

    printf("job is done\n");
    return 0;
}

void writeLedFile(char filename[], char value[]){
    FILE* fp;
    char fullfilename[100];
    sprintf(fullfilename, LED_PATH "/%s", filename);  //alternative for string concatenate 

    printf("opening file %s, writing %s\n", fullfilename, value);
    fp = fopen(fullfilename, "w");
    if (fp == NULL){
        perror("Error: can't open file!\n");
    }
    fprintf(fp, "%s", value);
    fclose(fp);
}

void removeTrigger(){
    writeLedFile("trigger","none");
}

void readLedFile(char filename[]){
    FILE* fp;
    char fullfilename[100];
    char line[100];
    strcpy(fullfilename, LED_PATH);
    strcat(fullfilename, "/");
    strcat(fullfilename, filename);
    printf("opening file %s to read\n", fullfilename);
    fp = fopen(fullfilename, "r");
    if(fp == NULL){
       perror("Error: can't open file!"); 
    }
    while(fgets(line, 80, fp) != NULL){
        //puts (line);
        printf("%s", line);
    }
    fclose(fp);
}
