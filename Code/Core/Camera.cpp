#include "Includes.h"

Camera::Camera() : Camera(80.0f) {}
Camera::Camera(float angle) : mTicks(CameraConstants::ticksOffset), mFocus(0, 0, 0), mOrientation(0, Vecteur::absoluteValue(sin(angle * 2 * M_PI / 360)), Vecteur::absoluteValue(cos(angle * 2 * M_PI / 360)))
{
	mActualPosition = mOrientation * mTicks + mFocus;
	mStartPosition = mActualPosition;
	mStopPosition = mActualPosition;
}
Camera::~Camera() {}

// ------------------------------------

void Camera::moveCamera(Vecteur direction)
{
	if (GeneralConstants::nbFrames <= 0)
		throw std::domain_error("Le nombre de frames ne peut pas être nul ou inférieur à zéro");
	if(mTicks <= 0)
		throw std::domain_error("Le nombre de ticks de zoom ne peut pas être nul ou inférieur à zéro");
	if (GeneralConstants::nbFrames > 0 && mTicks > 0)
	{
		mActualPosition += direction * CameraConstants::speed / GeneralConstants::nbFrames * mTicks;
		mStartPosition += direction * CameraConstants::speed / GeneralConstants::nbFrames * mTicks;
		mStopPosition += direction * CameraConstants::speed / GeneralConstants::nbFrames * mTicks;
		mFocus += direction * CameraConstants::speed / GeneralConstants::nbFrames * mTicks;
	}
}
void Camera::zoomCamera(int ticks)
{
	this->setTicks(mTicks + ticks * CameraConstants::sensisivity);
	mStopPosition = mFocus + mOrientation * mTicks;
}
void Camera::releaseZoom()
{
	if(CameraConstants::nbFramesTransition <= 0)
		throw std::domain_error("Le nombre de frames d'animation de zoom ne peut pas être nul ou inférieur à zéro");
	else
	{
		if (mActualPosition == mStopPosition)
			mStartPosition = mActualPosition;
		else
		{
			Vecteur Itineraire(mStopPosition - mStartPosition);
			double partieFaite((mActualPosition - mStartPosition) / Itineraire);

			if (partieFaite >= 1.0)
			{
				mStopPosition = mActualPosition;
				mStartPosition = mStopPosition;
			}
			else
			{
				mActualPosition += Itineraire / CameraConstants::nbFramesTransition;
			}
		}
	}
	
}

// ------------------------------------

int Camera::getTicks()
{
	return mTicks;
}
Vecteur Camera::getPosition()
{
	return mActualPosition;
}
Vecteur Camera::getFocus()
{
	return mFocus;
}
Vecteur Camera::getOrientation()
{
	return mOrientation;
}

void Camera::setTicks(int ticks)
{
	if (ticks < CameraConstants::minTicks + CameraConstants::ticksOffset)
		mTicks = CameraConstants::minTicks + CameraConstants::ticksOffset;
	else if (ticks > CameraConstants::maxTicks + CameraConstants::ticksOffset)
		mTicks = CameraConstants::maxTicks + CameraConstants::ticksOffset;
	else
		mTicks = ticks;
}
void Camera::setPosition(Vecteur position)
{
	mActualPosition = position;
	mStartPosition = mActualPosition;
	mStopPosition = mActualPosition;
	mFocus = mActualPosition - mOrientation;
}
void Camera::setFocus(Vecteur focus)
{
	mFocus = focus;
	mActualPosition = mFocus + mOrientation * mTicks;
	mStartPosition = mActualPosition;
	mStopPosition = mActualPosition;
}
void Camera::setOrientation(Vecteur orientation)
{
	mOrientation = orientation;
	mActualPosition = mFocus + mOrientation * mTicks;
	mStartPosition = mActualPosition;
	mStopPosition = mActualPosition;
}
void Camera::setOrientation(float angleOrientation)
{
	this->setOrientation(Vecteur(0, sin(angleOrientation), cos(angleOrientation)));
}
