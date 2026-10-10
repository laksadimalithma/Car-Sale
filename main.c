#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//CONSTANTS (Max limits for arrays)
#define MAX_CARS 7
#define MAX_SALES 100
#define MAX_NAME 50
#define MAX_FEEDBACK 200

//CAR DATA
char models[MAX_CARS][20]={"BMW","Audi","Toyota","Honda","Nissan","Suzuki","Mazda"};
int prices[MAX_CARS]={30000,28000,25000,22000,15000,20000,18000};
int years[MAX_CARS]={2021,2022,2023,2020,2019,2024,2018};
int stock[MAX_CARS]={5,5,8,5,5,8,10};

//SALES DATA ARRAYS
char buyer_Name[MAX_SALES][MAX_NAME];
int buyer_Age[MAX_SALES];
char purchased_Model[MAX_SALES][20];
int purchased_Qty[MAX_SALES];
float purchase_Total[MAX_SALES];
char purchase_Date[MAX_SALES][20];
int saleCount=0;
int customer_Rating[MAX_SALES];
char customer_Feedback[MAX_SALES][MAX_FEEDBACK];

//FUNCTION DECLARATIONS
void loadSales();
void saveSales();
void DisplayStock();
void sortCarsByYear();
void buyingCar();
void DisplaySalesData();
int validateInt(char prompt[],int low,int high);
void saveFeedback();
void viewFeedback();
void rebuildStockFromSales();

