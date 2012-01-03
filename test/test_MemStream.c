#include "unity.h"
#include "MemStream.h"

#include <string.h>

#define STREAM_SIZE 512

Stream stream;

void setUp(void)
{
    stream = MemStream_Create(STREAM_SIZE);
}

void tearDown(void)
{
    MemStream_Destroy(stream);
}

void streamPositionIs(streamPosition expected)
{
    TEST_ASSERT_EQUAL_INT(expected, Stream_getCurrentPosition(stream));
}

void streamPositionIsNot(streamPosition expected)
{
    streamPosition actual = Stream_getCurrentPosition(stream);
    if (expected == actual) {
        char fail_string[256];
        sprintf(fail_string,
                "expected <%lu> == actual <%lu>, but should not!",
                (long)expected, (long)actual);
        TEST_FAIL_MESSAGE(fail_string);
    }
}

void test_MemStream_BasicSeekToStart(void)
{
    Stream_seek(stream, 20, 0);
    Stream_seek(stream, 0, 0);
    streamPositionIs(0);
}

void test_MemStream_BasicSeekToEnd(void)
{
    Stream_seek(stream, STREAM_SIZE - 1, 0);
    streamPositionIs(STREAM_SIZE - 1);
}

void test_MemStream_SeekPastStreamEndDoesNothing(void)
{
    Stream_seek(stream, 0, 0);
    Stream_seek(stream, STREAM_SIZE, 0);
    streamPositionIs(0);
}

void test_MemStream_SeekPastStreamStartDoesNothing(void)
{
    Stream_seek(stream, 0, 0);
    Stream_seek(stream, -5, 0);
    streamPositionIs(0);
}

void test_MemStream_GetPosition(void)
{
    streamPositionIs(0);
}

void test_MemStream_SeekToLocation(void)
{
    Stream_seek(stream, 5, 0);
    streamPositionIs(5);
}

void test_MemStream_SeekFromStart(void)
{
    Stream_seekFromStart(stream, 10);
    streamPositionIs(10);
}

void test_MemStream_SeekFromCurrent(void)
{
    Stream_seek(stream, 10, 0);
    Stream_seekFromCurrent(stream, 10);
    streamPositionIs(20);
}

void test_MemStream_SeekFromEnd(void)
{
    Stream_seekFromEnd(stream, 10);
    streamPosition lastByte = STREAM_SIZE - 1;
    streamPositionIs(lastByte - 10);
}

void test_MemStream_SeekToStart(void)
{
    Stream_seek(stream, 10, 0);
    streamPositionIsNot(0);
    Stream_seekToStart(stream);
    streamPositionIs(0);
}

void test_MemStream_SeekToEnd(void)
{
    streamPosition lastByte = STREAM_SIZE - 1;
    Stream_seek(stream, 0, 0);
    streamPositionIsNot(lastByte);

    Stream_seekToEnd(stream);
    streamPositionIs(lastByte);
}

void test_MemStream_WriteSingleChunk(void)
{
    int8_t byteToWrite = 0xFF;
    Stream_writeChunk(stream, &byteToWrite, sizeof(byteToWrite));
    streamPositionIs(1);
    TEST_ASSERT_EQUAL_HEX(byteToWrite, MemStream_getByteAt(stream, 0));
}

void test_MemStream_WriteTooBigChunkFails(void)
{
    int8_t chunkToWrite[STREAM_SIZE + 1];
    memset(chunkToWrite, 1, sizeof(chunkToWrite));
    int isDone = Stream_writeChunk(stream, chunkToWrite, sizeof(chunkToWrite));

    TEST_ASSERT_EQUAL_INT(FALSE, isDone);
    streamPositionIs(0);
    TEST_ASSERT_EQUAL_HEX(0, MemStream_getByteAt(stream, 0));
}

void test_MemStream_WriteMultipleChunks(void)
{
    int8_t bytesToWrite[2] = {0xCA, 0xFE};
    Stream_writeMultipleChunks(stream, bytesToWrite, sizeof(int8_t), 2);
    streamPositionIs(2);
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[0], MemStream_getByteAt(stream, 0));
    TEST_ASSERT_EQUAL_HEX8(0xFE, MemStream_getByteAt(stream, 1));
}

void test_MemStream_ReadChunk(void)
{
    int8_t byteToWrite = 0xCA;
    Stream_writeChunk(stream, &byteToWrite, sizeof(int8_t));
    Stream_seekToStart(stream);

    int8_t byteFromStream;
    Stream_readChunk(stream, &byteFromStream, sizeof(int8_t));
    TEST_ASSERT_EQUAL_HEX8(0xCA, byteFromStream);
}

void test_MemStream_ReadMultipleChunks(void)
{
    int8_t bytesToWrite[2] = {0xCA, 0xFE};
    Stream_writeMultipleChunks(stream, bytesToWrite, sizeof(int8_t), 2);

    Stream_seekToStart(stream);
    int8_t bytesFromStream[2];
    Stream_readMultipleChunks(stream, bytesFromStream, sizeof(int8_t), 2);
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[0], bytesFromStream[0]);
    TEST_ASSERT_EQUAL_HEX8(bytesToWrite[1], bytesFromStream[1]);
}
