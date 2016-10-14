#include "ppd_options.h"

/**
 * @file ppd_options.c contains main functionality
 **/

bool display_items(struct ppd_system * system)
{
    int i;
    struct ppd_node * node_pointer;
    char name_padding[NAMELEN + 1];
    int padding_length;

    
    padding_length = get_length_longest_name(system) - strlen("Name");
    set_padding(name_padding,padding_length);

    printf("ID    | Name%s | Available | Price\n",name_padding);
    puts("-------------------------------------------------");
    node_pointer = system->item_list->head;
    for(i = 0;i < system->item_list->count;i++)
    {
        padding_length = get_length_longest_name(system) - strlen(node_pointer->data->name);
        set_padding(name_padding,padding_length);

        printf("%s | ",node_pointer->data->id);
        printf("%s%s | ",node_pointer->data->name,name_padding);
        printf("%d         | ",node_pointer->data->on_hand);
        printf("$%d.%.2d\n",node_pointer->data->price.dollars,node_pointer->data->price.cents);


        node_pointer = node_pointer->next;
    }
    return true;
}


bool set_padding(char * padding,int padding_length)
{
    int i;

    padding[0] = '\0';
    for(i = 0;i < padding_length; i++)
    {
        strcat(padding," ");
    }
    return true;
}


int get_length_longest_name(struct ppd_system * system)
{
    int length_longest_name;
    struct ppd_node *current, *previous;

    length_longest_name = 0;

    /*current (list pointer) point to head*/
    previous = NULL;
    current = system->item_list->head; 

    /*traverse until alphabetically ordered position is found*/
    while(current != NULL)
    {
        if(strlen(current->data->name) > length_longest_name)
        {
            length_longest_name = strlen(current->data->name);
        }
        previous = current;
        current = current->next;
    }

    return length_longest_name;
}

/*-------------*/
bool purchase_item(struct ppd_system * system)
{
    char buffer[BUFFER_SIZE + FGETS_EXTRA_SPACES];
    int change[MAX_NUM_COINS_CHANGE];
    int change_len;
    struct ppd_stock * stock_item;
    int due;
    int i;
    int j;
    int amount_entered;
    int stock_dollars;
    int stock_cents;
    int cents_difference;
    int dollars_difference;

    printf("Purchase Item\n");
    printf("-------------\n");
    printf("Please enter the id of the item you wish to purchase: ");
    fgets(buffer,BUFFER_SIZE + FGETS_EXTRA_SPACES,stdin);
    check_buffer_overflow(buffer);
    remove_newline(buffer); /*remove newline created by fgets*/
    if(strcmp(buffer,"") == 0)
    {
        return true;
    }
    stock_item = search_stock_by_id(system,buffer);

    /*check item exist*/
    if (stock_item == NULL)
    {
        printf("Item does not exist\n");
        return true;
    }

    stock_dollars = stock_item->price.dollars;
    stock_cents = stock_item->price.cents;
    due = (stock_dollars * DOLLARS_MULTIPLE) + (stock_cents * CENTS_MULTIPLE);
    printf("You have selected \"%s - %s\". ",stock_item->name,stock_item->desc);
    printf("This will cost you $%d.%.2d.\n",stock_dollars,stock_cents);
    printf("Please hand over the money - type in the value of each note/coin in cents.\n");
    printf("Press enter on a new and empty line to cancel this purchase:\n");
    while(due > 0)
    {
        cents_difference = due%DOLLARS_MULTIPLE;
        dollars_difference = (due - cents_difference)/DOLLARS_MULTIPLE;
        printf("You still need to give us $%d.%.2d: ",dollars_difference,cents_difference);

        fgets(buffer,BUFFER_SIZE + FGETS_EXTRA_SPACES,stdin);
        check_buffer_overflow(buffer);
        remove_newline(buffer); /*remove newline created by fgets*/
        if (strcmp(buffer,"") == 0)
        {
            printf("Thank you here is your change of ");
            printf("$%d.%.2d.\n",stock_dollars - dollars_difference,stock_cents - cents_difference);
            return true;
        }
        if (check_numeric(buffer) == false)
        {
            break;
        }
        amount_entered = strtol(buffer, NULL, 0);

        switch(amount_entered)
        {
            case 1000:
            {
                due -= 1000;
                break;
            }
            case 500:
            {
                due -= 500;
                break;
            }
            case 200:
            {
                due -= 200;
                break;
            }
            case 100:
            {
                due -= 100;
                break;
            }
            case 50:
            {
                due -= 50;
                break;
            }
            case 20:
            {
                due -= 20;
                break;
            }
            case 10:
            {
                due -= 10;
                break;
            }
            case 5:
            {
                due -= 5;
                break;
            }
            default:
            {
                printf("Error: $%d.%.2d is not a valid denomination of money.",amount_entered/DOLLARS_MULTIPLE,amount_entered/CENTS_MULTIPLE);
                break;
            }
        }
        if (due < 0)
        {
            if (due == 0)
            {
                printf("Thank you. Here is your %s.\n",stock_item->name);
            }
            else
            {
                cents_difference = abs(due)%DOLLARS_MULTIPLE;
                dollars_difference = (abs(due) - cents_difference)/DOLLARS_MULTIPLE;
                printf("Thank you. Here is your %s, and your change of $%d.%.2d: ",stock_item->name,dollars_difference,cents_difference);

                i = 0;
                change_len = 0;
                while(due < 0)
                {
                    for(j = 0;j < NUM_DENOMS;j++)
                    {
                        if(system->cash_register[j].denom <= abs(due) && system->cash_register[j].count > 0)
                        {
                            break;
                        }
                    }
                    due += system->cash_register[j].denom;
                    system->cash_register[j].count--;
                    change[i] = system->cash_register[j].denom;
                    change_len++;
                    i++;
                }
                
                for(i = 0;i < change_len;i++)
                {
                    if(change[i] < DOLLARS_MULTIPLE)
                    {
                        printf("%.2dc ",change[i]);
                    }
                    else
                    {
                        printf("$%d ",(change[i]/DOLLARS_MULTIPLE));
                    }
                }
                printf("\n");
            }
            stock_item->on_hand -= 1;
            printf("Please come back soon.\n");
        }
    }
    return true;
}



