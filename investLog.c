#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STOCKS 1024
#define MAX_LINE_SIZE 1024
#define MAX_NAME_SIZE 256
#define MAX_ISIN_SIZE 13
#define MAX_DATE_SIZE 11

// Structure to store information about a stock
typedef struct {
    char company_name[MAX_NAME_SIZE];
    char isin[MAX_ISIN_SIZE];
    int quantity; 
    double price;
    double dividend;
    char date[MAX_DATE_SIZE];
    char investor_name[MAX_NAME_SIZE];
} Stock;

// Function to clear the console screen
void clearConsole() {
#ifdef _WIN32
    system("cls"); 
#else
    system("clear");
#endif
}

// Function to read stock information from a file
void readStocksFromFile(const char *filename, Stock stocks[], int *count);

// Function to print information about all stocks
void printStocks(Stock stocks[], int count);

// Function to compare stocks by company name for qsort
int compareStocksByName(const void *company1_name, const void *company2_name);

// Function to compare stocks by quantity for qsort
int compareStocksByQuantity(const void *stock1_quantity, const void *stock2_quantity);

// Function to sort stocks by company name
void sortStocksByName(Stock stocks[], int count);

// Function to calculate and print dividend yield for each stock
void calculateAndPrintDividendYield(Stock stocks[], int count);

// Function to calculate average sale price
double calculateAverageSalePrice(Stock stocks[], int count);

// Function to print the top three stock positions
void printTopThreeStockPositions(Stock stocks[], int count);

// Function to add a new stock entry
void addStocksEntry(Stock stocks[], int *count);

// Function to display the main menu and handle user input
void showMainMenu(Stock stocks[]);

// Main function
int main() {
    Stock stocks[MAX_STOCKS];

    showMainMenu(stocks);

    return 0;
}


// Function to read stock information from a file
void readStocksFromFile(const char *filename, Stock stocks[], int *count) {
    // Open the file for reading
    FILE *file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_SIZE];

    // Read each line from the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Tokenize the line using semicolon as a delimiter
        char *token = strtok(line, ";");

        // Copy ISIN value to the stocks array
        strncpy(stocks[*count].isin, token, MAX_ISIN_SIZE - 1);
        token = strtok(NULL, ";");

        // Copy company name value to the stocks array
        strncpy(stocks[*count].company_name, token, MAX_NAME_SIZE - 1);
        token = strtok(NULL, ";");

        // Convert quantity from string to integer and store in the stocks array
        stocks[*count].quantity = atoi(token);
        token = strtok(NULL, ";");

        // Convert price from string to double and store in the stocks array
        stocks[*count].price = atof(token);
        token = strtok(NULL, ";");

        // Convert dividend from string to double and store in the stocks array
        stocks[*count].dividend = atof(token);
        token = strtok(NULL, ";");

        // Copy date value to the stocks array
        strncpy(stocks[*count].date, token, MAX_DATE_SIZE - 1);
        token = strtok(NULL, ";");

        // Copy investor name value to the stocks array
        strncpy(stocks[*count].investor_name, token, MAX_NAME_SIZE - 1);

        (*count)++;  // Increment the count of stocks

        // Check if the maximum number of stocks has been reached
        if (*count >= MAX_STOCKS) {
            fprintf(stderr, "Maximum number of stocks reached. Increase MAX_STOCKS if needed.\n");
            break;
        }
    }

    // If the last line doesn't end with a newline character, add one to the last investor name
    if (*count > 0 && line[strlen(line) - 1] != '\n') {
        strncat(stocks[*count - 1].investor_name, "\n", MAX_NAME_SIZE - 1);
    }

    // Close the file
    fclose(file);
}

// Function to print information about all stocks
void printStocks(Stock stocks[], int count) {
    // Loop through each stock and print its information
    for (int i = 0; i < count; i++) {
        printf("ISIN: %s; Company: %s; Quantity: %d; Price: %.2f; Dividend: %.2f; Date: %s; Investor: %s",
               stocks[i].isin, stocks[i].company_name, stocks[i].quantity,
               stocks[i].price, stocks[i].dividend, stocks[i].date,
               stocks[i].investor_name);
    }

    // Go back to main menu
    showMainMenu(stocks);
}

// Function to compare stocks by company name for qsort
int compareStocksByName(const void *company1_name, const void *company2_name) {
    // Compare the company names of two stocks and return the result
    return strcmp(((Stock *)company1_name)->company_name, ((Stock *)company2_name)->company_name);
}

// Function to sort stocks by company name using qsort
void sortStocksByName(Stock stocks[], int count) {
    qsort(stocks, count, sizeof(Stock), compareStocksByName);
}

