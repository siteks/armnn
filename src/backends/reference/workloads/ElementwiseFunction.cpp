//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "ElementwiseFunction.hpp"
#include "Broadcast.hpp"
#include <functional>
#include "Minimum.hpp"
#include "Maximum.hpp"
#include "Abs.hpp"
#include "Exp.hpp"
#include "Rsqrt.hpp"
#include "Sqrt.hpp"


namespace armnn
{

template <typename Functor>
ElementwiseBinaryFunction<Functor>::ElementwiseBinaryFunction(const TensorShape& inShape0,
                                                              const TensorShape& inShape1,
                                                              const TensorShape& outShape,
                                                              Decoder<InType>& inData0,
                                                              Decoder<InType>& inData1,
                                                              Encoder<OutType>& outData)
{
    BroadcastLoop(inShape0, inShape1, outShape).Unroll(Functor(), 0, inData0, inData1, outData);
}

template <typename Functor>
ElementwiseUnaryFunction<Functor>::ElementwiseUnaryFunction(const TensorShape& inShape,
                                                            const TensorShape& outShape,
                                                            Decoder<InType>& inData,
                                                            Encoder<OutType>& outData)
{
    BroadcastLoop(inShape, outShape).Unroll(Functor(), 0, inData, outData);
}

} //namespace armnn

template struct armnn::ElementwiseBinaryFunction<std::plus<float>>;
template struct armnn::ElementwiseBinaryFunction<std::minus<float>>;
template struct armnn::ElementwiseBinaryFunction<std::multiplies<float>>;
template struct armnn::ElementwiseBinaryFunction<std::divides<float>>;
template struct armnn::ElementwiseBinaryFunction<armnn::maximum<float>>;
template struct armnn::ElementwiseBinaryFunction<armnn::minimum<float>>;

template struct armnn::ElementwiseBinaryFunction<std::plus<int32_t>>;
template struct armnn::ElementwiseBinaryFunction<std::minus<int32_t>>;
template struct armnn::ElementwiseBinaryFunction<std::multiplies<int32_t>>;
template struct armnn::ElementwiseBinaryFunction<std::divides<int32_t>>;
template struct armnn::ElementwiseBinaryFunction<armnn::maximum<int32_t>>;
template struct armnn::ElementwiseBinaryFunction<armnn::minimum<int32_t>>;

// Comparison
template struct armnn::ElementwiseBinaryFunction<std::equal_to<float>>;
template struct armnn::ElementwiseBinaryFunction<std::greater<float>>;
template struct armnn::ElementwiseBinaryFunction<std::greater_equal<float>>;
template struct armnn::ElementwiseBinaryFunction<std::less<float>>;
template struct armnn::ElementwiseBinaryFunction<std::less_equal<float>>;
template struct armnn::ElementwiseBinaryFunction<std::not_equal_to<float>>;

// Unary
template struct armnn::ElementwiseUnaryFunction<armnn::abs<float>>;
template struct armnn::ElementwiseUnaryFunction<armnn::exp<float>>;
template struct armnn::ElementwiseUnaryFunction<std::negate<float>>;
template struct armnn::ElementwiseUnaryFunction<armnn::rsqrt<float>>;
template struct armnn::ElementwiseUnaryFunction<armnn::sqrt<float>>;
