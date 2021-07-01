#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    t--;

    while((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0';
        t--;
    }
}

int get_next_nonblank_line(FILE *ifp, char *buf, int max_length)
{
    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0'))
    {
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }

    if(buf[0] != '\0') {
        return 1;
    } else {
        return 0;
    }
}

struct item_node_struct
{
     char name[32];
     int count;
     struct item_node_struct *left, *right;
};
typedef struct item_node_struct item_node;

//Creates a new item node
item_node* create_item_node(char* item_name, int item_count){

    item_node* n = malloc(sizeof(item_node));

    strcpy(n->name,item_name);
    n->count = item_count;
    n->left = NULL;
    n->right = NULL;

    return n;
}

struct tree_name_node_struct
{
     char treeName[32];
     struct tree_name_node_struct *left, *right;
     item_node *theTree;
};
typedef struct tree_name_node_struct tree_name_node;

//Creates a new tree node
tree_name_node* create_tree_name_node(char* treeName)
{
    tree_name_node* n = malloc(sizeof(tree_name_node));

    strcpy(n->treeName,treeName);
    n->left = NULL;
    n->right = NULL;
    n->theTree = NULL;

    return n;
}

void insert_tree(FILE* ifp, char buf[63], tree_name_node* root, char* name)
{
        get_next_nonblank_line(ifp,buf,63);
        sscanf(buf,"%s", name);     //Gets the name of the tree to be inserted

        if(strcmp(name, root->treeName) < 0){ //Checks if tree needs to go to the left
            if(root->left == NULL){ //If nothing is to the left, insert there
                root->left = create_tree_name_node(name);
            }
            else{
                tree_name_node* temp = root->left; //Makes temp left so we can compare now with left
                int x = 0;
                while (x == 0){     //Loop to keep checking left and right until we create a tree
                    if(strcmp(name, temp->treeName) < 0){
                       if(temp->left == NULL){
                            temp->left = create_tree_name_node(name);
                            x = 1;
                       }
                       else{
                            temp = temp->left;
                        }
                    }
                    else{
                        if(temp->right == NULL){
                            temp->right = create_tree_name_node(name);
                            x = 1;
                        }
                        else{
                            temp = temp->right;
                        }
                    }
                }
            }
        }

        else{ //Does the same thing as top code but if we need to start inserting at the right of root

            if(root->right == NULL){
                root->right = create_tree_name_node(name);
            }

            else{
                tree_name_node* temp = root->right;
                int x = 0;
                while (x == 0){
                    if(strcmp(name, temp->treeName) < 0){
                        if(temp->left == NULL){
                            temp->left = create_tree_name_node(name);
                            x = 1;
                        }
                        else{
                            temp = temp->left;
                        }
                    }

                    else{
                        if(temp->right == NULL){
                            temp->right = create_tree_name_node(name);
                            x = 1;
                        }
                        else{
                            temp = temp->right;
                        }
                    }
                }
            }
        }
}

//Creates all the trees
tree_name_node* create_tree_of_trees(FILE* ifp, char buf[63],int n_trees, int n_items, int n_commands)
{
    //Gets the name for the root tree
    get_next_nonblank_line(ifp,buf,63);
    char* name = malloc(sizeof(char) * 31);
    sscanf(buf,"%s", name);

    tree_name_node* root = create_tree_name_node(name);

    for(int i = 1; i < n_trees; i++){ //Creates trees based on number of trees
        insert_tree(ifp,buf,root,name);
    }

    free(name);

    return root;
}

//Searches for a specific tree given a name
tree_name_node* search_for_name_node(tree_name_node* root, char* tree_name)
{
    //Loops until we find the correct tree
    while(strcmp(tree_name,root->treeName) != 0){


        if(strcmp(tree_name,root->treeName) < 0){
            root = root->left; //Goes left if we need to
        }
        else {
            root = root->right; //Goes right if we need to
        }

        if(root == NULL){ //If we reach null there is an error so we return null
            return NULL;
        }
   }

   return root;
}

