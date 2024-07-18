#include "SymmetricBlockMatrix.hpp"

int main() {
    constexpr auto m = symmetric_block_matrix<3>(
        1, 2, 3,
           4, 5,
              6
    );
    constexpr auto m_condensed = m.condense<0>();
    return m_condensed.get_block<0,0>();
}
