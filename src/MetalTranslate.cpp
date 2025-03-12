#include "MetalTranslate.h"

#include <ctranslate2/translator_pool.h>
#include <ctranslate2/models/model_reader.h>
#include <iostream>
#include <onmt/Tokenizer.h>
#include <fstream>

namespace MetalTranslate {

// Custom ModelReader to load the model from the specified path
    class CustomModelReader : public ctranslate2::models::ModelReader {
    public:
        explicit CustomModelReader(const std::string& model_path)
                : _model_path(model_path) {}

        // Implement the required methods
        std::unique_ptr<std::istream> get_model_stream(const std::string& filename) const override {
            return std::make_unique<std::ifstream>(_model_path + "/" + filename, std::ios::binary);
        }

        std::vector<std::string> list_files() const override {
            // List all files in the model directory
            // This is a simplified example; you may need to implement actual directory listing
            return {"model.bin", "vocabulary.txt"};
        }

    private:
        std::string _model_path;
    };

    MetalTranslate::MetalTranslate(MetalTranslateConfig config) {
        this->_config = config;
    }

    std::string MetalTranslate::Translate(const std::string& source,
                                          const std::string& source_code,
                                          const std::string& target_code) {

        // Tokenizer
        onmt::Tokenizer tokenizer(this->_config.ModelPath + "sentencepiece.model");
        std::vector<std::string> tokens;
        tokenizer.tokenize(source, tokens);

        std::string source_prefix = "__" + source_code + "__";
        tokens.insert(tokens.begin(), source_prefix);

        // CTranslate2
        const size_t num_translators = 1;
        const size_t num_threads_per_translator = 0; // Unused with DNNL

        // Instantiate the custom ModelReader
        auto model_reader = std::make_shared<CustomModelReader>(this->_config.ModelPath + "model");

        // Create the TranslatorPool with the custom ModelReader
        ctranslate2::TranslatorPool translator(
                num_translators,
                num_threads_per_translator,
                model_reader,
                ctranslate2::Device::CPU
        );

        const std::vector<std::vector<std::string>> batch = {tokens};
        const std::vector<std::vector<std::string>> target_prefix = {
                {"__" + target_code + "__"}
        };

        const std::vector<ctranslate2::TranslationResult> results =
                translator.translate_batch(batch, target_prefix);

        const std::vector<std::string> translatedTokens = results[0].output();

        std::string result = tokenizer.detokenize(translatedTokens);

        // Remove target prefix
        // __es__ Traducción de texto con MetalTranslate
        // -> Traducción de texto con MetalTranslate
        result = result.substr(7);

        return result;
    }

} // namespace MetalTranslate