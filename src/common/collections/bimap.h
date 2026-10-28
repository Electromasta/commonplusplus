#pragma once
#include <unordered_map>
#include <utility>

namespace common  {
    template <typename A, typename B>
    class bimap {
        public:
            const A NONE_A = {};
            const B NONE_B = {};

            bimap() = default;
            explicit bimap(A nonea, B noneb) : NONE_A(std::move(nonea)), NONE_B(std::move(noneb)) {};
            ~bimap() = default;

            bool insert(const A & a, const B & b)   {
                if (a == NONE_A || b == NONE_B) return false;
                if (abMap.count(a)) {   baMap.erase(abMap[a]);  }
                if (baMap.count(b)) {   abMap.erase(baMap[b]);  }
                abMap[a] = b;
                baMap[b] = a;
                return true;
            }

            const A & getA(const B & b) const {
                auto ba = baMap.find(b);
                if (ba != baMap.end()) return ba->second;
                return NONE_A;
            }

            const B & getB(const A & a) const {
                auto ab = abMap.find(a);
                if (ab != abMap.end()) return ab->second;
                return NONE_B;
            }

            bool containsA(const A & a) const {
                return abMap.count(a) > 0;
            }

            bool containsB(const B & b) const {
                return baMap.count(b) > 0;
            }

            void eraseA(const A & a)    {
                if (!containsA(a)) return;
                B b = abMap.at(a);
                abMap.erase(a);
                baMap.erase(b);
            }

            void eraseB(const B & b)    {
                if (!containsB(b)) return;
                A a = baMap.at(b);
                baMap.erase(b);
                abMap.erase(a);
            }

            size_t size() const {
                return abMap.size();
            }

            bool empty() const {
                return abMap.empty();
            }

            void clear()    {
                abMap.clear();
                baMap.clear();
            }
        private:
            std::unordered_map<A, B> abMap;
            std::unordered_map<B, A> baMap;
    };
}

template<typename T, typename U>
using BidirectionalMap = common::bimap<T, U>;

template<typename T, typename U>
using BiMap = common::bimap<T, U>;
