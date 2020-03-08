//
// Created by lhumphreys on 08/03/2020.
//

#ifndef LEDGEREMITTER_TESTUTILS_H
#define LEDGEREMITTER_TESTUTILS_H
#include <string>

class TestFile {
public:
    TestFile(const std::string& path, const std::string contents);

    ~TestFile();
private:
    bool owned;
    std::string path;
};

#endif //LEDGEREMITTER_TESTUTILS_H
