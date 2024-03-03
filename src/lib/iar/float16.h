/*
#define S10E5_MIN	5.96046448e-08	// Smallest positive s10e5

#define S10E5_NRM_MIN	6.10351562e-05	// Smallest positive normalized s10e5

#define S10E5_MAX	65504.0		// Largest positive s10e5

#define S10E5_EPSILON	0.00097656	// Smallest positive e for which
					// s10e5 (1.0 + e) != s10e5 (1.0)

#define S10E5_MANT_DIG	11		// Number of digits in mantissa
					// (significand + hidden leading 1)

#define S10E5_DIG	2		// Number of base 10 digits that
					// can be represented without change

#define S10E5_RADIX	2		// Base of the exponent

#define S10E5_MIN_EXP	-13		// Minimum negative integer such that
					// S10E5_RADIX raised to the power of
					// one less than that integer is a
					// normalized s10e5

#define S10E5_MAX_EXP	16		// Maximum positive integer such that
					// S10E5_RADIX raised to the power of
					// one less than that integer is a
					// normalized s10e5

#define S10E5_MIN_10_EXP	-4		// Minimum positive integer such
					// that 10 raised to that power is
					// a normalized s10e5

#define S10E5_MAX_10_EXP	4		// Maximum positive integer such
					// that 10 raised to that power is
					// a normalized s10e5
*/


// This union gives us an easy way to examine and/or set the individual bit-fields of an s23e8.
typedef union
{
        unsigned        int     i;
                        float   f;
} u_u32_s23e8_t;


// ------------------------ out-convert s10e5 to s23e8 ----------------------

static
float   s10e5_to_s23e8( int _h )
{
        register        int             s       = _h & 0x8000;
        register        int             e       = (_h & 0x7c00) >> 10;
        register        int             m       = _h & 0x03ff;
                        u_u32_s23e8_t   x;

        s       <<= 16;

        if(             e == 31         )
        {
                // infinity or NAN
                e       =   255 << 23;
                m       <<= 13;
                x.i     =   s | e | m;
        }
        else if(        e > 0           )
        {
                // normalized
                e       =   e + (127 - 15);
                e       <<= 23;
                m       <<= 13;
                x.i     =   s | e | m;
        }
        else if(        m == 0          )
        {
                // zero
                x.i     =   s;
        }
        else
        {
                // subnormal, value is m times 2^-24
                x.f     =   ( (float) m );
                x.i     =   s | (x.i - (24 << 23));
        }

        return( x.f );
}
