#include "../s21_matrix.h"

#include <check.h>
#include <stdlib.h>

Suite *s21_create_matrix_suite(void);
Suite *s21_remove_matrix_suite(void);
Suite *s21_eq_matrix_suite(void);
Suite *s21_sum_matrix_suite(void);
Suite *s21_sub_matrix_suite(void);
Suite *s21_mult_number_suite(void);
Suite *s21_mult_matrix_suite(void);
Suite *s21_transpose_suite(void);
Suite *s21_determinant_suite(void);
Suite *s21_calc_complements_suite(void);
Suite *s21_inverse_matrix_suite(void);
Suite *s21_NULL_matrix(void);

START_TEST(s21_test_create_matrix_valid) {
  matrix_t A;
  int result = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(A.rows, 3);
  ck_assert_int_eq(A.columns, 3);
  ck_assert_ptr_nonnull(A.matrix);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_test_create_matrix_invalid) {
  matrix_t A;

  // Неверные строки
  int result1 = s21_create_matrix(-1, 3, &A);
  ck_assert_int_eq(result1, 1);

  // Неверные столбцы
  int result2 = s21_create_matrix(3, 0, &A);
  ck_assert_int_eq(result2, 1);

  // NULL указатель
  int result3 = s21_create_matrix(3, 3, NULL);
  ck_assert_int_eq(result3, 1);

  // Слишком большой
  int result4 = s21_create_matrix(1000000, 1000000, &A);
  ck_assert_int_eq(result4, 1);
}
END_TEST

START_TEST(s21_test_remove_matrix) {
  matrix_t A;
  s21_create_matrix(2, 2, &A);

  // Заполняем матрицу
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  s21_remove_matrix(&A);

  // Проверяем, что матрица очищена
  ck_assert_ptr_null(A.matrix);
  ck_assert_int_eq(A.rows, 0);
  ck_assert_int_eq(A.columns, 0);

  // Двойное удаление должно быть безопасным
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_test_eq_matrix_equal) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  // Заполняем одинаковыми значениями
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      A.matrix[i][j] = B.matrix[i][j] = i * 2 + j + 1;
    }
  }

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_test_eq_matrix_not_equal) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  // Заполняем разными значениями
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      A.matrix[i][j] = i * 2 + j + 1;
      B.matrix[i][j] = A.matrix[i][j] + 0.1;
    }
  }

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_test_eq_matrix_not_relevant) {
  matrix_t A, B;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 2, &B);

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_test_sum_matrix) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 6.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 8.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 10.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 12.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(1, 1, &B);
  ck_assert_int_eq(s21_sum_matrix(&B, &A, &result), 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_test_sub_matrix) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int status = s21_sub_matrix(&B, &A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 4.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(1, 1, &B);
  ck_assert_int_eq(s21_sub_matrix(&B, &A, &result), 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_test_mult_number) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = s21_mult_number(&A, 4, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 8.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 12.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 16.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(s21_test_mult_matrix) {
  matrix_t A, B, result;
  s21_create_matrix(3, 2, &A);
  s21_create_matrix(2, 3, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 4.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 5.0;
  A.matrix[2][0] = 3.0;
  A.matrix[2][1] = 6.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = -1.0;
  B.matrix[0][2] = 1.0;
  B.matrix[1][0] = 2.0;
  B.matrix[1][1] = 3.0;
  B.matrix[1][2] = 4.0;

  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 9.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 11.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][2], 17.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 12.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 13.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][2], 22.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][0], 15.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][1], 15.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][2], 27.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 3, &B);
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_test_transpose) {
  matrix_t A, result;
  s21_create_matrix(3, 2, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 4.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 5.0;
  A.matrix[2][0] = 3.0;
  A.matrix[2][1] = 6.0;

  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 2.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][2], 3.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 5.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][2], 6.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(s21_test_complements) {
  matrix_t A, result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;

  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 0.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 10.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][2], -20.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], -14.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][2], 8.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][0], -8.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][1], -2.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][2], 4.0, 1e-6);
  matrix_t B, result2, C;
  s21_create_matrix(1, 1, &B);
  ck_assert_int_eq(s21_calc_complements(&B, &result2), 0);
  ck_assert_double_eq(result2.matrix[0][0], 1);
  s21_create_matrix(2, 1, &C);
  ck_assert_int_eq(s21_calc_complements(&C, &result2), 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result2);
}
END_TEST

