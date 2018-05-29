#include <stdio.h>
#include <stdlib.h>
#include "iir.h"

//external vairiables
extern double IIR_HP200_COEF[IIR_SECTION][IIR_TOTAL_ORDER] =
{
	{ 1.0f, -2.0f, 1.0f, 1.0f, -1.95962203540413f, 0.965681575688920f },
	{ 1.0f, -2.0f, 1.0f, 1.0f, -1.90084813073486f, 0.906725930440798f },
	{ 1.0f, -2.0f, 1.0f, 1.0f, -1.86219741688151f, 0.867955700908716f },
	{ 1.0f, -1.0f, 0.0f, 1.0f, -0.924390491658207f, 0.0f }
};

extern double IIR_HP200_SCALE[IIR_SECTION] =
{
	0.981325902773263f,
	0.951893515293914f,
	0.932538279447557f,
	0.962195245829104f
};