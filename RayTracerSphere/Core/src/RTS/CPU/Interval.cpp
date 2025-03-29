#include "Interval.h"

namespace RTS
{
	const Interval Interval::Empty = Interval(RTS::INF, -RTS::INF);
	const Interval Interval::Universe = Interval(-RTS::INF, RTS::INF);
}