void insert_item(FILE* ifp, char buf[63], char* tree_name, char* item_name, int item_count, tree_name_node* root, tree_name_node* temp)
{
        get_next_nonblank_line(ifp,buf,63);
        sscanf(buf,"%s %s %d", tree_name, item_name, &item_count); //Gets item data

        temp = search_for_name_node(root,tree_name);

        //Same method as tree insertion
        if(temp->theTree == NULL){
           temp->theTree = create_item_node(item_name,item_count);
        }

        else {
            if(strcmp(item_name, temp->theTree->name) < 0){
                if(temp->theTree->left == NULL){
                    temp->theTree->left = create_item_node(item_name,item_count);
                }
                else{
                    item_node* temp2 = temp->theTree->left;
                    int x = 0;
                    while (x == 0){
                        if(strcmp(item_name, temp2->name) < 0){
                           if(temp2->left == NULL){
                                temp2->left = create_item_node(item_name,item_count);
                                x = 1;
                           }
                           else{
                                temp2 = temp2->left;
                            }
                        }
                        else{
                            if(temp2->right == NULL){
                                temp2->right = create_item_node(item_name,item_count);
                                x = 1;
                            }
                            else{
                                temp2 = temp2->right;
                            }
                        }
                    }
                }
            }

            else{

                if(temp->theTree->right == NULL){
                    temp->theTree->right = create_item_node(item_name,item_count);
                }

                else{
                    item_node* temp3 = temp->theTree->right;
                    int x = 0;
                    while (x == 0){
                        if(strcmp(item_name, temp3->name) < 0){
                            if(temp3->left == NULL){
                                temp3->left = create_item_node(item_name,item_count);
                                x = 1;
                            }
                            else{
                                temp3 = temp3->left;
                            }
                        }

                        else{
                            if(temp3->right == NULL){
                                temp3->right = create_item_node(item_name,item_count);
                                x = 1;
                            }
                            else{
                                temp3 = temp3->right;
                            }
                        }
                    }
                }
            }
        }
}

//Creates all the items
void insert_all_items(FILE* ifp, char buf[63], tree_name_node* root, int n_items)
{
    char* tree_name = malloc(sizeof(char) * 31);
    char* item_name = malloc(sizeof(char) * 31);
    int item_count = 0;

    tree_name_node* temp = NULL;

    for (int i = 0; i < n_items;  i++){ //Loops based on number of items
        insert_item(ifp,buf,tree_name,item_name,item_count,root,temp);
    }


    free(tree_name);
    free(item_name);

}

//Searches for a specific item in a tree
item_node* search_in_name_node(item_node* temp, char* item_name)
{
    //Loops until item is found
    while(strcmp(item_name,temp->name) != 0) {


        if(strcmp(item_name,temp->name) < 0){
            temp = temp->left;
        }
        else {
            temp = temp->right;
        }

        if(temp == NULL){
            return NULL;
        }
   }

   return temp;
}

void search_command(tree_name_node* root, char* tree_name, char* item_name, FILE* ofp)
{
    tree_name_node* temp = search_for_name_node(root,tree_name); //Searches for which tree the item is in
    item_node* temp2;


    if(temp == NULL){ //If the tree that item is in does not exist
        printf("%s does not exist\n", tree_name);
        fprintf(ofp,"%s does not exist\n", tree_name);
    }
    else{
        temp2 = search_in_name_node(temp->theTree,item_name);
        if(temp2 == NULL){ //If the item doesnt exist within the tree
            printf("%s not found in %s\n", item_name, temp->treeName);
            fprintf(ofp,"%s not found in %s\n", item_name, temp->treeName);
        }
        else{
            printf("%d %s found in %s\n",temp2->count, temp2->name, temp->treeName);
            fprintf(ofp,"%d %s found in %s\n",temp2->count, temp2->name, temp->treeName); //If item is found, prints it
        }
    }
}

