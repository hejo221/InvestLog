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

void clearConsole() {
#ifdef _WIN32
    system("cls"); 
#else
    system("clear");
#endif
}

void readStocksFromFile(const char *filename, Stock stocks[], int *count);
void printStocks(Stock stocks[], int count);
int compareStocksByName(const void *company1_name, const void *company2_name);
int compareStocksByQuantity(const void *stock1_quantity, const void *stock2_quantity);
void sortStocksByName(Stock stocks[], int count);
void calculateAndPrintDividendYield(Stock stocks[], int count);
double calculateAverageSalePrice(Stock stocks[], int count);
void printTopThreeStockPositions(Stock stocks[], int count);
void addStocksEntry(Stock stocks[], int *count);
void showMainMenu(Stock stocks[]);

int main() {
    Stock stocks[MAX_STOCKS];

    showMainMenu(stocks);

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
        printf("ISIN: %s; Company: %s; Quantity: %d; Price: %.2f; Dividend: %.2f; Date: %s; Investor: %s",
               stocks[i].isin, stocks[i].company_name, stocks[i].quantity,
               stocks[i].price, stocks[i].dividend, stocks[i].date,
               stocks[i].investor_name);
    }

    showMainMenu(stocks);
}

int compareStocksByName(const void *company1_name, const void *company2_name) {
        return strcmp(((Stock *)company1_name)->company_name, ((Stock *)company2_name)->company_name);
    }

void sortStocksByName(Stock stocks[], int count) {
    qsort(stocks, count, sizeof(Stock), compareStocksByName);
}

double calculateAverageSalePrice(Stock stocks[], int count) {
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

    showMainMenu(stocks);
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
    Stock stocksCopy[MAX_STOCKS];
    memcpy(stocksCopy, stocks, sizeof(Stock) * count);

    qsort(stocksCopy, count, sizeof(Stock), compareStocksByQuantity);


    int maxPrint = count < 3 ? count : 3;
    for (int i = 0; i < maxPrint; i++) {
        printf("ISIN: %s; Company: %s; Quantity: %d; Price: %.2f; Dividend: %.2f; Date: %s; Investor: %s",
               stocksCopy[i].isin, stocksCopy[i].company_name, stocksCopy[i].quantity,
               stocksCopy[i].price, stocksCopy[i].dividend, stocksCopy[i].date,
               stocksCopy[i].investor_name);
    }

    showMainMenu(stocks);
}

void calculateAndPrintDividendYield(Stock stocks[], int count) {
    for (int i = 0; i < count; i++) {
        Stock *currentStock = &stocks[i];

        double dividend_yield = (currentStock->dividend * 100) / currentStock->price;

        printf("ISIN: %s; Company: %s; Quantity: %d; Price: %.2f; Dividend: %.2f; Date: %s; Investor: %s -> Dividend yield: %.2f%%\n",
               currentStock->isin, currentStock->company_name, currentStock->quantity,
               currentStock->price, currentStock->dividend, currentStock->date,
               currentStock->investor_name, dividend_yield);
    }

    showMainMenu(stocks);
}

