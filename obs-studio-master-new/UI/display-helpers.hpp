#pragma once

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
#define SUPPORTS_FRACTIONAL_SCALING
#endif

static inline void GetScaleAndCenterPos(int baseCX, int baseCY, int windowCX,
					int windowCY, int &x, int &y,
					float &scale)
{
	double windowAspect, baseAspect;
	int newCX, newCY;

	windowAspect = double(windowCX) / double(windowCY);
	baseAspect = double(baseCX) / double(baseCY);

	if (windowAspect > baseAspect) {
		scale = float(windowCY) / float(baseCY);
		newCX = int(double(windowCY) * baseAspect);
		newCY = windowCY;
	} else {
		scale = float(windowCX) / float(baseCX);
		newCX = windowCX;
		newCY = int(float(windowCX) / baseAspect);
	}

	x = windowCX / 2 - newCX / 2;
	y = windowCY / 2 - newCY / 2;
}

static inline void GetCenterPosFromFixedScale(int baseCX, int baseCY,
					      int windowCX, int windowCY,
					      int &x, int &y, float scale)
{
	x = (float(windowCX) - float(baseCX) * scale) / 2.0f;
	y = (float(windowCY) - float(baseCY) * scale) / 2.0f;
}

static inline QSize GetPixelSize(QWidget *widget)
{
#ifdef SUPPORTS_FRACTIONAL_SCALING
	return widget->size() * widget->devicePixelRatioF();
#else
	return widget->size() * widget->devicePixelRatio();
#endif
}
