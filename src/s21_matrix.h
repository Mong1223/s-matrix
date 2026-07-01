#ifndef S21_MATRIX_H
#define S21_MATRIX_H

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

// 0 - OK
// 1 - Ошибка, некоректная матрица
// 2 - Ошибка вычисления(несовпадающие размеры матриц;
//     матрица, для которой нельзя провести вычисления )
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);

// Сравнение матриц
#define SUCCESS 1
#define FAILURE 0
int s21_eq_matrix(matrix_t *A, matrix_t *B);
// Сложение и вычитание матриц
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
// Умножение матрицы на число и на матрицу
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int s21_transpose(matrix_t *A, matrix_t *result);

int s21_calc_complements(matrix_t *A, matrix_t *result);

int s21_determinant(matrix_t *A, double *result);

int s21_inverse_matrix(matrix_t *A, matrix_t *result);

// help
int s21_get_minor(matrix_t *A, int exclude_row, int exclude_col,
                  matrix_t *minor);
#endif