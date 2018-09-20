//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "ClPermuteWorkload.hpp"
#include "backends/ClTensorHandle.hpp"
#include "backends/ArmComputeTensorUtils.hpp"

#include <arm_compute/core/Error.h>

#include "ClWorkloadUtils.hpp"

namespace armnn
{

arm_compute::Status ClPermuteWorkloadValidate(const PermuteDescriptor& descriptor)
{
    const armnn::PermutationVector& perm = descriptor.m_DimMappings;

    ARM_COMPUTE_RETURN_ERROR_ON_MSG(!perm.IsEqual({ 0U, 3U, 1U, 2U })
                                    && !perm.IsEqual({ 0U, 2U, 3U, 1U })
                                    && !perm.IsEqual({ 3U, 2U, 0U, 1U }),
    "Only [0, 3, 1, 2], [0, 2, 3, 1] and [3, 2, 0, 1] permutations are supported");

    return arm_compute::Status{};
}

template <armnn::DataType... DataTypes>
ClPermuteWorkload<DataTypes...>::ClPermuteWorkload(const PermuteQueueDescriptor& descriptor,
                                               const WorkloadInfo& info)
    : TypedWorkload<PermuteQueueDescriptor, DataTypes...>(descriptor, info)
{
    using armcomputetensorutils::BuildArmComputePermutationVector;

    m_Data.ValidateInputsOutputs(GetName(), 1, 1);

    const arm_compute::ICLTensor& input = static_cast<IClTensorHandle*>(m_Data.m_Inputs[0])->GetTensor();
    arm_compute::ICLTensor& output = static_cast<IClTensorHandle*>(m_Data.m_Outputs[0])->GetTensor();
    const armnn::PermutationVector& mappings = m_Data.m_Parameters.m_DimMappings;

    // Run the layer.
    m_PermuteFunction.configure(&input, &output, BuildArmComputePermutationVector(mappings));
}

template <armnn::DataType... DataTypes>
void ClPermuteWorkload<DataTypes...>::Execute() const
{
    ARMNN_SCOPED_PROFILING_EVENT_CL( GetName() + "_Execute");
    m_PermuteFunction.run();
}

template class ClPermuteWorkload<DataType::Float16, DataType::Float32>;
template class ClPermuteWorkload<DataType::QuantisedAsymm8>;

} // namespace armnn
