//
// Created by Anier Velasco Sotomayor on 10/7/25.
//

#include <boost/multiprecision/cpp_int.hpp>
#include <fritters/rsa.cpp>

using namespace boost::multiprecision;

bool test_extended_euclid(const cpp_int &a, const cpp_int &b);
bool test_public_function();
bool test_secret_function();
bool test_encrypt_decrypt(const cpp_int & message, const cpp_int &n, const cpp_int &e, const cpp_int &d);
bool test_send_receive_message(std::string message);

int main() {
    int result = 0;

    for (int a = 1; a <= 10; a++) {
        for (int b = 1; b <= 10; b++) {
            if (!test_extended_euclid(a, b)) {
                std::cerr << "\textended euclid failed" << std::endl;
                result = -1;
            }
        }
    }

    if (!test_public_function()) {
        std::cerr << "\tpublic function failed" << std::endl;
        result = -1;
    }
    if (!test_secret_function()) {
        std::cerr << "\tsecret function failed" << std::endl;
        result = -1;
    }
    if (!test_encrypt_decrypt(100, 319, 3, 187) or
        !test_encrypt_decrypt(354, 3233, 65537, 2753)) {
        std::cerr << "\tencrypt-decrypt failed" << std::endl;
        result = -1;
    }
    if (!test_send_receive_message("Hi. I am a dinosaur!")) {
        std::cerr << "\tsend-receive failed" << std::endl;
        result = -1;
    }

    std::cerr << "Nothing failed!";

    return result;
}

bool test_extended_euclid(const cpp_int &a, const cpp_int &b) {
    const cpp_int expected_g = gcd(a, b);
    auto [g, x, y] = RSA::extended_gcd(a, b);
    return g == expected_g and a * x + b * y == g;
}

bool test_public_function() {
    const cpp_int message = 100;
    const cpp_int e = 3;
    const cpp_int n = 319;
    const cpp_int ciphered_text = RSA::public_function(message, e, n);
    return ciphered_text == 254;
}

bool test_secret_function() {
    const cpp_int ciphered_text = 254;
    const cpp_int d = 187;
    const cpp_int n = 319;
    const cpp_int deciphered_message = RSA::secret_function(ciphered_text, d, n);
    return deciphered_message == 100;
}

bool test_encrypt_decrypt(const cpp_int &message, const cpp_int &n, const cpp_int &e, const cpp_int &d) {
    const cpp_int cipher_text = RSA::public_function(message, e, n);
    const cpp_int deciphered_message = RSA::secret_function(cipher_text, d, n);
    return cipher_text != message and deciphered_message == message;
}

bool test_send_receive_message(const std::string message) {
    auto [n, e, d] = RSA::setup(1024);
    cpp_int cipher_text = RSA::send_message(message, e, n);
    std::string decrypted_message = RSA::receive_message(cipher_text, d, n);
    return decrypted_message == message;
}

