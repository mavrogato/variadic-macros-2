
#include <iostream>
#include <tuple>
#include <string_view>
#include <algorithm>
#include <variant>
#include <array>

#define SYMTBL(...) detail::symtbl(#__VA_ARGS__, __VA_ARGS__)

namespace detail
{
    // https://stackoverflow.com/questions/62108100/variant-with-unique-types
    template <typename T, typename... Ts>
    struct filter_duplicates { using type = T; };
    template <template <typename...> class C, typename... Ts, typename U, typename... Us>
    struct filter_duplicates<C<Ts...>, U, Us...>
        : std::conditional_t<(std::is_same_v<U, Ts> || ...)
                             , filter_duplicates<C<Ts...>, Us...>
                             , filter_duplicates<C<Ts..., U>, Us...>> {};
    template <typename T>
    struct unique_variant;
    template <typename... Ts>
    struct unique_variant<std::variant<Ts...>> : filter_duplicates<std::variant<>, Ts...> {};
    template <typename T>
    using unique_variant_t = typename unique_variant<T>::type;

    template <size_t N, typename... Args>
    struct symtbl {
        using variant_type = unique_variant_t<std::variant<Args...>>;
        std::string_view const symbols;
        std::array<variant_type, sizeof... (Args)> values;
        constexpr symtbl(char const (&symbols)[N], Args... values) noexcept 
            : symbols(symbols)
            , values{values...}
            {
            }
        struct iterator {
            symtbl const& src;
            mutable std::string_view::const_iterator cur;
            constexpr iterator(symtbl const& src, auto cur) noexcept
                : src(src)
                , cur(cur)
                {
                }
            constexpr auto operator*() const noexcept {
                return std::pair{
                    std::string_view(cur, std::find(cur, src.symbols.end(), ',')),
                    src.values[std::count(src.symbols.begin(), cur, ',')],
                };
            }
            constexpr auto operator++() const noexcept {
                cur = std::find(cur, src.symbols.end(), ',');
                if (cur != src.symbols.end()) {
                    while (*cur == ' ' || *cur == ',') ++cur;
                }
                return *this;
            }
            constexpr friend bool operator!=(iterator lhs, iterator rhs) noexcept {
                return lhs.cur != rhs.cur;
            }
        };
        constexpr iterator begin() const noexcept { return {*this, symbols.begin()}; }
        constexpr iterator end() const noexcept { return {*this, symbols.end()}; }
    };
}

#define A 1
#define B 2
constexpr auto C = 3;

int main() {
    auto test = SYMTBL(A, B, C);
    for (auto [sym, val] : test) {
        std::cout << sym << std::get<int>(val) << std::endl;
    }
    return 0;
}
