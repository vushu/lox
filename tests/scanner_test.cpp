#include<lox/scanner.hpp>
#include<string_view>


int main() {
   std::string_view code = "2 + 2;";
   lox::Scanner scanner{code};
   return 0;
}