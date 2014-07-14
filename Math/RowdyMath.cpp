#include "RowdyMath.h"

double RowdyMath::DoubleWeightedAverage(double a, double a_weight, double b, double b_weight)
{
	return (((a * a_weight) + (b * b_weight)) / (a_weight + b_weight));
}

void RowdyMath::DoubleWeightedAverage_InPlace(double *result, double a, double a_weight, double b, double b_weight)
{
	*result = (((a * a_weight) + (b * b_weight)) / (a_weight + b_weight));
}

float RowdyMath::FloatWeightedAverage(float a, float a_weight, float b, float b_weight)
{
	return (((a * a_weight) + (b * b_weight)) / (a_weight + b_weight));
}

void RowdyMath::FloatWeightedAverage_InPlace(float *result, float a, float a_weight, float b, float b_weight)
{
	*result = (((a * a_weight) + (b * b_weight)) / (a_weight + b_weight));
}

double RowdyMath::DoubleNullZonify(double value, double null_zone)
{
	if(value <= +null_zone ||
	   value >= -null_zone) {
		return 0.0;
	} else {
		return value;
	}
}

void RowdyMath::DoubleNullZonify_InPlace(double *value, double null_zone)
{
	if(*value <= +null_zone ||
	   *value >= -null_zone) {
		*value = 0.0;
	}
}

float RowdyMath::FloatNullZonify(float value, float null_zone)
{
	if(value <= +null_zone ||
	   value >= -null_zone) {
		return 0.0;
	} else {
		return value;
	}
}

void RowdyMath::FloatNullZonify_InPlace(float *value, float null_zone)
{
	if(*value <= +null_zone ||
	   *value >= -null_zone) {
		*value = 0.0;
	}
}
