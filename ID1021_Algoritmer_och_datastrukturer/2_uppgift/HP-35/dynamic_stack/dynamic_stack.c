#include <stdlib.h>
#include <stdio.h>

#define MIN 4

//skapa klassen stack
typedef struct stack {
    int top;
    int size;
    int *array;
} stack;

//intializera klassen
stack *new_stack() {
    int *array = (int*)malloc(MIN*sizeof(int));
    stack *stk = (stack*)malloc(sizeof(stack));
    stk->size = MIN;
    stk->top = -1;
    stk->array = array;
    return stk;
}

/*Klassmetod push, kollar om stacken är full, 
utvidgar storleken på stacken för att lägga till mer värden.
*/
void push(stack *stk, int val){
    if (stk->top == stk->size -1){
        int size = stk->size * 2;
        stk->size = size;
        int *copy = (int*)malloc(size*sizeof(int));
        for (int i =0; i <= stk->top; i++){
            copy[i] = stk->array[i];
        }
        free(stk->array);
        stk->array = copy;
    }

    stk->top++;
    stk->array[stk->top] = val;
}

/*pop läser först in värdet som är högst på listan, flyttar ner på 
listan och returnerar värdet*/
int pop(stack *stk){
    if (stk->top == -1){
        printf("Stack underflow\n");
        return -1;
    }

    int val = stk->array[stk->top];
    stk->top--;

    if (stk->top + 1 <= stk->size/2){
        int size = stk->size/2;
        stk->size = size;
        int *copy = (int*)malloc(size*sizeof(int));
        for (int i = 0; i <= stk->top; i++){
            copy[i] = stk->array[i];
        }
        free(stk->array);
        stk->array = copy;
    }

    return val;
}


int main(){
    stack *stk = new_stack(4);

    int n = 10;
    for(int i = 0; i < n; i++){
        push(stk, i+30);
    }

    for(int i = 0; i  <= stk->top; i++){
        printf("stack[%d] : %d\n", i, stk->array[i]);
    }

    int val = pop(stk);
    while(val != -1){
        printf("pop : %d\n", val);
        val = pop(stk);
    }
}
