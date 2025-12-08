#include "slog.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static char *captured = NULL;

static void capture_handler(const char *str) {
	free(captured);
	captured = strdup(str);
}

static int suite_cleanup(void) {
	SLOG_FREE();
	free(captured);
	captured = NULL;
	return 0;
}

void test_long_message_expands_buffer(void) {
	SLOG_SET_HANDLER(capture_handler);

	const size_t payload_len = PIPE_BUF * 2; // Force growth beyond default
	char *payload = malloc(payload_len + 1);
	CU_ASSERT_PTR_NOT_NULL_FATAL(payload);

	memset(payload, 'A', payload_len);
	payload[payload_len] = '\0';

	SLOG(SLOG_INFO, "long payload", SLOG_STRING("payload", payload));

	CU_ASSERT_PTR_NOT_NULL_FATAL(captured);
	CU_ASSERT_PTR_NOT_NULL(strstr(captured, payload));
	CU_ASSERT(strlen(captured) > payload_len);

	free(payload);
}

int main(void) {
	CU_initialize_registry();

	CU_pSuite suite_buffer = CU_add_suite("buffer", NULL, suite_cleanup);
	CU_add_test(suite_buffer, "long messages grow buffer",
		    test_long_message_expands_buffer);

	CU_basic_run_tests();
	CU_cleanup_registry();
}
