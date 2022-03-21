#pragma once
using std::exception;

namespace linalg {
    class LinAlgError : public std::runtime_error {
    public:
        LinAlgError(const char *ch_arr) : std::runtime_error(ch_arr) {}
    };
}