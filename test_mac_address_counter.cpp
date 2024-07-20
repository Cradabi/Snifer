#include <gtest/gtest.h>
#include <string>
#include "mac_address_counter.h"

std::string g_testFilename;

TEST(MacAddressCounterTest, CorrectOutput) {
    //std::string testFilename = "frames_parser.log";
    std::string result = process_file(g_testFilename);

    std::string expected = "b8:69:f4:7a:a5:ac: 15235\n"
                           "34:1c:f0:d3:40:a2: 5812\n"
                           "34:1c:f0:d2:78:5a: 5307\n"
                           "00:0c:29:65:08:ee: 3713\n"
                           "84:c5:a6:07:38:66: 124\n"
                           "6e:52:4e:5f:f9:eb: 107\n"
                           "4a:5f:99:ae:ea:99: 98\n"
                           "ff:ff:ff:ff:ff:ff: 98\n"
                           "b8:69:f4:7a:a5:93: 68\n"
                           "52:ff:20:52:16:9a: 14\n"
                           "70:c9:32:1b:54:e2: 13\n"
                           "80:b6:55:60:6f:58: 4\n"
                           "c8:7f:54:28:74:ac: 3\n";

    EXPECT_EQ(result, expected);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <test_filename>" << std::endl;
        return 1;
    }
    g_testFilename = argv[1];
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
