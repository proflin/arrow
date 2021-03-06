// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef ARROW_ARRAY_H
#define ARROW_ARRAY_H

#include <cstdint>
#include <cstdlib>
#include <memory>

#include "arrow/type.h"
#include "arrow/util/bit-util.h"
#include "arrow/util/macros.h"

namespace arrow {

class Buffer;

// Immutable data array with some logical type and some length. Any memory is
// owned by the respective Buffer instance (or its parents). May or may not be
// nullable.
//
// The base class only has a null array (if the data type is nullable)
//
// Any buffers used to initialize the array have their references "stolen". If
// you wish to use the buffer beyond the lifetime of the array, you need to
// explicitly increment its reference count
class Array {
 public:
  Array() : length_(0), nulls_(nullptr), null_bits_(nullptr) {}
  Array(const TypePtr& type, int64_t length,
      const std::shared_ptr<Buffer>& nulls = nullptr);

  virtual ~Array() {}

  void Init(const TypePtr& type, int64_t length, const std::shared_ptr<Buffer>& nulls);

  // Determine if a slot if null. For inner loops. Does *not* boundscheck
  bool IsNull(int64_t i) const {
    return nullable_ && util::get_bit(null_bits_, i);
  }

  int64_t length() const { return length_;}
  bool nullable() const { return nullable_;}
  const TypePtr& type() const { return type_;}
  TypeEnum type_enum() const { return type_->type;}

 protected:
  TypePtr type_;
  bool nullable_;
  int64_t length_;

  std::shared_ptr<Buffer> nulls_;
  const uint8_t* null_bits_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Array);
};


typedef std::shared_ptr<Array> ArrayPtr;

} // namespace arrow

#endif
