#pragma once

#include <eosio/vm/exceptions.hpp>

#include <array>
#include <cstdint>

namespace eosio { namespace vm {
   // TODO after C++20 use std::span as fundamental type
   template <typename T>
   class span {
      public:
         using iterator = T*;
         template <typename It>
         inline constexpr span(It first, std::size_t len) : first_elem(first), last_elem(first + len) {}
         template <typename It>
         inline constexpr span(It first, It last) : first_elem(first), last_elem(last) {
            EOS_VM_ASSERT(last >= first, span_exception, "last iterator < first iterator");
         }

         template <std::size_t N>
         inline constexpr span(T (&arr)[N]) : first_elem(&arr[0]), last_elem(&arr[N-1]) {}

         template <std::size_t N>
         inline constexpr span(std::array<T, N>& arr) : first_elem(arr.data()), last_elem(arr.data() + (N-1)) {}

         template <std::size_t N>
         inline constexpr span(const std::array<T, N>& arr) : first_elem(arr.data()), last_elem(arr.data() + (N-1)) {}
         inline constexpr span(const span&) = default;
         inline constexpr span(span&&) = default;

         inline constexpr iterator begin()  { return first_elem; }
         inline constexpr iterator end()    { return last_elem + 1; }
         inline constexpr iterator rbegin() { return last_elem; }
         inline constexpr iterator rend()   { return first_elem - 1; }

         inline constexpr T& front()             { return *first_elem; }
         inline constexpr const T& front() const { return *first_elem; }
         inline constexpr T& back()              { return *last_elem; }
         inline constexpr const T& back() const  { return *last_elem; }
         inline constexpr T& operator[](std::size_t n) { return first_elem[n]; }
         inline constexpr const T& operator[](std::size_t n) const { return first_elem[n]; }
         inline constexpr T& at(std::size_t n) {
            EOS_VM_ASSERT(first_elem + n <= last_elem, span_exception, "index overflows span");
            return operator[](n);
         }
         inline constexpr const T& at(std::size_t n) const {
            EOS_VM_ASSERT(first_elem + n <= last_elem, span_exception, "index overflows span");
            return operator[](n);
         }

         inline constexpr T* data() { return first_elem; }
         inline constexpr std::size_t size() const { return last_elem - first_elem; }
         inline constexpr std::size_t size_bytes() const { return size() * sizeof(T); }
         inline constexpr bool empty() const { return size() == 0; }

         inline constexpr span first(std::size_t len) const {
            EOS_VM_ASSERT(first_elem + len <= last_elem, span_exception, "length overflows span");
            return {first_elem, first_elem + len};
         }

         inline constexpr span last(std::size_t len) const {
            EOS_VM_ASSERT(last_elem - len >= first_elem, span_exception, "length underflows span");
            return {last_elem - len, last_elem};
         }

         inline constexpr span subspan(std::size_t offset, std::size_t len) const {
            return last(offset).first(len);
         }

         bool operator==(const span& other) const { return first_elem == other.first_elem && last_elem == other.last_elem; }

      private:
         iterator first_elem;
         iterator last_elem;
   };
}} // ns eosio::vm
