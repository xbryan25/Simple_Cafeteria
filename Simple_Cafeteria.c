#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>


// Lesson learned: declare structs first before declaring functions.
// Another lesson learned: POINTERS ARE THE GOAT!!!

struct FoodItem{
    int id_number;
    char food_name[64];
    int price;
};

struct Cart{
    char food_name[64];
    int quantity;
    int total_price;  
};

struct FoodItem* open_file();
int return_len_of_file(struct FoodItem food_items[]);

int get_str_len(char str[]);
int get_int_len(int num);
void remove_fgets_newline(char name_input[]);
void main_screen();

void admin(struct FoodItem food_items[], int *ptr_num_of_items);
void edit_name(struct FoodItem food_items[], int *ptr_num_of_items);
void edit_price(struct FoodItem food_items[], int *ptr_num_of_items);
void add_item(struct FoodItem food_items[], int *ptr_num_of_items);
void delete_item(struct FoodItem food_items[], int *ptr_num_of_items);

void show_menu(struct FoodItem food_items[], int *ptr_num_of_items, int state);
void display_cart(struct Cart cart[], int num_of_item_cart);
int get_entire_total(struct Cart cart[], int num_of_item_cart);
void accept_input(struct FoodItem food_items[], int *ptr_num_of_items);

int main(){
    main_screen();
    return 0;
}

struct FoodItem* open_file(){
    FILE *file;

    file = fopen("items.csv", "r");

    if (file == NULL){
        printf("Error opening file.\n");
        exit(-1); 
    }

    struct FoodItem* food_items = calloc(100, sizeof(struct FoodItem));
    // struct FoodItem food_items[100];

    int read = 0;
    int records = 0;

    do{
        read = fscanf(file, 
                      "%d,%64[^,],%d\n", 
                      &food_items[records].id_number,
                      food_items[records].food_name,
                      &food_items[records].price);

        if (read == 3){
            records++;
        }

        if (read != 3 && !feof(file)){
            printf("File format incorrect.\n");
            exit(-1);
        }

        if (ferror(file)){
            printf("Error reading file.\n");
            exit(-1); 
        }

    } while(!feof(file));

    fclose(file);
    return food_items;
}

int return_len_of_file(struct FoodItem food_items[]){
    int len_of_file = 0;
    while (get_str_len(food_items[len_of_file].food_name) != 0){
        len_of_file++;;
    }

    return len_of_file;
}

int get_str_len(char str[]){
    int str_len = 0;

    for(int i = 0; str[i]!='\0'; i++){
        str_len = str_len + 1;
    }

    return str_len;
}

int get_int_len(int num){
    int len = floor(log10(abs(num))) + 1;
    return len;
}

void remove_fgets_newline(char name_input[]){
    int name_len = get_str_len(name_input);

    if (name_input[name_len - 1] == '\n'){
        name_input[name_len - 1] = '\0';
    }
}

void main_screen(){
    while (1){
        int choice;
        char exit_choice = 'n';

        printf("--- Cafeteria System ---\n");
        printf("Select one of the options.\n");
        printf("[1] - Menu\n");
        printf("[2] - Admin\n");
        printf("[3] - Exit\n");

        printf("\nChoose: ");
        scanf("%d", &choice);
        system("cls");

        struct FoodItem* food_items = open_file();
        int len_of_file = return_len_of_file(food_items);
        int *ptr_len_of_file = &len_of_file;
        int state_for_show_menu = 0;

        switch(choice){
            case 1:
                show_menu(food_items, ptr_len_of_file, state_for_show_menu);
                break;

            case 2:
                admin(food_items, ptr_len_of_file);
                break;

            case 3:
                printf("Are you sure you want to exit the program? (y/N)");
                printf("\nChoose: ");
                scanf(" %c", &exit_choice);

                switch(exit_choice){
                    case 'y':
                        printf("\nThank you for running the program!");
                        exit(0);   
                    case 'n': 
                        system("cls");
                        break;
                    default:
                        printf("\nInvalid option.\n");
                        system("pause");
                        system("cls");
                }
                break;
                
            default:
                printf("Invalid option.\n");
                Sleep(1000);
                system("cls");
        }   
   }
}

