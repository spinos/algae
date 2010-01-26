#ifndef _T_VECTORS_H
#define _T_VECTORS_H

struct XYZ
{
	XYZ() {}
	XYZ(const float v) {x= y= z=v;}
	XYZ(const int v) {x= y= z=(float)v;}
	XYZ(const float* p) {x= p[0]; y=p[1]; z=p[2];}
	XYZ(const float& _x, const float& _y, const float& _z) 
	{
		x = _x;
		y = _y;
		z = _z;
	}
	XYZ(const XYZ& from, const XYZ& to) 
	{
		x = to.x - from.x; 
		y = to.y - from.y; 
		z = to.z - from.z; 
	}
	XYZ(float* p) 
	{
		x = p[0];
		y = p[1];
		z = p[2];
	}
	
	void operator+=( const XYZ& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	
	void operator-=( const XYZ& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	
	void operator/=( int& other )
	{
		x /= other;
		y /= other;
		z /= other;
	}
	
	void operator/=( float& other )
	{
		x /= other;
		y /= other;
		z /= other;
	}
	
	void operator/=( const float other )
	{
		x /= other;
		y /= other;
		z /= other;
	}
	
	void operator*=( float& other )
	{
		x *= other;
		y *= other;
		z *= other;
	}
	
	void operator*=( double& other )
	{
		x *= (float)other;
		y *= (float)other;
		z *= (float)other;
	}
	
	void operator*=( const double other )
	{
		x *= (float)other;
		y *= (float)other;
		z *= (float)other;
	}
	
	XYZ operator+( const XYZ& other ) const
	{
		return XYZ(x+other.x, y+other.y, z+other.z);
	}
	
	XYZ operator-( const XYZ& other ) const
	{
		return XYZ(x-other.x, y-other.y, z-other.z);
	}
	
	XYZ operator*( const XYZ& other ) const
	{
		return XYZ(x*other.x, y*other.y, z*other.z);
	}
	
	XYZ operator*( float scale ) const
	{
		return XYZ(x*scale,y*scale,z*scale);
	}
	
	XYZ operator/( float scale ) const
	{
		return XYZ(x/scale,y/scale,z/scale);
	}
	
	double length() const
	{
		return sqrt(x*x + y*y + z*z);
	}
	
	float lengthSquare() const
	{
		return (x*x + y*y + z*z);
	}
	
	double maxvalue() const
	{
		if(x>y && x>z) return x;
		else if(y>x && y>z) return y;
		else return z;
	}
	
	float dot(const XYZ& other) const
	{
		return ( x*other.x + y*other.y + z*other.z);
	}
	
	XYZ cross(const XYZ& other) const
	{
		return XYZ(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
	
	void normalize()
	{
		double len = length();
		if(len > 10e-8)
		{
		x /= (float)len;
		y /= (float)len;
		z /= (float)len;
		}
		else
		{
			x = y = z = 0.577350f;
		}
	}
	
	XYZ normal() const
	{
		double mag = sqrt( x * x + y * y + z * z ) + 10e-8;
		return XYZ(x /(float)mag, y /(float)mag, z /(float)mag);
	}
	
	XYZ operator^(const XYZ& other) const
	{
		return XYZ(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
	
	void zero() { x=y=z=0.0f;}
	
	float distanceSquaredTo(const XYZ& other) const
	{
		return ((other.x-x)*(other.x-x) + (other.y-y)*(other.y-y) + (other.z-z)*(other.z-z));
	}
	
	float distanceTo(const XYZ& other) const
	{
		return sqrt((other.x-x)*(other.x-x) + (other.y-y)*(other.y-y) + (other.z-z)*(other.z-z));
	}
	
	float operator[](int i) const
	{
		if(i==0) return x;
		else if(i==1) return y;
		else return z;
	}
	
	void perpendicular(XYZ& other) const
	{
		//normalize();
		XYZ up(0,1,0);
		if( y >0.9 || y <-0.9)
		{
			up.x = 1.0f; up.y = 0.0f;
		}
		other = cross(up);
		other.normalize();
	}
	
	void reverse()
	{
		x *= -1;
		y *= -1;
		z *= -1;
	}
	
	XYZ reversed() const
	{
		return XYZ(-x, -y, -z);
	}
	
	void findUpAndSide(XYZ& up, XYZ& side)
	{
		XYZ ref(0,1,0);
		if( y >0.9 || y <-0.9)
		{
			ref.x = 1.0f; ref.y = 0.0f;
		}
		side = cross(ref);
		side.reverse();
		side.normalize();
		up = cross(side);
		up.normalize();
	}
	
	void setcomp(int c, float v)
	{
		if(c==0) x = v;
		else if(c==1) y = v;
		else z = v;
	}
	
	void set(float vx, float vy, float vz)
	{
		x = vx; y = vy; z = vz;
	}
	
	void set(float v)
	{
		x = y = z = v;
	}
	
	void rotateXY(float ang)
	{
		float a = x;
		x = a*cos(ang);
		y = a*sin(ang);
	}
	
	void rotateXZ(float ang)
	{
		float a = x;
		x = a*cos(ang);
		z = a*sin(ang);
	}
	
	void rotateZY(float ang)
	{
		float a = z;
		z = a*cos(ang);
		y = a*sin(ang);
	}
	
	void rotateAroundAxis(const XYZ& axis, float theta)
	{
		if(theta==0) return;
		XYZ ori(x,y,z);
		float l = ori.length();
		ori.normalize();
		
		XYZ up = axis.cross(ori);
		up.normalize();
		
		XYZ side = ori - axis*(axis.dot(ori));
		
		up *=side.length();
		
		ori += side*(cos(theta) - 1);
		ori += up*sin(theta);
		
		ori.normalize();
		x = ori.x*l;
		y = ori.y*l;
		z = ori.z*l;
	}
	
	void rotateAlong(const XYZ& v, float eta)
	{
		XYZ ori(x,y,z);
		float l = ori.length();
		ori += v*eta;
		ori.normalize();
		x = ori.x*l;
		y = ori.y*l;
		z = ori.z*l;
	}
		
	float angleToVec(const XYZ& a, XYZ& axis) const
	{
		XYZ buf(x,y,z); buf.normalize();
		XYZ abuf = a; abuf.normalize();
		axis = buf.cross(abuf);
		axis.normalize();
		return acos(abuf.dot(buf));
	}
	
	void randRotate(const float size)
	{
		float leng = length();
		normalize();
		x += size*(rand( )%31/31.f - 0.5f);
		y += size*(rand( )%41/41.f - 0.5f);
		z += size*(rand( )%73/73.f - 0.5f);
		normalize();
		x *= leng;
		y *= leng;
		z *= leng;
	}
	
	void setLength(const float val)
	{
		normalize();
		x *= val;
		y *= val;
		z *= val;
	}
	
	float x,y,z;
};
#endif
