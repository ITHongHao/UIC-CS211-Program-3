/*-------------------------------------------
Program 3: Twenty-Four
Course: CS 211, Spring 2023, UIC
System: VSCode Windows 11
Author: Ivan Tang
Starter code provided by: Ellen Kidane
------------------------------------------- */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct CharNode* Stack;
typedef struct CharNode* Queue;
typedef char* String;

void displayStart();
int displayGame(String numBank, int* choiceAmount, int* index);
char play(int* puzzleSet);
void clearBuffer();
void freeNodes(Stack pHead);
void qPush(Stack* pHead, char data);
char qPop(Queue* pHead);
void push(Stack* pHead, char data);
char pop(Stack* pHead);
char peek(Stack* pHead);
void displayStart();
String chooseDifficulty(int* choiceAmount);
void printStack(Stack x);
int operatorCompare(char c1, char c2);
int validNums(int count[9]);
int validChar(char check, int count[9]);
int evaluate(Queue pHead);
void endMenu(int* gameState);


struct CharNode{
  char data;
  Stack pNext;
};

//push data onto the back of queue pHead
void qPush(Queue* pHead, char data){
  Queue pAdd = malloc(sizeof(struct CharNode));  
  pAdd->data = data;      //add data to new node
  pAdd->pNext = NULL;    //add pNext to new node//create space to store new node
  Queue pTemp = (*pHead);  //point to current head node

  if(pTemp == NULL){*pHead = pAdd;}
  else{
    while(pTemp->pNext != NULL){pTemp = pTemp->pNext;}  //go to pTail node
    pTemp->pNext = pAdd;        //make pHead equal the new node
  }
}

//Removes front of queue pHead and returns data stored in pHead
char qPop(Queue* pHead){
  Queue pTemp = *pHead;      //point to head node
  char ret = pTemp->data;  //save head data
  (*pHead) = pTemp->pNext;  //make head go to pNext of head
  free((pTemp));      //delete former pHead saved in pTemp
  return ret;          //return the popped data value
}

//Add new node to front of stack pHead with data stored in it
void push(Stack* pHead, char data){
  Stack pAdd = malloc(sizeof(struct CharNode));  //create space to store new node
  Stack pTemp = (*pHead);  //point to current head node
  pAdd->data = data;      //add data to new node
  pAdd->pNext = pTemp;    //add pNext to new node
  (*pHead) = pAdd;        //make pHead equal the new node
}

//Removes top of stack and returns data stored
char pop(Stack* pHead){
  if(*pHead == NULL){return ' ';} //return ' ' if stack is empty
  Stack pTemp = *pHead;      //point to head node
  char ret = pTemp->data;  //save head data
  (*pHead) = pTemp->pNext;  //make head go to pNext of head
  free((pTemp));      //delete former pHead saved in pTemp
  return ret;          //return the popped data value
}

//returns data on top or in the front of stack or queue
char peek(Stack* pHead){
  if(*pHead == NULL){return '\0';}  // if queue/stack is empty
  return (*pHead)->data;
}

//display starting messages
void displayStart() {
  printf("Welcome to the game of TwentyFour Part Two!\n");
  printf("Use each of the four numbers shown exactly once, \n");
  printf("combining them somehow with the basic mathematical operators "
         "(+,-,*,/) \n");
  printf("to yield the value twenty-four.\n");
}

//reads in user input, returns set of options from difficulty text file
String chooseDifficulty(int* choiceAmount) {
  String choices = malloc(100 * sizeof(char));    //loaded with all options
  memset(choices, 0, 100 * sizeof(char));         //make sure malloc isnt uninitialized
  
  printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\nYour choice --> ");
  
  char difficulty = '0';
  difficulty = getchar(); //grab difficulty

  FILE *filePtr = NULL;
  //open file
  if (difficulty == 'M')     {filePtr = fopen("medium.txt", "r");}
  else if(difficulty == 'H') {filePtr = fopen("hard.txt", "r");}
  else                       {filePtr = fopen("easy.txt", "r");}

  if(filePtr == NULL) {                       // Check that the file was able to be opened       
		printf("Error: could not open file for reading\n");
		exit(-1);
	}

  String inputString = malloc(8 * sizeof(char));    
  while(fscanf(filePtr, "%s", inputString) != EOF) {  //read in file
		strcat(choices,inputString);  //concatenate to choices string
    (*choiceAmount)++;        //counts amount of characters
	}
  (*choiceAmount) /= 4;   //divide by 4 to represent amount of puzzles
  free(inputString);
  fclose(filePtr);
  return choices;
}

//display the puzzle numbers and returns the puzzle numbers
int displayGame(String numBank, int* choiceAmount, int* index){
  int startPos = (*index);    //make pointer to random option in numBank
  int first = (numBank[startPos] - 48) * 1000;
  int second = (numBank[startPos+1] - 48) * 100;
  int third = (numBank[startPos+2] - 48) * 10;
  int fourth = (numBank[startPos+3]) - 48;
  int ret = first + second + third + fourth;
  
  printf("The numbers to use are: %d, %d, %d, %d.\n", ret / 1000, (ret / 100) % 10, (ret / 10) % 10, ret % 10); 
  return ret;
}

