#include "TCBSpline.h"

//private
void TCBSpline::Expand()
{
	//control points
	this->mControlCap += this->mControlCap;
	D3DXVECTOR3** temp = new D3DXVECTOR3*[this->mControlCap];
	for(int i = 0; i < this->mNrOfControlPoints; i++)
	{
		temp[i] = this->mControlPoints[i];
	}
	delete [] this->mControlPoints;
	this->mControlPoints = temp;
	temp = NULL;
}

void TCBSpline::CalculateTangents(int i)
{
	D3DXVECTOR3 v1, v2, src, dst;
	float s0, s1;
	float d0, d1;
	
	s0 = ((1 - this->mTension) * (1 + this->mBias) * (1 - this->mContinuity)) * 0.5f;
	s1 = ((1 - this->mTension) * (1 - this->mBias) * (1 + this->mContinuity)) * 0.5f;
	d0 = ((1 - this->mTension) * (1 + this->mBias) * (1 + this->mContinuity)) * 0.5f;
	d1 = ((1 - this->mTension) * (1 - this->mBias) * (1 - this->mContinuity)) * 0.5f;

	if(i == 0) //first
	{
		//source-tangent
		v1 = s0 * (*this->mControlPoints[0] - D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		v2 = s1 * (*this->mControlPoints[1] - *this->mControlPoints[0]);
		src = v1 + v2; 
		this->mSource[0] = new D3DXVECTOR3(src);
		//destination tangent
		v1 = d0 * (*this->mControlPoints[0] - D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		v2 = d1 * (*this->mControlPoints[1] - *this->mControlPoints[0]);
		src = v1 + v2;
		this->mDestination[0] = new D3DXVECTOR3(src);
	}
	else if(i == (this->mNrOfControlPoints - 1)) //last
	{
		//source-tangent
		v1 = s0 * (*this->mControlPoints[this->mNrOfControlPoints - 1] - *this->mControlPoints[this->mNrOfControlPoints - 2]);
		v2 = s1 * (D3DXVECTOR3(0.0f, 0.0f, 0.0f) - *this->mControlPoints[this->mNrOfControlPoints - 1]);
		src = v1 + v2;
		this->mSource[this->mNrOfControlPoints - 1] = new D3DXVECTOR3(src);
		//destination-tangent
		v1 = d0 * (*this->mControlPoints[this->mNrOfControlPoints - 1] - *this->mControlPoints[this->mNrOfControlPoints - 2]);
		v2 = d1 * (D3DXVECTOR3(0.0f, 0.0f, 0.0f) - *this->mControlPoints[this->mNrOfControlPoints - 1]);
		src = v1 + v2;
		this->mDestination[this->mNrOfControlPoints - 1] = new D3DXVECTOR3(src);
	}
	else //rest
	{
		//source(incoming) tangents
		v1 = s0 * (*this->mControlPoints[i] - *this->mControlPoints[i - 1]);
		v2 = s1 * (*this->mControlPoints[i + 1] - *this->mControlPoints[i]);
		src = v1 + v2;
		this->mSource[i] = new D3DXVECTOR3(src);
		//destination(outgoing) tangents
		v1 = d0 * (*this->mControlPoints[i] - *this->mControlPoints[i - 1]);
		v2 = d1 * (*this->mControlPoints[i + 1] - *this->mControlPoints[i]);
		dst = v1 + v2;
		this->mDestination[i] = new D3DXVECTOR3(dst);
	}
}

//de/con-structors, init, other
TCBSpline::TCBSpline(float tension, float bias, float continuity)
{
	this->mTension = tension;
	this->mBias = bias;
	this->mContinuity = continuity;

	this->mNrOfControlPoints = 0;
	//this->mNrOfPoints = 0;
	this->mControlCap = 10;
	this->mControlPoints = new D3DXVECTOR3*[this->mControlCap];
	this->mSource = NULL;
	this->mDestination = NULL;
	//this->mPoints = NULL;
}
TCBSpline::~TCBSpline()
{
	//control points
	for(int i = 0; i < this->mNrOfControlPoints; i++)
	{
		delete this->mControlPoints[i];
	}
	delete [] this->mControlPoints;
	//source & destination tangents
	if(this->mSource)
	{
		for(int i = 0; i < this->mNrOfControlPoints; i++)
		{	
			delete this->mSource[i];
			delete this->mDestination[i];
		}
		delete [] this->mSource;
		delete [] this->mDestination;
	}
}

HRESULT TCBSpline::Init()
{
	HRESULT hr = S_OK;

	if(this->mNrOfControlPoints > 1) //the number of control points has to be 2 or greater to create line(s)
	{
		this->mSource = new D3DXVECTOR3*[this->mNrOfControlPoints];
		this->mDestination = new D3DXVECTOR3*[this->mNrOfControlPoints];

		//calculate source & destination-tangents of the control points
		for(int i = 0; i < this->mNrOfControlPoints; i++)
		{
			TCBSpline::CalculateTangents(i);
		}
	}
	else
	{
		//remove control point if there is one
		if(this->mNrOfControlPoints == 1)
		{
			delete this->mControlPoints[0];
			this->mControlPoints[0] = NULL;
			this->mNrOfControlPoints--;
		}
		MaloW::Debug("TCBSpline: Warning: Failed to initilize spline: Not enough control points");

		return E_FAIL;
	}
	
	return hr;
}

//get
int TCBSpline::GetNrOfControlPoints() const
{
	return this->mNrOfControlPoints;
}
float TCBSpline::GetTension() const
{
	return this->mTension;
}
float TCBSpline::GetBias() const
{
	return this->mBias;
}
float TCBSpline::GetContinuity() const
{
	return this->mContinuity;
}

//set
void TCBSpline::SetTension(float tension)
{
	this->mTension = tension;
}
void TCBSpline::SetBias(float bias)
{
	this->mBias = bias;
}
void TCBSpline::SetContinuity(float continuity) 
{
	this->mContinuity = continuity;
}

//other
void TCBSpline::AddControlPoint(D3DXVECTOR3 controlPoint)
{
	if(this->mNrOfControlPoints == this->mControlCap)
	{
		TCBSpline::Expand();
	}
	this->mControlPoints[this->mNrOfControlPoints++] = new D3DXVECTOR3(controlPoint);
}


D3DXVECTOR3 TCBSpline::GetPoint(float t) const
{
	if(t != 1.0f)
	{
		//Split up the curves into 2 curves with local t value and create the points on the current curve using the local t value
		float tmp = (this->mNrOfControlPoints - 1) * t;
		int cpIndex = (int)tmp;
		float localT = tmp - cpIndex;
		float a0, a1, a2, a3;
		D3DXVECTOR3 cp0, cp1, d0, d1, p;
		//**src-vector används inte**
	
		cp0 = *this->mControlPoints[cpIndex];	
		cp1 = *this->mControlPoints[cpIndex + 1];	
		d0 = *this->mDestination[cpIndex];			
		d1 = *this->mDestination[cpIndex + 1];		

		a0 = 2.0f * pow(localT, 3.0f) - 3.0f * pow(localT, 2.0f) + 1;
		a1 = pow(localT, 3.0f) - 2.0f * pow(localT, 2.0f) + localT;
		a2 = pow(localT, 3.0f) - pow(localT, 2.0f);
		a3 = -2.0f * pow(localT, 3.0f) + 3.0f * pow(localT, 2.0f);

		p = a0 * cp0 +
			a1 * d0 +
			a2 * d1 +
			a3 * cp1;

		return p;
	}
	else
	{
		return *this->mControlPoints[this->mNrOfControlPoints - 1];
	}
}

D3DXVECTOR3** TCBSpline::CalculatePoints(int nrOfPoints)
{	
	D3DXVECTOR3** temp = new D3DXVECTOR3*[nrOfPoints];

	float t = 0.0f;
	for(int i = 0; i < nrOfPoints; i++)
	{
		t = (float)i / float(nrOfPoints);
		temp[i] = new D3DXVECTOR3(TCBSpline::GetPoint(t));
	}

	return temp;
}

void TCBSpline::WriteControlPointsToFile(string fileName)
{
	ofstream out;
	out.open(fileName, std::ios_base::out);

	int index = 0;
	char buffer[1000];
	if(out)
	{
		_itoa_s(this->mNrOfControlPoints, buffer, 10);
		while(buffer[index] != '\0')
		{
			index++;
		}
		buffer[index++] = '\n';
		out.write(buffer, index); 
		for(int i = 0; i < this->mNrOfControlPoints; i++)
		{
			//x-pos
			sprintf_s(buffer, "%f", this->mControlPoints[i]->x);
			index = 0;
			while(buffer[index] != '\0')
			{
				index++;
			}
			buffer[index++] = ' ';
			out.write(buffer, index);
			//y-pos
			sprintf_s(buffer, "%f", this->mControlPoints[i]->y);
			index = 0;
			while(buffer[index] != '\0')
			{
				index++;
			}
			buffer[index++] = ' ';
			out.write(buffer, index);
			//z-pos
			sprintf_s(buffer, "%f", this->mControlPoints[i]->z);
			index = 0;
			while(buffer[index] != '\0')
			{
				index++;
			}
			buffer[index++] = ' ';
			out.write(buffer, index);
			//new line
			buffer[0] = '\n';
			out.write(buffer, 1);
		}
		out.close();
	}
}
bool TCBSpline::ReadControlPointsFromFile(string fileName)
{
	ifstream in;
	in.open(fileName);
	if(in)
	{
		const int bufferSize = 512;
		char buffer[bufferSize];

		//get number of control points to read
		in.getline(buffer, bufferSize);
		sscanf_s(buffer, "%d", &this->mNrOfControlPoints);
		this->mControlCap = this->mNrOfControlPoints;
		this->mControlPoints = new D3DXVECTOR3*[this->mNrOfControlPoints];
		//read values
		for(int i = 0; i < this->mNrOfControlPoints; i++)
		{
			in.getline(buffer, bufferSize);
			this->mControlPoints[i] = new D3DXVECTOR3();
			sscanf_s(buffer, "%f %f %f", &this->mControlPoints[i]->x, &this->mControlPoints[i]->y, &this->mControlPoints[i]->z);
		}

		in.close();
		return true;
	}

	return false;
}
	

void TCBSpline::Clear()
{
	if(this->mControlPoints)
	{
		for(int i = 0; i < this->mNrOfControlPoints; i++)
		{
			SAFE_DELETE(this->mControlPoints[i]);
			//**delete source & destination tangents?**
		}
		this->mNrOfControlPoints = 0;
	}
}