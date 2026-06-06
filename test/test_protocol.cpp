#include <gtest/gtest.h>
#include "MyProtocolStream.h"
#include "Protocol.h"

using namespace net;

// ==================== MyProtocolStream 基础类型往返测试 ====================

TEST(MyProtocolStream, Int32RoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    int32_t val = 12345678;
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    int32_t out = 0;
    reader >> out;
    EXPECT_EQ(val, out);
}

TEST(MyProtocolStream, Int32Negative) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    int32_t val = -99999;
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    int32_t out = 0;
    reader >> out;
    EXPECT_EQ(val, out);
}

TEST(MyProtocolStream, Int32Zero) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    int32_t val = 0;
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    int32_t out = 1;
    reader >> out;
    EXPECT_EQ(0, out);
}

TEST(MyProtocolStream, Int64RoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    int64_t val = 9876543210LL;
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    int64_t out = 0;
    reader >> out;
    EXPECT_EQ(val, out);
}

TEST(MyProtocolStream, ShortRoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    short val = 12345;
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    short out = 0;
    reader >> out;
    EXPECT_EQ(val, out);
}

TEST(MyProtocolStream, CharRoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    char val = 'A';
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    char out = 0;
    reader >> out;
    EXPECT_EQ(val, out);
}

TEST(MyProtocolStream, BoolRoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    writer << true;
    writer << false;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    bool out1 = false, out2 = true;
    reader >> out1 >> out2;
    EXPECT_TRUE(out1);
    EXPECT_FALSE(out2);
}

TEST(MyProtocolStream, StringRoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    std::string val = "Hello, World!";
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    std::string out;
    reader >> out;
    EXPECT_EQ(val, out);
}

TEST(MyProtocolStream, EmptyStringRoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    std::string val = "";
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    std::string out = "not_empty";
    reader >> out;
    EXPECT_EQ("", out);
}

TEST(MyProtocolStream, FloatRoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    float val = 3.14f;
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    float out = 0;
    reader >> out;
    EXPECT_FLOAT_EQ(val, out);
}

TEST(MyProtocolStream, DoubleRoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    double val = 3.14159265358979;
    writer << val;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    double out = 0;
    reader >> out;
    EXPECT_DOUBLE_EQ(val, out);
}

// ==================== 多字段连续读写测试 ====================

TEST(MyProtocolStream, MultiFieldRoundTrip) {
    std::string buf;
    MyProtocolStream writer(buf, PROTOCOL_ROW);
    int32_t i = 42;
    std::string s = "test_string";
    float f = 2.718f;
    short sh = 999;
    char c = 'Z';
    writer << i << s << f << sh << c;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    int32_t i_out;
    std::string s_out;
    float f_out;
    short sh_out;
    char c_out;
    reader >> i_out >> s_out >> f_out >> sh_out >> c_out;

    EXPECT_EQ(42, i_out);
    EXPECT_EQ("test_string", s_out);
    EXPECT_FLOAT_EQ(2.718f, f_out);
    EXPECT_EQ(999, sh_out);
    EXPECT_EQ('Z', c_out);
}

// ==================== PROTOCOL_PACK 模式测试 ====================

TEST(MyProtocolStream, ProtocolPackFlush) {
    std::string buf;
    MyProtocolStream stream(buf, PROTOCOL_PACK);
    stream << (int32_t)100 << (std::string)"hello" << endll;

    // PROTOCOL_PACK 模式下前 4 字节是长度，5-6 字节是 checksum
    EXPECT_GT(stream.size(), 6);

    // 验证长度字段
    size_t len = 0;
    EXPECT_TRUE(stream.readLength(len));
    EXPECT_EQ(len, stream.size());

    // 读回数据
    MyProtocolStream reader(buf, PROTOCOL_PACK);
    int32_t i_out;
    std::string s_out;
    reader >> i_out >> s_out;
    EXPECT_EQ(100, i_out);
    EXPECT_EQ("hello", s_out);
}

// ==================== 7-bit 编码测试 ====================

TEST(SevenBitEncoding, SmallValue) {
    std::string buf;
    write7BitEncoded((uint32_t)0, buf);
    EXPECT_EQ(1u, buf.size());

    uint32_t out = 999;
    read7BitEncoded(buf.c_str(), buf.size(), out);
    EXPECT_EQ(0u, out);
}

TEST(SevenBitEncoding, Boundary127) {
    std::string buf;
    write7BitEncoded((uint32_t)127, buf);
    EXPECT_EQ(1u, buf.size());

    uint32_t out = 0;
    read7BitEncoded(buf.c_str(), buf.size(), out);
    EXPECT_EQ(127u, out);
}

TEST(SevenBitEncoding, Boundary128) {
    std::string buf;
    write7BitEncoded((uint32_t)128, buf);
    EXPECT_EQ(2u, buf.size());

    uint32_t out = 0;
    read7BitEncoded(buf.c_str(), buf.size(), out);
    EXPECT_EQ(128u, out);
}

TEST(SevenBitEncoding, Boundary16383) {
    std::string buf;
    write7BitEncoded((uint32_t)16383, buf);
    EXPECT_EQ(2u, buf.size());

    uint32_t out = 0;
    read7BitEncoded(buf.c_str(), buf.size(), out);
    EXPECT_EQ(16383u, out);
}

