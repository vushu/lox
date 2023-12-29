#include <lox/scanner.hpp>
#include <string_view>
#include <iostream>


int main() {
   std::string_view code = "2+2;";
   lox::Scanner scanner{code};
   std::vector<lox::Token> tokens = scanner.scan_tokens();
   for(lox::Token& tok : tokens) {
        std::cout << tok.to_string() << std::endl;
   }

   return 0;
}