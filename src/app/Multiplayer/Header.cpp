#include "Header.hpp"

inline std::ostream& operator<<(std::ostream &out, const Header head) {
    out << head.header.c_str() << head.value.c_str();
    return out;
}


extern Header HEAD = Header("TYPE", std::to_string((int)5));