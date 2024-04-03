#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define LOG_ERROR std::cerr << std::endl << "ERROR: "
#define LOG_INFO std::cout << std::endl

bool readLastLine(const std::string &filepath, const int linecount, std::vector<std::string> &lastlines) {
    LOG_INFO << __func__ << ", filepath: " << filepath << ", linecount: " << linecount;
    lastlines.clear();
    std::ifstream fin(filepath, std::ios::ate);
    if (!fin.good()) {
        LOG_ERROR << __func__ << ", read file failed!";
        return false;
    }
    fin.seekg(-2, fin.end);
    if (!fin.good()) {
        LOG_ERROR << __func__ << ", seekg failed!";
        return false;
    }
    for (int i = 0; i < linecount; ++i) {
        while (fin.peek() != fin.widen('\n') && fin.good()) {
            if (fin.tellg() == 0) break;
            fin.seekg(-1, fin.cur);
            if (!fin.good() || fin.tellg() < 0) {
                LOG_ERROR << __func__ << ", seekg failed, good: " << fin.good() << ", tellg: " << fin.tellg();
                return false;
            }
        }
        if (fin.tellg() == 0) break;
        fin.seekg(-1, fin.cur);
        if (!fin.good()) {
            LOG_ERROR << __func__ << ", peek failed!";
            return false;
        }
    }
    if (fin.tellg() != 0) fin.seekg(2, fin.cur);
    std::string line;
    while (std::getline(fin, line)) {
        lastlines.push_back(line);
        LOG_INFO << "got line: " << line;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "add filepath" << std::endl;
        return -1;
    }
    std::vector<std::string> result;
    bool ret = readLastLine(argv[1], 3, result);
    LOG_INFO << "run ret: " << ret << ", result.size: " << result.size() << std::endl;
}
