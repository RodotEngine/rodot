#pragma once
#ifndef __CVTT_BCCOMMON_H__
#define __CVTT_BCCOMMON_H__

#include "ConvectionKernels_AggregatedError.h"
#include "ConvectionKernels_ParallelMath.h"

namespace cvtt
{
    namespace Internal
    {
        class BCCommon
        {
        public:
            typedef ParallelMath::Float MFloat;
            typedef ParallelMath::UInt16 MUInt16;
            typedef ParallelMath::UInt15 MUInt15;
            typedef ParallelMath::AInt16 MAInt16;
            typedef ParallelMath::SInt16 MSInt16;
            typedef ParallelMath::SInt32 MSInt32;

            static int TweakRoundsForRange(int range);

            template<int THectorSize>
            static void ComputeErrorLDR(uint32_t flags, const MUInt15 reconstructed[THectorSize], const MUInt15 original[THectorSize], int numRealChannels, AggregatedError<THectorSize> &aggError)
            {
                for (int ch = 0; ch < numRealChannels; ch++)
                    aggError.Add(ParallelMath::SqDiffUInt8(reconstructed[ch], original[ch]), ch);
            }

            template<int THectorSize>
            static void ComputeErrorLDR(uint32_t flags, const MUInt15 reconstructed[THectorSize], const MUInt15 original[THectorSize], AggregatedError<THectorSize> &aggError)
            {
                ComputeErrorLDR<THectorSize>(flags, reconstructed, original, THectorSize, aggError);
            }

            template<int THectorSize>
            static MFloat ComputeErrorLDRSimple(uint32_t flags, const MUInt15 reconstructed[THectorSize], const MUInt15 original[THectorSize], int numRealChannels, const float *channelWeightsSq)
            {
                AggregatedError<THectorSize> aggError;
                ComputeErrorLDR<THectorSize>(flags, reconstructed, original, numRealChannels, aggError);
                return aggError.Finalize(flags, channelWeightsSq);
            }

            template<int THectorSize>
            static MFloat ComputeErrorHDRFast(uint32_t flags, const MSInt16 reconstructed[THectorSize], const MSInt16 original[THectorSize], const float channelWeightsSq[THectorSize])
            {
                MFloat error = ParallelMath::MakeFloatZero();
                if (flags & Flags::Uniform)
                {
                    for (int ch = 0; ch < THectorSize; ch++)
                        error = error + ParallelMath::SqDiffSInt16(reconstructed[ch], original[ch]);
                }
                else
                {
                    for (int ch = 0; ch < THectorSize; ch++)
                        error = error + ParallelMath::SqDiffSInt16(reconstructed[ch], original[ch]) * ParallelMath::MakeFloat(channelWeightsSq[ch]);
                }

                return error;
            }

            template<int THectorSize>
            static MFloat ComputeErrorHDRSlow(uint32_t flags, const MSInt16 reconstructed[THectorSize], const MSInt16 original[THectorSize], const float channelWeightsSq[THectorSize])
            {
                MFloat error = ParallelMath::MakeFloatZero();
                if (flags & Flags::Uniform)
                {
                    for (int ch = 0; ch < THectorSize; ch++)
                        error = error + ParallelMath::SqDiff2CL(reconstructed[ch], original[ch]);
                }
                else
                {
                    for (int ch = 0; ch < THectorSize; ch++)
                        error = error + ParallelMath::SqDiff2CL(reconstructed[ch], original[ch]) * ParallelMath::MakeFloat(channelWeightsSq[ch]);
                }

                return error;
            }

            template<int TChannelCount>
            static void PreWeightPixelsLDR(MFloat preWeightedPixels[16][TChannelCount], const MUInt15 pixels[16][TChannelCount], const float channelWeights[TChannelCount])
            {
                for (int px = 0; px < 16; px++)
                {
                    for (int ch = 0; ch < TChannelCount; ch++)
                        preWeightedPixels[px][ch] = ParallelMath::ToFloat(pixels[px][ch]) * channelWeights[ch];
                }
            }

            template<int TChannelCount>
            static void PreWeightPixelsHDR(MFloat preWeightedPixels[16][TChannelCount], const MSInt16 pixels[16][TChannelCount], const float channelWeights[TChannelCount])
            {
                for (int px = 0; px < 16; px++)
                {
                    for (int ch = 0; ch < TChannelCount; ch++)
                        preWeightedPixels[px][ch] = ParallelMath::ToFloat(pixels[px][ch]) * channelWeights[ch];
                }
            }
        };
    }
}

#endif