/*-------------*/
bool save_and_exit_system(struct ppd_system * system)
{
    int i;
    FILE *fileptr;
    struct ppd_node * current, * previous;
    char num_string[PRICE_STR_SIZE];
    char buffer[BUFFER_SIZE + FGETS_EXTRA_SPACES];

    /*remove contents in file*/
    fileptr = fopen(system->stock_file_name,"w"); 
    fputs("",fileptr);
    fclose(fileptr);

    /*save stock to file*/
    fileptr = fopen(system->stock_file_name,"a"); 

    current = system->item_list->head; 
    previous = NULL;

    while(current != NULL)
    {
        fputs(current->data->id,fileptr);
        fputs(STOCK_DELIM,fileptr);
        fputs(current->data->name,fileptr);
        fputs(STOCK_DELIM,fileptr);
        fputs(current->data->desc,fileptr);
        fputs(STOCK_DELIM,fileptr);
        sprintf(num_string,"%d%s%d",current->data->price.dollars,PRICE_DELIM,current->data->price.cents);
        fputs(num_string,fileptr);
        fputs(STOCK_DELIM,fileptr);
        sprintf(num_string,"%d",current->data->on_hand);
        fputs(num_string,fileptr);
        fputs("\n",fileptr);

        previous = current;
        current = current->next;
    }

    fclose(fileptr);

    /*remove contents in file*/
    fileptr = fopen(system->coin_file_name,"w");
    fputs("",fileptr);
    fclose(fileptr);

    /*save coins to file*/
    fileptr = fopen(system->coin_file_name,"a"); 

    for(i = 0;i < NUM_DENOMS;i++)
    {
        sprintf(buffer,"%d",system->cash_register[i].denom);
        fputs(buffer,fileptr);
        fputs(COIN_DELIM,fileptr);
        sprintf(buffer,"%d",system->cash_register[i].count);
        fputs(buffer,fileptr);
        fputs("\n",fileptr);
    }
    fclose(fileptr);

    system_free(system);
    exit(0);
}

