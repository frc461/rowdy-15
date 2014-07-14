double WeightedAverage(double a, double a_weight, double b, double b_weight)
{
	return (((a * a_weight) + (b * b_weight)) / (a_weight + b_weight))
}

void WeightedAverage_InPlace(double *result, double a, double a_weight, double b, double b_weight)
{
	*result = (((a * a_weight) + (b * b_weight)) / (a_weight + b_weight))
}

float FloatWeightedAverage(float a, float a_weight, float b, float b_weight)
{
	return (((a * a_weight) + (b * b_weight)) / (a_weight + b_weight))
}

void FloatWeightedAverage_InPlace(float *result, float a, float a_weight, float b, float b_weight)
{
	*result = (((a * a_weight) + (b * b_weight)) / (a_weight + b_weight))
}

float NullZonify(float value, float null_zone)
{
	if(value <= +null_zone ||
	   value >= -null_zone) {
		return 0.0;
	} else {
		return value;
	}
}

void NullZonify_InPlace(float *value, float null_zone)
{
	if(*value <= +null_zone ||
	   *value >= -null_zone) {
		*value = 0.0;
	}
}
