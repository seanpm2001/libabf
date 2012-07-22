#include "unity.h"
#include "abf2_struct.h"

#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

#define PACK_STRUCT

#define BIT_SIZE(type) (sizeof(type) * CHAR_BIT)

#define BIT_SIZE_EQUAL(expected_bits, type)                     \
    TEST_ASSERT_EQUAL_INT(expected_bits, BIT_SIZE(type))

#define BYTE_SIZE_EQUAL(expected_bytes, type)           \
    TEST_ASSERT_EQUAL_INT(expected_bytes, sizeof(type))

void setUp(void) {}
void tearDown(void) {}

void test_char_is_8bits(void)
{
    TEST_ASSERT_EQUAL_INT(8, CHAR_BIT);
}

void test_float_is_4bytes(void)
{
    BYTE_SIZE_EQUAL(4, float);
}

void test_double_is_8bytes(void)
{
    BYTE_SIZE_EQUAL(8, double);
}

void test_uint64_is_8bytes(void)
{
    BYTE_SIZE_EQUAL(8, uint64_t);
}

void test_bool_is_1byte(void)
{
    BYTE_SIZE_EQUAL(1, bool);
}

/* void test_GUID_is_128bit(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BIT_SIZE_EQUAL(128, struct guid); */
/* } */

/* void test_ABF_Section_is_16bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(16, struct abf2_section); */
/* } */

/* void test_ABF_FileInfo_is_512bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(512, struct abf2_fileinfo); */
/* } */

/* void test_ABF_ProtocolInfo_is_512bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(512, struct abf2_protocolinfo); */
/* } */

/* void test_ABF_MathInfo_is_128bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(128, struct abf2_mathinfo); */
/* } */

/* void test_ABF_ADCInfo_is_128bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(128, struct abf2_adcinfo); */
/* } */

/* void test_ABF_DACInfo_is_256bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(256, struct abf2_dacinfo); */
/* } */

/* void test_ABF_EpochInfoPerDAC_is_48bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(48, struct abf2_epochinfoperdac); */
/* } */

/* void test_ABF_EpochInfo_is_32bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(32, ABF_EpochInfo); */
/* } */

/* void test_ABF_StatsRegionInfo_is_128bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(128, struct abf2_statsregioninfo); */
/* } */

/* void test_ABF_UserListInfo_is_64bytes(void) */
/* { */
/*     TEST_IGNORE(); */
/*     BYTE_SIZE_EQUAL(64, struct abf2_userlistinfo); */
/* } */
