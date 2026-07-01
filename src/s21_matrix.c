#include "s21_matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = 0;

  if (rows <= 0 || columns <= 0 || result == NULL) {
    status = 1;
  } else {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;

    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix != NULL) {
      double *data = (double *)calloc(rows * columns, sizeof(double));
      if (data != NULL) {
        for (int i = 0; i < rows; i++) {
          result->matrix[i] = data + i * columns;
        }
        result->rows = rows;
        result->columns = columns;
      } else {
        free(result->matrix);
        result->matrix = NULL;
        status = 1;
      }
    } else {
      status = 1;
    }
  }

  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && A->matrix != NULL) {
    if (A->rows > 0 && A->matrix[0] != NULL) {
      free(A->matrix[0]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;

  if (A == NULL || B == NULL || A->matrix == NULL || B->matrix == NULL) {
    status = FAILURE;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    status = FAILURE;
  } else {
    for (int i = 0; i < A->rows && status == SUCCESS; i++) {
      for (int j = 0; j < A->columns && status == SUCCESS; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) >= 1e-6) {
          status = FAILURE;
        }
      }
    }
  }

  return status;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = 0;

  if (A == NULL || B == NULL || result == NULL || A->matrix == NULL ||
      B->matrix == NULL) {
    status = 1;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    status = 2;
  } else if (s21_create_matrix(A->rows, A->columns, result) != 0) {
    status = 1;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }

  return status;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = 0;

  if (A == NULL || B == NULL || result == NULL || A->matrix == NULL ||
      B->matrix == NULL) {
    status = 1;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    status = 2;
  } else if (s21_create_matrix(A->rows, A->columns, result) != 0) {
    status = 1;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }

  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status = 0;

  if (A == NULL || result == NULL || A->matrix == NULL) {
    status = 1;
  } else if (s21_create_matrix(A->rows, A->columns, result) != 0) {
    status = 1;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }

  return status;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = 0;

  if (A == NULL || B == NULL || result == NULL || A->matrix == NULL ||
      B->matrix == NULL) {
    status = 1;
  } else if (A->columns != B->rows) {
    status = 2;
  } else if (s21_create_matrix(A->rows, B->columns, result) != 0) {
    status = 1;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        result->matrix[i][j] = 0;
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }

  return status;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = 0;

  if (A == NULL || result == NULL || A->matrix == NULL) {
    status = 1;
  } else if (s21_create_matrix(A->columns, A->rows, result) != 0) {
    status = 1;
  } else {
    for (int i = 0; i < A->columns; i++) {
      for (int j = 0; j < A->rows; j++) {
        result->matrix[i][j] = A->matrix[j][i];
      }
    }
  }

  return status;
}

int s21_get_minor(matrix_t *A, int exclude_row, int exclude_col,
                  matrix_t *minor) {
  int status = 0;

  if (A == NULL || minor == NULL || A->matrix == NULL) {
    status = 1;
  } else if (A->rows <= 1 || A->columns <= 1) {
    status = 2;
  } else if (s21_create_matrix(A->rows - 1, A->columns - 1, minor) != 0) {
    status = 1;
  } else {
    int minor_i = 0;
    for (int i = 0; i < A->rows; i++) {
      if (i != exclude_row) {
        int minor_j = 0;
        for (int j = 0; j < A->columns; j++) {
          if (j != exclude_col) {
            minor->matrix[minor_i][minor_j] = A->matrix[i][j];
            minor_j++;
          }
        }
        minor_i++;
      }
    }
  }

  return status;
}

int s21_determinant(matrix_t *A, double *result) {
  int status = 0;
  if (A == NULL || result == NULL || A->matrix == NULL) {
    status = 1;
  } else if (A->rows != A->columns) {
    status = 2;
  } else {
    *result = 0;

    if (A->rows == 1) {
      *result = A->matrix[0][0];
    } else if (A->rows == 2) {
      *result =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    } else {
      for (int j = 0; j < A->columns && status == 0; j++) {
        matrix_t minor;
        if (s21_get_minor(A, 0, j, &minor) == 0) {
          double minor_det;
          if (s21_determinant(&minor, &minor_det) == 0) {
            *result += A->matrix[0][j] * pow(-1, j) * minor_det;
          } else {
            status = 1;
          }
          s21_remove_matrix(&minor);
        } else {
          status = 1;
        }
      }
    }
  }
  return status;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = 0;

  if (A == NULL || result == NULL || A->matrix == NULL) {
    status = 1;
  } else if (A->rows != A->columns) {
    status = 2;
  } else if (s21_create_matrix(A->rows, A->columns, result) != 0) {
    status = 1;
  } else {
    // Особый случай для матрицы 1x1
    if (A->rows == 1) {
      result->matrix[0][0] = 1.0;  // Алгебраическое дополнение для 1x1 всегда 1
    } else {
      for (int i = 0; i < A->rows && status == 0; i++) {
        for (int j = 0; j < A->columns && status == 0; j++) {
          matrix_t minor;
          if (s21_get_minor(A, i, j, &minor) == 0) {
            double minor_det;
            if (s21_determinant(&minor, &minor_det) == 0) {
              result->matrix[i][j] = pow(-1, i + j) * minor_det;
            } else {
              status = 1;
            }
            s21_remove_matrix(&minor);
          } else {
            status = 1;
          }
        }
      }
    }
  }

  return status;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int status = 0;

  if (A == NULL || result == NULL || A->matrix == NULL) {
    status = 1;
  } else {
    double det = 0;
    if (s21_determinant(A, &det) != 0) {
      status = 2;
    } else if (fabs(det) < 1e-6) {
      status = 2;
    } else {
      matrix_t algdop, transponse;
      if (s21_calc_complements(A, &algdop) == 0) {
        if (s21_transpose(&algdop, &transponse) == 0) {
          if (s21_mult_number(&transponse, 1.0 / det, result) != 0) {
            status = 1;
          }
        } else {
          status = 1;
        }
        s21_remove_matrix(&algdop);
        s21_remove_matrix(&transponse);
      } else {
        status = 1;
      }
    }
  }

  return status;
}
