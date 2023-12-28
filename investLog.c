#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STOCKS 1024
#define MAX_LINE_SIZE 1024
#define MAX_NAME_SIZE 256
#define MAX_ISIN_SIZE 13
#define MAX_DATE_SIZE 11

typedef struct {
    char company_name[MAX_NAME_SIZE];
    char isin[MAX_ISIN_SIZE];
    int quantity; 
    double price;
    double dividend;
    char date[MAX_DATE_SIZE];
    char investor_name[MAX_NAME_SIZE];
} Stock;

void readStocksFromFile(const char *filename, Stock stocks[], int *count);

void printUnsortedStocks(Stock stocks[], int count);

void sortStocksByName(Stock stocks[], int count);

void printSortedStocks(Stock stocks[], int count);

double calculateDividendYield(double price, double dividend);

void printDividendYield(Stock stocks[], int count);

double calculateAverageSellPrice(Stock stocks[], int count);

void printTopThreeStocksPositions(Stock stocks[], int count);

void addStocksEntry(const char *filename);

void showMainMenu(Stock stocks[], int count);

int main() {
    Stock stocks[MAX_STOCKS];
    int count = 0;

    readStocksFromFile("stocks.txt", stocks, &count);

    printf("Unsorted Stocks:\n");
    printUnsortedStocks(stocks, count);

    return 0;
}

void readStocksFromFile(const char *filename, Stock stocks[], int *count) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_SIZE];

    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ";");

        strncpy(stocks[*count].isin, token, MAX_ISIN_SIZE - 1);
        token = strtok(NULL, ";");

        strncpy(stocks[*count].company_name, token, MAX_NAME_SIZE - 1);
        token = strtok(NULL, ";");

        stocks[*count].quantity = atoi(token);
        token = strtok(NULL, ";");

        stocks[*count].price = atof(token);
        token = strtok(NULL, ";");

        stocks[*count].dividend = atof(token);
        token = strtok(NULL, ";");

        strncpy(stocks[*count].date, token, MAX_DATE_SIZE - 1);
        token = strtok(NULL, ";");

        strncpy(stocks[*count].investor_name, token, MAX_NAME_SIZE - 1);

        (*count)++;

        if (*count >= MAX_STOCKS) {
            fprintf(stderr, "Maximum number of stocks reached. Increase MAX_STOCKS if needed.\n");
            break;
        }
    }

    fclose(file);
}

void printUnsortedStocks(Stock stocks[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s;%s;%d;%.2f;%.2f;%s;%s",
               stocks[i].isin, stocks[i].company_name, stocks[i].quantity,
               stocks[i].price, stocks[i].dividend, stocks[i].date,
               stocks[i].investor_name);
    }
}