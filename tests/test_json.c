#include "slog.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
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

static void assert_contains(const char *haystack, const char *needle) {
	CU_ASSERT_PTR_NOT_NULL(strstr(haystack, needle));
}

void test_json_format(void) {
	SLOG_SET_HANDLER(capture_handler);

	SLOG(SLOG_INFO, "hello", SLOG_STRING("user", "bob"),
	     SLOG_INT("count", 2),
	     SLOG_ARRAY("ids", SLOG_INT(NULL, 1), SLOG_INT(NULL, 2)),
	     SLOG_OBJECT("meta", SLOG_BOOL("active", true)));

	CU_ASSERT_PTR_NOT_NULL(captured);
	const size_t len = strlen(captured);
	CU_ASSERT(len > 2);
	CU_ASSERT_EQUAL(captured[0], '{');
	CU_ASSERT_EQUAL(captured[len - 1], '}');
	CU_ASSERT_NOT_EQUAL(captured[len - 2], ',');

	assert_contains(captured, "\"file\":\"");
	assert_contains(captured, "\"line\":");
	assert_contains(captured, "\"func\":\"");
	assert_contains(captured, "\"level\":\"INFO\"");
	assert_contains(captured, "\"time\":\"");
	assert_contains(captured, "\"msg\":\"hello\"");
	assert_contains(captured, "\"user\":\"bob\"");
	assert_contains(captured, "\"count\":2");
	assert_contains(captured, "\"ids\":[1,2]");
	assert_contains(captured, "\"meta\":{\"active\":true}");
}

void test_json_escape(void) {
	SLOG_SET_HANDLER(capture_handler);

	SLOG(SLOG_INFO, "quote \" and newline\n",
	     SLOG_STRING("note", "line\nbreak"));

	CU_ASSERT_PTR_NOT_NULL(captured);
	assert_contains(captured, "\\\"");
	assert_contains(captured, "\\n");
	assert_contains(captured, "\"note\":\"line\\nbreak\"");
}

int main(void) {
	CU_initialize_registry();

	CU_pSuite suite_json = CU_add_suite("json", NULL, suite_cleanup);
	CU_add_test(suite_json, "json format", test_json_format);
	CU_add_test(suite_json, "json escape", test_json_escape);

	CU_basic_run_tests();
	CU_cleanup_registry();
}
