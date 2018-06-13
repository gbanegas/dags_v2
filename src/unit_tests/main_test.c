/*
 * main_test.c
 *
 *  Created on: Jun 11, 2018
 *      Author: vader
 */
#include <check.h>
#include <stdlib.h>

#include <sys/resource.h>

#include "check_gf.h"
#include "check_matrix.h"
#include "check_key_gen.h"

#if defined(TEST)
int main(void) {
	const rlim_t kStackSize = 64L * 1024L * 1024L;   // min stack size = 64 Mb
	struct rlimit rl;
	int result;

	result = getrlimit(RLIMIT_STACK, &rl);
	if (result == 0) {
		if (rl.rlim_cur < kStackSize) {
			rl.rlim_cur = kStackSize;
			result = setrlimit(RLIMIT_STACK, &rl);
			if (result != 0) {
				fprintf(stderr, "setrlimit returned result = %d\n", result);
			}
		}
	}
	int number_failed;
	Suite *s;
	SRunner *sr;

	/*s = gf_suite();
	 sr = srunner_create(s);

	 srunner_run_all(sr, CK_VERBOSE);
	 number_failed = srunner_ntests_failed(sr);
	 srunner_free(sr);*/

	s = key_gen_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_VERBOSE);

	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
