#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "vector.h"

int main()
{
    char sort_option;
    char name[MAX_NAME_LENGTH];
    float price;
    int n, qtd, sales;

    scanf("%d", &n);

    Vector *products = vector_construct();

    for (int i = 0; i < n; ++i)
    {
        scanf("\n%[^\n]\n", name);
        scanf("%f", &price);
        scanf("%d", &qtd);
        scanf("%d", &sales);

        Product *product = product_constructor(name, price, qtd);
        // product_print(product);
        product_sell(product, sales);
        vector_push_back(products, product);
    }

    scanf("\n%c", &sort_option);

    if (sort_option == 'N')
        vector_sort(products, product_compare_name);
    else if (sort_option == 'P')
        vector_sort(products, product_compare_price);
    else if (sort_option == 'S')
        vector_sort(products, product_compare_sales);

    for (int i = 0; i < n; ++i)
        product_print(vector_get(products, i));

    for (int i = 0; i < n; ++i)
        product_destructor(vector_get(products, i));

    vector_destroy(products);

    return 0;
}