TEST(SevenBitEncoding, Boundary16384) {
    std::string buf;
    write7BitEncoded((uint32_t)16384, buf);
    EXPECT_EQ(3u, buf.size());

    uint32_t out = 0;
    read7BitEncoded(buf.c_str(), buf.size(), out);
    EXPECT_EQ(16384u, out);
}

TEST(SevenBitEncoding, MaxUint32) {
    std::string buf;
    write7BitEncoded((uint32_t)0xFFFFFFFF, buf);
    EXPECT_EQ(5u, buf.size());

    uint32_t out = 0;
    read7BitEncoded(buf.c_str(), buf.size(), out);
    EXPECT_EQ(0xFFFFFFFF, out);
}

TEST(SevenBitEncoding, Uint64RoundTrip) {
    std::string buf;
    uint64_t val = 123456789012345ULL;
    write7BitEncoded(val, buf);

    uint64_t out = 0;
    read7BitEncoded(buf.c_str(), buf.size(), out);
    EXPECT_EQ(val, out);
}

// ==================== Checksum 测试 ====================

TEST(Checksum, Deterministic) {
    const char data[] = "Hello, World!";
    unsigned short cs1 = checksum(reinterpret_cast<const unsigned short*>(data), sizeof(data));
    unsigned short cs2 = checksum(reinterpret_cast<const unsigned short*>(data), sizeof(data));
    EXPECT_EQ(cs1, cs2);
}

TEST(Checksum, DifferentDataDifferentChecksum) {
    const char data1[] = "Hello";
    const char data2[] = "World";
    unsigned short cs1 = checksum(reinterpret_cast<const unsigned short*>(data1), sizeof(data1));
    unsigned short cs2 = checksum(reinterpret_cast<const unsigned short*>(data2), sizeof(data2));
    EXPECT_NE(cs1, cs2);
}

// ==================== 工具方法测试 ====================

TEST(MyProtocolStream, EmptyCheck) {
    std::string buf;
    MyProtocolStream stream(buf, PROTOCOL_PACK);
    EXPECT_TRUE(stream.empty());

    stream << (int32_t)1;
    EXPECT_FALSE(stream.empty());
}

TEST(MyProtocolStream, IsEnd) {
    std::string buf;
    MyProtocolStream stream(buf, PROTOCOL_ROW);
    stream << (int32_t)42;

    MyProtocolStream reader(buf, PROTOCOL_ROW);
    EXPECT_FALSE(reader.isEnd());
    int32_t out;
    reader >> out;
    EXPECT_TRUE(reader.isEnd());
}

TEST(MyProtocolStream, ClearAndReset) {
    std::string buf;
    MyProtocolStream stream(buf, PROTOCOL_PACK);
    stream << (int32_t)42 << endll;
    EXPECT_FALSE(stream.empty());

    stream.clear();
    EXPECT_TRUE(stream.empty());

    stream.setPos(0);
    EXPECT_EQ(6, stream.currentIndex()); // PROTOCOL_PACK header size
}

// ==================== Request 往返测试 ====================

TEST(Request, SerialDeserialRoundTrip) {
    Request req(1, 6, 0, 1, 1234567890, "hello payload", 42, 7);
    std::string data = req.serial();

    Request req2;
    req2.deserial(data);

    EXPECT_EQ(req.mType, req2.mType);
    EXPECT_EQ(req.mFunctionCode, req2.mFunctionCode);
    EXPECT_EQ(req.mFlag, req2.mFlag);
    EXPECT_EQ(req.mDirection, req2.mDirection);
    EXPECT_EQ(req.mTimeStamp, req2.mTimeStamp);
    EXPECT_EQ(req.mUserId, req2.mUserId);
    EXPECT_EQ(req.mSecret, req2.mSecret);
    EXPECT_EQ(req.mData, req2.mData);
}

TEST(Request, EmptyData) {
    Request req(0, 0, 0, 0, 0, "", 0, 0);
    std::string data = req.serial();

    Request req2;
    req2.deserial(data);

    EXPECT_EQ(0, req2.mType);
    EXPECT_EQ(0, req2.mFunctionCode);
    EXPECT_EQ("", req2.mData);
}

// ==================== Response 往返测试 ====================

TEST(Response, SerialDeserialWithData) {
    Response resp(1, 6, 0, 2, 1234567890, 200, 42, 7, true, "response data");
    std::string data = resp.serial();

    Response resp2;
    resp2.deserial(data);

    EXPECT_EQ(resp.mType, resp2.mType);
    EXPECT_EQ(resp.mFunctionCode, resp2.mFunctionCode);
    EXPECT_EQ(resp.mFlag, resp2.mFlag);
    EXPECT_EQ(resp.mDirection, resp2.mDirection);
    EXPECT_EQ(resp.mTimeStamp, resp2.mTimeStamp);
    EXPECT_EQ(resp.mCode, resp2.mCode);
    EXPECT_EQ(resp.mUserId, resp2.mUserId);
    EXPECT_EQ(resp.mSecret, resp2.mSecret);
    EXPECT_EQ(resp.mhasData, resp2.mhasData);
    EXPECT_EQ(resp.mData, resp2.mData);
}

TEST(Response, SerialDeserialNoData) {
    Response resp(0, 0, 0, 0, 0, 404, 0, 0, false, "");
    std::string data = resp.serial();

    Response resp2;
    resp2.deserial(data);

    EXPECT_EQ(404, resp2.mCode);
    EXPECT_FALSE(resp2.mhasData);
}
