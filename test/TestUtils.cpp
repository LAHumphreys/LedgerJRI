#include "TestUtils.h"
#include <sstream>
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

TestFile::TestFile(const std::string &path, const std::string contents)
        : path(path)
        , owned(false)
{
    std::ofstream file(path.c_str());

    if (file.good()) {
        owned = true;
        file << contents;
    } else {
        throw "Could not open file for writing!";
    }
    file.flush();
    file.close();
}

TestFile::~TestFile()
{
    if (owned) {
        std::filesystem::remove(path);
    }
}