/*-------------*/
bool add_item(struct ppd_system * system)
{
    char buffer[BUFFER_SIZE + FGETS_EXTRA_SPACES];
    char buffer_copy[BUFFER_SIZE + FGETS_EXTRA_SPACES];
    struct ppd_stock stock_item;
    char id[IDLEN + 1];

    if(!get_next_id(system->item_list,id))
    {
        printf("Item list is full\n");
        return true;
    }

    printf("This new meal item will have the Item id of %s.\n",id);

    strcpy(stock_item.id,id);

    printf("Enter the item name: ");
    fgets(buffer,BUFFER_SIZE + FGETS_EXTRA_SPACES,stdin);
    check_buffer_overflow(buffer);
    remove_newline(buffer); /*remove newline created by fgets*/
    if(strcmp(buffer,"") == 0)
    {
        return true;
    }
    strcpy(stock_item.name,buffer);

    printf("Enter the item description: ");
    fgets(buffer,BUFFER_SIZE + FGETS_EXTRA_SPACES,stdin);
    check_buffer_overflow(buffer);
    remove_newline(buffer); /*remove newline created by fgets*/
    if(strcmp(buffer,"") == 0)
    {
        return true;
    }
    strcpy(stock_item.desc,buffer);

    printf("Enter the price for this item: ");
    fgets(buffer,BUFFER_SIZE + FGETS_EXTRA_SPACES,stdin);
    check_buffer_overflow(buffer);
    remove_newline(buffer); /*remove newline created by fgets*/

    if(strcmp(buffer,"") == 0)
    {
        return true;
    }

    strcpy(buffer_copy,buffer);

    if(!is_valid_price(buffer_copy))
    {
        printf("Invalid Price\n");
        return true;
    }

    stock_item.price.dollars = atoi(strtok(buffer,PRICE_DELIM));
    stock_item.price.cents = atoi(strtok(NULL,PRICE_DELIM));

    stock_item.on_hand = DEFAULT_STOCK_LEVEL;

    if (insert_stock(system->item_list,&stock_item) ==  false) 
    {
        printf("Could not add item.\n");
        return true;
    }

    printf("This item \"%s - %s\" has now been added to the menu.\n",stock_item.name,stock_item.desc);
    return true;
}

/*-------------*/
bool remove_item(struct ppd_system * system)
{
    char buffer[BUFFER_SIZE + FGETS_EXTRA_SPACES];
    struct ppd_stock * stock_item;

    printf("Enter the item id of the item to remove from the menu: ");
    fgets(buffer,BUFFER_SIZE + FGETS_EXTRA_SPACES,stdin);
    check_buffer_overflow(buffer);
    remove_newline(buffer); /*remove newline created by fgets*/
    if(strcmp(buffer,"") == 0)
    {
        return true;
    }

    stock_item = search_stock_by_id(system,buffer);

    /*check item exist*/
    if (stock_item == NULL)
    {
        printf("Item does not exist\n");
        return true;
    }
    else
    {
        printf("\"%s - %s - %s\" has been removed from the system.\n",stock_item->id,stock_item->name,stock_item->desc);
        remove_stock(system->item_list,stock_item);
    }
    return true;
}

/*-------------*/
bool reset_stock(struct ppd_system * system)
{
    struct ppd_node * current, * previous;

    /*current (list pointer) point to head*/
    previous = NULL;
    current = system->item_list->head; 

    /*traverse until alphabetically ordered position is found*/
    while(current != NULL)
    {
        current->data->on_hand = DEFAULT_STOCK_LEVEL; /*reset stock level*/
        previous = current;
        current = current->next;
    }
    printf("All stock has been reset to the default level of %d\n",DEFAULT_STOCK_LEVEL);
    return true;
}

/*-------------*/
bool reset_coins(struct ppd_system * system)
{
    int i;

    for(i = 0;i < NUM_DENOMS;i++)
    {
        system->cash_register[i].count = DEFAULT_COIN_COUNT;
    }
    printf("All coins have been reset to the default level of %d\n",DEFAULT_COIN_COUNT);
    return true;
}

/*-------------*/
bool display_coins(struct ppd_system * system)
{
    puts("Coin Summary");
    puts("---------");
    puts("Denomination | Count");
    printf("5 cents      | %d\n", system->cash_register[7].count);
    printf("10 ents      | %d\n", system->cash_register[6].count);
    printf("20 cents     | %d\n", system->cash_register[5].count);
    printf("50 cents     | %d\n", system->cash_register[4].count);
    printf("1 dollar     | %d\n", system->cash_register[3].count);
    printf("2 dollar     | %d\n", system->cash_register[2].count);
    printf("5 dollar     | %d\n", system->cash_register[1].count);
    printf("10 dollar    | %d\n", system->cash_register[0].count);

    return true;
}

bool abort_program(struct ppd_system * system)
{
    system_free(system);
    exit(0);
}