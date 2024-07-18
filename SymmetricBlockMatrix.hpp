#include <tuple>
#include <cstddef>

template <typename Expression>
class Inverse {
public:
    constexpr Inverse(Expression&& expression): expression(std::forward<Expression>(expression)) { }
    operator int() const {
        return 1/this->expression;
    }

protected:
    const Expression expression;
};

template <typename Expression>
constexpr Inverse<Expression> invert(Expression&& expression) {
    return {std::forward<Expression>(expression)};
}

// https://stackoverflow.com/questions/20162903/template-parameter-packs-access-nth-type-and-nth-element
template <size_t N, typename... T>
struct nth_type_of_pack;

template <typename T0, typename... T>
struct nth_type_of_pack<0, T0, T...> {
    using type = T0;
};
template <size_t N, typename T0, typename... T>
struct nth_type_of_pack<N, T0, T...> {
    using type = typename nth_type_of_pack<N-1, T...>::type;
};

template <size_t Dimension, typename... Blocks>
class SymmetricBlockMatrix {
protected:
    const std::tuple<Blocks...> blocks;

    // convert matrix indices (row_index, column_index) into the index in the parameter pack
    template <size_t row_index, size_t column_index>
    static constexpr size_t pack_index() {
        return ((2*dimension() - 1 - row_index)*row_index)/2 + column_index;
    }

public:
    // Declare protected constructor to force construction via helper function make_SymmetricBlockMatrix
    // It checks that the number of blocks is consistent with the dimension
    constexpr explicit SymmetricBlockMatrix(Blocks... blocks): blocks(blocks...) { }

public:
    static constexpr size_t dimension() {
        return Dimension;
    }

    static constexpr size_t number_blocks() {
        return sizeof...(Blocks);
    }

    template <size_t row_index, size_t column_index>
    constexpr auto get_block() const {
        static_assert(row_index <= column_index, "Only the upper triangular part of the matrix is described.");
        static_assert(row_index < Dimension, "The row index exceeds the dimension.");
        static_assert(column_index < Dimension, "The column index exceeds the dimension.");

        constexpr size_t block_index = SymmetricBlockMatrix::pack_index<row_index, column_index>();
        return std::get<block_index>(this->blocks);
    }

/*
    template <size_t row_index, size_t column_index>
    class block_type {
        using value = typename nth_type_of_pack<pack_index<row_index, column_index>(), Blocks...>::type;
    };
*/

    template <size_t Index>
    constexpr auto condense() const {
        static_assert(Dimension >= 2, "The linear system has less than two blocks and cannot be condensed.");
        static_assert(Index < Dimension, "The condensation index is larger than the dimension.");
    
        // TODO: invert the (Index, Index) block
        // just using a hard-coded block at the moment
        // What I'm trying to achieve does not work yet
        constexpr auto inverted_block = invert(1);

        //using RemainingBlocks = typename std::tuple_element<0, decltype(this->blocks)>::type;
        //constexpr auto remaining_blocks = std::get<0>(this->blocks);
        return SymmetricBlockMatrix<1, decltype(inverted_block)>{inverted_block};
        //return 0;
    }

    // Helper function via which all the instances of the class should be created
    template <size_t D, typename... B>
    friend constexpr SymmetricBlockMatrix<D, B...> symmetric_block_matrix(B... blocks);
};

template <size_t Dimension, typename... Blocks>
constexpr SymmetricBlockMatrix<Dimension, Blocks...> symmetric_block_matrix(Blocks... blocks) {
    // check that the number of blocks is consistent with the dimension
    static_assert(sizeof...(Blocks) == (Dimension*(Dimension + 1))/2, "The number of blocks is not consistent with the dimension.");
    
    return SymmetricBlockMatrix<Dimension, Blocks...>{blocks...};
}