//Traverses items in a tree lexicographically
void traverse_in_order_items(item_node* temp, FILE* ofp)
{
    if(temp == NULL){
        return;
    }
    traverse_in_order_items(temp->left,ofp); //Keeps going left until null is reached
    printf("%s ", temp->name);
    fprintf(ofp,"%s ", temp->name);
    traverse_in_order_items(temp->right,ofp); //Keeps going right until null is reached
}

//Traverses trees lexicographically and prints their names and items in order
void traverse_in_order_trees(tree_name_node* temp, FILE* ofp)
{
    if(temp == NULL){
        return;
    }
    traverse_in_order_trees(temp->left,ofp);

    printf("\n==%s==\n", temp->treeName);
    fprintf(ofp,"\n==%s==\n", temp->treeName);
    traverse_in_order_items(temp->theTree,ofp); //Once the correct tree is found we traverse it's items

    traverse_in_order_trees(temp->right,ofp);
}

//lexicographically prints the names of the trees
void traverse_in_order_print_trees(tree_name_node* temp, FILE* ofp)
{
    if(temp == NULL){
        return;
    }

    traverse_in_order_print_trees(temp->left,ofp);

    printf("%s ", temp->treeName);
    fprintf(ofp,"%s ", temp->treeName);

    traverse_in_order_print_trees(temp->right,ofp);
}

//Prints all the tree names, then prints their names and their items in order
void traverse_in_order(tree_name_node* root, FILE* ofp)
{
    printf("\n");
    fprintf(ofp,"\n");
    traverse_in_order_print_trees(root,ofp);
    traverse_in_order_trees(root,ofp);
}

//Counts the number of items before a specific item
void traverse_in_order_item_before(item_node* temp, char* item_name, int* count)
{
    if(temp == NULL){
        return;
    }

    traverse_in_order_item_before(temp->left,item_name,count);

    if(strcmp(temp->name,item_name) < 0){ //Whenever the current item is less than our specific item count increases
        (*count)++;
    }

    traverse_in_order_item_before(temp->right,item_name,count);
}

//Counts the number of items after a specific item
void traverse_in_order_item_after(item_node* temp, char* item_name, int* right_side)
{
    if(temp == NULL){
        return;
    }
    traverse_in_order_item_before(temp->right,item_name,right_side);


    if(strcmp(temp->name,item_name) > 0){ //Whenever the current item is greater than our specific item count increases
        (*right_side)++;
    }

    traverse_in_order_item_before(temp->left,item_name,right_side);
}

void item_before_command(char* tree_name, char* item_name, tree_name_node* root, FILE* ofp)
{
    tree_name_node* temp = search_for_name_node(root, tree_name); //Searches for which tree the item should be in
    int count = 0;
    traverse_in_order_item_before(temp->theTree,item_name, &count);
    printf("item before %s: %d\n", item_name,count);
    fprintf(ofp,"item before %s: %d\n", item_name,count);
}

void count_total_number_of_items(item_node* temp, int* count)
{
    if(temp == NULL){
        return;
    }

    count_total_number_of_items(temp->left,count); //Keeps going left until null is reached

    (*count) += temp->count; //Gets the item count and adds to total number of items

    count_total_number_of_items(temp->right,count); //Keeps going right until null is reached
}

void count_command(char* tree_name, tree_name_node* root, FILE* ofp)
{
    tree_name_node* temp = search_for_name_node(root, tree_name); //Finds which tree to use
    int count = 0;
    count_total_number_of_items(temp->theTree,&count); //Gets the correct count
    printf("%s count %d\n",temp->treeName,count);
    fprintf(ofp,"%s count %d\n",temp->treeName,count);
}

//Compares whether the left or right side is at a lower depth
int find_depth(int a, int b)
{
    if(a >= b){ //If a is greater return a
        return a;
    }

    else{ //If be is greater return b
        return b;
    }

}

//Recursive function that goes to the lowest depth until null is reached
int find_height(item_node* temp)
{
    if(temp == NULL){
        return 0;
    }

    return find_depth(find_height(temp->left), find_height(temp->right)) + 1;
}