//debugger function to print out all elements on a stack or queue
void printStack(Stack x){
  if(x == NULL){
    printf("NULL");
    return;
  }
  printf("%c ", x->data);
  printStack(x->pNext);
}

//return 1 if c1 > c2, return 0 if c1 = c2, return -1 if c1 < c2
int operatorCompare(char c1, char c2){
  if(c1 == '(' || c1 == ')' || c2 == '(' || c2 == ')'){return 2;}   //if parenthesis
  else if((c1 == '+' && c2 == '-') || (c1 == '-' && c2 == '+') || (c1 == '*' && c2 == '/') || (c1 == '/' && c2 == '*') || c1 == c2) {return 0;}  //if equal ops
  else if(c1 > c2) {return -1;}//less than
  return 1;  //if greater than
}

//check if user inputted nums perfectly match the amount given in the puzzle. 0 for false, 1 for true
int validNums(int count[9]){
  for(int i = 0; i < 9; i++){
    if(count[i] != 0) return 0;
  }
  return 1;
}

//checks if check is a valid input, 1 is valid, 0 is invalid
int validChar(char check, int count[9]){
  if(check >= '1' && check <= '9'){
    count[check - '0' - 1]--;
    return 1;
  }
  switch(check){  
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
      return 1;
    default:
      return 0;
  }
}

//Given postfix expression in queue pHead, evaluate what is in there, returns the next option to go to
int evaluate(Queue pHead){
  Stack eval = NULL;
  int num1;
  int num2;
  int num3 = 0;
  char oper;
  
  while(pHead != NULL){   //loop until end of the queue/expression
    if(pHead->data == '!'){ //detects that there is an extra closing parenthesis at that location
      printf("Error! Too many closing parentheses in the expression.\n");
      freeNodes(eval);
      return '\n'; //parenthesis error
    }
    else if(pHead->data == ' '){  //(edge case when operator comparison returns '\0')
      //do nothing in order to not break the rest of the logic
    }
    else if(pHead->data >= '1' && pHead->data <= '9'){push(&eval, pHead->data);}  //if character is a number
    else{
      num2 = pop(&eval) - '0';
      num1 = pop(&eval) - '0';
      oper = pHead->data;

      switch(oper){        //depending on user operator input, different arithmatic will execute
        case '+':
          num3 = num1 + num2;
          break;
        case '-':
          num3 = num1 - num2;
          break;
        case '*':
          num3 = num1 * num2;
          break;
        case '/':
          num3 = num1 / num2;
          break;
      }
      push(&eval, num3 +'0'); //push the sum back onto the stack
      printf("%d %c %d = %d.\n", num1, oper, num2, num3); //print out the equation
    }
    pHead = pHead->pNext; //progress to the next pHead
  }
  char ret = pop(&eval) - '0';  //solution of the equation
  
  if(peek(&eval) != '\0'){  //check if the stack is empty after, or there are enough operators for each value
    printf("Error! Too many values in the expression.\n");
    freeNodes(pHead);
    freeNodes(eval);
    return '\n';
  }
  
  freeNodes(pHead);
  freeNodes(eval);
  return ret;
}

