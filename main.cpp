#include "floatx.hpp"
#include <iostream>
#include <string>
#include <sstream>

void print_help() {
  std::cout << "Please input <from> <to> <value>. (e.g. fp64 hex 1.0)" << std::endl;
  std::cout << "(<from>, <to>) should be one of the following patterns." << std::endl;
  std::cout << "(bf16, hex), (fp16, hex), (fp32, hex), (fp64, hex)," << std::endl;
  std::cout << "(hex, bf16), (hex, fp16), (hex, fp32), (hex, fp64)" << std::endl;
}

int main(int argc, char *argv[]) {
  //print_help();

  // Check if the correct number of command-line arguments are provided
  if (argc != 4) {
      std::cerr << "Usage: " << argv[0] << " <from> <to> <value>" << std::endl;
      return 1; // Return error code
  }

  // Get command-line arguments
  std::string from = argv[1];
  std::string to = argv[2];
  std::string value_str = argv[3];

  // Your code to process the arguments goes here
  std::cout << "From: " << from << ", To: " << to << ", Value: " << value_str << std::endl;

  double value;

  if (from == "bf16" && to == "hex") {
    std::bitset<16> bits = flx::detail::get_fullbit_representation_BS<8,7>(std::stod(value_str));
    std::cout << std::hex << bits.to_ulong() << std::endl;
  } else if (from == "fp16" && to == "hex") {
    std::bitset<16> bits = flx::detail::get_fullbit_representation_BS<5,10>(std::stod(value_str));
    std::cout << std::hex << bits.to_ulong() << std::endl;
  } else if (from == "fp32" && to == "hex") {
    std::bitset<32> bits = flx::detail::get_fullbit_representation_BS<8,23>(std::stod(value_str));
    std::cout << std::hex << bits.to_ulong() << std::endl;
  } else if (from == "fp64" && to == "hex") {
    std::bitset<64> bits = flx::detail::get_fullbit_representation_BS<11,52>(std::stod(value_str));
    std::cout << std::hex << bits.to_ulong() << std::endl;
  } else if (from == "hex" && to == "bf16") {
    uint16_t x;
    std::stringstream ss;
    ss << std::hex << value_str;
    ss >> x;
    std::bitset<16> bits(x);
    std::cout << std::fixed << flx::detail::construct_number<8,7>(bits) << std::endl;
    // Extract sign bit, exponent, and mantissa
    int sign_bit = (x >> 15) & 0x1;
    int exponent = (x >> 7) & 0xFF;
    int mantissa = x & 0x7F;

    // Calculate sign (1/-1)
    int sign = (sign_bit == 0) ? 1 : -1;

    // Print binary representation of sign bit, exponent, and mantissa
    std::cout << "Sign bit: " << std::bitset<1>(sign_bit) << std::endl;
    std::cout << "Exponent: " << std::bitset<8>(exponent) << std::endl;
    std::cout << "Mantissa: " << std::bitset<7>(mantissa) << std::endl;

    // Print parsed values
    std::cout << "Sign: " << sign << std::endl;
    std::cout << "Exponent: " << exponent << std::endl;
    std::cout << "Mantissa: " << mantissa << std::endl;
    std::cout << "if normalized: (-1)^sign x 2^(" << exponent - 127 <<
     ") x " << 128 + mantissa << " x 2^(-7)" << std::endl;
    double value = std::pow(-1,sign) * std::pow(2,exponent - 127) * (128 + mantissa) * std::pow(2,-7);
    std::cout << value << std::endl;
  } else if (from == "hex" && to == "fp16") {
    uint16_t x;
    std::stringstream ss;
    ss << std::hex << value_str;
    ss >> x;
    std::bitset<16> bits(x);
    std::cout << std::fixed << flx::detail::construct_number<5,10>(bits) << std::endl;
  } else if (from == "hex" && to == "fp32") {
    uint32_t x;
    std::stringstream ss;
    ss << std::hex << value_str;
    ss >> x;
    std::bitset<32> bits(x);
    std::cout << std::fixed << flx::detail::construct_number<8,23>(bits) << std::endl;
  } else if (from == "hex" && to == "fp64") {
    uint64_t x;
    std::stringstream ss;
    ss << std::hex << value_str;
    ss >> x;
    std::bitset<64> bits(x);
    std::cout << std::fixed << flx::detail::construct_number<11,52>(bits) << std::endl;
  } else {
    std::cout << "Error" << std::endl;
    print_help();
    return 1;
  }

  return 0;
}