// Function to calculate average sale price
double calculateAverageSalePrice(Stock stocks[], int count) {
    int totalSaleAmount = 0;
    double totalSaleValue = 0.0;

    // Iterate through each stock
    for (int i = 0; i < count; i++) {
        // Check if the stock has a negative quantity (sale)
        if (stocks[i].quantity < 0) {
            // Accumulate the total sale quantity and value
            totalSaleAmount += abs(stocks[i].quantity);
            totalSaleValue += (abs(stocks[i].quantity) * stocks[i].price);
        }
    }

    // Calculate and return the average sale price if there were sales
    if (totalSaleAmount > 0) {
        return totalSaleValue / totalSaleAmount;
    } else {
        return 0.0;
    }

    // Go back to main menu
    showMainMenu(stocks);
}

// Function to compare stocks by quantity for qsort
int compareStocksByQuantity(const void *stock1_quantity, const void *stock2_quantity) {
    // Extract quantity values from the two stocks
    int quantity_stock1 = ((Stock *)stock1_quantity)->quantity;
    int quantity_stock2 = ((Stock *)stock2_quantity)->quantity;

    // Compare quantities and return the result
    if (quantity_stock1 < quantity_stock2) {
        return 1;
    } else if (quantity_stock1 > quantity_stock2) {
        return -1;
    } else {
        return 0;
    }
}

// Function to print the top three stock positions
void printTopThreeStockPositions(Stock stocks[], int count) {
    Stock stocksCopy[MAX_STOCKS];
    // Create a copy of the stocks array
    memcpy(stocksCopy, stocks, sizeof(Stock) * count);

    // Sort the copied array by quantity using compareStocksByQuantity function
    qsort(stocksCopy, count, sizeof(Stock), compareStocksByQuantity);

    // Determine the maximum number of stocks to print (up to 3)
    int maxPrint = count < 3 ? count : 3;

    // Print information about the top three stock positions
    for (int i = 0; i < maxPrint; i++) {
        printf("ISIN: %s; Company: %s; Quantity: %d; Price: %.2f; Dividend: %.2f; Date: %s; Investor: %s",
               stocksCopy[i].isin, stocksCopy[i].company_name, stocksCopy[i].quantity,
               stocksCopy[i].price, stocksCopy[i].dividend, stocksCopy[i].date,
               stocksCopy[i].investor_name);
    }

    // Go back to main menu
    showMainMenu(stocks); 
}

// Function to calculate and print dividend yield for each stock
void calculateAndPrintDividendYield(Stock stocks[], int count) {
    // Iterate through each stock
    for (int i = 0; i < count; i++) {
        Stock *currentStock = &stocks[i];

        // Calculate dividend yield
        double dividend_yield = (currentStock->dividend * 100) / currentStock->price;

        // Print information about the stock and its dividend yield
        printf("ISIN: %s; Company: %s; Quantity: %d; Price: %.2f; Dividend: %.2f; Date: %s; Investor: %s -> Dividend yield: %.2f%%\n",
               currentStock->isin, currentStock->company_name, currentStock->quantity,
               currentStock->price, currentStock->dividend, currentStock->date,
               currentStock->investor_name, dividend_yield);
    }

    // Go back to main menu
    showMainMenu(stocks);
}

