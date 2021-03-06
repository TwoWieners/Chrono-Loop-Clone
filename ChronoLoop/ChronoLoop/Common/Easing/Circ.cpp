#include "Circ.h"
#include <math.h>

namespace Epoch {
	namespace Easing {

		float CircIn (float t, float b, float c, float d) {
			return -c * ((float)sqrt(1 - (t/=d)*t) - 1) + b;
		}

		float CircOut(float t, float b, float c, float d) {
			return c * (float)sqrt(1 - (t=t/d-1)*t) + b;
		}

		float CircInOut(float t, float b, float c, float d) {
			if ((t/=d/2) < 1) return -c/2 * ((float)sqrt(1 - t*t) - 1) + b;
			return c/2 * ((float)sqrt(1 - t*(t-=2)) + 1) + b;
		}

	}
}
