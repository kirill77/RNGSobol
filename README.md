# RNGSobol
Sobol quadi-random numbers generator (C++)

Note that unlike pseudo-random numbers, quasi-random numbers care about dimensionality of points. Don't forget to call nextSeed()
when you start new point. To generate uniformly distributed points in 3 dimensions, do this:

<pre>
RNGSobol rng;
for (NvU32 u = 0; u < 10000; ++u) // generate 10000 points
{
  m_rng.nextSeed(); // start from dimension 0 for each point
  float vPoint[3];
  for (NvU32 uDim = 0; uDim < 3; ++uDim)
  {
     point[uDim] = rng.generate01(); // generates number and goes to next dimension
  }
}
</pre>
