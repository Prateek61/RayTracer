#include "Interval.h"

namespace RT
{
	const Interval Interval::Empty = Interval(INF, -INF);
	const Interval Interval::Universe = Interval(-INF, INF);
}