//main game decider for everything
char play(int* puzzleSet){
  Stack ops = NULL;
  Queue order = NULL;
  int bracketCounter = 0; //counts amount of opening brackets
  int count[9] = {0,0,0,0,0,0,0,0,0}; //create counter from 0 to 9
  //edit counter depending on what the number is in the counter
  count[-1 + (*puzzleSet) / 1000]++;
  count[-1 + ((*puzzleSet) / 100) % 10]++;
  count[-1 + ((*puzzleSet) / 10) % 10]++;
  count[-1 + (*puzzleSet) % 10]++;
  
  printf("Enter your solution: ");
  String input = malloc(20 * sizeof(char));  
  size_t bufsize = 20;
  getline(&input, &bufsize, stdin); //get user solution, store in input
  
  for(int i = 0; input[i] != '\0'; i++){    //loop until end of input string
    char check = input[i];

    if(check == ' ' || check == '\n') continue; //any newlines or spaces will be ignored
    else if(!validChar(check, count)){    //detects any invalid chars
      printf("Error! Invalid symbol entered. Please try again.\n");
      freeNodes(ops);
      freeNodes(order);
      free(input);
      return '\0';
    }

    //put this into a separate function
    if(check >= '1' && check <= '9'){ //if number, push to queue
      qPush(&order, check);
    }
    else{ //not a number
      int opCompare = operatorCompare(check, peek(&ops)); //sets opCompare to 
      if(check == '('){ //if chekced character is an open parenthesis
        bracketCounter++;
        push(&ops, check);
      }
      else if(check == ')'){  //if closing bracket
        if(bracketCounter <= 0){  //if there exists an open bracket
          while(ops != NULL){     //pop all elements in op stack
            qPush(&order, pop(&ops));
          }
          qPush(&order, '!'); //put a closing bracket char identifyer telling the eval function that there was a closing bracket error
        }
        //if valid closing parenthesis
        bracketCounter--; //decrease bracket counter
        while(peek(&ops) != '\0' && ops->data != '('){  //loop until stack is either empty or encounters the next open bracket
          qPush(&order, pop(&ops));     //pus hall ops into queue
        }
        if(peek(&ops) != '\0') pop(&ops);  //pop off open parenthesis if its not empty
      }
      else if(peek(&ops) == '\0'){push(&ops, check);} //if if the ops stack is empty, push to op stack
      else if(opCompare == 1){push(&ops, check);}  //op coming in is greater than the one currently on the stack
      else if(opCompare == 0){  //op coming in has equal precedence
        qPush(&order, pop(&ops));
        push(&ops, check);
      }
      else if(opCompare == -1){  //op has less precedence than the op on top of stack
        qPush(&order, pop(&ops)); //push op on stack onto the queue
        
        switch(opCompare){
          case -1:
            qPush(&order, pop(&ops));
            push(&ops, check);
            break;
          case 0:
            qPush(&order, pop(&ops));
            push(&ops, check);
            break;
          case 1:
            push(&ops, check);
            break;
          case 2:
            push(&ops, check);
            break;
        }
      }
      else if(opCompare == 2){push(&ops, check);} //if op is 
    }
  }
  
  while(ops != NULL){qPush(&order, pop(&ops));} //empty the rest of the operators onto the order queue

  if(!validNums(count)){  //check if the numbers inputted were the numbers provided
    printf("Error! You must use all four numbers, and use each one only once. Please try again.\n");
    freeNodes(ops);
    freeNodes(order);
    free(input);
    return '\0';
  }
  free(input);
  int winCondition = evaluate(order); //evaluate the postfix expression
  freeNodes(ops);
  freeNodes(order);

  if(winCondition == 24){return 'W';} //if user inputs a correct expression
  else if(winCondition == '\n'){return '\n';} //"Skip loss menu thingy menu display"
  return 'L';        
}

//display ending menu, asks user for their option, changes game state to do their option
void endMenu(int* gameState){
  printf("Enter: \t1 to play again,\n");
  printf("\t2 to change the difficulty level and then play again, or\n");
  printf("\t3 to exit the program.\n");
  printf("Your choice --> ");

  char input = '0';
  scanf("%c", &input);
  switch(input){
    case '1': 
      (*gameState) = 2;
      break;
    case '2': 
      (*gameState) = 1;
      break;
    default : 
      (*gameState) = 0;
  }
}

//assigns whatever is on the input buffer to a string and frees it, effectively clearing the input buffer
void clearBuffer(){
  String trash = malloc(20*sizeof(char));  //freed
  fgets(trash, 20,  stdin);
  free(trash);    
}

//given pHead, free all memory allocated for that node
void freeNodes(Stack pHead){
  if(pHead == NULL)return;  //base case
  
  Stack pTemp = pHead->pNext;
  free(pHead);
  freeNodes(pTemp);
}

int main() {
  srand(1);
  int choiceAmount = 0;     //holds amount of options in each mode(ex. easy has 12 options, so choiceAmount = 12)
  int gameState = 1;	      //1 on, 0 off
  int change = 1;           //1 means to change the difficulty, 0 means to keep the difficulty
  int index = 0;            //holds random int that determines random puzzle for each difficulty
  
  displayStart();
  String numBank;           //holds all puzzle options for selected difficulty as a string

  do{
    if(change == 1){                                  //determines difficulty select
      choiceAmount = 0;                              //reset amount of options to load new amount of options
      numBank = chooseDifficulty(&choiceAmount);    //load puzzle options for the user inputted difficulty
      clearBuffer();                               //make sure nothing is on the input buffer after choosing difficulty
    }
    index = 4*(rand() % choiceAmount);        //generate the random puzzle option at that index
    int choice = displayGame(numBank, &choiceAmount, &index);  //holds the 4 numbers able to be operated on
    char correctCheck = play(&choice);  //call main game function, holds the win/lose/repeat character to be checked
    
    if (correctCheck == 'W'){          //if user inputs a correct solution
      printf("Well done! You are a math genius.\n");
    }
    else if(correctCheck == '\0'){  //if user input is unwanted and the user must input again
      change = 0;    //make sure the difficulty does not change
      continue;
    }
    else if(correctCheck == '\n'){  //if user input is unwanted and the user must input again
      //do nothing and just reset with whatever you had initially
    }
    else{  //user input a valid solution but it wasnot correct
      printf("Sorry. Your solution did not evaluate to 24.\n");
    }
    endMenu(&gameState);    //print out ending menu that asks user if they would like to exit or play again
    if(gameState == 2){change = 0;}    //replay the same game difficulty
    else{        //play different difficulty
      change = 1;
      free(numBank);
    }
    clearBuffer();  //make sure nothing is on the buffer so the user can input things
  }while(gameState);  //loop until gameState = 0, or the user wants to exit

  printf("\nThanks for playing!\n");
  printf("Exiting...\n");
  return 0;
}