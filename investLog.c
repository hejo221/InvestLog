#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1024
#define MAX_COMPANY_NAME_SIZE 256
#define MAX_ISIN_SIZE 13
#define MAX_DATE_SIZE 11
#define MAX_NAME_SIZE 256

typedef struct {
    char company_name[MAX_COMPANY_NAME_SIZE];
    char isin[MAX_ISIN_SIZE];
    int quantity; 
    double price;
    double dividend;
    char date[MAX_DATE_SIZE];
    char investor_name[MAX_NAME_SIZE];
} Stock;

void readStocksFromFile(const char *filename, Stock stocks[], int *count);

void sortStocksByName(Stock stocks[], int count);

void printSortedStockNames(Stock stocks[], int count);

double calculateDividendYield(double price, double dividend);

void printDividendYield(Stock stocks[], int count);

double calculateAverageSellPrice(Stock stocks[], int count);

void printTopThreeBuyingPositions(Stock stocks[], int count);

void addStocksEntry(const char *filename);

void showMainMenu(Stock stocks[], int count);

int main() {
    return 0;
}