void admin(struct FoodItem food_items[], int *ptr_num_of_items){
    int choice; 
    int state_of_show_menu = 1;

    while (1){
        printf("What would you like to do?\n\n");

        printf("Select one of the options.\n");
        printf("[1] - Edit name of an item\n");
        printf("[2] - Edit price of an item\n");
        printf("[3] - Add an item\n");
        printf("[4] - Delete an item\n");
        printf("[5] - Return to main menu\n");

        printf("\nChoose: ");
        scanf("%d", &choice);
        system("cls");

        switch(choice){
            case 1:
                show_menu(food_items, ptr_num_of_items, state_of_show_menu);
                edit_name(food_items, ptr_num_of_items);
                show_menu(food_items, ptr_num_of_items, state_of_show_menu);
                break;

            case 2:
                show_menu(food_items, ptr_num_of_items, state_of_show_menu);
                edit_price(food_items, ptr_num_of_items);
                show_menu(food_items, ptr_num_of_items, state_of_show_menu);
                break;

            case 3:
                show_menu(food_items, ptr_num_of_items, state_of_show_menu);
                add_item(food_items, ptr_num_of_items);
                show_menu(food_items, ptr_num_of_items, state_of_show_menu);
                break;

            case 4:
                show_menu(food_items, ptr_num_of_items, state_of_show_menu);
                delete_item(food_items, ptr_num_of_items);
                show_menu(food_items, ptr_num_of_items, state_of_show_menu);
                break;

            case 5:
                system("cls");
                printf("Going back to the main menu...");
                Sleep(1000);
                system("cls");
                return ;
                
            default:
                printf("Invalid option.\n");
                Sleep(1000);
                system("cls");
        }   

    }

}

void edit_name(struct FoodItem food_items[], int *ptr_num_of_items){
    int change_id_number;
    char new_name[64];
    char store_name[64];

    int num_of_items = *ptr_num_of_items;

    printf("Which item would you like to edit its name? (Input ID number)\n");
    scanf("%d", &change_id_number);

    if (change_id_number < 1 || change_id_number > num_of_items){
        system("cls");
        printf("ID number is out of bounds.\n");
        Sleep(1000);
        printf("Try again.");
        Sleep(1000);
        system("cls");
        return ;
    } else{
        // Edit in current array of structs
        memcpy(store_name, &food_items[change_id_number - 1].food_name,
            sizeof(food_items[change_id_number - 1].food_name));

        printf("Input new name for %s: ", store_name);
        scanf("%c", (char *) stdin);            // Remove trailing new line from pressing Enter key
        fgets(new_name, 64, stdin);

        remove_fgets_newline(new_name);

        // Edit the CSV

        char line[100];
        int i = 0;

        FILE *file, *temp;

        file = fopen("items.csv", "r");
        temp = fopen("temp.csv", "w");

        while (fgets(line, sizeof(line), file) != NULL){
            if (change_id_number == food_items[i].id_number){
                sscanf(line, "%d,%64[^,], %d\n", &food_items[i].id_number,
                food_items[i].food_name, &food_items[i].price);

                fprintf(temp, "%d,%s, %d\n", food_items[i].id_number,
                new_name, food_items[i].price);
            } else {
                fputs(line, temp);
            }
            
            i++;
        }

        memcpy(&food_items[change_id_number - 1].food_name, new_name, 
               sizeof(new_name));

        fclose(file);
        fclose(temp);

        remove("items.csv");
        rename("temp.csv", "items.csv");

        system("cls");
    }
}

