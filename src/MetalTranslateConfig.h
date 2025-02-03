#include <string>

namespace MetalTranslate {
class MetalTranslateConfig {
public:
  std::string ModelPath = "models/";
  int maxBatchSize = 2048;
};
} // namespace MetalTranslate
