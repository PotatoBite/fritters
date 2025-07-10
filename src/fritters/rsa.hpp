//
// Created by Anier Velasco Sotomayor on 9/7/25.
//

#ifndef RSA_HPP
#define RSA_HPP

#endif //RSA_HPP

#include <boost/multiprecision/cpp_int.hpp>
#include <array>

namespace RSA{
    using boost::multiprecision::cpp_int;

    std::array<cpp_int, 3> extended_gcd(cpp_int a, cpp_int b);

    cpp_int generate_random_number(int nbits);
    cpp_int generate_prime(int nbits);
    std::pair<cpp_int, cpp_int> generate_keys(int nbits);

    std::array<cpp_int, 3> setup(int nbits);

    cpp_int public_function(const cpp_int& message, const cpp_int& e, const cpp_int& n);
    cpp_int secret_function(const cpp_int& cipher_text, const cpp_int& d, const cpp_int& n);

    cpp_int send_message(const std::string& message, const cpp_int& e_recipient, const cpp_int& n_recipient);
    std::string receive_message(const cpp_int& cipher_text, const cpp_int &d_recipient, const cpp_int &n_recipient);

    std::pair<cpp_int, cpp_int> sign_message(const cpp_int& message, const cpp_int& d_sender, const cpp_int& n_sender);
    bool verify_message(const cpp_int& message, const cpp_int& digital_signature, const cpp_int& e_sender, const cpp_int& n_sender);

};

