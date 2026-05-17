#include "int64_div.hpp"
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cfloat>

static int passed = 0, failed = 0;

#define TEST(name) printf("  %-55s", name)
#define OK()       do { printf("PASS\n"); passed++; } while(0)
#define FAIL(fmt, ...) do { printf("FAIL: " fmt "\n", ##__VA_ARGS__); failed++; } while(0)

static bool check(const char* label, int64_t got, int64_t expected) {
    if (got == expected) { OK(); return true; }
    FAIL("[%s] got %ld expected %ld", label, (long)got, (long)expected);
    return false;
}

static void test_basic() {
    printf("\n--- basic division ---\n");

    TEST("10 / 3");    check("10/3",   int64_div(10, 3), 3);
    TEST("10 / -3");   check("10/-3",  int64_div(10, -3), -3);
    TEST("-10 / 3");   check("-10/3",  int64_div(-10, 3), -3);
    TEST("-10 / -3");  check("-10/-3", int64_div(-10, -3), 3);
    TEST("7 / 2");     check("7/2",    int64_div(7, 2), 3);
    TEST("-7 / 2");    check("-7/2",   int64_div(-7, 2), -3);
    TEST("7 / -2");    check("7/-2",   int64_div(7, -2), -3);
    TEST("0 / 5");     check("0/5",    int64_div(0, 5), 0);
    TEST("0 / -5");    check("0/-5",   int64_div(0, -5), 0);
    TEST("100 / 10");  check("100/10", int64_div(100, 10), 10);
    TEST("100 / 7");   check("100/7",  int64_div(100, 7), 14);
}

static void test_by_zero() {
    printf("\n--- division by zero ---\n");

    TEST("5 / 0 = INT64_MAX");   check("5/0",   int64_div(5, 0),   INT64_MAX);
    TEST("1 / 0 = INT64_MAX");   check("1/0",   int64_div(1, 0),   INT64_MAX);
    TEST("MAX / 0 = INT64_MAX"); check("MAX/0", int64_div(INT64_MAX, 0), INT64_MAX);

    TEST("-5 / 0 = INT64_MIN");  check("-5/0",  int64_div(-5, 0),  INT64_MIN);
    TEST("-1 / 0 = INT64_MIN");  check("-1/0",  int64_div(-1, 0),  INT64_MIN);
    TEST("MIN / 0 = INT64_MIN"); check("MIN/0", int64_div(INT64_MIN, 0), INT64_MIN);

    TEST("0 / 0 throws");
    bool threw = false;
    try { int64_div(0, 0); } catch (const std::domain_error& e) { threw = true; (void)e; }
    if (threw) OK(); else FAIL("0/0 did not throw");
}

static void test_plus_minus_one() {
    printf("\n--- divisor ±1 ---\n");

    TEST("42 / 1");          check("42/1",     int64_div(42, 1), 42);
    TEST("-42 / 1");         check("-42/1",    int64_div(-42, 1), -42);
    TEST("42 / -1");         check("42/-1",    int64_div(42, -1), -42);
    TEST("-42 / -1");        check("-42/-1",   int64_div(-42, -1), 42);
    TEST("MAX / 1");         check("MAX/1",    int64_div(INT64_MAX, 1), INT64_MAX);
    TEST("MAX / -1");        check("MAX/-1",   int64_div(INT64_MAX, -1), -INT64_MAX);
    TEST("MIN / 1");         check("MIN/1",    int64_div(INT64_MIN, 1), INT64_MIN);
    TEST("MIN / -1");        check("MIN/-1",   int64_div(INT64_MIN, -1), INT64_MAX);
    TEST("0 / 1");           check("0/1",      int64_div(0, 1), 0);
    TEST("0 / -1");          check("0/-1",     int64_div(0, -1), 0);
}

static void test_int64min_divisor() {
    printf("\n--- INT64_MIN as divisor ---\n");

    int64_t m = INT64_MIN;
    TEST("0 / MIN");         check("0/MIN",        int64_div(0, m), 0);
    TEST("1 / MIN");         check("1/MIN",        int64_div(1, m), 0);
    TEST("-1 / MIN");        check("-1/MIN",       int64_div(-1, m), 0);
    TEST("42 / MIN");        check("42/MIN",       int64_div(42, m), 0);
    TEST("-42 / MIN");       check("-42/MIN",      int64_div(-42, m), 0);
    TEST("MAX / MIN");       check("MAX/MIN",      int64_div(INT64_MAX, m), 0);
    TEST("MIN / MIN");       check("MIN/MIN",      int64_div(m, m), 1);
    TEST("MIN+1 / MIN");     check("MIN+1/MIN",    int64_div(static_cast<int64_t>(INT64_MIN + 1ULL), m), 0);
}

static void test_int64min_dividend() {
    printf("\n--- INT64_MIN as dividend ---\n");

    int64_t m = INT64_MIN;
    TEST("MIN / 2");         check("MIN/2",    int64_div(m, 2),   -4611686018427387904LL);
    TEST("MIN / 3");         check("MIN/3",    int64_div(m, 3),   -3074457345618258602LL);
    TEST("MIN / -2");        check("MIN/-2",   int64_div(m, -2),   4611686018427387904LL);
    TEST("MIN / -3");        check("MIN/-3",   int64_div(m, -3),   3074457345618258602LL);
    TEST("MIN / 100");       check("MIN/100",  int64_div(m, 100), INT64_MIN / 100);
    TEST("MIN / MAX");       check("MIN/MAX",  int64_div(m, INT64_MAX), -1);
}

