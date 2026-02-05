#include "slog.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

static int handler_calls = 0;

static void capture_handler(const char *str) {
	(void)str;
	handler_calls++;
}

static int suite_cleanup(void) {
	SLOG_FREE();
	handler_calls = 0;
	return 0;
}

void test_level_filtering(void) {
	SLOG_SET_HANDLER(capture_handler);
	SLOG_SET_LEVEL(SLOG_WARN);

	handler_calls = 0;
	SLOG(SLOG_INFO, "filtered");
	CU_ASSERT_EQUAL(handler_calls, 0);
	CU_ASSERT_EQUAL(SLOG_GET_LEVEL(), SLOG_WARN);

	SLOG(SLOG_WARN, "warned");
	CU_ASSERT_EQUAL(handler_calls, 1);

	SLOG(SLOG_ERROR, "error");
	CU_ASSERT_EQUAL(handler_calls, 2);
}

int main(void) {
	CU_initialize_registry();

	CU_pSuite suite_level = CU_add_suite("level", NULL, suite_cleanup);
	CU_add_test(suite_level, "level filtering", test_level_filtering);

	CU_basic_run_tests();
	CU_cleanup_registry();
}
