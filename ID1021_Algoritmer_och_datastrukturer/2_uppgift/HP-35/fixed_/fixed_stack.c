#include <stdlib.h>
#include <stdio.h>

//skapa klassen stack
typedef struct stack {
    int top;
    int size;
    int *array;
} stack;

//intializera klassen
stack *new_stack(int size) {
    int *array = (int*)malloc(size*sizeof(int));
    stack *stk = (stack*)malloc(sizeof(stack));
    stk->size = size;
    stk->top = -1;
    stk->array = array;
    return stk;
}

/*Klassmetod push, kollar om stacken är full, returnera "Stack overflow" om det 
är fullt. Annars lägger den till val.
*/
void push(stack *stk, int val){
    if (stk->top == stk->size -1){
        printf("Stack overflow\n");
        return;
    }

    stk->top++;
    stk->array[stk->top] = val;
}

/*pop läser först in värdet som är högst på listan, flyttar ner på 
listan och returnerar värdet*/
int pop(stack *stk){
    if (stk->top == -1){
        printf("Stack overflow\n");
        return -1;
    }

    int val = stk->array[stk->top];
    stk->top--;
    return val;
}

/*int main() {
    stack *stk = new_stack(4);

    push(stk, 32);
    push(stk, 33);
    push(stk, 34);
    push(stk, 45);
    push(stk, 44);

    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk)); 
}
*/

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
/* The pointer points to the top of the stack.
I've put the value of the pointer to -1.
Ive set the program to return stack overflow
I've set the program to return stack underflow
*/