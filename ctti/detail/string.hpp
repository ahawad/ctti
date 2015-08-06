//
// Created by manu343726 on 4/08/15.
//

#ifndef CTTI_STRING_HPP
#define CTTI_STRING_HPP

#include <ostream>
#include <array>

#include "hash.hpp"
#include "array.hpp"

namespace ctti
{
    namespace detail
    {
#ifdef CTTI_STRING_MAX_LENGTH
		constexpr std::size_t max_string_length = CTTI_STRING_MAX_LENGTH;
#else
		constexpr std::size_t max_string_length = 256;
#endif

        struct string
        {
			template<std::size_t begin, std::size_t end>
			static constexpr string from_c_str(const char* str)
			{
				return string{ str + begin, std::make_index_sequence<end - begin - 1>{}, std::make_index_sequence<max_string_length - (end - begin - 1)>{} };
			}

            constexpr hash_t hash() const
            {
                return hash_;
            }

            // note: not necessarily null-terminated!
            constexpr const char* c_str() const
            {
                return str_.data();
            }

            constexpr std::size_t length() const
            {
                return length_;
            }

            constexpr char operator[](std::size_t i) const
            {
                return str_[i];
            }

            template <std::size_t Begin, std::size_t End>
            constexpr string substr() const
            {
				return from_c_str<Begin, End>(str_.data());
            }

            friend std::ostream& operator<<(std::ostream& os, const string& str)
            {
                for (std::size_t i = 0u; i != str.length_; ++i)
                    os << str[i];
                return os;
            }

        private:
			const ctti::detail::array<char, max_string_length> str_;
            std::size_t length_;
            hash_t hash_;

			template<std::size_t... Is, std::size_t... Js>
			constexpr string(const char* str, std::index_sequence<Is...>, std::index_sequence<Js...>) :
				length_{ sizeof...(Is) },
				hash_{ sid_hash(sizeof...(Is)+1, str) },
				str_{ str[Is]..., (Js, '\0')... }
			{
				static_assert(sizeof...(Is)+sizeof...(Js) == max_string_length, "");
			}
        };

        template<std::size_t N>
        constexpr string make_string(const char (&str)[N])
        {
            return string::from_c_str<0,N>(str);
        }
    }
}

#endif //CTTI_STRING_HPP
