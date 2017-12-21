///*
// *  Camera.h
// *  Lab04
// *
// *  Created by Amr Elsehemy on 3/21/10.
// *  Copyright 2010 Elsehemy Dev. All rights reserved.
// *
// */
//
//#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glut.h>
//
//#include "Vector3.h"
//const float PI = 3.1415926535f;
//
//class FirstPersonCamera
//{
//	
//public:
//	/// The position of the eye.
//	 Vector3 P;
//	
//	/// The right direction. x
//	Vector3 R;
//	
//	/// The up direction. y
//	Vector3 U;
//	
//	/// The -ve of the look direction. z
//	Vector3 D;
//	
//	/// Returns the direction in which the camera is looking (-n).
//	 Vector3 getLookDirection()
//	{
//		return D * -1 ;
//		
//	}
//	
//	/// <summary>
//	/// Eye = (0, 0, 0), Center = (0, 0, -1), Up = (0, 1, 0)
//	/// </summary>
//	FirstPersonCamera()
//	{
//		Reset( Vector3(0, 0, 0), Vector3(0, 0, -1), Vector3(0, 1, 0));
//	}
//	
//	FirstPersonCamera(Vector3 eye, Vector3 center, Vector3 up)
//	{
//		Reset(eye, center, up);
//	}
//	
//	 FirstPersonCamera(
//							 float eyeX,
//							 float eyeY,
//							 float eyeZ,
//							 float centerX,
//							 float centerY,
//							 float centerZ,
//							 float upX,
//							 float upY,
//							 float upZ)
//	{
//		Reset(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
//	}
//	
//	void Reset(Vector3 eye, Vector3 center, Vector3 up)
//	{
//		P = eye;
//		//still needs normalization
//		D = eye - center;
//		//i = j x k
//	
//		R = Vector3::CrossProduct(up, D);
//		//j = k x i
//		U = Vector3::CrossProduct(D, R);
//		//normalize all
//		R.Normalize();
//		U.Normalize();
//		D.Normalize();
//	}
//	
//	void Reset(
//					  float eyeX,
//					  float eyeY,
//					  float eyeZ,
//					  float centerX,
//					  float centerY,
//					  float centerZ,
//					  float upX,
//					  float upY,
//					  float upZ)
//	{
//		Vector3 eyePt =  Vector3(eyeX, eyeY, eyeZ);
//		Vector3 centerPt = Vector3(centerX, centerY, centerZ);
//		Vector3 upVec = Vector3(upX, upY, upZ);
//		Reset(eyePt, centerPt, upVec);
//	}
//	
//	/// <summary>
//	/// Multiplies the current matrix with the camera view matrix.
//	/// </summary>
//	/// <remarks>
//	/// Generally, you will use this function to tell opengl the
//	/// camera matrix as follows:
//	/// Gl.glMatrixMode(Gl.GL_MODELVIEW);
//	/// Gl.glLoadIdentity(); //MV = I
//	/// camera.Tellgl(); //MV = I * V = V
//	/// </remarks>
//	void Tellgl()
//	{
//		/*
//		 * Rx Ry Rz -eye.R
//		 * Ux Uy Uz -eye.U
//		 * Dx Dy Dz -eye.D
//		 * 0  0  0   1
//		 */
//		float m[16];
//		
//		m[0] = R.x; m[4] = R.y; m[8] = R.z; 
//		m[12] = -Vector3::DotProduct(P, R);
//		m[1] = U.x; m[5] = U.y; m[9] = U.z; 
//		m[13] = -Vector3::DotProduct(P, U);
//		m[2] = D.x; m[6] = D.y; m[10] = D.z; 
//		m[14] = -Vector3::DotProduct(P, D);
//		m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1;
//		//post-multiply the current matrix by m: ct = ct * M
//		glMultMatrixf(m);
//	}
//	
//	/// <summary>
//	/// Rotates the axes (D, R) about the U-axis with the specified angle.
//	/// </summary>//y--->z--->x
//	 void Yaw(float angleDegrees)
//	{
//		float angleRad = ToRadians(angleDegrees);
//		float c = cos(angleRad);
//		float s = sin(angleRad);
//		
//		Vector3 Dnew =  D * c +  R * s ;
//		Vector3 Rnew =  D * -s + R * c;
//		//Replace the old vectors with the new ones
//		D = Dnew;
//		R = Rnew;
//	}
//	
//	/// <summary>
//	/// Rotates the axes (U, D) about the R-axis with the specified angle.
//	/// </summary>//x--->y--->z
//	 void Pitch(float angleDegrees)
//	{
//		float angleRad = ToRadians(angleDegrees);
//		float c = cos(angleRad);
//		float s = sin(angleRad);
//		Vector3 Unew = U * c   + D * s;
//		Vector3 Dnew = U * -s + D * c;
//		//Replace the old vectors with the new ones
//		U = Unew;
//		D = Dnew;
//	}
//	
//	/// <summary>
//	/// Rotates the axes (R, U) about the D-axis with the specified angle.
//	/// </summary>//z--->x--->y
//	 void Roll(float angleDegrees)
//	{
//		float angleRad = ToRadians(angleDegrees);
//		float c = cos(angleRad);
//		float s = sin(angleRad);
//		Vector3 Rnew = R * c  + U * s;
//		Vector3 Unew = R * -s + U * c;
//		//Replace the old vectors with the new ones
//		R = Rnew;
//		U = Unew;
//	}
//	
//	/// <summary>
//	/// Moves the eye point a distance dist forward == -dist * D
//	/// Walk
//	/// </summary>
//	 void MoveForward(float dist)
//	{
//		P -=  D * dist;
//	}
//	
//	/// <summary>
//	/// Moves the eye point a distance dist backward == +dist * D
//	/// Walk
//	/// </summary>
//	 void MoveBackward(float dist)
//	{
//		P +=  D * dist;
//	}
//	
//	/// <summary>
//	/// Moves the eye point a distance dist to the right == +dist * R
//	/// Strafe
//	/// </summary>
//	 void MoveRight(float dist)
//	{
//		P +=  R * dist;
//	}
//	
//	/// <summary>
//	/// Moves the eye point a distance dist to the left == -dist * R
//	/// Strafe
//	/// </summary>
//	 void MoveLeft(float dist)
//	{
//		P -= R * dist;
//	}
//	
//	/// <summary>
//	/// Moves the eye point a distance dist upward == +dist * U
//	/// Fly
//	/// </summary>
//	 void MoveUpward(float dist)
//	{
//		P += U * dist;
//	}
//	
//	/// <summary>
//	/// Moves the eye point a distance dist downward == -dist * U
//	/// Fly
//	/// </summary>
//	 void MoveDownward(float dist)
//	{
//		P -= U * dist;
//	}
//	
//private:
//	
//	float ToRadians(float degrees)
//	{
//		return (float)(PI/180 * degrees);
//	}
//	
//	float ToDegrees(float radians)
//	{
//		return (float)(180/PI * radians);
//	}
//};