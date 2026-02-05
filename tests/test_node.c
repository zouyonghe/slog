#include "slog.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

static void node_put_list(struct slog_node *node) {
	while (node) {
		struct slog_node *next = node->next;
		if (node->type == SLOG_TYPE_ARRAY) {
			node_put_list(node->value.array);
		} else if (node->type == SLOG_TYPE_OBJECT) {
			node_put_list(node->value.object);
		}
		slog_node_put(node);
		node = next;
	}
}

static int suite_cleanup(void) {
	SLOG_FREE();
	return 0;
}

void test_array_clears_keys(void) {
	struct slog_node *array =
		SLOG_ARRAY("items", SLOG_INT("id", 1), SLOG_STRING(NULL, "ok"));

	CU_ASSERT_EQUAL(array->type, SLOG_TYPE_ARRAY);
	CU_ASSERT_STRING_EQUAL(array->key, "items");

	struct slog_node *item = array->value.array;
	CU_ASSERT_PTR_NOT_NULL(item);
	CU_ASSERT_PTR_NULL(item->key);
	CU_ASSERT_EQUAL(item->type, SLOG_TYPE_INT);
	CU_ASSERT_EQUAL(item->value.integer, 1);

	item = item->next;
	CU_ASSERT_PTR_NOT_NULL(item);
	CU_ASSERT_PTR_NULL(item->key);
	CU_ASSERT_EQUAL(item->type, SLOG_TYPE_STRING);
	CU_ASSERT_STRING_EQUAL(item->value.string, "ok");
	CU_ASSERT_PTR_NULL(item->next);

	node_put_list(array);
}

void test_object_keeps_keys(void) {
	struct slog_node *object = SLOG_OBJECT("meta", SLOG_STRING("name", "slog"),
				      SLOG_INT("version", 1));

	CU_ASSERT_EQUAL(object->type, SLOG_TYPE_OBJECT);
	CU_ASSERT_STRING_EQUAL(object->key, "meta");

	struct slog_node *field = object->value.object;
	CU_ASSERT_PTR_NOT_NULL(field);
	CU_ASSERT_STRING_EQUAL(field->key, "name");
	CU_ASSERT_EQUAL(field->type, SLOG_TYPE_STRING);
	CU_ASSERT_STRING_EQUAL(field->value.string, "slog");

	field = field->next;
	CU_ASSERT_PTR_NOT_NULL(field);
	CU_ASSERT_STRING_EQUAL(field->key, "version");
	CU_ASSERT_EQUAL(field->type, SLOG_TYPE_INT);
	CU_ASSERT_EQUAL(field->value.integer, 1);
	CU_ASSERT_PTR_NULL(field->next);

	node_put_list(object);
}

int main(void) {
	CU_initialize_registry();

	CU_pSuite suite_node = CU_add_suite("node", NULL, suite_cleanup);
	CU_add_test(suite_node, "array clears keys", test_array_clears_keys);
	CU_add_test(suite_node, "object keeps keys", test_object_keeps_keys);

	CU_basic_run_tests();
	CU_cleanup_registry();
}
