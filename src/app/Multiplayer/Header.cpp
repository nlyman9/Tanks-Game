#include "Header.hpp"

inline std::ostream& operator<<(std::ostream &out, const Header head) {
    out << head.type.c_str() << head.value.c_str();
    return out;
}
