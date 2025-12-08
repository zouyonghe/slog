#include "slog.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

void test_string(void) {
	struct slog_node *node = SLOG_STRING("key", "value");

	CU_ASSERT_EQUAL(node->type, SLOG_TYPE_STRING);
	CU_ASSERT_STRING_EQUAL(node->key, "key");
	CU_ASSERT_STRING_EQUAL(node->value.string, "value");
	CU_ASSERT_PTR_NULL(node->next);

	// free
	slog_node_put(node);
}

int main(void) {
	CU_initialize_registry();

	CU_pSuite suite_string = CU_add_suite("string", NULL, NULL);
	CU_add_test(suite_string, "basic string", test_string);

	CU_basic_run_tests();
	CU_cleanup_registry();
}
