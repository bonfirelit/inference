#pragma once


class Tensor {
public:
    Tensor(const std::vector<uint32_t>& shape, DataType dtype)
        : shape_(shape), datatype_(dtype) {
          size_t num_elements = std::accumulate(
          shape.begin(), shape.end(), 1u, std::multiplies<uint32_t>()
          );
          size_ = num_elements * getElementSize(dtype);
          data_.resize(size_);
        }

    Tensor(std::vector<uint8_t> data, const std::vector<uint32_t>& shape, DataType dtype)
        : Tensor(shape, dtype) {
          data_ = std::move(data);
          if (data_.size() != size_) {
            data.resize(size_);
          }
        }


    // Tensor(const Tensor&) = delete;
    // Tensor& operator=(const Tensor&) = delete;

    ~Tensor() = default;

    void* data() { return data_.data(); }
    // tensor的字节长度
    const size_t size() const { return size_; }
    const std::vector<uint32_t>& shape() const { return shape_; }
    DataType dataType() const { return datatype_; }

    inline size_t getElementSize(DataType dtype) const {
      switch (dtype) {
        case DataType::FLOAT32: return 4;
        case DataType::INT8:   return 1;
        case DataType::FLOAT16: return 2;
        case DataType::UINT8: return 1;
        default: assert(0);
      }
    }

    template<typename T>
    T* as() {
      return reinterpret_cast<T*>(data_.data());
    }

    template<typename T>
    const T* as() const {
      return reinterpret_cast<const T*>(data_.data());
    }

    // template<typename T>
    // std::vector<T> asVector() const {
    //   assert(getElementSize(dataType()) == sizeof(T));
    //   size_t count = size_ / sizeof(T);
    //   const T* ptr = reinterpret_cast<const T*>(data_.data());
    //   return std::vector<T>(ptr, ptr + count);
    // }
    
    std::vector<uint8_t> asVector() const {
        return data_;
    }


private:
    std::vector<uint8_t> data_;
    size_t size_{};
    std::vector<uint32_t> shape_;
    DataType datatype_;
};
