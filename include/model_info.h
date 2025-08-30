#pragma once

#include "common.h"

class ModelInfo {
  public:
    ModelInfo(size_t batch_size, size_t input_size, 
              size_t output_size, uint32_t input_num, uint32_t output_num,
              std::vector<std::vector<uint32_t>>&& inputs_shape,
              std::vector<std::vector<uint32_t>>&& outputs_shape,
              DataType input_type,
              DataType output_type)
    : batch_size_(batch_size)
    , input_size_(input_size)
    , output_size_(output_size)
    , input_num_(input_num)
    , output_num_(output_num)
    , inputs_shape_(std::move(inputs_shape))
    , outputs_shape_(std::move(outputs_shape))
    , input_type_(input_type)
    , output_type_(output_type) {}


    /** 一个batch的模型输入大小 */
    const size_t getInputSize() const {return input_size_; }
    /** 一个batch的模型输出大小 */
    const size_t getOutputSize() const { return output_size_; }
    const size_t getBatchSize() const { return batch_size_; }
    const uint32_t getInputNum() const { return input_num_; }
    const uint32_t getOutputNum() const { return output_num_; }
    /** 输入张量数据类型 */
    const DataType getInputType() const { return input_type_; }
    /** 输出张量数据类型 */
    const DataType getOutputType() const { return output_type_; }

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
    /**
     * 模型一个batch的输出大小字节数
     */
    size_t output_size_;
    /**
     * 模型一个batch的输入大小字节数
     */
    size_t input_size_;
    uint32_t input_num_;
    uint32_t output_num_;
    std::vector<std::vector<uint32_t>> inputs_shape_;
    std::vector<std::vector<uint32_t>> outputs_shape_;
    DataType input_type_;
    DataType output_type_;
};