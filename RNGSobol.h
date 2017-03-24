#ifndef _RNG_SOBOL_HPP_
#define _RNG_SOBOL_HPP_

#include <assert.h>

class RNGSobol
{
public:
    static inline unsigned getNDims()
    {
        return QRNG_NDMS;
    }
    /// default constructor
    RNGSobol();
    inline void prepareForIntegration(unsigned nDims)
    {
        setSeed(2048 + nDims * 1024);
    }
    /// set current vector index
    inline void setSeed(unsigned uSeed)
    {
        m_uCurSeed = uSeed;
        m_uCurDim = 0;
        updateUValue();
        updateDValue();
    }
    inline void nextSeed()
    {
        ++m_uCurSeed;
        m_uCurDim = 0;
        updateUValue();
        updateDValue();
    }
    // set current dimension index
    inline void setDim(unsigned uDim)
    {
        assert(uDim < QRNG_NDMS);
        m_uCurDim = uDim;
        updateUValue();
        updateDValue();
    }
    /// generate pseudo-random number >= 0 and < 1
    double generate01()
    {
        double fValue = m_fRenormalizedValue;
        if (++m_uCurDim >= QRNG_NDMS)
        {
            assert(false); // probably something is wrong - why do you need such a high dimension?
            m_uCurDim = 0;
            ++m_uCurSeed;
        }
        updateUValue();
        updateDValue();
        return fValue;
    }
    // generate unsigned integer value >= uMin and < uMax
    inline unsigned generateUnsigned(unsigned uMin, unsigned uMax)
    {
        unsigned uValue = getUValueAndRenormalize(uMax - uMin);
        assert(uMin + uValue < uMax);
        return uMin + uValue;
    }
    // generate double value >= fMin and < fMax;
    inline double generateBetween(double fMin, double fMax)
    {
        double fTmp = generate01();
        return fMin * (1 - fTmp) + fMax * fTmp;
    }

private:
    inline void updateDValue()
    {
        m_uRenormalizationProduct = 1;
        m_fRenormalizedValue = m_prev[m_uCurDim].uValue / (double)MAX_INT64;
        assert(m_fRenormalizedValue >= 0 && m_fRenormalizedValue < 1);
    }
    inline unsigned getUValueAndRenormalize(unsigned uMax)
    {
        m_fRenormalizedValue *= uMax;
        unsigned uValue = (unsigned)m_fRenormalizedValue;
        m_uRenormalizationProduct *= uMax;
        if (m_uRenormalizationProduct > RENORMALIZATION_POTENTIAL)
        {
            if (++m_uCurDim >= QRNG_NDMS)
            {
                m_uCurDim = 0;
                ++m_uCurSeed;
            }
            updateUValue();
            updateDValue();
            return uValue;
        }
        m_fRenormalizedValue -= uValue;
        return uValue;
    }
    void updateUValue();
    static const unsigned QRNG_NDMS = 32; // number of dimensions of sequence we're going to generate
    static const unsigned RENORMALIZATION_POTENTIAL = 2048;
    struct
    {
        unsigned __int64 uValue;
        unsigned uPrevSeed;
    } m_prev[QRNG_NDMS];
    double m_fRenormalizedValue;
    unsigned m_uCurSeed, m_uCurDim, m_uRenormalizationProduct;

    // static values used for quasi-random numbers generation
    static unsigned __int64 cjn[QRNG_NDMS][63];
    const static unsigned __int64 MAX_INT64 = 0x8000000000000000ULL;
    static void GenerateCJ();
    static int GeneratePolynomials(int buffer[QRNG_NDMS], bool primitive);
};

#endif
