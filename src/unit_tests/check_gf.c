/*
 * check_gf.c
 *
 *  Created on: Jun 8, 2018
 *      Author: vader
 */


#include "check_gf.h"

#if defined(TEST)
#if defined(DAGS_3) || defined(DAGS_5) || defined(DAGS_TOY)

START_TEST(test_gf_inv_f_q_m)
	{
		gf temp = gf_q_m_inv(1);
		ck_assert_int_eq(temp, 1);

		temp = gf_q_m_inv(33);
		ck_assert_int_eq(temp, 23262);

		temp = gf_q_m_inv(23262);
		ck_assert_int_eq(temp, 33);

		temp = gf_q_m_inv(32768);
		ck_assert_int_eq(temp, 41321);

	}END_TEST

START_TEST(test_gf_inv_f_q)
	{
		gf temp = gf_inv(1);
		ck_assert_int_eq(temp, 1);
		temp = gf_inv(4);
		ck_assert_int_eq(temp, 71);
		temp = gf_inv(128);
		ck_assert_int_eq(temp, 27);
		temp = gf_inv(27);
		ck_assert_int_eq(temp, 128);

	}END_TEST

START_TEST(test_absolut_representation)
	{
		gf temp[2] = { 169, 222 };
		gf result = absolut_field_representation(temp);
		ck_assert_int_eq(result, 32771);

		gf temp_2[2] = { 184, 50 };
		result = absolut_field_representation(temp_2);
		ck_assert_int_eq(result, 18601);

		gf temp_3[2] = { 114, 7 };
		result = absolut_field_representation(temp_3);
		ck_assert_int_eq(result, 65503);

	}END_TEST

START_TEST(test_relative_representation)
	{
		gf value = 256;
		gf temp = relative_field_representation(value, 0);
		ck_assert_int_eq(temp, 255);
		temp = relative_field_representation(value, 1);
		ck_assert_int_eq(temp, 226);

		value = 32771;
		temp = relative_field_representation(value, 0);
		ck_assert_int_eq(temp, 169);
		temp = relative_field_representation(value, 1);
		ck_assert_int_eq(temp, 222);

		value = 33410;
		temp = relative_field_representation(value, 0);
		ck_assert_int_eq(temp, 129);
		temp = relative_field_representation(value, 1);
		ck_assert_int_eq(temp, 157);

		value = 65503;
		temp = relative_field_representation(value, 0);
		ck_assert_int_eq(temp, 114);
		temp = relative_field_representation(value, 1);
		ck_assert_int_eq(temp, 7);

		value = 1;
		temp = relative_field_representation(value, 0);
		ck_assert_int_eq(temp, 1);
		temp = relative_field_representation(value, 1);
		ck_assert_int_eq(temp, 0);

	}END_TEST

Suite * gf_suite(void) {
	Suite *s;
	TCase *tc_core;

	s = suite_create("GF_DAGS");

	/* Core test case */
	tc_core = tcase_create("GF");

	tcase_add_test(tc_core, test_gf_inv_f_q_m);
	tcase_add_test(tc_core, test_gf_inv_f_q);
	tcase_add_test(tc_core, test_absolut_representation);
	tcase_add_test(tc_core, test_relative_representation);
	suite_add_tcase(s, tc_core);

	return s;
}

#endif

#if defined(DAGS_1)
START_TEST(test_)
{
	gf temp = gf_q_m_inv(in);
	ck_assert_int_eq(money_amount(m), 5);
}
END_TEST
#endif

#endif

