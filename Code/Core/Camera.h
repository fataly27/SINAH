#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vecteur.h"

#include "Constants.h"

#define M_PI  3.14159265358979323846

class Camera
{
	public:
		Camera();
		Camera(float angle);
		~Camera();

		void moveCamera(Vecteur direction);
		void zoomCamera(int ticks);
		void releaseZoom();

		int getTicks();
		Vecteur getPosition();
		Vecteur getFocus();
		Vecteur getOrientation();

		void setTicks(int ticks);
		void setPosition(Vecteur position);
		void setFocus(Vecteur focus);
		void setOrientation(Vecteur orientation);
		void setOrientation(float angleOrientation);

	private:
		int mTicks;
		Vecteur mOrientation;
		Vecteur mActualPosition;
		Vecteur mFocus;
		Vecteur mStartPosition;
		Vecteur mStopPosition;
};

#endif // CAMERA_H_