// Function to add a new stock entry
void addStocksEntry(Stock stocks[], int *count) {
    // Check if the maximum number of stocks has been reached
    if (*count >= MAX_STOCKS) {
        fprintf(stderr, "Maximum number of stocks reached. Increase MAX_STOCKS if needed.\n");
        return;
    }

    printf("\nEnter stock details:\n");

    // Input ISIN, ensuring it is a valid 12-character ISIN
    while (1) {
        printf("ISIN: ");
        scanf("%s", stocks[*count].isin);

        // Check if ISIN is 12 characters long
        if (strlen(stocks[*count].isin) == 12) {
            break;
        } else {
            printf("Invalid input. Please enter a valid 12-character ISIN.\n");
            while (getchar() != '\n');  // Clear input buffer
        }
    }

    // Input company name
    printf("Company Name: ");
    scanf("%s", stocks[*count].company_name);

    // Input quantity, ensuring it is a valid integer
    printf("Quantity: ");
    while (scanf("%d", &stocks[*count].quantity) != 1) {
        printf("Invalid input. Please enter a valid integer for Quantity.\n");
        while (getchar() != '\n');  // Clear input buffer
        printf("Quantity: ");
    }

    // Input price, ensuring it is a non-negative number
    while (1) {
        printf("Price: ");
        if (scanf("%lf", &stocks[*count].price) == 1 &&
            stocks[*count].price >= 0.0) {
            break;
        }

        printf("Invalid input. Please enter a non-negative number for Price.\n");
        while (getchar() != '\n');  // Clear input buffer
    }

    // Input dividend, ensuring it is a non-negative number
    while (1) {
        printf("Dividend: ");
        if (scanf("%lf", &stocks[*count].dividend) == 1 &&
            stocks[*count].dividend >= 0.0) {
            break;
        }

        printf("Invalid input. Please enter a non-negative number for Dividend.\n");
        while (getchar() != '\n');  // Clear input buffer
    }

    // Input date in the DD.MM.YYYY format, ensuring it is valid
    while (1) {
        printf("Date (DD.MM.YYYY): ");
        char input[MAX_DATE_SIZE];

        if (scanf("%10s", input) == 1 && getchar() == '\n') {
            if (strlen(input) <= MAX_DATE_SIZE - 1) {
                int day, month, year;

                // Validate and parse the date
                if (sscanf(input, "%2d.%2d.%4d", &day, &month, &year) == 3 &&
                    day >= 1 && day <= 31 &&
                    month >= 1 && month <= 12 &&
                    year >= 1900 && year <= 2100) {

                    // Copy the validated date to the stocks array
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
            while (getchar() != '\n');  // Clear input buffer
        }
    }

    // Input investor name
    printf("Investor Name: ");
    scanf("%s", stocks[*count].investor_name);

    (*count)++;  // Increment the count of stocks

    // Open the file for appending
    FILE *file = fopen("stocks.txt", "a");
    if (file != NULL) {
        // Append the new stock entry to the file
        fprintf(file, "\n%s;%s;%d;%.2f;%.2f;%s;%s",
                stocks[*count-1].isin, stocks[*count-1].company_name,
                stocks[*count-1].quantity, stocks[*count-1].price,
                stocks[*count-1].dividend, stocks[*count-1].date,
                stocks[*count-1].investor_name);

        // Close the file
        fclose(file);

        printf("Stock entry added successfully.\n");
    } else {
        printf("Error opening file.\n");
    }

    // Go back to main menu
    showMainMenu(stocks);
}

// Function to display the main menu and handle user input
void showMainMenu(Stock stocks[]) {
    int choice;
    int count;

    do {
        // Display the main menu options
        printf("\nMain Menu:\n");
        printf("1. Print Stocks\n");
        printf("2. Sort Stocks by Company Name\n");
        printf("3. Calculate Average Sale Price\n");
        printf("4. Print Top Three Stock Positions\n");
        printf("5. Calculate and Print Dividend Yield\n");
        printf("6. Add Stock Entry\n");
        printf("0. Exit\n");

        // Prompt the user for their choice
        printf("Enter your choice (0-6): ");
        scanf("%d", &choice);

        count = 0;
        readStocksFromFile("stocks.txt", stocks, &count);  // Read stocks from file

        // Perform the action based on the user's choice
        switch (choice) {
            case 1:
                clearConsole();
                printf("\nSaved Stocks:\n");
                printStocks(stocks, count);  // Print all saved stocks
                break;
            case 2:
                clearConsole();
                printf("\nSorting stocks by name...\n");
                sortStocksByName(stocks, count);  // Sort stocks by company name
                printf("\nSorted stocks:\n");
                printStocks(stocks, count);  // Print the sorted stocks
                break;
            case 3:
                clearConsole();
                printf("\nCalculating average sale price...\n");
                double averageSalePrice = calculateAverageSalePrice(stocks, count);  // Calculate average sale price
                printf("\nThe average sale price is: %.2f \n", averageSalePrice);  // Print the result
                break;
            case 4:
                clearConsole();
                printf("\nPrinting top three stock positions...\n");
                printf("\nThe Top Three Stock Positions are:\n");
                printTopThreeStockPositions(stocks, count);  // Print top three stock positions
                break;
            case 5:
                clearConsole();
                printf("\nCalculating and printing dividend yield...\n");
                calculateAndPrintDividendYield(stocks, count);  // Calculate and print dividend yield
                break;
            case 6:
                clearConsole();
                printf("\nAdding stock entry...\n");
                addStocksEntry(stocks, &count);  // Add a new stock entry
                break;
            case 0:
                clearConsole();
                printf("\nExiting program. Goodbye!\n");
                exit(0);  // Exit the program
                break;
            default:
                clearConsole();
                printf("\nInvalid choice. Please enter a number between 0 and 6.\n");
        }

    } while (choice != 0);  // Continue the loop until the user chooses to exit
}