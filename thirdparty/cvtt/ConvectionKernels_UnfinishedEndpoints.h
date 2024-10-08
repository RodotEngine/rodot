#pragma once

#include "ConvectionKernels_Util.h"

namespace cvtt
{
    namespace Internal
    {
        template<int THectorSize>
        class UnfinishedEndpoints
        {
        public:
            typedef ParallelMath::Float MFloat;
            typedef ParallelMath::UInt16 MUInt16;
            typedef ParallelMath::UInt15 MUInt15;
            typedef ParallelMath::SInt16 MSInt16;
            typedef ParallelMath::SInt32 MSInt32;

            UnfinishedEndpoints()
            {
            }

            UnfinishedEndpoints(const MFloat *base, const MFloat *offset)
            {
                for (int ch = 0; ch < THectorSize; ch++)
                    m_base[ch] = base[ch];
                for (int ch = 0; ch < THectorSize; ch++)
                    m_offset[ch] = offset[ch];
            }

            UnfinishedEndpoints(const UnfinishedEndpoints& other)
            {
                for (int ch = 0; ch < THectorSize; ch++)
                    m_base[ch] = other.m_base[ch];
                for (int ch = 0; ch < THectorSize; ch++)
                    m_offset[ch] = other.m_offset[ch];
            }

            void FinishHDRUnsigned(int tweak, int range, MSInt16 *outEP0, MSInt16 *outEP1, ParallelMath::RoundTowardNearestForScope *roundingMode)
            {
                float tweakFactors[2];
                Util::ComputeTweakFactors(tweak, range, tweakFactors);

                for (int ch = 0; ch < THectorSize; ch++)
                {
                    MUInt15 channelEPs[2];
                    for (int epi = 0; epi < 2; epi++)
                    {
                        MFloat f = ParallelMath::Clamp(m_base[ch] + m_offset[ch] * tweakFactors[epi], 0.0f, 31743.0f);
                        channelEPs[epi] = ParallelMath::RoundAndConvertToU15(f, roundingMode);
                    }

                    outEP0[ch] = ParallelMath::LosslessCast<MSInt16>::Cast(channelEPs[0]);
                    outEP1[ch] = ParallelMath::LosslessCast<MSInt16>::Cast(channelEPs[1]);
                }
            }

            void FinishHDRSigned(int tweak, int range, MSInt16* outEP0, MSInt16* outEP1, ParallelMath::RoundTowardNearestForScope* roundingMode)
            {
                float tweakFactors[2];
                Util::ComputeTweakFactors(tweak, range, tweakFactors);

                for (int ch = 0; ch < THectorSize; ch++)
                {
                    MSInt16 channelEPs[2];
                    for (int epi = 0; epi < 2; epi++)
                    {
                        MFloat f = ParallelMath::Clamp(m_base[ch] + m_offset[ch] * tweakFactors[epi], -31743.0f, 31743.0f);
                        channelEPs[epi] = ParallelMath::RoundAndConvertToS16(f, roundingMode);
                    }

                    outEP0[ch] = channelEPs[0];
                    outEP1[ch] = channelEPs[1];
                }
            }

            void FinishLDR(int tweak, int range, MUInt15* outEP0, MUInt15* outEP1)
            {
                ParallelMath::RoundTowardNearestForScope roundingMode;

                float tweakFactors[2];
                Util::ComputeTweakFactors(tweak, range, tweakFactors);

                for (int ch = 0; ch < THectorSize; ch++)
                {
                    MFloat ep0f = ParallelMath::Clamp(m_base[ch] + m_offset[ch] * tweakFactors[0], 0.0f, 255.0f);
                    MFloat ep1f = ParallelMath::Clamp(m_base[ch] + m_offset[ch] * tweakFactors[1], 0.0f, 255.0f);
                    outEP0[ch] = ParallelMath::RoundAndConvertToU15(ep0f, &roundingMode);
                    outEP1[ch] = ParallelMath::RoundAndConvertToU15(ep1f, &roundingMode);
                }
            }

            template<int TNewHectorSize>
            UnfinishedEndpoints<TNewHectorSize> ExpandTo(float filler)
            {
                MFloat newBase[TNewHectorSize];
                MFloat newOffset[TNewHectorSize];

                for (int ch = 0; ch < TNewHectorSize && ch < THectorSize; ch++)
                {
                    newBase[ch] = m_base[ch];
                    newOffset[ch] = m_offset[ch];
                }

                MFloat fillerV = ParallelMath::MakeFloat(filler);

                for (int ch = THectorSize; ch < TNewHectorSize; ch++)
                {
                    newBase[ch] = fillerV;
                    newOffset[ch] = ParallelMath::MakeFloatZero();
                }

                return UnfinishedEndpoints<TNewHectorSize>(newBase, newOffset);
            }

        private:
            MFloat m_base[THectorSize];
            MFloat m_offset[THectorSize];
        };
    }
}
