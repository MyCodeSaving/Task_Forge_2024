#include <stdio.h>
#include <string.h>

struct product {
    int prono;
    char fullname[20];
    float price;
} products[20];

struct sale {
    int prono;
    char month[10];
    int sales_volume;
    char discount[20];
} salelist[100];

// Function prototypes
struct product* sort_records_price();
struct sale* sort_sales_month();
void display_products(struct product* (*func_sort)());
void display_sales(struct sale* (*func_sort)());
void calculate_sales_volume();
void output_sales_data();
void output_top_3_products();

// Read product data from file
void read_products() {
    printf("// Reading Product Data from File...\n");
    FILE *file = fopen("product.txt", "r");
    if (!file) {
        printf("Error: Unable to open product.txt\n");
        return;
    }

    char header[100]; // Buffer to store the first line
    fgets(header, sizeof(header), file); // Skip the first line

    int i = 0;
    while (fscanf(file, "%d %s %f", &products[i].prono, products[i].fullname, &products[i].price) == 3) {
        printf("// %d %s %f\n", products[i].prono, products[i].fullname, products[i].price);
        i++;
    }
    fclose(file);
}

// Read sales data from file
void read_sales() {
    printf("// Reading Sales Data from File...\n");
    FILE *file = fopen("salelist.txt", "r");
    if (!file) {
        printf("Error: Unable to open salelist.txt\n");
        return;
    }

    char header[100]; // Buffer to store the first line
    fgets(header, sizeof(header), file); // Skip the first line

    int i = 0;
    while (fscanf(file, "%d %s %d %s", &salelist[i].prono, salelist[i].month, &salelist[i].sales_volume, salelist[i].discount) == 4) {
        printf("// %d %s %d %s\n", salelist[i].prono, salelist[i].month, salelist[i].sales_volume, salelist[i].discount);
        i++;
    }
    fclose(file);
}

// Sort product records by price in ascending order
struct product* sort_records_price() {
    printf("// Sorting Product Records by Price...\n");
    static struct product sorted[20];
    memcpy(sorted, products, sizeof(products));

    for (int i = 0; i < 20 - 1; i++) {
        for (int j = 0; j < 20 - i - 1; j++) {
            if (sorted[j].price > sorted[j + 1].price) {
                struct product temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    return sorted;
}

// Sort sales records by month in alphabetical order
struct sale* sort_sales_month() {
    printf("// Sorting Sales Records by Month...\n");
    static struct sale sorted[100];
    memcpy(sorted, salelist, sizeof(salelist));

    for (int i = 0; i < 100 - 1; i++) {
        for (int j = 0; j < 100 - i - 1; j++) {
            if (strcmp(sorted[j].month, sorted[j + 1].month) > 0) {
                struct sale temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    return sorted;
}

// Display sorted product list
void display_products(struct product* (*func_sort)()) {
    struct product *sorted = func_sort();
    printf("Sorted Products:\n");
    for (int i = 0; i < 20; i++) {
        if (sorted[i].prono != 0)
            printf("Product No: %d, Name: %s, Price: %.2f\n", sorted[i].prono, sorted[i].fullname, sorted[i].price);
    }
}

// Display sorted sales list
void display_sales(struct sale* (*func_sort)()) {
    struct sale *sorted = func_sort();
    printf("Sorted Sales:\n");
    for (int i = 0; i < 100; i++) {
        if (sorted[i].prono != 0) {
            char fullname_tmp[20];
            float price_tmp = 0.0;
            for (int j = 0; j < 20; j++) {
                if (sorted[i].prono == products[j].prono) {
                    strcpy(fullname_tmp, products[j].fullname);
                    price_tmp = products[j].price;
                    break;
                }
            }
            printf("%d | %s | %d | %s | %s | %.2f\n",
                sorted[i].prono, sorted[i].month, sorted[i].sales_volume, sorted[i].discount, fullname_tmp, price_tmp);
        }
    }
}

// Calculate total and average sales volume of each product
void calculate_sales_volume() {
    printf("Sales Volume and Average Sales Volume per Product:\n");
    for (int i = 0; i < 20 && products[i].prono != 0; i++) {
        int total_volume = 0, count = 0;

        for (int j = 0; j < 100 && salelist[j].prono != 0; j++) {
            if (salelist[j].prono == products[i].prono) {
                total_volume += salelist[j].sales_volume;
                count++;
            }
        }

        float average_volume = (count == 0) ? 0 : (float)total_volume / count;
        printf("Product No: %d, Total Sales Volume: %d, Average Volume: %.2f\n",
               products[i].prono, total_volume, average_volume);
    }
}

// Output all September sales with product information and write to file
void output_sales_data() {
    FILE *file = fopen("september_sales.txt", "w");
    if (!file) {
        printf("Error: Unable to create september_sales.txt\n");
        return;
    }

    printf("September Sales:\n");
    for (int i = 0; i < 100 && salelist[i].prono != 0; i++) {
        if (strcmp(salelist[i].month, "Sep.") == 0) {
            for (int j = 0; j < 20 && products[j].prono != 0; j++) {
                if (products[j].prono == salelist[i].prono) {
                    printf("%d | %s | %d | %s\n",
                           products[j].prono, products[j].fullname, salelist[i].sales_volume, salelist[i].discount);
                    fprintf(file, "%d | %s | %d | %s\n",
                            products[j].prono, products[j].fullname, salelist[i].sales_volume, salelist[i].discount);
                }
            }
        }
    }
    fclose(file);
}

// Output top 3 products based on total sales
void output_top_3_products() {
    struct product top_products[20];
    int total_sales[20] = {0};

    for (int i = 0; i < 20 && products[i].prono != 0; i++) {
        for (int j = 0; j < 100 && salelist[j].prono != 0; j++) {
            if (products[i].prono == salelist[j].prono) {
                total_sales[i] += salelist[j].sales_volume;
            }
        }
        top_products[i] = products[i];
    }

    for (int i = 0; i < 20 - 1; i++) {
        for (int j = 0; j < 20 - i - 1; j++) {
            if (total_sales[j] < total_sales[j + 1]) {
                int temp_sales = total_sales[j];
                total_sales[j] = total_sales[j + 1];
                total_sales[j + 1] = temp_sales;

                struct product temp_product = top_products[j];
                top_products[j] = top_products[j + 1];
                top_products[j + 1] = temp_product;
            }
        }
    }

    printf("Top 3 Products by Total Sales Volume:\n");
    for (int i = 0; i < 3; i++) {
        printf("Product No: %d, Name: %s, Total Sales Volume: %d\n",
               top_products[i].prono, top_products[i].fullname, total_sales[i]);
    }
}

int main() {
    read_products();
    read_sales();

    // printf("All Products:\n");
    // for (int i = 0; i < 20 && products[i].prono != 0; i++) {
    //     printf("Product No: %d, Name: %s, Price: %.2f\n", products[i].prono, products[i].fullname, products[i].price);
    // }
    // printf("All Sales:\n");
    // for (int i = 0; i < 100 && salelist[i].prono != 0; i++) {
    //     printf("Product No: %d, Month: %s, Sales Volume: %d, Discount: %s\n",
    //            salelist[i].prono, salelist[i].month, salelist[i].sales_volume, salelist[i].discount);
    // }

    display_products(sort_records_price);
    display_sales(sort_sales_month);
    calculate_sales_volume();
    output_sales_data();
    output_top_3_products();

    return 0;
}
