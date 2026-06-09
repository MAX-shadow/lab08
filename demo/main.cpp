#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Account.h"
#include "Transaction.h"

// Demo: reads transfer sums from stdin, runs them through the banking
// library and writes the result to the file pointed by LOG_PATH
// (or to stdout if LOG_PATH is not set).
int main() {
  const char* log_path = std::getenv("LOG_PATH");

  Account from(1, 100000);
  Account to(2, 100000);
  Transaction tx;

  int sum = 0;
  while (std::cin >> sum) {
    std::string line;
    try {
      bool ok = tx.Make(from, to, sum);
      line = "transfer " + std::to_string(sum) + (ok ? " : ok" : " : declined");
    } catch (const std::exception& e) {
      line = "transfer " + std::to_string(sum) + " : error (" + e.what() + ")";
    }

    if (log_path != nullptr) {
      std::ofstream out{log_path, std::ios_base::app};
      out << line << std::endl;
    } else {
      std::cout << line << std::endl;
    }
  }

  return 0;
}
