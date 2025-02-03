#include "MetalTranslate.h"

#include <iostream>
#include <string>

void help() {
  std::cout << "Usage: metaltranslate <model> <text> <source code> <target code>\n";
}

int main(int argc, char **argv) {
  MetalTranslate::MetalTranslateConfig config;

  MetalTranslate::MetalTranslate metalTranslate(config);

  if (argc < 3) {
    help();
    return 1;
  }

  std::string model(argv[1]);
  std::string q(argv[2]);

  std::string sourceCode = "en";
  if (argc > 3) {
    sourceCode = std::string(argv[3]);
  }

  std::string targetCode = "es";
  if (argc > 4) {
    targetCode = std::string(argv[4]);
  }

  std::string result = metalTranslate.Translate(model, q, sourceCode, targetCode);

  std::cout << result << std::endl;

  return 0;
}