static void test_large_numbers() {
    printf("\n--- large numbers ---\n");

    int64_t a, b, expected;
    a = 4611686018427387903LL;  // ~INT64_MAX/2
    b = 2147483647LL;
    expected = a / b;
    TEST("~2^62 / ~2^31"); check("large1", int64_div(a, b), expected);

    TEST("MAX / 2");       check("MAX/2",   int64_div(INT64_MAX, 2), INT64_MAX / 2);
    TEST("MAX / 3");       check("MAX/3",   int64_div(INT64_MAX, 3), INT64_MAX / 3);
    TEST("MAX / MAX");     check("MAX/MAX", int64_div(INT64_MAX, INT64_MAX), 1);
    TEST("-MAX / MAX");    check("-MAX/MAX",int64_div(-INT64_MAX, INT64_MAX), -1);

    // large divisor
    TEST("MAX / (MAX-1)"); check("nbig",    int64_div(INT64_MAX, INT64_MAX - 1), 1);
    TEST("(MAX-1) / MAX"); check("nbig2",   int64_div(INT64_MAX - 1, INT64_MAX), 0);
}

static void test_random() {
    printf("\n--- random sampling (10000 tests) ---\n");

    srand(42);
    int ok = 0, ng = 0;
    for (int i = 0; i < 10000; i++) {
        int64_t a = static_cast<int64_t>(
            (static_cast<uint64_t>(rand()) << 32) ^
            (static_cast<uint64_t>(rand()) << 16) ^
            static_cast<uint64_t>(rand()));
        int64_t b;
        do {
            b = static_cast<int64_t>(
                (static_cast<uint64_t>(rand()) << 32) ^
                (static_cast<uint64_t>(rand()) << 16) ^
                static_cast<uint64_t>(rand()));
        } while (b == 0);

        if (a == INT64_MIN && b == -1) continue;  // overflow avoided

        int64_t expected = a / b;
        int64_t got = int64_div(a, b);
        if (got == expected) {
            ok++;
        } else {
            ng++;
            if (ng <= 10)
                printf("  mismatch: %ld / %ld = %ld  (expected %ld)\n",
                       (long)a, (long)b, (long)got, (long)expected);
        }
    }
    printf("  %-53s", "random 10000");
    if (ng == 0) { printf("PASS (%d ok)\n", ok); passed++; }
    else         { printf("FAIL (%d/%d)\n", ng, ok+ng); failed++; }
}

// Test that internal types meet the constraint (check sizeof)
static void test_type_sizes() {
    printf("\n--- type size constraints ---\n");
    TEST("sizeof(uint32_t) <= 4");  if (sizeof(uint32_t) == 4) OK(); else FAIL("got %zu", sizeof(uint32_t));
    TEST("sizeof(float)    <= 4");  if (sizeof(float)    == 4) OK(); else FAIL("got %zu", sizeof(float));
    TEST("sizeof(int16_t)  <= 2");  if (sizeof(int16_t)  == 2) OK(); else FAIL("got %zu", sizeof(int16_t));
}

// Manual enumeration of all edge cases
static void test_enum_edges() {
    printf("\n--- enumerated edge cases ---\n");

    struct { int64_t a, b, expect; } cases[] = {
        {0, 1, 0}, {0, -1, 0}, {0, 2, 0}, {0, -2, 0},
        {1, 1, 1}, {1, -1, -1}, {-1, 1, -1}, {-1, -1, 1},
        {2, 1, 2}, {2, -1, -2}, {-2, 1, -2}, {-2, -1, 2},
        {2, 2, 1}, {2, -2, -1}, {-2, 2, -1}, {-2, -2, 1},
        {3, 2, 1}, {3, -2, -1}, {-3, 2, -1}, {-3, -2, 1},
        {4, 2, 2}, {4, -2, -2}, {-4, 2, -2}, {-4, -2, 2},
        {5, 2, 2}, {5, -2, -2}, {-5, 2, -2}, {-5, -2, 2},
        {7, 3, 2}, {8, 3, 2}, {9, 3, 3},
        {100, 10, 10}, {99, 10, 9}, {101, 10, 10},
        {1, INT64_MAX, 0}, {-1, INT64_MAX, 0},
        {INT64_MAX, 1, INT64_MAX}, {INT64_MIN, 1, INT64_MIN},
    };
    int n = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < n; i++) {
        char buf[64];
        snprintf(buf, sizeof(buf), "case[%d] %ld/%ld", i, (long)cases[i].a, (long)cases[i].b);
        TEST(buf);
        check(buf, int64_div(cases[i].a, cases[i].b), cases[i].expect);
    }
}

int main() {
    printf("=== int64_div tests ===\n");

    test_type_sizes();
    test_basic();
    test_plus_minus_one();
    test_by_zero();
    test_enum_edges();
    test_large_numbers();
    test_int64min_divisor();
    test_int64min_dividend();
    test_random();

    printf("\n=== results: %d passed, %d failed ===\n", passed, failed);
    return failed ? 1 : 0;
}