void edit_price(struct FoodItem food_items[], int *ptr_num_of_items){
    int change_id_number;
    int new_price;
    char store_name[64];

    int num_of_items = *ptr_num_of_items;

    printf("Which item would you like to edit its price? (Input ID number)\n");
    scanf("%d", &change_id_number);

    if (change_id_number < 1 || change_id_number > num_of_items){
        system("cls");
        printf("ID number is out of bounds.\n");
        Sleep(1000);
        printf("Try again.");
        Sleep(1000);
        system("cls");
        return ;

    } else{
        // Edit in current array of structs
        memcpy(store_name, &food_items[change_id_number - 1].food_name,
            sizeof(food_items[change_id_number - 1].food_name));

        printf("Input new price for %s: ", store_name);
        scanf("%d", &new_price); // TODO: Use fgets for names with spaces

        // Edit the CSV

        char line[100];
        int i = 0;
        // char temp_text[] = "temp.csv";

        FILE *file, *temp;

        file = fopen("items.csv", "r");
        temp = fopen("temp.csv", "w");

        while (fgets(line, sizeof(line), file) != NULL){
            if (change_id_number == food_items[i].id_number){
                sscanf(line, "%d,%64[^,], %d\n", &food_items[i].id_number,
                food_items[i].food_name, &food_items[i].price);

                fprintf(temp, "%d,%s, %d\n", food_items[i].id_number,
                food_items[i].food_name, new_price);
            } else {
                fputs(line, temp);
            }
            
            i++;
        }

        food_items[change_id_number - 1].price = new_price;
        // TODO: Figure out why ngano diari rani pwede sa ubos, ngano dili sa taas

        fclose(file);
        fclose(temp);

        remove("items.csv");
        rename("temp.csv", "items.csv");

        system("cls");
    }
}

void add_item(struct FoodItem food_items[], int *ptr_num_of_items){
    int num_of_items = *ptr_num_of_items;
    char store_name[64];
    int item_price;

    printf("Enter the name of the food item that you want to add: ");
    scanf("%c", (char *) stdin);            // Remove trailing new line from pressing Enter key
    fgets(store_name, 64, stdin);

    remove_fgets_newline(store_name);

    printf("Enter the price of the food item that you want to add: ");
    scanf("%d", &item_price);

    // ----------Edit the CSV----------
    char line[100];

    FILE *file, *temp;

    file = fopen("items.csv", "a");

    fprintf(file, "%d,%s, %d\n", num_of_items + 1, store_name, item_price);

    fclose(file);

     // ----------Edit in current array of structs----------

    memcpy(&food_items[num_of_items].food_name, &store_name,
            sizeof(store_name));

    food_items[num_of_items].id_number = num_of_items + 1;
    food_items[num_of_items].price = item_price;

    *ptr_num_of_items = num_of_items + 1;

    system("cls");
}

void delete_item(struct FoodItem food_items[], int *ptr_num_of_items){
    int num_of_items = *ptr_num_of_items;
    int change_id_number;
    char store_name[64];
    int after_change_id_number = 0;

    printf("Which item would you like to delete? (Input ID number)\n");
    scanf("%d", &change_id_number);

    if (change_id_number < 1 || change_id_number > num_of_items){
        system("cls");
        printf("ID number is out of bounds.\n");
        Sleep(1000);
        printf("Try again.");
        Sleep(1000);
        system("cls");
        return ;
    } else{
        system("cls");

        printf("Deleting %s... ", food_items[change_id_number - 1].food_name);
        Sleep(1000);
        system("cls");

        // ----------Edit the CSV----------
        char line[100];
        int i = 0;
        // char temp_text[] = "temp.csv";

        FILE *file, *temp;

        file = fopen("items.csv", "r");
        temp = fopen("temp.csv", "w");

        while (fgets(line, sizeof(line), file) != NULL){
            if (change_id_number == food_items[i].id_number){
                after_change_id_number = 1;
            } 
            
            if (after_change_id_number == 1){
                if (i < num_of_items - 1){
                    fprintf(temp, "%d,%s, %d\n", food_items[i].id_number,
                    food_items[i + 1].food_name, food_items[i + 1].price);
                }
            }
            else{
                fputs(line, temp);
            }

            i++;
        }

        // ----------Edit in current array of structs----------

        after_change_id_number = 0; // reset to 0

        // reset i to 0
        for (i = 0; i < num_of_items; i++){
            if (i + 1 == change_id_number){
                after_change_id_number = 1;
            }

            if (after_change_id_number == 1 && i < num_of_items - 1){
                memcpy(&food_items[i].food_name, &food_items[i + 1].food_name,
                sizeof(food_items[i + 1].food_name));

                food_items[i].price = food_items[i + 1].price;
            } 
        }

        fclose(file);
        fclose(temp);

        remove("items.csv");
        rename("temp.csv", "items.csv");

        system("cls");

        //TODO: Don't forget to free the memory.
        *ptr_num_of_items = num_of_items - 1;
    }
}