START_TEST(s21_test_determinant) {
  matrix_t A;
  double det;

  // Матрица 1x1
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;
  ck_assert_int_eq(s21_determinant(&A, &det), 0);
  ck_assert_double_eq_tol(det, 5.0, 1e-6);
  s21_remove_matrix(&A);

  // Матрица 2x2
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;
  ck_assert_int_eq(s21_determinant(&A, &det), 0);
  ck_assert_double_eq_tol(det, -2.0, 1e-6);
  s21_remove_matrix(&A);

  // Матрица 3x3
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  ck_assert_int_eq(s21_determinant(&A, &det), 0);
  ck_assert_double_eq_tol(det, -40.0, 1e-6);
  s21_remove_matrix(&A);

  s21_create_matrix(2, 1, &A);
  ck_assert_int_eq(s21_determinant(&A, &det), 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_test_inverse) {
  matrix_t A, result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2.0;
  A.matrix[0][1] = 5.0;
  A.matrix[0][2] = 7.0;
  A.matrix[1][0] = 6.0;
  A.matrix[1][1] = 3.0;
  A.matrix[1][2] = 4.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = -2.0;
  A.matrix[2][2] = -3.0;

  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], -1.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][2], 1.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], -38.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 41.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][2], -34.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][0], 27.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][1], -29.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[2][2], 24.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);

  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;
  A.matrix[2][0] = 7.0;
  A.matrix[2][1] = 8.0;
  A.matrix[2][2] = 9.0;

  status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(s21_test_eq_matrix_NULL) {
  matrix_t A, B, result;
  double a;
  s21_create_matrix(2, 2, &A);
  B.matrix = NULL;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), 1);
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), 1);
  ck_assert_int_eq(s21_mult_number(&B, 4.0, &result), 1);
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 1);
  ck_assert_int_eq(s21_transpose(&B, &result), 1);
  ck_assert_int_eq(s21_get_minor(&B, 0, 0, &result), 1);
  ck_assert_int_eq(s21_calc_complements(&B, &result), 1);
  ck_assert_int_eq(s21_determinant(&B, &a), 1);
  ck_assert_int_eq(s21_inverse_matrix(&B, &result), 1);
  s21_remove_matrix(&A);
  s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(s21_get_minor(&A, 0, 0, &result), 2);
  s21_remove_matrix(&A);
}
END_TEST

Suite *s21_create_matrix_suite(void) {
  Suite *s = suite_create("create_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_create_matrix_valid);
  tcase_add_test(tc, s21_test_create_matrix_invalid);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_remove_matrix_suite(void) {
  Suite *s = suite_create("remove_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_remove_matrix);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_eq_matrix_suite(void) {
  Suite *s = suite_create("eq_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_eq_matrix_equal);
  tcase_add_test(tc, s21_test_eq_matrix_not_equal);
  tcase_add_test(tc, s21_test_eq_matrix_not_relevant);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_sum_matrix_suite(void) {
  Suite *s = suite_create("sum_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_sum_matrix);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_sub_matrix_suite(void) {
  Suite *s = suite_create("sub_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_sub_matrix);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_mult_number_suite(void) {
  Suite *s = suite_create("mult_number");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_mult_number);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_mult_matrix_suite(void) {
  Suite *s = suite_create("mult_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_mult_matrix);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_transpose_suite(void) {
  Suite *s = suite_create("transpose");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_transpose);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_calc_complements_suite(void) {
  Suite *s = suite_create("complements");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_complements);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_determinant_suite(void) {
  Suite *s = suite_create("determinant");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_determinant);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_inverse_matrix_suite(void) {
  Suite *s = suite_create("inverse");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_inverse);

  suite_add_tcase(s, tc);
  return s;
}

Suite *s21_NULL_matrix(void) {
  Suite *s = suite_create("Null");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, s21_test_eq_matrix_NULL);

  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  int failed = 0;
  SRunner *runner = srunner_create(NULL);

  srunner_add_suite(runner, s21_create_matrix_suite());
  srunner_add_suite(runner, s21_remove_matrix_suite());
  srunner_add_suite(runner, s21_eq_matrix_suite());
  srunner_add_suite(runner, s21_sum_matrix_suite());
  srunner_add_suite(runner, s21_sub_matrix_suite());
  srunner_add_suite(runner, s21_mult_number_suite());
  srunner_add_suite(runner, s21_mult_matrix_suite());
  srunner_add_suite(runner, s21_transpose_suite());
  srunner_add_suite(runner, s21_calc_complements_suite());
  srunner_add_suite(runner, s21_determinant_suite());
  srunner_add_suite(runner, s21_inverse_matrix_suite());
  srunner_add_suite(runner, s21_NULL_matrix());

  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0) ? 0 : 1;
}