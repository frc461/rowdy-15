#ifndef ROWDYMATH_H
#define ROWDYMATH_H

class RowdyMath
{
 public:
	double DoubleWeightedAverage(double a, double a_weight, double b, double b_weight);
	void DoubleWeightedAverage_InPlace(double *result, double a, double a_weight, double b, double b_weight);
	float FloatWeightedAverage(float a, float a_weight, float b, float b_weight);
	void FloatWeightedAverage_InPlace(float *result, float a, float a_weight, float b, float b_weight);

	double DoubleNullZonify(double value, double null_zone);
	void DoubleNullZonify_InPlace(double *value, double null_zone);
	float FloatNullZonify(float value, float null_zone);
	void FloatNullZonify_InPlace(float *value, float null_zone);
};

#endif