void show_menu(struct FoodItem food_items[], int *ptr_num_of_items, int state){
    int num_of_items = *ptr_num_of_items;

    int max_fooditem_len = 0;
    int max_id_number = num_of_items;
    int max_price = 0;

    // Checks the max length and max price of the food items

    for (int h = 0; h < num_of_items; h++){
        if (h == 0){
            max_fooditem_len = get_str_len(food_items[h].food_name);
            max_price = food_items[h].price;
        } else if (get_str_len(food_items[h].food_name) > max_fooditem_len){
            max_fooditem_len = get_str_len(food_items[h].food_name);
        } else if (food_items[h].price > max_price){
            max_price = food_items[h].price;
        }
    }

    // Gets the length of max_price
    int len_id_num = get_int_len(max_id_number);
    int len_price = get_int_len(max_price);

    // TODO: Fix string formatting using padding

    // This was derived using the complex printf statement below 
    int length_of_table = 16 + max_fooditem_len + len_id_num + 3/2 + 2;

    for (int i = 0; i < num_of_items; i++){

        if (i == 0){
            // Padding value was determined via trial and error.

            printf("+%.*s+\n", length_of_table, "---------------------------------------------");
            printf("|%-*sID #%*s|%-*sName%*s|%-*sPrice%*s|\n", ((len_id_num - 4) + 5) / 2,
                   "", len_id_num, "", ((max_fooditem_len - 4) + 3) / 2, "", ((max_fooditem_len - 4) + 4) / 2, "",
                   ((len_id_num - 5) + 6) / 2, "", ((len_id_num - 5) + 5) / 2, "");
            printf("+%.*s+\n", length_of_table, "---------------------------------------------");       
        }

        // I don't know why left padding has to be n-1 than the right padding.
        // String concatenation in C is so confusing.
        // %-*s means left padding
        // %*s means right padding

        // ID # | Name | Price

        //TODO: make unsigned long compatible with int

        printf("|%-*s%d%*s|%-*s%s%*s|%-*s%d%*s|\n", ((len_id_num - get_int_len(food_items[i].id_number)) + 5) / 2, 
               "", food_items[i].id_number, ((len_id_num - get_int_len(food_items[i].id_number)) + 6) / 2, "", 
               ((max_fooditem_len - get_str_len(food_items[i].food_name)) + 3) / 2, "", food_items[i].food_name, 
               ((max_fooditem_len - get_str_len(food_items[i].food_name)) + 4) / 2, "",
               ((len_id_num - get_int_len(food_items[i].price)) + 6) / 2, "", food_items[i].price, 
               ((len_id_num - get_int_len(food_items[i].price)) + 5) / 2, "");


        printf("+%.*s+\n", length_of_table, "---------------------------------------------");
    }

    printf("\n\n");

    if (state == 0){
        accept_input(food_items, ptr_num_of_items);
    } 
    
}

void display_cart(struct Cart cart[], int num_of_item_cart){
    // Food item name, quantity, total
    //                         , grand total

    printf("\n---Items in Cart---");

    for(int i = 0; i < num_of_item_cart; i++){
        if (i == 0){
            printf("\n%s (%d) - %d\n", cart[i].food_name, cart[i].quantity, cart[i].total_price);
        } else{
            printf("%s (%d) - %d\n", cart[i].food_name, cart[i].quantity, cart[i].total_price);
        }
        
    }

    int grand_total = get_entire_total(cart, num_of_item_cart);

    printf("\nTotal: %d\n\n", grand_total);
}

