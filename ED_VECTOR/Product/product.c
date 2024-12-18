#include "product.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 64

struct product
{
    char* name;
    float price;
    float discount;
    int qtd;
    int sales;
};

Product* product_constructor(const char *name, float price, int qtd)
{
    if ( price < 0 || qtd < 0)
    {
        printf("VALOR INVALIDO\n");
        return NULL;
    }

    Product* product = (Product* ) calloc(1, sizeof(Product));
    product -> name = (char* ) calloc(MAX_NAME_LENGTH,sizeof(char));
    product -> price = price;
    product -> qtd = qtd;
    product -> sales = 0;
    product -> discount = 0;

    strncpy(product -> name, name, MAX_NAME_LENGTH);
    if (product -> name != NULL )
    {
        if (product -> name[MAX_NAME_LENGTH] != '\0') product -> name[MAX_NAME_LENGTH] = '\0';
        for (int i = 0; i < MAX_NAME_LENGTH; i++)
        {
            if (product -> name[i] == '\n' ) product -> name[i] ='\0';
        }
    }

    return product;

}

void product_destructor(Product* product)
{
    if ( product != NULL )
    {
        free(product->name);
        free(product);
    }
}

const char* product_get_name(Product* product)
{
    if ( product != NULL ) return product->name;
    return NULL;
}

float product_get_price(Product* product)
{
    if ( product != NULL ) return product->price;
    return -1;
}

float product_get_discount(Product* product)
{
    if ( product != NULL ) return product->discount;
    return -1;
}

int product_get_qtd(Product* product)
{
    if ( product != NULL ) return product->qtd;
    return -1;
}

int product_get_desconto(Product* product)
{
    if ( product != NULL ) return product->sales;
    return -1;
}

void product_set_name(Product *product, const char *name)
{
    if ( product != NULL )
    {
        free(product->name);
        strncpy(product -> name, name, MAX_NAME_LENGTH);
        if (product -> name[MAX_NAME_LENGTH] != '\0') product -> name[MAX_NAME_LENGTH] = '\0';
    }
}

void product_set_price(Product* product,float price)
{
    if ( product != NULL )
    {
        if ( price >= 0 ) product->price = price;

        else printf("VALOR INVALIDO\n");
    }
}

void product_set_discount(Product* product,float discount)
{
    if ( product != NULL )
    {
        if ( discount >= 0 && discount <= 1) product->discount = discount;

        else printf("VALOR INVALIDO\n");
    }
}

void product_sell(Product* product, int quantity)
{
    if ( product != NULL )
    {
        if ( quantity >= 0 )
        {
            if (quantity > product->qtd) printf("ESTOQUE INSUFICIENTE\n");

            else
            {
                product->sales += quantity;
                product->qtd -= quantity;
            }
        }

        else printf("QUANTIDADE INVALIDA\n");
    }
}

void product_buy(Product *product, int quantity)
{
    if ( product != NULL )
    {
        if ( quantity > 0 )product -> qtd += quantity;

        else printf("QUANTIDADE INVALIDA\n");
    }
}

float product_get_price_with_discount(Product *product)
{
    return product->price - product->price * product->discount;
}

void product_print(Product *product)
{
    printf("Product(%s, %.2f, %.2f, %.2f, %d, %d)\n",product->name,product->price,product->discount,(product->price) - product->price * product->discount, product->qtd, product->sales);
}

int product_compare_name(void* a, void* b)
{
    Product* product_a = (Product *)a;
    Product* product_b = (Product *)b;

    return strcmp(product_a->name,product_b->name);
}

int product_compare_price(void* a, void* b)
{
    Product* product_a = (Product *)a;
    Product* product_b = (Product *)b;

    if ( product_a->price < product_b->price ) return -1;
    else if ( product_a->price > product_b->price ) return 1;
    
    return 0;
}

int product_compare_sales(void* a, void* b)
{
    Product* product_a = (Product *)a;
    Product* product_b = (Product *)b;

    if ( product_a->sales < product_b->sales ) return -1;
    else if ( product_a->sales > product_b->sales ) return 1;
    
    return 0;
}