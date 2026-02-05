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

void test_int(void) {
	struct slog_node *node = SLOG_INT("id", 42);

	CU_ASSERT_EQUAL(node->type, SLOG_TYPE_INT);
	CU_ASSERT_STRING_EQUAL(node->key, "id");
	CU_ASSERT_EQUAL(node->value.integer, 42);
	CU_ASSERT_PTR_NULL(node->next);

	slog_node_put(node);
}

void test_float(void) {
	const double score = 96.5;
	struct slog_node *node = SLOG_FLOAT("score", score);

	CU_ASSERT_EQUAL(node->type, SLOG_TYPE_FLOAT);
	CU_ASSERT_STRING_EQUAL(node->key, "score");
	CU_ASSERT_DOUBLE_EQUAL(node->value.number, score, 0.0001);
	CU_ASSERT_PTR_NULL(node->next);

	slog_node_put(node);
}

void test_bool(void) {
	struct slog_node *node = SLOG_BOOL("active", true);

	CU_ASSERT_EQUAL(node->type, SLOG_TYPE_BOOL);
	CU_ASSERT_STRING_EQUAL(node->key, "active");
	CU_ASSERT_TRUE(node->value.boolean);
	CU_ASSERT_PTR_NULL(node->next);

	slog_node_put(node);
}

void test_time(void) {
	struct slog_node *node = slog_node_create(SLOG_TYPE_TIME, "time");

	CU_ASSERT_EQUAL(node->type, SLOG_TYPE_TIME);
	CU_ASSERT_STRING_EQUAL(node->key, "time");
	CU_ASSERT(node->value.time.tv_sec > 0);
	CU_ASSERT_PTR_NULL(node->next);

	slog_node_put(node);
}

int main(void) {
	CU_initialize_registry();

	CU_pSuite suite_string = CU_add_suite("string", NULL, NULL);
	CU_add_test(suite_string, "basic string", test_string);
	CU_add_test(suite_string, "basic int", test_int);
	CU_add_test(suite_string, "basic float", test_float);
	CU_add_test(suite_string, "basic bool", test_bool);
	CU_add_test(suite_string, "basic time", test_time);

	CU_basic_run_tests();
	CU_cleanup_registry();
}