int get_entire_total(struct Cart cart[], int num_of_item_cart){
    int grand_total = 0;

    for(int i = 0; i < num_of_item_cart; i++){
        grand_total = grand_total + cart[i].total_price;
    }

    return grand_total;

}

void accept_input(struct FoodItem food_items[], int *ptr_num_of_items){
    int total_price = 0;
    int id_number_input = 0;
    int amount = 0;
    int checkout_choice = 0;
    int num_of_item_cart = 0;

    int item_already_in_cart = 0;

    struct Cart current_cart[100];
    int cart_struct_counter = 0;

    while (1){
        printf("Enter item to buy (use ID number) (enter 0 to quit): ");
        scanf("%d", &id_number_input);

        if (id_number_input == 0){
            system("cls");
            break;
        }

        printf("\nHow many %s do you want? ", food_items[id_number_input - 1].food_name);
        scanf("%d", &amount);

        if (num_of_item_cart > 0){
            for(int i = 0; i < num_of_item_cart; i++){
                if (strcmp(current_cart[i].food_name, food_items[id_number_input - 1].food_name) == 0){
                    current_cart[i].quantity = current_cart[i].quantity + amount;
                    
                    total_price = amount * food_items[id_number_input - 1].price;
                    current_cart[i].total_price = current_cart[i].total_price + total_price;
                    item_already_in_cart = 1;
                    // If there is already an existing item in the cart, it will just add the amount
                    // and the total price. It leaves the if statement afterwards.
                } 
            } 

            if (item_already_in_cart != 1){
                // Copy food name from FoodItem struct to Cart struct.
                memcpy(&current_cart[cart_struct_counter].food_name, 
                &food_items[id_number_input - 1].food_name, 
                sizeof(food_items[id_number_input - 1].food_name));

                current_cart[cart_struct_counter].quantity = amount;

                total_price = amount * food_items[id_number_input - 1].price;
                current_cart[cart_struct_counter].total_price = total_price;

                num_of_item_cart = num_of_item_cart + 1;
                cart_struct_counter = cart_struct_counter + 1;
            }

        } else{
            memcpy(&current_cart[cart_struct_counter].food_name, 
            &food_items[id_number_input - 1].food_name, 
            sizeof(food_items[id_number_input - 1].food_name));

            current_cart[cart_struct_counter].quantity = amount;

            total_price = amount * food_items[id_number_input - 1].price;
            current_cart[cart_struct_counter].total_price = total_price;

            num_of_item_cart = num_of_item_cart + 1;
            cart_struct_counter = cart_struct_counter + 1;
            
        }

        item_already_in_cart = 0;

        // // Copy food name from FoodItem struct to Cart struct.
        // memcpy(&current_cart[cart_struct_counter].food_name, 
        //        &food_items[id_number_input - 1].food_name, 
        //        sizeof(food_items[id_number_input - 1].food_name));

        // current_cart[cart_struct_counter].quantity = amount;

        // // Get total price of item and assign it to Cart struct.
        // total_price = amount * food_items[id_number_input - 1].price;
        // current_cart[cart_struct_counter].total_price = total_price;

        // num_of_item_cart = num_of_item_cart + 1;
        // cart_struct_counter = cart_struct_counter + 1;

        display_cart(current_cart, num_of_item_cart);

        printf("Do you want to check out (1)? Or do you want to add more items (2)? ");
        scanf("%d", &checkout_choice);
        printf("\n");

        switch(checkout_choice){
            case 1:
                int entire_total_price = get_entire_total(current_cart, num_of_item_cart);

                printf("The total price of all the items that you want to buy is %d.", entire_total_price);

                Sleep(2000);
                system("cls");
                printf("\nThanks for shopping!");
                Sleep(1000);
                system("cls");
                break;

            case 2:
                system("cls");
                show_menu(food_items, ptr_num_of_items, 1);
                break;

            default:
                printf("\nInvalid choice.\n\n");
        }

        if (checkout_choice == 1){
            break;
        }
    }
}
