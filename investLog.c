#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
void printStocks(Stock stocks[], int count);
int compareStocksByName(const void *company1_name, const void *company2_name);
int compareStocksByQuantity(const void *stock1_quantity, const void *stock2_quantity);
void sortStocksByName(Stock stocks[], int count);
void calculateAndPrintDividendYield(Stock stocks[], int count);
double calculateAverageSellPrice(Stock stocks[], int count);
void printTopThreeStockPositions(Stock stocks[], int count);
void addStocksEntry(Stock stocks[], int *count);
void showMainMenu(Stock stocks[], int count);

int main() {
    Stock stocks[MAX_STOCKS];
    int count = 0;

    readStocksFromFile("stocks.txt", stocks, &count);

    printf("Unsorted stocks: \n");
    printStocks(stocks, count);
    printf("\n");

    sortStocksByName(stocks, count);

    printf("Sorted stocks: \n");
    printStocks(stocks, count);
    printf("\n");

    double averageSellPrice = calculateAverageSellPrice(stocks, count);
    printf("The average sale price is: %.2f \n", averageSellPrice);
    printf("\n");


    printf("Top Three Stock Positions: \n");
    printTopThreeStockPositions(stocks, count);
    printf("\n");

    printf("The stocks have the following dividend yields: \n");
    calculateAndPrintDividendYield(stocks, count);

    printf("\n");
    addStocksEntry(stocks, &count);

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

    if (*count > 0 && line[strlen(line) - 1] != '\n') {
        strncat(stocks[*count - 1].investor_name, "\n", MAX_NAME_SIZE - 1);
    }

    fclose(file);
}

void printStocks(Stock stocks[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s;%s;%d;%.2f;%.2f;%s;%s",
               stocks[i].isin, stocks[i].company_name, stocks[i].quantity,
               stocks[i].price, stocks[i].dividend, stocks[i].date,
               stocks[i].investor_name);
    }
}

int compareStocksByName(const void *company1_name, const void *company2_name) {
        return strcmp(((Stock *)company1_name)->company_name, ((Stock *)company2_name)->company_name);
    }

void sortStocksByName(Stock stocks[], int count) {
    qsort(stocks, count, sizeof(Stock), compareStocksByName);
}

double calculateAverageSellPrice(Stock stocks[], int count) {
    int totalSaleAmount = 0;
    double totalSaleValue = 0.0;

    for (int i = 0; i < count; i++) {
        if (stocks[i].quantity < 0) {
            totalSaleAmount += abs(stocks[i].quantity);
            totalSaleValue += (abs(stocks[i].quantity) * stocks[i].price);
        }
    }

    if (totalSaleAmount > 0) {
        return totalSaleValue / totalSaleAmount;
    } else {
        return 0.0;
    }
}

int compareStocksByQuantity(const void *stock1_quantity, const void *stock2_quantity) {
    int quantity_stock1 = ((Stock *)stock1_quantity)->quantity;
    int quantity_stock2 = ((Stock *)stock2_quantity)->quantity;

    if (quantity_stock1 < quantity_stock2) {
        return 1;
    } else if (quantity_stock1 > quantity_stock2) {
        return -1;
    } else {
        return 0;
    }
}

void printTopThreeStockPositions(Stock stocks[], int count) {
    qsort(stocks, count, sizeof(Stock), compareStocksByQuantity);

    int maxPrint = count < 3 ? count : 3;
    for (int i = 0; i < maxPrint; i++) {
        printf("%s;%s;%d;%.2f;%.2f;%s;%s",
               stocks[i].isin, stocks[i].company_name, stocks[i].quantity,
               stocks[i].price, stocks[i].dividend, stocks[i].date,
               stocks[i].investor_name);
    }
}

void calculateAndPrintDividendYield(Stock stocks[], int count) {
    for (int i = 0; i < count; i++) {
        Stock *currentStock = &stocks[i];

        double dividend_yield = (currentStock->dividend * 100) / currentStock->price;

        printf("%s;%s;%d;%.2f;%.2f;%s;%s -> Dividend yield: %.2f%%\n",
               currentStock->isin, currentStock->company_name, currentStock->quantity,
               currentStock->price, currentStock->dividend, currentStock->date,
               currentStock->investor_name, dividend_yield);
    }
}