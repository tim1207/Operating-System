#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int sudoku[9][9] = {
        {6, 2, 4, 5, 3, 9, 1, 8, 7},
        {5, 1, 9, 7, 2, 8, 6, 3, 4},
        {8, 3, 7, 6, 1, 4, 2, 9, 5},
        {1, 4, 3, 8, 6, 5, 7, 2, 9},
        {9, 5, 8, 2, 4, 7, 3, 6, 1},
        {7, 6, 2, 3, 9, 1, 4, 5, 8},
        {3, 7, 1, 9, 5, 6, 8, 4, 2},
        {4, 9, 6, 1, 8, 2, 5, 7, 3},
        {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

bool verification[11]; 
int Squares_3x3 = 2; 

typedef struct {   
    int row;
    int column;
} parameter;


pthread_t columns_thread, rows_thread, first_thread, second_thread, third_thread,
        fourth_thread, fifth_thread, six_thread, seventh_thread, eighth_thread, ninth_thread;

void *column_worker(void *parametros) {

    parameter *column = (parameter *) parametros;
    int columnInitial = column->column;
    int rowInitial = column->row;

    for (int i = columnInitial; i < 9; i++) {
        int col[9] = {0}; 
        for (int j = rowInitial; j < 9; j++) {
            int aux = sudoku[i][j];
            if (col[aux - 1] == 0 && aux > 0) { 
                col[aux - 1] = aux;
            } else if (aux <= 0) {
                verification[0] = 0;
                printf("There is an invalid number in the column [%d].\n", j + 1);
                pthread_exit(0);
            } else {
                verification[0] = 0;
                printf("There is a repeated number in the column [%d].\n", j + 1);
                pthread_exit(0);
            }
        }
    }
    verification[0] = 1;
    pthread_exit(0);    
}


void *rows_worker(void *parametros) {

    parameter *row = (parameter *) parametros;
    int columnInitial = row->column;
    int rowInitial = row->row;

    for (int i = columnInitial; i < 9; i++) {
        int fil[9] = {0};
        for (int j = rowInitial; j < 9; j++) {
            int aux = sudoku[i][j];
            if (fil[aux - 1] == 0 && aux > 0) {
                fil[aux - 1] = aux;
            } else if (aux <= 0) {
                verification[0] = 0;
                printf("There is an invalid number in the row [%d].\n", i + 1);
                pthread_exit(0);
            } else {
                verification[0] = 0;
                printf("There is a repeated number in the row [%d].\n", i + 1);
                pthread_exit(0);
            }
        }
    }
    verification[1] = 1;
    pthread_exit(0);
}


void *Square_worker(void *parametros) {

    parameter *Square_row = (parameter *) parametros;
    int rowInitial = Square_row->row;
    int columnInitial = Square_row->column;
    int square[9] = {0};

    for (int i = rowInitial; i < rowInitial + 3; ++i) {
        for (int j = columnInitial; j < columnInitial + 3; ++j) {
            int aux = sudoku[i][j];
            if (square[aux - 1] == 0 && aux > 0) {
                square[aux - 1] = aux;
            }
            else {
                verification[Squares_3x3] = 0;
                Squares_3x3++;
                pthread_exit(0);
            }
        }
    }
    verification[Squares_3x3] = 1;
    Squares_3x3++;
    pthread_exit(0);
}

int main() {


    parameter *verificarrows = (parameter *) malloc(sizeof(parameter));
    verificarrows->row = 0;
    verificarrows->column = 0;

    parameter *verificarcolumns = (parameter *) malloc(sizeof(parameter));
    verificarcolumns->row = 0;
    verificarcolumns->column = 0;

    parameter *bos1 = (parameter *) malloc(sizeof(parameter));
    bos1->row = 0;
    bos1->column = 0;

    parameter *bos2 = (parameter *) malloc(sizeof(parameter));
    bos2->row = 0;
    bos2->column = 3;

    parameter *bos3 = (parameter *) malloc(sizeof(parameter));
    bos3->row = 0;
    bos3->column = 6;

    parameter *bos4 = (parameter *) malloc(sizeof(parameter));
    bos4->row = 3;
    bos4->column = 0;

    parameter *bos5 = (parameter *) malloc(sizeof(parameter));
    bos5->row = 3;
    bos5->column = 3;

    parameter *bos6 = (parameter *) malloc(sizeof(parameter));
    bos6->row = 3;
    bos6->column = 6;

    parameter *bos7 = (parameter *) malloc(sizeof(parameter));
    bos7->row = 6;
    bos7->column = 0;

    parameter *bos8 = (parameter *) malloc(sizeof(parameter));
    bos8->row = 6;
    bos8->column = 3;

    parameter *bos9 = (parameter *) malloc(sizeof(parameter));
    bos9->row = 6;
    bos9->column = 6;


    pthread_create(&columns_thread, NULL, column_worker, (void *) verificarcolumns);
    pthread_create(&rows_thread, NULL, rows_worker, (void *) verificarrows);
    pthread_create(&first_thread, NULL, Square_worker, (void *) bos1);
    pthread_create(&second_thread, NULL, Square_worker, (void *) bos2);
    pthread_create(&third_thread, NULL, Square_worker, (void *) bos3);
    pthread_create(&fourth_thread, NULL, Square_worker, (void *) bos4);
    pthread_create(&fifth_thread, NULL, Square_worker, (void *) bos5);
    pthread_create(&six_thread, NULL, Square_worker, (void *) bos6);
    pthread_create(&seventh_thread, NULL, Square_worker, (void *) bos7);
    pthread_create(&eighth_thread, NULL, Square_worker, (void *) bos8);
    pthread_create(&ninth_thread, NULL, Square_worker, (void *) bos9);


    
    pthread_join(columns_thread, NULL);
    pthread_join(rows_thread, NULL);
    pthread_join(first_thread, NULL);
    pthread_join(second_thread, NULL);
    pthread_join(third_thread, NULL);
    pthread_join(fourth_thread, NULL);
    pthread_join(fifth_thread, NULL);
    pthread_join(six_thread, NULL);
    pthread_join(seventh_thread, NULL);
    pthread_join(eighth_thread, NULL);
    pthread_join(ninth_thread, NULL);
    printf("The Sudoku is: \n");
    for(int i =0;i<9;i++){
        for(int j=0;j<9;j++){
            printf("%d ",sudoku[i][j]);
        }
        printf("\n");
    }
    for (int k = 0; k < 11; ++k) {
        if (!verification[k]) {
            printf("\nThe Sudoku was not solved.\n");
            exit(0);
        }
    }
    printf("\nCongratulations, the Sudoku was solved.\n");
    return 0;
}

