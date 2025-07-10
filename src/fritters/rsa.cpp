//
// Created by Anier Velasco Sotomayor on 9/7/25.
//

#include "rsa.hpp"

#include <algorithm>
#include <boost/multiprecision/miller_rabin.hpp>
#include <chrono>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
using namespace boost::multiprecision;

std::array<cpp_int, 3> RSA::extended_gcd(cpp_int a, cpp_int b) {
    if (b == 0) {
        return {a, 1, 0};
    }
    auto [g, x, y] = RSA::extended_gcd(b, a % b);
    return {g, y, x - a / b * y};
}

cpp_int RSA::generate_random_number(int bits) {
    cpp_int result = 1;
    for (int i = 1; i < bits; i++) {
        result = result * 2 + rng() % 2;
    }
    return result;
}


cpp_int RSA::generate_prime(int nbits) {
    cpp_int prime_candidate;
    do {
        prime_candidate = generate_random_number(nbits);
    }while (!miller_rabin_test(prime_candidate, 25));

    return prime_candidate;
}

std::pair<cpp_int, cpp_int> RSA::generate_keys(int nbits) {
    cpp_int p = generate_prime(nbits);
    cpp_int q = p;
    while (q == p) {
        q = generate_prime(nbits);
    }


    return {p, q};
}

std::array<cpp_int, 3> RSA::setup(int nbits) {
    cpp_int p, q, e, phi_n;
    e = 65537;
    std::cerr << "Finding the two primes. Please wait...\n";
    do {
        auto t = generate_keys(nbits);
        p = t.first;
        q = t.second;
        phi_n = (p - 1) * (q - 1);
    }while (gcd(phi_n, e) != 1);

    phi_n = (p - 1) * (q - 1);
    auto [g, d, _] = RSA::extended_gcd(e, phi_n);
    assert(g == 1);
    d %= phi_n;
    if (d < 0 ) d += phi_n;
    assert(d * e % phi_n == 1);

    cpp_int n = p * q;
    std::cerr << "Primes found successfully.\nHere are your keys:\n";
    std::cerr << "public: (" << e << ", " << p * q << ")\n";
    std::cerr << "secret: (" << d << ", " << p * q << ")\n";

    return {n, e, d};
}


cpp_int RSA::public_function(const cpp_int& message, const cpp_int& e, const cpp_int& n) {
    return powm(message % n, e, n);
}

cpp_int RSA::secret_function(const cpp_int& cipher_text, const cpp_int& d, const cpp_int& n) {
    return powm(cipher_text, d, n);
}

cpp_int RSA::send_message(const std::string &message, const cpp_int &e_recipient, const cpp_int &n_recipient) {
    assert(8 * message.size() < n_recipient);
    cpp_int encoded_message = 0;
    for (unsigned char c: message) {
        encoded_message <<= 8;
        encoded_message |= c;
    }
    return public_function(encoded_message, e_recipient, n_recipient);
}

std::string RSA::receive_message(const cpp_int &cipher_text, const cpp_int &d_recipient, const cpp_int &n_recipient) {
    cpp_int message = secret_function(cipher_text, d_recipient, n_recipient);
    std::string decoded_message;

    while (message > 0) {
        unsigned char c = static_cast<char>(message & ((1 << 8) - 1));
        decoded_message += c;
        message >>= 8;
    }
    std::reverse(decoded_message.begin(), decoded_message.end());

    return decoded_message;
}