void height_balance_command(tree_name_node* root, char* tree_name, FILE* ofp)
{
    tree_name_node* temp = search_for_name_node(root,tree_name); //Searches for which tree to use

    if(temp->theTree == NULL){ //If tree has no items
        printf("%s tree has a height of -1\n",tree_name);
        fprintf(ofp,"%s tree has a height of -1\n",tree_name);
    }

    if(temp->theTree != NULL){
        if(temp->theTree->right == NULL && temp->theTree->left == NULL){ //If the tree only has a root
        printf("%s tree has a height of 0\n",temp->treeName);
        fprintf(ofp,"%s tree has a height of 0\n",temp->treeName);
        }
        else{
            int left_side = find_height(temp->theTree->left) - 1; //Gets height of left side
            int right_side = find_height(temp->theTree->right) - 1; //Gets height of right side
            int difference = left_side - right_side;

            printf("%s: left height %d, right height %d, difference %d, ",temp->treeName,left_side,right_side,abs(difference));
            fprintf(ofp,"%s: left height %d, right height %d, difference %d, ",temp->treeName,left_side,right_side,abs(difference));

            if(difference >= -1 && difference <= 1){ //Checks if the difference is no greater than 1
                printf("balanced\n");
                fprintf(ofp,"balanced\n");
            }
            else{
                printf("not balanced\n");
                fprintf(ofp,"not balanced\n");
            }
        }
    }
}

//Finds leftmost item
item_node* leftmost(item_node* temp)
{
    //Loops to leftmost item
    while(temp && temp->left != NULL)
        temp = temp->left;

    return temp;
}

//Deletes an item
item_node* delete_command_delete(item_node* root, char* item_name)
{
    if(root == NULL){
        return root;
    }

    //Goes to the left if item is to the left
    if(strcmp(item_name,root->name) < 0){
        root->left = delete_command_delete(root->left, item_name);
    }

    //Goes right if item is to the right
    else if(strcmp(item_name,root->name) > 0){
       root->right = delete_command_delete(root->right,item_name);
    }

    //Item node found
    else {
        //Item that has at most 1 child
        if(root->left == NULL) {
            item_node* temp = root->right;
            free(root);
            return temp;
        }
        else if(root->right == NULL) {
            item_node* temp = root->left;
            free(root);
            return temp;
        }

        //item that has two children
        item_node* temp = leftmost(root->right);

        //Copy necessary data
        strcpy(root->name,temp->name);
        root->count = temp->count;

        // Delete the item
        root->right = delete_command_delete(root->right,temp->name);
    }
    return root;
}

void delete_command(tree_name_node* root, char* tree_name, char* item_name, FILE* ofp)
{
    tree_name_node* temp = search_for_name_node(root, tree_name); //Find which tree item is in

    delete_command_delete(temp->theTree,item_name);

    printf("%s deleted from %s\n",item_name,tree_name);
    fprintf(ofp,"%s deleted from %s\n",item_name,tree_name);
}

//Finds the leftmost tree
tree_name_node* leftmost_tree(tree_name_node* temp)
{
    //Loops to leftmost tree
    while(temp && temp->left != NULL){
       temp = temp->left;
    }

    return temp;
}

//Traverses tree then deletes all items of the tree
void delete_all_items(item_node* temp)
{
    if(temp == NULL){
        return;
    }
    delete_all_items(temp->left); //Keeps going left until null is reached
    free(temp);
    delete_all_items(temp->right); //Keeps going right until null is reached
}

tree_name_node* delete_tree_command_delete(tree_name_node* root, char* tree_name)
{
    //Same idea has item delete command
    if(root == NULL){
        return root;
    }

    if(strcmp(tree_name,root->treeName) < 0){
        root->left = delete_tree_command_delete(root->left,tree_name);
    }

    else if(strcmp(tree_name,root->treeName) > 0){
       root->right = delete_tree_command_delete(root->right,tree_name);
    }

    else {
        if(root->left == NULL) {
            tree_name_node* temp = root->right;
            delete_all_items(root->theTree); //Deletes all items before deleting tree
            free(root);
            return temp;
        }
        else if(root->right == NULL) {
            tree_name_node* temp = root->left;
            delete_all_items(root->theTree); //Deletes all items before deleting tree
            free(root);
            return temp;
        }

        tree_name_node* temp = leftmost_tree(root->right);

        strcpy(root->treeName,temp->treeName);

        root->right = delete_tree_command_delete(root->right,temp->treeName);
    }
    return root;
}

