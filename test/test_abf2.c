#include <stdlib.h>
#include "unity.h"

#include "abf2.h"
#include "deserialize.h"
#include "swap.h"

void setUp(void) {}
void tearDown(void) {}

void test_get_repeated_string(void)
{
    char *dest = get_repeated_string('A', 4);
    TEST_ASSERT_EQUAL_STRING("AAAA", dest);
    free(dest);
}

void test_abf2_read_section(void)
{
    char bytes[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0X09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    char *buf = bytes;
    bool to_swap = 0;

    struct abf2_section sec;
    abf2_read_sectionp(buf, &sec, to_swap);
    TEST_ASSERT_EQUAL_HEX32(0x00010203, sec.uBlockIndex);
    TEST_ASSERT_EQUAL_HEX32(0x04050607, sec.uBytes);
    TEST_ASSERT_EQUAL_HEX64(0x08090A0B0C0D0E0F, sec.llNumEntries);
}

void test_abf2_read_section_swapped(void)
{
    char bytes[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0X09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    char *buf = bytes;
    bool to_swap = 1;

    char *head = buf;
    struct abf2_section sec;
    buf = abf2_read_sectionp(buf, &sec, to_swap);
    TEST_ASSERT_EQUAL_INT(16, (buf - head));
    TEST_ASSERT_EQUAL_HEX32(0x03020100, sec.uBlockIndex);
    TEST_ASSERT_EQUAL_HEX32(0x07060504, sec.uBytes);
    TEST_ASSERT_EQUAL_HEX64(0x0F0E0D0C0B0A0908, sec.llNumEntries);
}

void test_abf2_read_guid(void)
{
    /* example GUID from MSDN docs: 6B29FC40-CA47-1067-B31D-00DD010662DA */
    char bytes[16] = {0x6B, 0x29, 0xFC, 0x40,
                      0xCA, 0x47,
                      0x10, 0x67,
                      0xB3, 0x1D,
                      0x00, 0xDD, 0x01, 0x06, 0x62, 0xDA};
    char *buf = bytes;
    bool to_swap = 0;

    char *head = buf;
    struct guid guid;
    buf = abf2_read_guidp(buf, &guid, to_swap);
    TEST_ASSERT_EQUAL_INT(16, (buf - head)); /* returned pointer moves to next unread byte */
    TEST_ASSERT_EQUAL_HEX64(0x6B29FC40, guid.Data1);
    TEST_ASSERT_EQUAL_HEX32(0xCA47, guid.Data2);
    TEST_ASSERT_EQUAL_HEX32(0x1067, guid.Data3);
    TEST_ASSERT_EQUAL_HEX8(0xB3, guid.Data4[0]);
    TEST_ASSERT_EQUAL_HEX8(0x1D, guid.Data4[1]);
    TEST_ASSERT_EQUAL_HEX8(0x00, guid.Data4[2]);
    TEST_ASSERT_EQUAL_HEX8(0xDD, guid.Data4[3]);
    TEST_ASSERT_EQUAL_HEX8(0x01, guid.Data4[4]);
    TEST_ASSERT_EQUAL_HEX8(0x06, guid.Data4[5]);
    TEST_ASSERT_EQUAL_HEX8(0x62, guid.Data4[6]);
    TEST_ASSERT_EQUAL_HEX8(0xDA, guid.Data4[7]);
}

void test_abf2_read_fileinfo(void)
{
    char bytes[512] = {0x41,0x42,0x46,0x32,0x00,0x00,0x00,0x02,
                       0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x43,0x02,0x33,0x01,0x4b,0xe3,0x82,0x03,
                       0xc6,0x44,0x01,0x00,0x01,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x08,0x15,0xe7,0x1d,0xf2,0x3b,0xdc,0x41,
                       0xb7,0x04,0xa5,0xb8,0x42,0x06,0xa8,0x69,
                       0x10,0x00,0x02,0x0a,0x01,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x02,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
                       0x00,0x02,0x00,0x00,0x01,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
                       0x80,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
                       0x00,0x01,0x00,0x00,0x04,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,
                       0x80,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,
                       0xcb,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,
                       0x02,0x00,0x00,0x00,0x60,0xf3,0x3d,0x00,
                       0x00,0x00,0x00,0x00,0xfd,0x3d,0x00,0x00,
                       0x40,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,
                       0x01,0x03,0x00,0x00,0x01,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0xfe,0x3d,0x00,0x00,
                       0x0c,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    char *buf = bytes;
    bool to_swap = 1;
    char *head = buf;
    struct abf2_fileinfo finfo;
    abf2_read_fileinfo(buf, &finfo, to_swap);
    TEST_ASSERT_EQUAL_HEX32(0x32464241, finfo.uFileSignature);
}

void test_abf2_need_swap(void)
{
    uint32_t fsig = ABF2_FILESIGNATURE;
    uint32_t fsig_swap = _swap32(ABF2_FILESIGNATURE);
//    TEST_ASSERT_EQUAL_HEX32(0x32464241, fsig_swap);
    TEST_ASSERT_EQUAL_INT(ENDIAN_LITTLE, get_endian());
}