//MAIN MENU LOOP
int main(void) {
    int option;
    loadSales();
    rebuildStockFromSales();

    printf("\nWELCOME TO THE PRIME CAR DEALERS\n");

    while (1) {
        printf("\n========PRIME CAR DEALERS SYSTEM========\n");
        printf("1. Display Stock\n");
        printf("2. Buying Car\n");
        printf("3. Display Sales Data\n");
        printf("4. Customer Feedback\n");
        printf("5. Exiting Program\n");

        option=validateInt("Enter Choice: ",1,5);
        switch (option) {
            case 1:
                DisplayStock();
                break;
            case 2:
                buyingCar();
                break;
            case 3:
                DisplaySalesData();
                break;
            case 4:
                viewFeedback();
                break;
            case 5:
                saveSales();
                saveFeedback();
                printf("\nExiting program!Come again.Goodbye\nThank you\n");
                return 0;;
            default:
                printf("\nInvalid Selection.Enter again!");
                break;
        }
    }
}
//VALIDATED INTEGER INPUT
//Ensures user enters int only and within range
int validateInt(char prompt[],int low, int high) {
    int index;
    char term;
    while (1) {
        printf("%s",prompt);
        //Scanf ensures integer
        if (scanf("%d%c",&index,&term) != 2 || term !='\n'){
            printf("Invalid Input!Enter numbers only\n");
            while (getchar()!='\n');
        }else if (index<low || index>high) {
            printf("Input out of range\n");
        }else {
            return index;
        }
    }
}
//SHOW STOCK(SORTED BY YEAR DESC)
//Sorts before printing
void DisplayStock() {
    sortCarsByYear();
    printf("\n==========CAR STOCK(Sorted by Year DESC==========)\n");
    printf("| %-6s | %-12s | %-5s | %-12s | %-6s |\n",
           "No", "Model", "Year", "Price($)", "Stock");
    printf("=====================================================\n");

    for (int i = 0; i < MAX_CARS; i++) {
        printf("| %-6d | %-12s | %-5d | %-12d | %-6d |\n",
               i + 1,models[i],years[i],prices[i],stock[i]);
    }

    printf("==================================================\n");
}
void rebuildStockFromSales() {
    // reset stock to original values
    int initialStock[MAX_CARS] = {5,5,8,5,5,8,10};
    memcpy(stock, initialStock, sizeof(stock));

    // subtract quantities from past sales
    for (int i = 0; i < saleCount; i++) {
        for (int l = 0; l < MAX_CARS; l++) {
            if (strcmp(purchased_Model[i], models[l]) == 0) {
                stock[l] -= purchased_Qty[i];
            }
        }
    }
}
//SORTS CARS BY YEAR DESCENDING ORDER
//Swap model,price,year,stock
void sortCarsByYear() {
    for (int i=0;i<MAX_CARS-1;i++) {
        for (int l=i+1;l<MAX_CARS;l++) {
            if (years[i]<years[l]) {
                //Swap years
                int temYear=years[i];
                years[i]=years[l];
                years[l]=temYear;
                //Swap prices
                int temPrice=prices[i];
                prices[i]=prices[l];
                prices[l]=temPrice;
                //Swap stock
                int temStock=stock[i];
                stock[i]=stock[l];
                stock[l]=temStock;
                //Swap model name
                char tempModel[20];
                strcpy(tempModel,models[i]);
                strcpy(models[i],models[l]);
                strcpy(models[l],tempModel);
            }
        }
    }
}
//BUY CAR AND STORE DETAILS
void buyingCar() {
    DisplayStock();
    int carChoice = validateInt("\nSelect a car to buy(1-7):",1,7);
    carChoice--;
    if (stock[carChoice]==0) {
        printf("\n You have no cars left\n");
        return;
    }
    printf("Enter customer name:");
    scanf(" %49[^\n]", buyer_Name[saleCount]);

    buyer_Age[saleCount]=validateInt("Enter customer age(18-100):",18,100);

    purchased_Qty[saleCount]=validateInt("Enter quantity:",1,stock[carChoice]);

    stock[carChoice]-=purchased_Qty[saleCount];

    //Save selected model
    strcpy(purchased_Model[saleCount],models[carChoice]);

    //Calculate price
    float total=prices[carChoice]*purchased_Qty[saleCount];

    //Apply discount for buy more than 2 cars
    float discount=0;
    if (purchased_Qty[saleCount]>2) {
        discount = 10.0f;
        total*= (1.0f- discount/100.0f);
    }
    purchase_Total[saleCount]=total;

    time_t t = time(NULL);
    struct tm *time_information = localtime(&t);
    strftime(purchase_Date[saleCount], 20, "%d/%m/%Y %H:%M:%S", time_information);


    printf("Rate your purchase experience(1-5):");
    customer_Rating[saleCount]=validateInt("",1,5);

    printf("Leave a short comment:");
    scanf(" %199[^\n]",customer_Feedback[saleCount]);

    saleCount++;
    saveSales(); //Save to sales.txt
    saveFeedback(); //Save to feedback.txt

    printf("Purchase successful Total:$%.2f\n",total);
}
//SHOW CAR SALES DATA
void DisplaySalesData() {
    if (saleCount == 0) {
        printf("\nNo sales are available now.\n");
        return;
    }
    printf("\n============================= SALES STOCK SUMMARY =============================\n");
    printf("| %-3s | %-12s | %-3s | %-12s | %-3s | %-12s | %-20s | %-3s |\n",
           "No", "Customer", "Age", "Model", "Qty", "Total($)", "Date & Time", "Rate");
    printf("=========================================================================\n");

    for (int i = 0; i < saleCount; i++) {
        printf("| %-3d | %-12s | %-3d | %-12s | %-3d | %-12.2f | %-20s | %-3d |\n",
               i + 1,buyer_Name[i],buyer_Age[i],purchased_Model[i],purchased_Qty[i],purchase_Total[i],purchase_Date[i],
               customer_Rating[i]);
    }

    printf("========================================================================\n");

    int idx[MAX_SALES];
    for (int m = 0; m < saleCount; m++) idx[m] = m;

    // Sort index array by purchaseTotal descending
    for (int m = 0; m < saleCount - 1; m++) {
        for (int l = m + 1; l < saleCount; l++) {
            if (purchase_Total[idx[m]] < purchase_Total[idx[l]]) {
                int temp = idx[m];
                idx[m] = idx[l];
                idx[l] = temp;
            }
        }
    }
    //sortSalesDescending();
    printf("\n====================== SALES SORTED BY REVENUE ======================\n");
    printf("| %-3s | %-12s | %-12s | %-3s | %-12s |\n",
           "No", "Customer", "Model", "Qty", "Revenue($)");
    printf("======================================================================\n");

    for (int i = 0; i < saleCount; i++) {
        int m = idx[i];
        printf("| %-3d | %-12s | %-12s | %-3d | %-12.2f |\n",
               i + 1,buyer_Name[m],purchased_Model[m],purchased_Qty[m],purchase_Total[m]);
    }

    printf("=====================================================================\n");
}
//SAVES DATA TO SALES FILE
void saveSales() {
    FILE *sp=fopen("sales.txt","w");
    if (!sp) {
        perror("File is not found(save)");
        return;
    }
    fprintf(sp, "%d\n",saleCount);
    for (int i=0;i<saleCount;i++) {
        fprintf(sp, "%s,%d,%s,%d,%.2f,%s,%d,%s\n",
            buyer_Name[i],buyer_Age[i],purchased_Model[i],
            purchased_Qty[i],purchase_Total[i],purchase_Date[i],
            customer_Rating[i],customer_Feedback[i]);
    }
    fclose(sp);
}
//LOAD DATA TO SALES FILE(if exists)
void loadSales() {
    FILE *sp=fopen("sales.txt","r");
    if (!sp) {
        return;
    }
    fscanf(sp,"%d\n",&saleCount);

    char text[450];
    for (int i=0;i<saleCount;i++) {
        if (fgets(text,sizeof(text),sp)==NULL)
            break;
        sscanf(text,"%[^,],%d,%[^,],%d,%f,%[^,],%d,%[^\n]",
        buyer_Name[i],&buyer_Age[i],purchased_Model[i],
        &purchased_Qty[i],&purchase_Total[i],purchase_Date[i],
        &customer_Rating[i],customer_Feedback[i]);
    }
    fclose(sp);
}
//SAVE FEEDBACK TO FILE
void saveFeedback() {
    FILE *sp=fopen("feedback.txt","w");
    if (!sp) {
        printf(" Feedback file not found\n");
        return;
    }
    for (int i=0;i<saleCount;i++) {
        fprintf(sp,"Customer: %s\nModel: %s\nRating: %d/5\nFeedback: %s\n\n",
            buyer_Name[i],purchased_Model[i],customer_Rating[i],customer_Feedback[i]);
    }
    fclose(sp);
}
//DISPLAY FEEDBACK FILE DATA
void viewFeedback() {
    FILE *sp=fopen("feedback.txt","r");
    if (!sp) {
        printf("Feedback.txt not found\n");
        return;
    }
    char text[350];
    printf("\n===CONTENTS OF feedback.txt===\n");
    while (fgets(text,sizeof(text),sp)) {
        printf("%s",text);
    }
    fclose(sp);
}
