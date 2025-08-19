#pragma once

#include "common.h"

class ModelInfo {
  public:
    ModelInfo(size_t bactch_size, size_t input_size, 
              size_t output_size, uint32_t input_num, uint32_t output_num,
              std::vector<std::vector<uint32_t>>&& inputs_shape,
              std::vector<std::vector<uint32_t>>&& outputs_shape)
    : batch_size_(bactch_size)
    , input_size_(input_size)
    , output_size_(output_size)
    , input_num_(input_num)
    , output_num_(output_num)
    , inputs_shape_(std::move(inputs_shape))
    , outputs_shape_(std::move(outputs_shape)) {}


    // 一个batch的模型输入大小
    const size_t getInputSize() const {return input_size_; }
    // 一个batch的模型输出大小
    const size_t getOutputSize() const { return output_size_; }
    const size_t getBatchSize() const { return batch_size_; }
    const uint32_t getInputNum() const { return input_num_; }
    const uint32_t getOutputNum() const { return output_num_; }

    void setInputsShape(std::vector<std::vector<uint32_t>>&& dims) {
      inputs_shape_ = std::move(dims);
    }

    void setOutputsShape(std::vector<std::vector<uint32_t>>&& dims) {
      outputs_shape_ = std::move(dims);
    }
    
    const std::vector<std::vector<uint32_t>>& getInputsShape() const {
      return inputs_shape_;
    }

    const std::vector<std::vector<uint32_t>>& getOutputsShape() const {
      return outputs_shape_;
    }

  private:
    size_t batch_size_;
    size_t output_size_;
    size_t input_size_;
    uint32_t input_num_;
    uint32_t output_num_;
    std::vector<std::vector<uint32_t>> inputs_shape_;
    std::vector<std::vector<uint32_t>> outputs_shape_;
};