void delete_tree_command(tree_name_node* root, char* tree_name, FILE* ofp)
{
    delete_tree_command_delete(root,tree_name);
    printf("%s deleted", tree_name);
    fprintf(ofp,"%s deleted", tree_name);

}

//Runs all the commands
void run_commands(FILE* ifp, char buf[63], tree_name_node* root, int n_commands, FILE* ofp){
    printf("\n==Processing Commands==\n");
    fprintf(ofp,"\n==Processing Commands==\n");

    //Strings to store input file commands
    char* command = malloc(sizeof(char) * 31);
    char* tree_name = malloc(sizeof(char) * 31);
    char* item_name = malloc(sizeof(char) * 31);

    for(int i = 0; i < n_commands; i++){ //Loops for total number of commands
        get_next_nonblank_line(ifp,buf,63);
        sscanf(buf,"%s",command);

        //Scans item name for commands that require it
        if( strcmp(command,"search") == 0 || strcmp(command,"item_before") == 0 || strcmp(command,"delete") == 0 ){
            sscanf(buf,"%s %s %s", command, tree_name, item_name);
        }
        else{
            sscanf(buf,"%s %s", command, tree_name);
        }

        if(strcmp(command,"search") == 0){
            search_command(root,tree_name,item_name,ofp);
        }

        if(strcmp(command,"item_before") == 0){
            item_before_command(tree_name,item_name,root,ofp);
        }

        if(strcmp(command,"count") == 0){
            count_command(tree_name,root,ofp);
        }

        if(strcmp(command,"height_balance") == 0){
            height_balance_command(root,tree_name,ofp);
        }
        if(strcmp(command,"delete") == 0){
            delete_command(root,tree_name,item_name,ofp);
        }
        if(strcmp(command,"delete_tree") == 0){
            delete_tree_command(root,tree_name,ofp);
        }
    }
    free(command);
    free(tree_name);
    free(item_name);
}

//Traverses tree then deletes all trees and its items
void delete_all_trees(tree_name_node* temp)
{
    if(temp == NULL){
        return;
    }
    delete_all_trees(temp->left);

    delete_all_items(temp->theTree);
    free(temp);

    delete_all_trees(temp->right);
}

int main()
{
    char buf[63];
    int n_trees, n_items, n_commands;

    //Opens input file
    FILE* ifp = fopen("cop3502-as4-input.txt","r");
    if(ifp != NULL){
        printf("Input file opened successfully\n");
    }
    else{
        printf("Error input file not opened");
    }

    //Opens output file
    FILE* ofp = fopen("cop3502-as4-output-Uriarte-Robert.txt","w");
    if(ofp != NULL){
        printf("Output file opened successfully\n");
    }
    else{
        printf("Error output file not opened");
    }

    //Gets binary tree data
    get_next_nonblank_line(ifp,buf,63);
    sscanf(buf, "%d %d %d", &n_trees, &n_items, &n_commands);
    printf("\nTrees: %d  Items: %d  Commands: %d\n", n_trees, n_items, n_commands);

    tree_name_node* root = create_tree_of_trees(ifp,buf,n_trees,n_items,n_commands); //Creates the binary tree

    insert_all_items(ifp,buf,root,n_items);

    traverse_in_order(root,ofp);

    run_commands(ifp,buf,root,n_commands,ofp);

    delete_all_trees(root); //Deletes everything

    //Closes out of input and output files
    fclose(ifp);
    fclose(ofp);

    printf("\n\nHello world!\n"); //End of code
    return 0;
}
