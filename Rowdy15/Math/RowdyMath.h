#ifndef ROWDYMATH_H
#define ROWDYMATH_H

class RowdyMath
{
 public:
	static double DoubleWeightedAverage(double a, double a_weight, double b, double b_weight);
	static void DoubleWeightedAverage_InPlace(double *result, double a, double a_weight, double b, double b_weight);
	static float FloatWeightedAverage(float a, float a_weight, float b, float b_weight);
	static void FloatWeightedAverage_InPlace(float *result, float a, float a_weight, float b, float b_weight);

	static double DoubleNullZonify(double value, double null_zone);
	static void DoubleNullZonify_InPlace(double *value, double null_zone);
	static float FloatNullZonify(float value, float null_zone);
	static void FloatNullZonify_InPlace(float *value, float null_zone);
};

#endif
