#include "streamtest_utils.h"

void streamPositionIs(streampos_dt expected)
{
    streampos_dt curr_pos;
    if (STREAMERROR_SUCCESS == stream_getCurrentPosition(test_stream, &curr_pos))
        TEST_ASSERT_EQUAL_INT(expected, curr_pos);
    else
        TEST_FAIL_MESSAGE("Stream error");
}

void streamPositionIsNot(streampos_dt expected)
{
    streampos_dt actual;
    if (STREAMERROR_SUCCESS == stream_getCurrentPosition(test_stream, &actual)) {
        if (expected == actual) {
            char fail_string[256];
            sprintf(fail_string,
                    "expected <%lu> == actual <%lu>, but should not!",
                    (long)expected, (long)actual);
            TEST_FAIL_MESSAGE(fail_string);
        }
    } else {
        TEST_FAIL_MESSAGE("Stream error");
    }
}
