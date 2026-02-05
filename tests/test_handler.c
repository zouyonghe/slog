#include "slog.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <string.h>

static char *captured = NULL;
static int handler_calls = 0;

static void capture_handler(const char *str) {
	handler_calls++;
	free(captured);
	captured = strdup(str);
}

static int suite_cleanup(void) {
	SLOG_FREE();
	free(captured);
	captured = NULL;
	handler_calls = 0;
	return 0;
}

void test_handler_receives_output(void) {
	SLOG_SET_HANDLER(capture_handler);

	SLOG(SLOG_WARN, "handler test", SLOG_INT("id", 7));

	CU_ASSERT_EQUAL(handler_calls, 1);
	CU_ASSERT_PTR_NOT_NULL(captured);
	CU_ASSERT_PTR_NOT_NULL(strstr(captured, "\"msg\":\"handler test\""));
	CU_ASSERT_PTR_NOT_NULL(strstr(captured, "\"level\":\"WARN\""));
	CU_ASSERT_PTR_NOT_NULL(strstr(captured, "\"id\":7"));
}

int main(void) {
	CU_initialize_registry();

	CU_pSuite suite_handler = CU_add_suite("handler", NULL, suite_cleanup);
	CU_add_test(suite_handler, "handler receives output",
		    test_handler_receives_output);

	CU_basic_run_tests();
	CU_cleanup_registry();
}