void addStocksEntry(Stock stocks[], int *count) {
    if (*count >= MAX_STOCKS) {
        fprintf(stderr, "Maximum number of stocks reached. Increase MAX_STOCKS if needed.\n");
        return;
    }

    printf("\nEnter stock details:\n");

    while (1) {
        printf("ISIN: ");
        scanf("%s", stocks[*count].isin);

        if (strlen(stocks[*count].isin) == 12) {
            break;
        } else {
            printf("Invalid input. Please enter a valid 12-character ISIN.\n");
            while (getchar() != '\n');
        }
    }

    printf("Company Name: ");
    scanf("%s", stocks[*count].company_name);

    printf("Quantity: ");
    while (scanf("%d", &stocks[*count].quantity) != 1) {
        printf("Invalid input. Please enter a valid integer for Quantity.\n");
        while (getchar() != '\n');
        printf("Quantity: ");
    }

    while (1) {
        printf("Price: ");
        if (scanf("%lf", &stocks[*count].price) == 1 &&
            stocks[*count].price >= 0.0) {
            break;
        }

        printf("Invalid input. Please enter a non-negative number for Dividend.\n");
        while (getchar() != '\n');
    }

    while (1) {
        printf("Dividend: ");
        if (scanf("%lf", &stocks[*count].dividend) == 1 &&
            stocks[*count].dividend >= 0.0) {
            break;
        }

        printf("Invalid input. Please enter a non-negative number for Dividend.\n");
        while (getchar() != '\n');
    }

    while (1) {
        printf("Date (DD.MM.YYYY): ");
        char input[MAX_DATE_SIZE];

        if (scanf("%10s", input) == 1 && getchar() == '\n') {
            if (strlen(input) <= MAX_DATE_SIZE - 1) {
                int day, month, year;

                if (sscanf(input, "%2d.%2d.%4d", &day, &month, &year) == 3 &&
                    day >= 1 && day <= 31 &&
                    month >= 1 && month <= 12 &&
                    year >= 1900 && year <= 2100) {

                    strcpy(stocks[*count].date, input);

                    break;
                } else {
                    printf("Invalid date. Please enter a valid date in the DD.MM.YYYY format.\n");
                }
            } else {
                printf("Input too long. Please enter a date in the DD.MM.YYYY format.\n");
            }
        } else {
            printf("Invalid input. Please enter a valid date in the DD.MM.YYYY format.\n");
            while (getchar() != '\n');
        }
    }

    printf("Investor Name: ");
    scanf("%s", stocks[*count].investor_name);

    (*count)++;

    FILE *file = fopen("stocks.txt", "a");
    if (file != NULL) {
        fprintf(file, "\n%s;%s;%d;%.2f;%.2f;%s;%s",
                stocks[*count-1].isin, stocks[*count-1].company_name,
                stocks[*count-1].quantity, stocks[*count-1].price,
                stocks[*count-1].dividend, stocks[*count-1].date,
                stocks[*count-1].investor_name);

        fclose(file);

        printf("Stock entry added successfully.\n");
    } else {
        printf("Error opening file.\n");
    }

    showMainMenu(stocks);
}

void showMainMenu(Stock stocks[]) {
    int choice;
    int count;

    do {
        printf("\nMain Menu:\n");
        printf("1. Print Stocks\n");
        printf("2. Sort Stocks by Company Name\n");
        printf("3. Calculate Average Sale Price\n");
        printf("4. Print Top Three Stock Positions\n");
        printf("5. Calculate and Print Dividend Yield\n");
        printf("6. Add Stock Entry\n");
        printf("0. Exit\n");

        printf("Enter your choice (0-6): ");
        scanf("%d", &choice);

        count = 0;
        readStocksFromFile("stocks.txt", stocks, &count);

        switch (choice) {
            case 1:
                clearConsole();
                printf("\nSaved Stocks:\n");
                printStocks(stocks, count);
                break;
            case 2:
                clearConsole();
                printf("\nSorting stocks by name...\n");
                sortStocksByName(stocks, count);
                printf("\nSorted stocks:\n");
                printStocks(stocks, count);
                break;
            case 3:
                clearConsole();
                printf("\nCalculating average sale price...\n");
                double averageSalePrice = calculateAverageSalePrice(stocks, count);
                printf("\nThe average sale price is: %.2f \n", averageSalePrice);
                break;
            case 4:
                clearConsole();
                printf("\nPrinting top three stock positions...\n");
                printf("\nThe Top Three Stock Positions are:\n");
                printTopThreeStockPositions(stocks, count);
                break;
            case 5:
                clearConsole();
                printf("\nCalculating and printing dividend yield...\n");
                calculateAndPrintDividendYield(stocks, count);
                break;
            case 6:
                clearConsole();
                printf("\nAdding stock entry...\n");
                addStocksEntry(stocks, &count);
                break;
            case 0:
                clearConsole();
                printf("\nExiting program. Goodbye!\n");
                exit(0);
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 0 and 6.\n");
        }

    } while (choice != 0);
}